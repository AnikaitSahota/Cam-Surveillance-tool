#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>                                                     // for standard I/O
#include <string>   // for strings
#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat)
// #include <opencv2/opencv.hpp>     // Basic OpenCV structures (cv::Mat)
#include <opencv2/highgui.hpp>
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

// auto start = high_resolution_clock::now();
// void SignalHandler(int SIG)
// {
//     auto stop = high_resolution_clock::now();
//     auto duration = duration_cast<microseconds>(stop - start);
//     throw (duration.count());
// }


void download_stream(String rtsp_link )// , string NAME = "client_camera_recording.avi")
{
    VideoCapture rtsp_cap(rtsp_link);                                                             // Open input
    if (!rtsp_cap.isOpened())
    {
        cout  << "Could not open the stream\n";
        return ;
    }

    const string NAME = "client_camera_record.avi";                                            // Form the new name with container
    int ex = static_cast<int>(rtsp_cap.get(CAP_PROP_FOURCC));                             // Get Codec Type- Int form
 
    char EXT[] = {(char)(ex & 0XFF), (char)((ex & 0XFF00) >> 8),                        // Transform from int to char via Bitwise operators
                  (char)((ex & 0XFF0000) >> 16), (char)((ex & 0XFF000000) >> 24), 0};
    Size S = Size((int) rtsp_cap.get(CAP_PROP_FRAME_WIDTH),                               // Acquire input size
                  (int) rtsp_cap.get(CAP_PROP_FRAME_HEIGHT));
    
    VideoWriter outputVideo ;                                                            // Open the output
    outputVideo.open(NAME, ex, rtsp_cap.get(CAP_PROP_FPS), S, true);

    if (!outputVideo.isOpened())
    {
        cout  << "Could not open backfile : " << NAME << endl;
        return ;
    }
    cout << "Input frame resolution: Width=" << S.width << "  Height=" << S.height
         << " with " << rtsp_cap.get(CAP_PROP_FPS) << " FPS" << endl;
        //  << " of nr#: " << rtsp_cap.get(CAP_PROP_FRAME_COUNT) 
    cout << "Input codec type: " << EXT << endl;

    Mat frame ;
    while(::stream_active)
    {
        rtsp_cap >> frame;                                                              // read and decoding
        if(frame.empty())   break;
        outputVideo.write(frame);                                                     // writting
    }
    cout << "Recording Ended\n" ;
}

void client_func(int sockfd)
{
	char buffer[MAX_BUFF_SIZE] = { 0 };
	int n;

    for (;;) {
        cout << "Choose recording resolution\n0\tDefault (i.e., 640 x 480)\n1\t240p (i.e., 320 x 240)\n"
            << "2\t480p (i.e., 720 x 480)\n3\t720p (i.e., 1280 x 720)\n" ;
        cout << "Enter the choice : " ;
        n = 0;
        bzero(buffer, sizeof(buffer));
        while ((buffer[n++] = getchar()) != '\n')
        write(sockfd, buffer, sizeof(buffer));

        bzero(buffer, MAX_BUFF_SIZE);   
        read(sockfd, buffer, sizeof(buffer));

        cout << "Stream at " << buffer << '\n' ;

        ::stream_active = true ;

        thread downloading_thread(download_stream , buffer) ;

        String command ;
        cout << "Enter \"exit\" to close stream\n" ;
        while(::stream_active)
        {
            // n = 0;
            // while ((buffer[n++] = getchar()) != '\n')
            cin >> command ;
            if(strcasecmp(command.c_str() , "exit") == 0)
                ::stream_active = false ;
        }
        // cout << "Exit detected\n" ;
        strcpy(buffer , command.c_str()) ;
        write(sockfd, buffer, sizeof(buffer));

        downloading_thread.join() ;
    }
}

int main(int argc, char const* argv[])
{
	int sock = 0 ;
	struct sockaddr_in serv_addr;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
	client_func(sock) ;
	return 0;
}
