// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>                                                     // for standard I/O
#include <string>   // for strings
#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat)
#include <opencv2/opencv.hpp>     // Basic OpenCV structures (cv::Mat)
#include <chrono>
#include <opencv2/videoio.hpp>  // Video write
#include<signal.h>
#include <thread>

using namespace std;
using namespace chrono;
using namespace cv;

#define PORT 8080
#define MAX_BUFF_SIZE 1024
bool stream_active = false ;

void start_stream(int choice)
{
    VideoCapture cap(0);
    // ./test-launch "( udpsrc port=5000 caps=\"application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264\" ! rtph264depay ! rtph264pay name=pay0 )"

    int Width , Height ;
    switch(choice)
    {
        case 1 :
                Width = 320 ;
                Height = 240 ;
                break ;
        case 2 :
                Width = 720 ;
                Height = 480 ;
                break ;
        case 3 :
                Width = 1280 ;
                Height = 720 ;
                break ;
    }

    cap.set(CAP_PROP_FRAME_WIDTH, Width);                                                 //Setting the width of the video
    cap.set(CAP_PROP_FRAME_HEIGHT, Height);                                                //Setting the height of the video

    int fps = cap.get(CAP_PROP_FPS);
    Size size(Width,Height);
    int ex = static_cast<int>(cap.get(CAP_PROP_FOURCC));                             // Get Codec Type- Int form

    // std::cout << getBuildInformation() << '\n' ;

    // VideoWriter out("appsrc ! videoconvert ! x264enc ! h264parse ! rtph264pay ! udpsink host=127.0.0.1 port=5000",CAP_GSTREAMER,ex,fps,size,true) ;
    VideoWriter out(
        "appsrc ! videoconvert ! x264enc tune=zerolatency bitrate=500 speed-preset=superfast ! rtph264pay ! udpsink host=127.0.0.1 port=5000",
        // "appsrc ! videoconvert ! x264enc speed-preset=veryfast tune=zerolatency bitrate=800 ! h264parse ! rtspclientsink location=rtsp://localhost:8554/test ",
    //     "tmp.mp4",
        // "appsrc ! videoconvert ! x264enc ! h264parse ! rtph264pay ! udpsink host=127.0.0.1 port=5000",
        // "appsrc ! videoconvert ! nvvideoconvert ! nvv4l2h264enc ! h264parse ! rtph264pay ! udpsink host=127.0.0.1 port=5000",
    ex,(double)fps,size,true);

    if(!cap.isOpened())
    {
        std::cout<<"VideoCapture not opened"<<std::endl;
        exit(-1);
    }

    if(!out.isOpened())
    {
        std::cout<<"VideoWriter not opened"<<std::endl;
        exit(-1);
    }


    Mat frame;
    while(stream_active) {
        cap.read(frame);
        resize(frame, frame, size , INTER_LINEAR);
        if(frame.empty())
            break;
        out.write(frame);
        waitKey(10) ;

    }
    cap.release() ;
}

void server_func(int socket)
{
	char buffer[MAX_BUFF_SIZE] ;
    // infinite loop for chat
    for (;;) {
        bzero(buffer, MAX_BUFF_SIZE);   
        // read the message from client and copy it in buffer
        read(socket, buffer, sizeof(buffer));
		int choice ;
		try {
			choice = stoi(buffer) ;
		}
		catch (...)
		{
			cout << "Client not available\n" ;
			return ;
		}

        cout << "Choice is " << choice << '\n' ;
		
        ::stream_active = true ;

        thread streaming_thread(start_stream , choice) ;

        strcpy(buffer , "rtsp://localhost:8554/test") ;
        write(socket, buffer, sizeof(buffer));
        while(::stream_active)
        {
            bzero(buffer, MAX_BUFF_SIZE);   
            read(socket, buffer, sizeof(buffer));
            if(strcasecmp(buffer , "exit") == 0)
                ::stream_active = false ;
        }

        streaming_thread.join() ;

	}
}


int main(int argc, char const* argv[])
{
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
			sizeof(address))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket
		= accept(server_fd, (struct sockaddr*)&address,
				(socklen_t*)&addrlen))
		< 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	printf("Server Active\n") ;
	server_func(new_socket) ;
	printf("Server Exit\n") ;
	return 0;
}
