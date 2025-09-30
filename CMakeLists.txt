project(ODAS)
cmake_minimum_required(VERSION 2.4.6..3.16)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

option(ODAS_INSTALL_EXECUTABLES "Install the odaslive and odasserver executables along with the odas library" OFF)
option(ODAS_FORCE_BIN_AND_LIB_DIRS "Force the installation of the odas library and executables in the bin and lib directories" ON)
option(ODAS_DISABLE_INSTALL "Disable the installation of the odas library and executables" OFF)


find_package(PkgConfig REQUIRED)

# Handle FFTW dependency differently for cross-compilation
if(CMAKE_CROSSCOMPILING)
    # For cross-compilation, use our custom FFTW installation
    set(FFTW3F_INCLUDE_DIR "/mnt/c/Users/mail/dev/personal/teensy_ambisonic_microphone/odas/fftw-mingw-build/install/include")
    set(FFTW3F_LIBRARY "/mnt/c/Users/mail/dev/personal/teensy_ambisonic_microphone/odas/fftw-mingw-build/install/lib/libfftw3f.a")
    set(PC_FFTW3_INCLUDE_DIRS ${FFTW3F_INCLUDE_DIR})
    set(PC_FFTW3_LIBRARIES ${FFTW3F_LIBRARY})
else()
    # For native compilation, use pkg-config
    pkg_check_modules(PC_FFTW3 REQUIRED fftw3f)
endif()

# libconfig is only needed for demo executables
if(NOT CMAKE_CROSSCOMPILING)
    pkg_check_modules(PC_LIBCONFIG REQUIRED libconfig)
endif()

# Handle Windows vs Linux audio APIs
if(WIN32 OR CMAKE_CROSSCOMPILING)
    # Windows: Use Windows audio APIs
    set(PC_ALSA_FOUND FALSE)
    set(PC_PULSEAUDIO_FOUND FALSE)
    add_definitions(-D_WINDOWS -DWIN32)
    # For cross-compilation, avoid including Linux system headers
    include_directories(BEFORE SYSTEM)
else()
    # Linux: Use ALSA/PulseAudio
    pkg_check_modules(PC_ALSA REQUIRED alsa)
    pkg_check_modules(PC_PULSEAUDIO REQUIRED libpulse-simple)
endif()

include_directories("${PROJECT_SOURCE_DIR}/include")

# Add FFTW include directories from pkg-config
include_directories(${PC_FFTW3_INCLUDE_DIRS})

