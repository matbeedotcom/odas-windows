#ifndef __ODAS_SOURCE_HOPS
#define __ODAS_SOURCE_HOPS

/**
 * \file     src_hops.h
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifndef _WIN32
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <alsa/asoundlib.h>
    #include <pulse/simple.h>
    #include <pulse/error.h>
#else
    #include <winsock2.h>
    #include <ws2tcpip.h>
    // Windows headers define 'interface' as a macro, which conflicts with our variable names
    #ifdef interface
        #undef interface
    #endif
#endif

#include "../general/format.h"
#include "../general/interface.h"
#include "../message/msg_hops.h"
#include "../signal/hop.h"
#include "../utils/pcm.h"

typedef struct src_hops_obj {

    unsigned long long timeStamp;

    unsigned int hopSize;
    unsigned int nChannels;
    unsigned int fS;

    format_obj *format;
    interface_obj *interface;

    FILE * fp;
    #ifndef _WIN32
        snd_pcm_t * ch;
        pa_simple * pa;
        pa_sample_spec ss;
        pa_channel_map cm;
    #else
        // WASAPI fields for Windows audio capture
        void * audio_client;      // IAudioClient*
        void * capture_client;    // IAudioCaptureClient*
        void * wave_format;       // WAVEFORMATEX*
        void * event_handle;      // HANDLE
        void * mm_device;         // IMMDevice*
        float * internal_buffer;  // Internal buffer for excess samples
        unsigned int internal_buffer_size;
        unsigned int internal_buffer_pos;
    #endif
    struct sockaddr_in sserver;
    #ifndef _WIN32
        int sid;
    #else
        SOCKET sid;
    #endif

    char *buffer;
    unsigned int bufferSize;

    char bytes[4];

    msg_hops_obj * out;

} src_hops_obj;

typedef struct src_hops_cfg {

    format_obj * format;
    interface_obj * interface;
    #ifndef _WIN32
        pa_channel_map * channel_map;
    #else
        void * channel_map; // Placeholder for Windows builds
    #endif

} src_hops_cfg;

src_hops_obj * src_hops_construct(const src_hops_cfg * src_hops_config, const msg_hops_cfg * msg_hops_config);

void src_hops_destroy(src_hops_obj * obj);

void src_hops_connect(src_hops_obj * obj, msg_hops_obj * out);

void src_hops_disconnect(src_hops_obj * obj);

void src_hops_open(src_hops_obj * obj);

void src_hops_open_interface_file(src_hops_obj * obj);

void src_hops_open_interface_soundcard(src_hops_obj * obj);

void src_hops_open_interface_pulseaudio(src_hops_obj * obj);

void src_hops_open_interface_socket(src_hops_obj * obj);

void src_hops_close(src_hops_obj * obj);

void src_hops_close_interface_file(src_hops_obj * obj);

void src_hops_close_interface_soundcard(src_hops_obj * obj);

void src_hops_close_interface_pulseaudio(src_hops_obj * obj);

void src_hops_close_interface_socket(src_hops_obj * obj);

int src_hops_process(src_hops_obj * obj);

int src_hops_process_interface_file(src_hops_obj * obj);

int src_hops_process_interface_soundcard(src_hops_obj * obj);

int src_hops_process_interface_socket(src_hops_obj * obj);

int src_hops_process_interface_pulseaudio(src_hops_obj * obj);

void src_hops_process_format_binary_int08(src_hops_obj * obj);

void src_hops_process_format_binary_int16(src_hops_obj * obj);

void src_hops_process_format_binary_int24(src_hops_obj * obj);

void src_hops_process_format_binary_int32(src_hops_obj * obj);

src_hops_cfg * src_hops_cfg_construct(void);

void src_hops_cfg_destroy(src_hops_cfg * src_hops_config);

#endif
