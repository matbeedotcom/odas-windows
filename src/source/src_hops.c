/**
* \file     src_hops.c
* \author   François Grondin <francois.grondin2@usherbrooke.ca>
* \version  2.0
* \date     2018-03-18
* \copyright
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*/

#include <source/src_hops.h>

// Forward declarations for WASAPI functions
#ifdef _WIN32
    void src_hops_open_interface_wasapi(src_hops_obj * obj);
    void src_hops_close_interface_wasapi(src_hops_obj * obj);
    int src_hops_process_interface_wasapi(src_hops_obj * obj);

    static int winsock_initialized = 0;
    static void init_winsock() {
        if (!winsock_initialized) {
            WSADATA wsa;
            if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
                printf("Failed to initialize Winsock\n");
                exit(EXIT_FAILURE);
            }
            winsock_initialized = 1;
        }
    }
#endif

src_hops_obj * src_hops_construct(const src_hops_cfg * src_hops_config, const msg_hops_cfg * msg_hops_config) {

    src_hops_obj * obj;
    unsigned int nBytes;

    obj = (src_hops_obj *) malloc(sizeof(src_hops_obj));

    obj->timeStamp = 0;

    obj->hopSize = msg_hops_config->hopSize;
    obj->nChannels = msg_hops_config->nChannels;
    obj->fS = msg_hops_config->fS;

    obj->format = format_clone(src_hops_config->format);
    obj->interface = interface_clone(src_hops_config->interface);
    #ifndef _WIN32
        if (src_hops_config->channel_map != NULL)
        {
            // Will not be null if in pulseaudio mode
            memcpy(&obj->cm, src_hops_config->channel_map, sizeof(pa_channel_map));
        }
        else if (obj->interface->type == interface_pulseaudio)
        {
            // Can't be null if we are in pulseaudio mode
            printf("Error: Pulseaudio interface requires channel map.\n");
            exit(EXIT_FAILURE);
        }
    #endif

    memset(obj->bytes, 0x00, 4 * sizeof(char));

    if (!((obj->interface->type == interface_file ||
            obj->interface->type == interface_soundcard ||
            obj->interface->type == interface_pulseaudio ||
            obj->interface->type == interface_socket ||
            obj->interface->type == interface_wasapi) &&
          (obj->format->type == format_binary_int08 ||
           obj->format->type == format_binary_int16 ||
           obj->format->type == format_binary_int24 ||
           obj->format->type == format_binary_int32))) {

        printf("Source hops: Invalid interface and/or format.\n");
        exit(EXIT_FAILURE);
    }

    obj->buffer = (char *) malloc(sizeof(char) * obj->hopSize * obj->nChannels * 4);
    memset(obj->buffer, 0x00, sizeof(char) * obj->hopSize * obj->nChannels * 4);

    switch (obj->format->type) {

    case format_binary_int08:
        obj->bufferSize = obj->hopSize * obj->nChannels * 1;
        break;
    case format_binary_int16:
        obj->bufferSize = obj->hopSize * obj->nChannels * 2;
        break;
    case format_binary_int24:
        obj->bufferSize = obj->hopSize * obj->nChannels * 3;
        break;
    case format_binary_int32:
        obj->bufferSize = obj->hopSize * obj->nChannels * 4;
        break;
    }

    obj->out = (msg_hops_obj *) NULL;

    return obj;
}

void src_hops_destroy(src_hops_obj * obj) {

    free((void *) obj->buffer);
    format_destroy(obj->format);
    interface_destroy(obj->interface);

    free((void *) obj);
}

void src_hops_connect(src_hops_obj * obj, msg_hops_obj * out) {

    obj->out = out;
}

void src_hops_disconnect(src_hops_obj * obj) {

    obj->out = (msg_hops_obj *) NULL;
}

void src_hops_open(src_hops_obj * obj) {

    switch (obj->interface->type) {

        case interface_file:

            src_hops_open_interface_file(obj);

        break;

        case interface_soundcard:

            src_hops_open_interface_soundcard(obj);

        break;

        case interface_pulseaudio:

                src_hops_open_interface_pulseaudio(obj);

        break;

        case interface_socket:

            src_hops_open_interface_socket(obj);

        break;

        case interface_wasapi:

            src_hops_open_interface_wasapi(obj);

        break;

        default:

            printf("Source hops: Invalid interface type.\n");
            exit(EXIT_FAILURE);

        break;

    }

}

void src_hops_open_interface_file(src_hops_obj * obj) {

    obj->fp = fopen(obj->interface->fileName, "rb");

    if (obj->fp == NULL) {
        printf("Cannot open file %s\n", obj->interface->fileName);
        exit(EXIT_FAILURE);
    }

}

