cmake_minimum_required(VERSION 3.4.1)
project(RnWavToFlac)

set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 14)

add_library(rn-wav-to-flac            SHARED
            ../cpp/src/libFLAC/include/private/all.h
            ../cpp/src/libFLAC/include/private/bitmath.h
            ../cpp/src/libFLAC/include/private/bitreader.h
            ../cpp/src/libFLAC/include/private/bitwriter.h
            ../cpp/src/libFLAC/include/private/cpu.h
            ../cpp/src/libFLAC/include/private/crc.h
            ../cpp/src/libFLAC/include/private/fixed.h
            ../cpp/src/libFLAC/include/private/flac_float.h
            ../cpp/src/libFLAC/include/private/format.h
            ../cpp/src/libFLAC/include/private/lpc.h
            ../cpp/src/libFLAC/include/private/macros.h
            ../cpp/src/libFLAC/include/private/md5.h
            ../cpp/src/libFLAC/include/private/memory.h
            ../cpp/src/libFLAC/include/private/metadata.h
            ../cpp/src/libFLAC/include/private/ogg_mapping.h
            ../cpp/src/libFLAC/include/private/stream_encoder_framing.h
            ../cpp/src/libFLAC/include/private/stream_encoder.h
            ../cpp/src/libFLAC/include/private/window.h
            ../cpp/src/libFLAC/include/protected/all.h
            ../cpp/src/libFLAC/include/protected/stream_decoder.h
            ../cpp/src/libFLAC/include/protected/stream_encoder.h
            ../cpp/src/libFLAC/all.h
            ../cpp/src/libFLAC/bitmath.c
            ../cpp/src/libFLAC/bitreader.c
            ../cpp/src/libFLAC/bitwriter.c
            ../cpp/src/libFLAC/cpu.c
            ../cpp/src/libFLAC/crc.c
            ../cpp/src/libFLAC/fixed_intrin_sse2.c
            ../cpp/src/libFLAC/fixed_intrin_ssse3.c
            ../cpp/src/libFLAC/fixed.c
            ../cpp/src/libFLAC/float.c
            ../cpp/src/libFLAC/format.c
            ../cpp/src/libFLAC/lpc_intrin_avx2.c
            ../cpp/src/libFLAC/lpc_intrin_sse.c
            ../cpp/src/libFLAC/lpc_intrin_sse2.c
            ../cpp/src/libFLAC/lpc_intrin_sse41.c
            ../cpp/src/libFLAC/lpc_intrin_vsx.c
            ../cpp/src/libFLAC/lpc.c
            ../cpp/src/libFLAC/md5.c
            ../cpp/src/libFLAC/memory.c 
            ../cpp/src/libFLAC/metadata_iterators.c 
            ../cpp/src/libFLAC/metadata_object.c
            ../cpp/src/libFLAC/ogg_mapping.c
            ../cpp/src/libFLAC/stream_decoder.c 
            ../cpp/src/libFLAC/stream_encoder_framing.c
            ../cpp/src/libFLAC/stream_encoder_intrin_avx2.c
            ../cpp/src/libFLAC/stream_encoder_intrin_sse2.c
            ../cpp/src/libFLAC/stream_encoder_intrin_ssse3.c
            ../cpp/src/libFLAC/stream_encoder.c
            ../cpp/src/libFLAC/window.c
            # libFLAC++
            ../cpp/src/libFLAC++/metadata.cpp
            ../cpp/src/libFLAC++/stream_decoder.cpp
            ../cpp/src/libFLAC++/stream_encoder.cpp
            # share/
            ../cpp/src/share/grabbag/alloc.c
            ../cpp/src/share/grabbag/cuesheet.c 
            ../cpp/src/share/grabbag/file.c 
            ../cpp/src/share/grabbag/picture.c 
            ../cpp/src/share/grabbag/replaygain.c 
            ../cpp/src/share/grabbag/seektable.c 
            ../cpp/src/share/grabbag/snprintf.c 
            ../cpp/src/share/replaygain_analysis/replaygain_analysis.c
            ../cpp/src/share/replaygain_synthesis/replaygain_synthesis.c
            # Other
            ../cpp/src/rn-wav-to-flac.cpp 
            ../cpp/src/WavMetadataReader.cpp 
            ../cpp/src/WavToFlacEncoder.cpp 
            ../cpp/src/WavToFlacFileConverter.cpp
            # Adapter
            cpp-adapter.cpp
)

# Specifies paths to native header files
include_directories(
            ../cpp/include
            ../cpp/include/FLAC
            ../cpp/include/FLAC++
            ../cpp/src/libFLAC/include
)
