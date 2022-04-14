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

    docker run --device /dev/video0 -it opencv_cpp 

Note : flag -it in above command is for interactive shell (use as per your preference)

Copy essential files to docker container from git repo

    docker cp CMakeLists.txt  <container_id>:/CodeSpace/CMakeLists.txt
    docker cp main.cpp <container_id>:/CodeSpace/main.cpp

# Usage

Use following commands for making project binary

    cmake .
    make

Now, you can use CamSurveillance binary for execution.

    ./CamSurveillance

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