void src_hops_open_interface_soundcard(src_hops_obj * obj) {

    #ifndef _WIN32
    snd_pcm_hw_params_t * hw_params;
    snd_pcm_format_t format;
    int err;

    switch (obj->format->type) {

        case format_binary_int08:

            format = SND_PCM_FORMAT_S8;

        break;

        case format_binary_int16:

            format = SND_PCM_FORMAT_S16_LE;

        break;

        case format_binary_int24:

            format = SND_PCM_FORMAT_S24_LE;

        break;

        case format_binary_int32:

            format = SND_PCM_FORMAT_S32_LE;

        break;

        default:

            printf("Source hops: Invalid format.\n");
            exit(EXIT_FAILURE);

        break;

    }

    if ((err = snd_pcm_open(&(obj->ch), obj->interface->deviceName, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        printf("Source hops: Cannot open audio device %s: %s\n", obj->interface->deviceName, snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
        printf("Source hops: Cannot allocate hardware parameter structure: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    if ((err = snd_pcm_hw_params_any(obj->ch, hw_params)) < 0) {
        printf("Source hops: Cannot initialize hardware parameter structure: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    if ((err = snd_pcm_hw_params_set_access(obj->ch, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        printf("Source hops: Cannot set access type: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    if ((err = snd_pcm_hw_params_set_format(obj->ch, hw_params, format)) < 0) {
        printf("Source hops: Cannot set sample format: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    if ((err = snd_pcm_hw_params_set_rate(obj->ch, hw_params, obj->fS, 0)) < 0) {
        printf("Source hops: Cannot set sample rate: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    if ((err = snd_pcm_hw_params_set_channels(obj->ch, hw_params, obj->nChannels)) < 0) {
        printf("Source hops: Cannot set channel count: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    if ((err = snd_pcm_hw_params(obj->ch, hw_params)) < 0) {
        printf("Source hops: Cannot set parameters: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    snd_pcm_hw_params_free(hw_params);

    if ((err = snd_pcm_prepare(obj->ch)) < 0) {
        printf("Source hops: Cannot prepare audio interface for use: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }
    #else
    printf("Source hops: ALSA soundcard interface not supported on Windows\n");
    exit(EXIT_FAILURE);
    #endif

}

void src_hops_open_interface_pulseaudio(src_hops_obj * obj) {

    #ifndef _WIN32
    pa_sample_format_t format;

    switch (obj->format->type) {

        case format_binary_int08:

            format = PA_SAMPLE_U8;

        break;

        case format_binary_int16:

            format = PA_SAMPLE_S16LE;

        break;

        case format_binary_int24:

            format = PA_SAMPLE_S24LE;

        break;

        case format_binary_int32:

            format = PA_SAMPLE_S32LE;

        break;

        default:

            printf("Source hops: Invalid format.\n");
            exit(EXIT_FAILURE);

        break;

    }

    obj->ss.format = format;
    obj->ss.rate = obj->fS;
    obj->ss.channels = obj->nChannels;

    int err;
    if (!(obj->pa = pa_simple_new(NULL, "Odas", PA_STREAM_RECORD, obj->interface->deviceName, "record", &obj->ss, &obj->cm, NULL, &err))) {
        printf("Source hops: Cannot open pulseaudio device %s: %s\n", obj->interface->deviceName, pa_strerror(err));
        exit(EXIT_FAILURE);
    }
    #else
    printf("Source hops: PulseAudio interface not supported on Windows\n");
    exit(EXIT_FAILURE);
    #endif

}

void src_hops_open_interface_socket(src_hops_obj * obj) {

    #ifdef _WIN32
        init_winsock();
    #endif

    memset(&(obj->sserver), 0x00, sizeof(struct sockaddr_in));

    obj->sserver.sin_family = AF_INET;
    obj->sserver.sin_addr.s_addr = inet_addr(obj->interface->ip);
    obj->sserver.sin_port = htons(obj->interface->port);
    obj->sid = socket(AF_INET, SOCK_STREAM, 0);

    #ifdef _WIN32
        if (obj->sid == INVALID_SOCKET) {
            printf("Source hops: Cannot create socket\n");
            exit(EXIT_FAILURE);
        }
    #endif

    if ((connect(obj->sid, (struct sockaddr *) &(obj->sserver), sizeof(obj->sserver))) < 0) {
        printf("Source hops: Cannot connect to server\n");
        exit(EXIT_FAILURE);
    }
}

void src_hops_close(src_hops_obj * obj) {

    switch (obj->interface->type) {

        case interface_file:

            src_hops_close_interface_file(obj);

        break;

        case interface_soundcard:

            src_hops_close_interface_soundcard(obj);

        break;

        case interface_pulseaudio:

            src_hops_close_interface_pulseaudio(obj);

        break;

        case interface_socket:

            src_hops_close_interface_socket(obj);

        break;

        case interface_wasapi:

            src_hops_close_interface_wasapi(obj);

        break;

        default:

            printf("Source hops: Invalid interface type.\n");
            exit(EXIT_FAILURE);

        break;
    }
}

void src_hops_close_interface_file(src_hops_obj * obj) {

    fclose(obj->fp);

}

void src_hops_close_interface_soundcard(src_hops_obj * obj) {

    #ifndef _WIN32
    snd_pcm_close(obj->ch);
    #else
    printf("Source hops: ALSA soundcard interface not supported on Windows\n");
    exit(EXIT_FAILURE);
    #endif

}
   
void src_hops_close_interface_pulseaudio(src_hops_obj * obj) {

    #ifndef _WIN32
    if (obj->pa != NULL)
        pa_simple_free(obj->pa);
    #else
    printf("Source hops: PulseAudio interface not supported on Windows\n");
    exit(EXIT_FAILURE);
    #endif

}

void src_hops_close_interface_socket(src_hops_obj * obj) {

    #ifdef _WIN32
        closesocket(obj->sid);
    #else
        close(obj->sid);
    #endif

}

int src_hops_process(src_hops_obj * obj) {

    int rtnValue;

    switch (obj->format->type) {

        case format_binary_int08:

            src_hops_process_format_binary_int08(obj);

        break;

        case format_binary_int16:

            src_hops_process_format_binary_int16(obj);

        break;

        case format_binary_int24:

            src_hops_process_format_binary_int24(obj);

        break;

        case format_binary_int32:

            src_hops_process_format_binary_int32(obj);

        break;

        default:

            printf("Source hops: Invalid format type.\n");
            exit(EXIT_FAILURE);

        break;
    }

    switch (obj->interface->type) {

        case interface_file:

            rtnValue = src_hops_process_interface_file(obj);

        break;

        case interface_soundcard:

            rtnValue = src_hops_process_interface_soundcard(obj);

        break;

        case interface_pulseaudio:

                rtnValue = src_hops_process_interface_pulseaudio(obj);

        break;

        case interface_socket:

            rtnValue = src_hops_process_interface_socket(obj);

        break;

        case interface_wasapi:

            rtnValue = src_hops_process_interface_wasapi(obj);

        break;

        default:

            printf("Source hops: Invalid interface type.\n");
            exit(EXIT_FAILURE);

        break;
    }

    obj->timeStamp++;
    obj->out->timeStamp = obj->timeStamp;

    return rtnValue;
}

int src_hops_process_interface_file(src_hops_obj * obj) {

    unsigned int nBytesTotal;
    int rtnValue;

    nBytesTotal = fread(obj->buffer, sizeof(char), obj->bufferSize, obj->fp);

    if (nBytesTotal == obj->bufferSize) {

        rtnValue = 0;
        
    }
    else {

        rtnValue = -1;
    
    }

    return rtnValue;
}

int src_hops_process_interface_soundcard(src_hops_obj * obj) {

    #ifndef _WIN32
    unsigned int nSamples;
    int rtnValue;
    int err;

    if (err = snd_pcm_readi(obj->ch, obj->buffer, obj->hopSize) > 0) {

        rtnValue = 0;

    }
    else {

        rtnValue = -1;

    }

    return rtnValue;
    #else
    printf("Source hops: ALSA soundcard interface not supported on Windows\n");
    exit(EXIT_FAILURE);
    #endif
}

int src_hops_process_interface_pulseaudio(src_hops_obj * obj) {

    #ifndef _WIN32
    int rtnValue;
    int err;

    if (pa_simple_read(obj->pa, obj->buffer, obj->bufferSize, &err) < 0) {

        rtnValue = -1;

    }
    else {

        rtnValue = 0;

    }

    return rtnValue;
    #else
    printf("Source hops: PulseAudio interface not supported on Windows\n");
    exit(EXIT_FAILURE);
    #endif

}


int src_hops_process_interface_socket(src_hops_obj * obj) {
    
    unsigned int nBytesTotal;
    int rtnValue;

    // ssize_t recv(int sockfd, void *buf, size_t len, int flags);
    nBytesTotal = recv(obj->sid, obj->buffer, obj->bufferSize, MSG_WAITALL);

    if (nBytesTotal == obj->bufferSize) {

        rtnValue = 0;
        
    }
    else {

        rtnValue = -1;
    
    }

    return rtnValue;
}

void src_hops_process_format_binary_int08(src_hops_obj * obj) {

    unsigned int iSample;
    unsigned int iChannel;
    unsigned int nBytes;
    float sample;

    nBytes = 1;

    for (iSample = 0; iSample < obj->hopSize; iSample++) {

        for (iChannel = 0; iChannel < obj->nChannels; iChannel++) {

            memcpy(&(obj->bytes[4 - nBytes]),
                   &(obj->buffer[(iSample * obj->nChannels + iChannel) * nBytes]),
                   sizeof(char) * nBytes);

            sample = pcm_signedXXbits2normalized(obj->bytes, nBytes);

            obj->out->hops->array[iChannel][iSample] = sample;
            
        }
        
    }
    
}

void src_hops_process_format_binary_int16(src_hops_obj * obj) {

    unsigned int iSample;
    unsigned int iChannel;
    unsigned int nBytes;
    float sample;

    nBytes = 2;

    for (iSample = 0; iSample < obj->hopSize; iSample++) {

        for (iChannel = 0; iChannel < obj->nChannels; iChannel++) {

            memcpy(&(obj->bytes[4 - nBytes]),
                   &(obj->buffer[(iSample * obj->nChannels + iChannel) * nBytes]),
                   sizeof(char) * nBytes);

            sample = pcm_signedXXbits2normalized(obj->bytes, nBytes);

            obj->out->hops->array[iChannel][iSample] = sample;
            
        }
        
    }
    
}

void src_hops_process_format_binary_int24(src_hops_obj * obj) {

    unsigned int iSample;
    unsigned int iChannel;
    unsigned int nBytes;
    float sample;

    nBytes = 3;

    for (iSample = 0; iSample < obj->hopSize; iSample++) {

        for (iChannel = 0; iChannel < obj->nChannels; iChannel++) {

            memcpy(&(obj->bytes[4 - nBytes]),
                   &(obj->buffer[(iSample * obj->nChannels + iChannel) * nBytes]),
                   sizeof(char) * nBytes);

            sample = pcm_signedXXbits2normalized(obj->bytes, nBytes);

            obj->out->hops->array[iChannel][iSample] = sample;
            
        }
        
    }
    
}

void src_hops_process_format_binary_int32(src_hops_obj * obj) {

    unsigned int iSample;
    unsigned int iChannel;
    unsigned int nBytes;
    float sample;

    nBytes = 4;

    for (iSample = 0; iSample < obj->hopSize; iSample++) {

        for (iChannel = 0; iChannel < obj->nChannels; iChannel++) {

            memcpy(&(obj->bytes[4 - nBytes]),
                   &(obj->buffer[(iSample * obj->nChannels + iChannel) * nBytes]),
                   sizeof(char) * nBytes);

            sample = pcm_signedXXbits2normalized(obj->bytes, nBytes);

            obj->out->hops->array[iChannel][iSample] = sample;
        
        }
    
    }

}

src_hops_cfg * src_hops_cfg_construct(void) {

    src_hops_cfg * cfg;

    cfg = (src_hops_cfg *) malloc(sizeof(src_hops_cfg));

    cfg->format = (format_obj *) NULL;
    cfg->interface = (interface_obj *) NULL;

    return cfg;
}


void src_hops_cfg_destroy(src_hops_cfg * src_hops_config) {

    if (src_hops_config->format != NULL) {
        format_destroy(src_hops_config->format);
    }
    if (src_hops_config->interface != NULL) {
        interface_destroy(src_hops_config->interface);
    }
    if (src_hops_config->channel_map != NULL) {
        free((void *) src_hops_config->channel_map);
    }

    free((void *) src_hops_config);

}

// ============================================================================
// WASAPI Implementation for Windows
// ============================================================================

#ifdef _WIN32

// Define INITGUID before including headers to get GUID definitions
#ifndef INITGUID
#define INITGUID
#endif

#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <functiondiscoverykeys_devpkey.h>
#include <stdint.h>

// Windows headers define 'interface' as a macro, which conflicts with our variable names
#ifdef interface
    #undef interface
#endif

// GUID definitions (need to be defined for MinGW)
static const CLSID CLSID_MMDeviceEnumerator_val = {0xbcde0395, 0xe52f, 0x467c, {0x8e,0x3d,0xc4,0x57,0x92,0x91,0x69,0x2e}};
static const IID IID_IMMDeviceEnumerator_val = {0xa95664d2, 0x9614, 0x4f35, {0xa7,0x46,0xde,0x8d,0xb6,0x36,0x17,0xe6}};
static const IID IID_IAudioClient_val = {0x1cb9ad4c, 0xdbfa, 0x4c32, {0xb1,0x78,0xc2,0xf5,0x68,0xa7,0x03,0xb2}};
static const IID IID_IAudioCaptureClient_val = {0xc8adbd64, 0xe71e, 0x48a0, {0xa4,0xde,0x18,0x5c,0x39,0x5c,0xd3,0x17}};

void src_hops_open_interface_wasapi(src_hops_obj * obj) {

    HRESULT hr;
    IMMDeviceEnumerator *enumerator = NULL;
    IMMDevice *device = NULL;
    IAudioClient *audio_client = NULL;
    IAudioCaptureClient *capture_client = NULL;
    WAVEFORMATEX *wave_format = NULL;
    HANDLE event_handle = NULL;

    // Initialize COM
    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr) && hr != RPC_E_CHANGED_MODE) {
        printf("Source hops WASAPI: Failed to initialize COM: 0x%08X\n", hr);
        exit(EXIT_FAILURE);
    }

    // Create device enumerator
    hr = CoCreateInstance(&CLSID_MMDeviceEnumerator_val, NULL, CLSCTX_ALL,
                         &IID_IMMDeviceEnumerator_val, (void**)&enumerator);
    if (FAILED(hr)) {
        printf("Source hops WASAPI: Failed to create device enumerator: 0x%08X\n", hr);
        CoUninitialize();
        exit(EXIT_FAILURE);
    }

    // Get audio device (use default if deviceName is NULL or empty)
    if (obj->interface->deviceName == NULL || strlen(obj->interface->deviceName) == 0) {
        hr = enumerator->lpVtbl->GetDefaultAudioEndpoint(enumerator, eCapture, eMultimedia, &device);
        if (FAILED(hr)) {
            printf("Source hops WASAPI: Failed to get default audio endpoint: 0x%08X\n", hr);
            enumerator->lpVtbl->Release(enumerator);
            CoUninitialize();
            exit(EXIT_FAILURE);
        }
        printf("Source hops WASAPI: Using default audio capture device\n");
    } else {
        // Search for device by name
        IMMDeviceCollection *devices = NULL;
        hr = enumerator->lpVtbl->EnumAudioEndpoints(enumerator, eCapture, DEVICE_STATE_ACTIVE, &devices);
        if (FAILED(hr)) {
            printf("Source hops WASAPI: Failed to enumerate audio endpoints: 0x%08X\n", hr);
            enumerator->lpVtbl->Release(enumerator);
            CoUninitialize();
            exit(EXIT_FAILURE);
        }

        UINT device_count = 0;
        devices->lpVtbl->GetCount(devices, &device_count);

        for (UINT i = 0; i < device_count; i++) {
            IMMDevice *temp_device = NULL;
            hr = devices->lpVtbl->Item(devices, i, &temp_device);
            if (SUCCEEDED(hr)) {
                IPropertyStore *props = NULL;
                hr = temp_device->lpVtbl->OpenPropertyStore(temp_device, STGM_READ, &props);
                if (SUCCEEDED(hr)) {
                    PROPVARIANT varName;
                    PropVariantInit(&varName);
                    hr = props->lpVtbl->GetValue(props, &PKEY_Device_FriendlyName, &varName);
                    if (SUCCEEDED(hr)) {
                        char device_name[256] = {0};
                        WideCharToMultiByte(CP_UTF8, 0, varName.pwszVal, -1, device_name, sizeof(device_name), NULL, NULL);

                        if (strstr(device_name, obj->interface->deviceName) != NULL) {
                            printf("Source hops WASAPI: Found device: %s\n", device_name);
                            device = temp_device;
                            temp_device->lpVtbl->AddRef(temp_device);
                        }
                        PropVariantClear(&varName);
                    }
                    props->lpVtbl->Release(props);
                }
                if (device == NULL) {
                    temp_device->lpVtbl->Release(temp_device);
                }
            }
            if (device != NULL) break;
        }
        devices->lpVtbl->Release(devices);

        if (device == NULL) {
            printf("Source hops WASAPI: Device '%s' not found, using default\n", obj->interface->deviceName);
            hr = enumerator->lpVtbl->GetDefaultAudioEndpoint(enumerator, eCapture, eMultimedia, &device);
            if (FAILED(hr)) {
                printf("Source hops WASAPI: Failed to get default audio endpoint: 0x%08X\n", hr);
                enumerator->lpVtbl->Release(enumerator);
                CoUninitialize();
                exit(EXIT_FAILURE);
            }
        }
    }

    // Activate audio client
    hr = device->lpVtbl->Activate(device, &IID_IAudioClient_val, CLSCTX_ALL, NULL, (void**)&audio_client);
    if (FAILED(hr)) {
        printf("Source hops WASAPI: Failed to activate audio client: 0x%08X\n", hr);
        device->lpVtbl->Release(device);
        enumerator->lpVtbl->Release(enumerator);
        CoUninitialize();
        exit(EXIT_FAILURE);
    }

    // Get mix format
    hr = audio_client->lpVtbl->GetMixFormat(audio_client, &wave_format);
    if (FAILED(hr)) {
        printf("Source hops WASAPI: Failed to get mix format: 0x%08X\n", hr);
        audio_client->lpVtbl->Release(audio_client);
        device->lpVtbl->Release(device);
        enumerator->lpVtbl->Release(enumerator);
        CoUninitialize();
        exit(EXIT_FAILURE);
    }

    printf("Source hops WASAPI: Device format: %d channels, %d Hz, %d bits\n",
           wave_format->nChannels, wave_format->nSamplesPerSec, wave_format->wBitsPerSample);

    // Check if format matches our requirements
    if (wave_format->nChannels != obj->nChannels || wave_format->nSamplesPerSec != obj->fS) {
        printf("Source hops WASAPI: Warning - Device format (%dch/%dHz) differs from requested (%dch/%dHz)\n",
               wave_format->nChannels, wave_format->nSamplesPerSec, obj->nChannels, obj->fS);
    }

    // Initialize audio client with event-driven mode
    REFERENCE_TIME buffer_duration = 30 * 10000; // 30ms
    hr = audio_client->lpVtbl->Initialize(audio_client, AUDCLNT_SHAREMODE_SHARED,
                                         AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
                                         buffer_duration, 0, wave_format, NULL);

    // Handle buffer alignment error
    if (hr == AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED) {
        UINT32 buffer_frames;
        hr = audio_client->lpVtbl->GetBufferSize(audio_client, &buffer_frames);
        if (SUCCEEDED(hr)) {
            audio_client->lpVtbl->Release(audio_client);
            hr = device->lpVtbl->Activate(device, &IID_IAudioClient_val, CLSCTX_ALL, NULL, (void**)&audio_client);
            if (SUCCEEDED(hr)) {
                buffer_duration = (REFERENCE_TIME)((10000.0 * 1000 * buffer_frames / wave_format->nSamplesPerSec) + 0.5);
                hr = audio_client->lpVtbl->Initialize(audio_client, AUDCLNT_SHAREMODE_SHARED,
                                                     AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
                                                     buffer_duration, 0, wave_format, NULL);
            }
        }
    }

    if (FAILED(hr)) {
        printf("Source hops WASAPI: Failed to initialize audio client: 0x%08X\n", hr);
        CoTaskMemFree(wave_format);
        audio_client->lpVtbl->Release(audio_client);
        device->lpVtbl->Release(device);
        enumerator->lpVtbl->Release(enumerator);
        CoUninitialize();
        exit(EXIT_FAILURE);
    }

    // Get capture client
    hr = audio_client->lpVtbl->GetService(audio_client, &IID_IAudioCaptureClient_val, (void**)&capture_client);
    if (FAILED(hr)) {
        printf("Source hops WASAPI: Failed to get capture client: 0x%08X\n", hr);
        CoTaskMemFree(wave_format);
        audio_client->lpVtbl->Release(audio_client);
        device->lpVtbl->Release(device);
        enumerator->lpVtbl->Release(enumerator);
        CoUninitialize();
        exit(EXIT_FAILURE);
    }

    // Create event for notifications
    event_handle = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (!event_handle) {
        printf("Source hops WASAPI: Failed to create event\n");
        capture_client->lpVtbl->Release(capture_client);
        CoTaskMemFree(wave_format);
        audio_client->lpVtbl->Release(audio_client);
        device->lpVtbl->Release(device);
        enumerator->lpVtbl->Release(enumerator);
        CoUninitialize();
        exit(EXIT_FAILURE);
    }

    // Set event handle
    hr = audio_client->lpVtbl->SetEventHandle(audio_client, event_handle);
    if (FAILED(hr)) {
        printf("Source hops WASAPI: Failed to set event handle: 0x%08X\n", hr);
        CloseHandle(event_handle);
        capture_client->lpVtbl->Release(capture_client);
        CoTaskMemFree(wave_format);
        audio_client->lpVtbl->Release(audio_client);
        device->lpVtbl->Release(device);
        enumerator->lpVtbl->Release(enumerator);
        CoUninitialize();
        exit(EXIT_FAILURE);
    }

    // Start capture
    hr = audio_client->lpVtbl->Start(audio_client);
    if (FAILED(hr)) {
        printf("Source hops WASAPI: Failed to start audio capture: 0x%08X\n", hr);
        CloseHandle(event_handle);
        capture_client->lpVtbl->Release(capture_client);
        CoTaskMemFree(wave_format);
        audio_client->lpVtbl->Release(audio_client);
        device->lpVtbl->Release(device);
        enumerator->lpVtbl->Release(enumerator);
        CoUninitialize();
        exit(EXIT_FAILURE);
    }

    // Allocate internal buffer for excess samples
    obj->internal_buffer_size = obj->hopSize * obj->nChannels * 4; // 4x buffer
    obj->internal_buffer = (float*)calloc(obj->internal_buffer_size, sizeof(float));
    obj->internal_buffer_pos = 0;

    // Store handles in object
    obj->audio_client = (void*)audio_client;
    obj->capture_client = (void*)capture_client;
    obj->wave_format = (void*)wave_format;
    obj->event_handle = (void*)event_handle;
    obj->mm_device = (void*)device;

    enumerator->lpVtbl->Release(enumerator);

    printf("Source hops WASAPI: Audio capture initialized and started successfully\n");
}

int src_hops_process_interface_wasapi(src_hops_obj * obj) {

    IAudioClient *audio_client = (IAudioClient*)obj->audio_client;
    IAudioCaptureClient *capture_client = (IAudioCaptureClient*)obj->capture_client;
    WAVEFORMATEX *wave_format = (WAVEFORMATEX*)obj->wave_format;
    HANDLE event_handle = (HANDLE)obj->event_handle;

    unsigned int samples_needed = obj->hopSize * obj->nChannels;
    unsigned int samples_collected = 0;

    // Temporary buffer for reading samples as floats
    float *temp_buffer = (float*)malloc(samples_needed * sizeof(float));
    if (!temp_buffer) {
        printf("Source hops WASAPI: Failed to allocate temporary buffer\n");
        return -1;
    }

    // Copy any leftover samples from internal buffer
    if (obj->internal_buffer_pos > 0) {
        unsigned int samples_to_copy = (samples_needed < obj->internal_buffer_pos) ?
                                       samples_needed : obj->internal_buffer_pos;
        memcpy(temp_buffer, obj->internal_buffer, samples_to_copy * sizeof(float));
        samples_collected = samples_to_copy;

        // Move remaining data to beginning of buffer
        if (obj->internal_buffer_pos > samples_to_copy) {
            memmove(obj->internal_buffer,
                   obj->internal_buffer + samples_to_copy,
                   (obj->internal_buffer_pos - samples_to_copy) * sizeof(float));
        }
        obj->internal_buffer_pos -= samples_to_copy;
    }

    // Read more data from WASAPI if needed
    while (samples_collected < samples_needed) {
        // Wait for audio data (with timeout)
        DWORD wait_result = WaitForSingleObject(event_handle, 100);
        if (wait_result != WAIT_OBJECT_0) {
            // Timeout - fill remaining with zeros
            for (unsigned int i = samples_collected; i < samples_needed; i++) {
                temp_buffer[i] = 0.0f;
            }
            break;
        }

        BYTE *data = NULL;
        UINT32 frames_available = 0;
        DWORD flags = 0;

        HRESULT hr = capture_client->lpVtbl->GetBuffer(capture_client, &data, &frames_available, &flags, NULL, NULL);

        if (hr == AUDCLNT_S_BUFFER_EMPTY) {
            continue;
        }

        if (FAILED(hr)) {
            if (hr == 0x88890004) {
                printf("Source hops WASAPI: Device disconnected (0x88890004)\n");
            } else if (hr == 0x88890001) {
                printf("Source hops WASAPI: Device not initialized (0x88890001)\n");
            } else {
                printf("Source hops WASAPI: Failed to get buffer: 0x%08X\n", hr);
            }
            free(temp_buffer);
            return -1;
        }

        if (frames_available == 0) {
            capture_client->lpVtbl->ReleaseBuffer(capture_client, 0);
            continue;
        }

        // Convert audio data based on format
        unsigned int samples_available = frames_available * wave_format->nChannels;
        unsigned int samples_to_copy = (samples_collected + samples_available <= samples_needed) ?
                                      samples_available : (samples_needed - samples_collected);

        // Handle different audio formats
        if (wave_format->wBitsPerSample == 32) {
            // 32-bit float or int
            if (wave_format->wFormatTag == WAVE_FORMAT_IEEE_FLOAT ||
                (wave_format->wFormatTag == WAVE_FORMAT_EXTENSIBLE &&
                 ((WAVEFORMATEXTENSIBLE*)wave_format)->SubFormat.Data1 == WAVE_FORMAT_IEEE_FLOAT)) {
                // 32-bit float - direct copy
                memcpy(temp_buffer + samples_collected, data, samples_to_copy * sizeof(float));
            } else {
                // 32-bit integer - convert to float
                int32_t *int_data = (int32_t*)data;
                for (unsigned int i = 0; i < samples_to_copy; i++) {
                    temp_buffer[samples_collected + i] = int_data[i] / 2147483648.0f;
                }
            }
        } else if (wave_format->wBitsPerSample == 16) {
            // 16-bit integer - convert to float
            int16_t *int_data = (int16_t*)data;
            for (unsigned int i = 0; i < samples_to_copy; i++) {
                temp_buffer[samples_collected + i] = int_data[i] / 32768.0f;
            }
        } else if (wave_format->wBitsPerSample == 24) {
            // 24-bit integer - convert to float
            uint8_t *byte_data = (uint8_t*)data;
            for (unsigned int i = 0; i < samples_to_copy; i++) {
                int32_t sample = (byte_data[i*3] << 8) | (byte_data[i*3+1] << 16) | (byte_data[i*3+2] << 24);
                temp_buffer[samples_collected + i] = sample / 2147483648.0f;
            }
        }

        samples_collected += samples_to_copy;

        // Store excess samples in internal buffer
        if (samples_available > samples_to_copy) {
            unsigned int excess = samples_available - samples_to_copy;
            if (obj->internal_buffer_pos + excess <= obj->internal_buffer_size) {
                // Convert and store excess data
                if (wave_format->wBitsPerSample == 32) {
                    if (wave_format->wFormatTag == WAVE_FORMAT_IEEE_FLOAT ||
                        (wave_format->wFormatTag == WAVE_FORMAT_EXTENSIBLE &&
                         ((WAVEFORMATEXTENSIBLE*)wave_format)->SubFormat.Data1 == WAVE_FORMAT_IEEE_FLOAT)) {
                        memcpy(obj->internal_buffer + obj->internal_buffer_pos,
                              ((float*)data) + samples_to_copy,
                              excess * sizeof(float));
                    } else {
                        int32_t *int_data = ((int32_t*)data) + samples_to_copy;
                        for (unsigned int i = 0; i < excess; i++) {
                            obj->internal_buffer[obj->internal_buffer_pos + i] = int_data[i] / 2147483648.0f;
                        }
                    }
                } else if (wave_format->wBitsPerSample == 16) {
                    int16_t *int_data = ((int16_t*)data) + samples_to_copy;
                    for (unsigned int i = 0; i < excess; i++) {
                        obj->internal_buffer[obj->internal_buffer_pos + i] = int_data[i] / 32768.0f;
                    }
                }
                obj->internal_buffer_pos += excess;
            }
        }

        capture_client->lpVtbl->ReleaseBuffer(capture_client, frames_available);

        if (samples_collected >= samples_needed) {
            break;
        }
    }

    // Convert float samples to buffer format expected by ODAS
    // The buffer stores samples in interleaved format as raw bytes
    for (unsigned int iSample = 0; iSample < obj->hopSize; iSample++) {
        for (unsigned int iChannel = 0; iChannel < obj->nChannels; iChannel++) {
            float sample = temp_buffer[iSample * obj->nChannels + iChannel];
            unsigned int buffer_idx = (iSample * obj->nChannels + iChannel);

            // Convert float (-1.0 to 1.0) to the format specified in config
            // Note: format_type enum values are the bit counts (8, 16, 24, 32)
            switch (obj->format->type) {
                case 8: {  // format_binary_int08
                    int8_t int_sample = (int8_t)(sample * 127.0f);
                    obj->buffer[buffer_idx] = int_sample;
                    break;
                }
                case 16: {  // format_binary_int16
                    int16_t int_sample = (int16_t)(sample * 32767.0f);
                    memcpy(&obj->buffer[buffer_idx * 2], &int_sample, 2);
                    break;
                }
                case 24: {  // format_binary_int24
                    int32_t int_sample = (int32_t)(sample * 8388607.0f);
                    // Store as 24-bit (3 bytes)
                    obj->buffer[buffer_idx * 3 + 0] = (int_sample >> 0) & 0xFF;
                    obj->buffer[buffer_idx * 3 + 1] = (int_sample >> 8) & 0xFF;
                    obj->buffer[buffer_idx * 3 + 2] = (int_sample >> 16) & 0xFF;
                    break;
                }
                case 32: {  // format_binary_int32
                    int32_t int_sample = (int32_t)(sample * 2147483647.0f);
                    memcpy(&obj->buffer[buffer_idx * 4], &int_sample, 4);
                    break;
                }
                default:
                    break;
            }
        }
    }

    free(temp_buffer);
    return 0;
}

void src_hops_close_interface_wasapi(src_hops_obj * obj) {

    IAudioClient *audio_client = (IAudioClient*)obj->audio_client;
    IAudioCaptureClient *capture_client = (IAudioCaptureClient*)obj->capture_client;
    WAVEFORMATEX *wave_format = (WAVEFORMATEX*)obj->wave_format;
    HANDLE event_handle = (HANDLE)obj->event_handle;
    IMMDevice *device = (IMMDevice*)obj->mm_device;

    if (audio_client) {
        audio_client->lpVtbl->Stop(audio_client);
        audio_client->lpVtbl->Release(audio_client);
    }

    if (capture_client) {
        capture_client->lpVtbl->Release(capture_client);
    }

    if (wave_format) {
        CoTaskMemFree(wave_format);
    }

    if (event_handle) {
        CloseHandle(event_handle);
    }

    if (device) {
        device->lpVtbl->Release(device);
    }

    if (obj->internal_buffer) {
        free(obj->internal_buffer);
        obj->internal_buffer = NULL;
    }

    CoUninitialize();

    printf("Source hops WASAPI: Audio capture closed\n");
}

#else

// Stub implementations for non-Windows platforms
void src_hops_open_interface_wasapi(src_hops_obj * obj) {
    printf("Source hops: WASAPI interface not supported on this platform\n");
    exit(EXIT_FAILURE);
}

int src_hops_process_interface_wasapi(src_hops_obj * obj) {
    printf("Source hops: WASAPI interface not supported on this platform\n");
    exit(EXIT_FAILURE);
    return -1;
}

void src_hops_close_interface_wasapi(src_hops_obj * obj) {
    printf("Source hops: WASAPI interface not supported on this platform\n");
    exit(EXIT_FAILURE);
}

#endif
