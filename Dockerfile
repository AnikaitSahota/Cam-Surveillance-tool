FROM ubuntu:bionic

LABEL maintainer="Anikait <anikait18016@iiitd.ac.in>"

WORKDIR /CodeSpace

RUN apt-get update -y

RUN apt-get install build-essential cmake git libgtk2.0-dev pkg-config \
    libavcodec-dev libavformat-dev libswscale-dev -y && \
    git clone https://github.com/opencv/opencv.git && \
    cd opencv && mkdir build && cd build && \
    cmake -D CMAKE_BUILD_TYPE=Release -D BUILD_SHARED_LIBS=NO .. && \
    cat /proc/cpuinfo | grep "processor" | wc -l | xargs make -j && \
    make install && cd ../.. && \
    rm -rf opencv


