FROM ubuntu:bionic

LABEL maintainer="Anikait <anikait18016@iiitd.ac.in>"

WORKDIR /CodeSpace

RUN apt-get update --fix-missing -y

# RUN apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev \
#     libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base libgstrtspserver-1.0-dev \
#     gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-rtsp\
#     gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc \
#     gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl \
#     gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio -y

# RUN apt-get install build-essential cmake git libgtk2.0-dev pkg-config \
#     libavcodec-dev libavformat-dev libswscale-dev -y && \
#     git clone https://github.com/opencv/opencv.git && \
#     cd opencv && mkdir build && cd build && \
#     cmake -D WITH_GSTREAMER=ON -D CMAKE_BUILD_TYPE=Release -D BUILD_SHARED_LIBS=NO .. && \
#     cat /proc/cpuinfo | grep "processor" | wc -l | xargs make -j && \
#     make install && cd ../.. && \
#     rm -rf opencv