# Compile in release mode if not set
if(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
endif(NOT CMAKE_BUILD_TYPE)

if(ODAS_FORCE_BIN_AND_LIB_DIRS)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
endif()

#Add base directory for includes (global)
include_directories(include/odas)

set(SRC_BASE
    src/general/format.c
    src/general/interface.c
    src/general/link.c
    src/general/mic.c
    src/general/samplerate.c
    src/general/soundspeed.c
    src/general/spatialfilter.c
    src/general/thread.c

    src/init/combining.c
    src/init/delay.c
    src/init/directivity.c
    src/init/hit.c
    src/init/linking.c
    src/init/scanning.c
    src/init/space.c
    src/init/windowing.c

    src/signal/acorr.c
    src/signal/aimg.c
    src/signal/area.c
    src/signal/assignation.c
    src/signal/beampattern.c
    src/signal/category.c
    src/signal/coherence.c
    src/signal/delta.c
    src/signal/demixing.c
    src/signal/env.c
    src/signal/frame.c
    src/signal/freq.c
    src/signal/gain.c
    src/signal/hop.c
    src/signal/index.c
    src/signal/kalman.c
    src/signal/map.c
    src/signal/mask.c
    src/signal/mixture.c
    src/signal/pair.c
    src/signal/particle.c
    src/signal/pitch.c
    src/signal/point.c
    src/signal/postprob.c
    src/signal/pot.c
    src/signal/scan.c
    src/signal/spatialgain.c
    src/signal/spatialindex.c
    src/signal/spatialmask.c
    src/signal/steer.c
    src/signal/target.c
    src/signal/tau.c
    src/signal/tdoa.c
    src/signal/track.c
    src/signal/triangle.c
    src/signal/window.c
    src/signal/xcorr.c

    src/system/acorr2pitch.c
    src/system/demixing2env.c
    src/system/demixing2freq.c
    src/system/env2env.c
    src/system/hop2hop.c
    src/system/hop2frame.c
    src/system/frame2freq.c
    src/system/freq2acorr.c
    src/system/freq2env.c
    src/system/freq2freq.c
    src/system/freq2frame.c
    src/system/freq2xcorr.c
    src/system/frame2hop.c
    src/system/gain2mask.c
    src/system/kalman2coherence.c
    src/system/kalman2kalman.c
    src/system/mixture2mixture.c
    src/system/particle2coherence.c
    src/system/particle2particle.c
    src/system/pitch2category.c
    src/system/steer2demixing.c
    src/system/track2gain.c
    src/system/track2steer.c
    src/system/xcorr2aimg.c
    src/system/xcorr2xcorr.c

    src/utils/cmatrix.c
    src/utils/fft.c
    src/utils/fifo.c
    src/utils/gaussian.c
    src/utils/gaussjordan.c
    src/utils/inverse.c
    src/utils/matrix.c
    src/utils/pcm.c
    src/utils/random.c
    src/utils/transcendental.c

    src/aconnector/acon_categories.c
    src/aconnector/acon_hops.c
    src/aconnector/acon_pots.c
    src/aconnector/acon_powers.c
    src/aconnector/acon_spectra.c
    src/aconnector/acon_targets.c
    src/aconnector/acon_tracks.c
    src/ainjector/ainj_targets.c
    src/amessage/amsg_categories.c
    src/amessage/amsg_hops.c
    src/amessage/amsg_pots.c
    src/amessage/amsg_powers.c
    src/amessage/amsg_spectra.c
    src/amessage/amsg_targets.c
    src/amessage/amsg_tracks.c
    src/amodule/amod_classify.c
    src/amodule/amod_istft.c
    src/amodule/amod_mapping.c
    src/amodule/amod_noise.c
    src/amodule/amod_resample.c
    src/amodule/amod_ssl.c
    src/amodule/amod_sss.c
    src/amodule/amod_sst.c
    src/amodule/amod_stft.c
    src/amodule/amod_volume.c

    src/connector/con_categories.c
    src/connector/con_hops.c
    src/connector/con_pots.c
    src/connector/con_powers.c
    src/connector/con_spectra.c
    src/connector/con_targets.c
    src/connector/con_tracks.c
    src/injector/inj_targets.c
    src/message/msg_categories.c
    src/message/msg_hops.c
    src/message/msg_pots.c
    src/message/msg_powers.c
    src/message/msg_spectra.c
    src/message/msg_targets.c
    src/message/msg_tracks.c
    src/module/mod_classify.c
    src/module/mod_istft.c
    src/module/mod_mapping.c
    src/module/mod_noise.c
    src/module/mod_resample.c
    src/module/mod_ssl.c
    src/module/mod_sss.c
    src/module/mod_sst.c
    src/module/mod_stft.c
    src/module/mod_volume.c
)

# Sink and source files with socket support (now cross-platform)
set(SRC_SINK_SOURCE
    src/sink/snk_categories.c
    src/sink/snk_hops.c
    src/sink/snk_pots.c
    src/sink/snk_powers.c
    src/sink/snk_spectra.c
    src/sink/snk_tracks.c
    src/source/src_hops.c
    # Abstract sink/source files
    src/asink/asnk_categories.c
    src/asink/asnk_hops.c
    src/asink/asnk_pots.c
    src/asink/asnk_powers.c
    src/asink/asnk_spectra.c
    src/asink/asnk_tracks.c
    src/asource/asrc_hops.c
)

# Include all files on all platforms (socket code is now cross-platform)
set(SRC ${SRC_BASE} ${SRC_SINK_SOURCE})

add_library(odas SHARED
    ${SRC}
)

if(WIN32 OR CMAKE_CROSSCOMPILING)
    # Windows: Use Windows audio APIs with pthread and socket support
    target_link_libraries(odas
        ${PC_FFTW3_LIBRARIES}
        ${PC_LIBCONFIG_LIBRARIES}
        kernel32   # Windows Kernel32 API (Sleep, etc.)
        winmm      # Windows Multimedia API
        ws2_32     # Winsock2 for socket support
        ole32      # COM library for WASAPI
        uuid       # UUID library for WASAPI GUIDs
        propsys    # Property System library for PKEY_* symbols
        winpthread # MinGW pthread support (use winpthread for MinGW)
    )
    # Ensure all symbols are exported for DLL
    set_target_properties(odas PROPERTIES LINK_FLAGS "-Wl,--export-all-symbols")
    # Add library search paths for Windows cross-compilation
    target_link_directories(odas PRIVATE ${PC_FFTW3_LIBRARY_DIRS} ${PC_LIBCONFIG_LIBRARY_DIRS})
else()
    # Linux: Use ALSA/PulseAudio
    target_link_libraries(odas
        ${PC_FFTW3_LIBRARIES}
        ${PC_ALSA_LIBRARIES}
        ${PC_LIBCONFIG_LIBRARIES}
        ${PC_PULSEAUDIO_LIBRARIES}
        m
        pthread
    )
endif()

# Build demo executables (now supports Windows with winpthread)
if(NOT CMAKE_CROSSCOMPILING OR WIN32)
    add_executable(odaslive
        demo/odaslive/main.c
        demo/odaslive/configs.c
        demo/odaslive/objects.c
        demo/odaslive/parameters.c
        demo/odaslive/profiler.c
        demo/odaslive/threads.c
    )

    # Add local libconfig paths for MinGW cross-compilation
    if(WIN32 OR CMAKE_CROSSCOMPILING)
        target_include_directories(odaslive PRIVATE ${CMAKE_BINARY_DIR}/deps/include)
        target_link_libraries(odaslive
            ${CMAKE_BINARY_DIR}/libodas_fixed.dll.a
            ${CMAKE_BINARY_DIR}/deps/lib/libconfig.dll.a
        )
    else()
        target_link_libraries(odaslive
            odas
            ${PC_LIBCONFIG_LIBRARIES}
        )
    endif()

    add_executable(odasserver
        demo/odasserver/main.c
    )

    # Add local libconfig paths for MinGW cross-compilation
    if(WIN32 OR CMAKE_CROSSCOMPILING)
        target_include_directories(odasserver PRIVATE ${CMAKE_BINARY_DIR}/deps/include)
        target_link_libraries(odasserver
            ${CMAKE_BINARY_DIR}/libodas_fixed.dll.a
            ${CMAKE_BINARY_DIR}/deps/lib/libconfig.dll.a
        )
    else()
        target_link_libraries(odasserver
            odas
            ${PC_LIBCONFIG_LIBRARIES}
        )
    endif()
endif()

if (NOT ODAS_DISABLE_INSTALL)
    install(TARGETS odas LIBRARY DESTINATION lib)
    install(
        DIRECTORY include/
        DESTINATION include
        FILES_MATCHING
            PATTERN "*.h"
    )

    if(ODAS_INSTALL_EXECUTABLES)
        install(TARGETS odaslive DESTINATION bin)
        install(TARGETS odasserver DESTINATION bin)
    endif()
endif()


if(ODAS_FORCE_BIN_AND_LIB_DIRS)
    unset(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    unset(CMAKE_LIBRARY_OUTPUT_DIRECTORY)
endif()
