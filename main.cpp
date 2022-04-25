#include <iostream>                                                     // for standard I/O
#include <string>   // for strings
#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat)
#include <chrono>
#include <opencv2/videoio.hpp>  // Video write
// #include <stdlib.h>
// #include "signalHandler.hpp"
#include<signal.h>

using namespace std;
using namespace chrono;
using namespace cv;


auto start = high_resolution_clock::now();
void SignalHandler(int SIG)
{
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    throw (duration.count());
}

int main(int argc, char *argv[])
{
    VideoCapture camera(0);                                                             // Open input
    if (!camera.isOpened())
    {
        cout  << "Could not open the camera\n";
        return -1;
    }

    cout << "Choose recording resolution\n0\tDefault (i.e., 640 x 480)\n1\t240p (i.e., 320 x 240)\n"
    << "2\t480p (i.e., 720 x 480)\n3\t720p (i.e., 1280 x 720)\n" ;
    int choice ;
    cin >> choice ;

    switch(choice)
    {
        case 1 :
                camera.set(CAP_PROP_FRAME_WIDTH, 320);                                                 //Setting the width of the video
                camera.set(CAP_PROP_FRAME_HEIGHT, 240);                                                //Setting the height of the video
                break ;
        case 2 :
                camera.set(CAP_PROP_FRAME_WIDTH, 720);                                                 //Setting the width of the video
                camera.set(CAP_PROP_FRAME_HEIGHT, 480);                                                //Setting the height of the video
                break ;
        case 3 :
                camera.set(CAP_PROP_FRAME_WIDTH, 1280);                                                 //Setting the width of the video
                camera.set(CAP_PROP_FRAME_HEIGHT, 720);                                                //Setting the height of the video
                break ;
        // default :
    // camera.set(CAP_PROP_FRAME_WIDTH, 1920);                                                 //Setting the width of the video
    // camera.set(CAP_PROP_FRAME_HEIGHT, 1080);                                                //Setting the height of the video
        //         break ;
    }
    
    const string NAME = "camera_record.avi";                                            // Form the new name with container
    int ex = static_cast<int>(camera.get(CAP_PROP_FOURCC));                             // Get Codec Type- Int form
 
    char EXT[] = {(char)(ex & 0XFF), (char)((ex & 0XFF00) >> 8),                        // Transform from int to char via Bitwise operators
                  (char)((ex & 0XFF0000) >> 16), (char)((ex & 0XFF000000) >> 24), 0};
    Size S = Size((int) camera.get(CAP_PROP_FRAME_WIDTH),                               // Acquire input size
                  (int) camera.get(CAP_PROP_FRAME_HEIGHT));
    
    VideoWriter outputVideo;                                                            // Open the output
    outputVideo.open(NAME, ex, camera.get(CAP_PROP_FPS), S, true);

    if (!outputVideo.isOpened())
    {
        cout  << "Could not open backfile : " << NAME << endl;
        return -1;
    }
    cout << "Input frame resolution: Width=" << S.width << "  Height=" << S.height
         << " with " << camera.get(CAP_PROP_FPS) << " FPS" << endl;
        //  << " of nr#: " << camera.get(CAP_PROP_FRAME_COUNT) 
    cout << "Input codec type: " << EXT << endl;

    Mat src, res;
    vector<Mat> spl;
    
    try
    {
        signal(SIGINT, SignalHandler);
        for(;;)
        {
            camera >> src;                                                              // read and decoding
            outputVideo.write(src);                                                     // writting

            // char c = (char)waitKey(25);                                                 //Allowing 25 milliseconds frame processing time and initiating break condition//
            // if (c == 27){                                                               //If 'Esc' is entered break the loop//
            //     break;
            // }
        }
    }
    catch (long time_duration)
    {
        cout << "Backup of last " << time_duration << " microseconds (i.e., "
        << time_duration / 1000000 << " seconds)"
        << " saved\n" ;
    }
    cout << "Video Recording Ended" << endl ;
    return 0;
}
