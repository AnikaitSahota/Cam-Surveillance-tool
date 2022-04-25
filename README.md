# Cam-Surveillance-tool
Various featurs for CCTV to hold


# Installation

To use this project, first clone this git repository using following commands

    git clone https://github.com/AnikaitSahota/Cam-Surveillance-tool.git

Then comes the, installation of dependences
<!-- I have use following dependences
- opencv
- cpp -->
for installing all the dependencies, use requirement file with following command

    docker build -t opencv_cpp .

# Environment Setup

To create environment for application, use following commands

    docker run --device /dev/video0 -it --net=host --name cam_surv opencv_cpp 

Note : flag -it in above command is for interactive shell (use as per your preference)

Copy essential files to docker container from git repo

    docker cp <filename>  <container_id>:/CodeSpace/<filename>

    filename can be main.cpp, client.cpp, server.cpp, installation.sh

# Usage

Install the dependencies

    bash installation.sh

Use following commands for making project binary

    cmake .
    make

For running in centralized setting (i.e., without server-client)

    ./centralized

For server-client architecture

Use pre-build binary to host rtsp server

    ./gst-rtsp-server-1.4.0/examples/test-launch "( udpsrc port=5000 caps=\"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264\" ! rtph264depay ! rtph264pay name=pay0 )

Then server and client binary for their respective usage

    ./server
    ./client

# Authors

- [AnikaitSahota](https://github.com/AnikaitSahota)

See also the list of [contributors](https://github.com/AnikaitSahota/Cam-Surveillance-tool/contributors) who participated in this project.

# License

This project is licensed under the MIT License - see the [LICENSE.md](https://github.com/AnikaitSahota/Cam-Surveillance-tool/blob/master/LICENSE) file for details

# Footer
- If you want to contribute, fork the repository for yourself. Check out [Fork list](https://github.com/AnikaitSahota/Cam-Surveillance-tool/network/members)
- If you liked the project, leave a star. Check out [stargazers](https://github.com/AnikaitSahota/Cam-Surveillance-tool/stargazers)




<!-- TODO : 
  * delete the unrequired file
    * All images in main directory (like final.png, test.png)
    * Report doc file -->