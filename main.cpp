#include <iostream>                                                                     // for standard I/O
#include <string>                                                                       // for strings
#include <opencv2/core.hpp>                                                             // Basic OpenCV structures (cv::Mat)
#include <chrono>
#include <opencv2/videoio.hpp>                                                          // Video write
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
         << " with " << camera.get(CAP_PROP_FPS) << " FPS" 
        //  << " of nr#: " << camera.get(CAP_PROP_FRAME_COUNT) 
         << endl;
    cout << "Input codec type: " << EXT << endl;

    Mat src, res;
    vector<Mat> spl;
    
    try
    {
        signal(SIGINT, SignalHandler);
        for(;;)
        {
            camera >> src;                                                              // read
            outputVideo.write(src);                                                     // writting
        }
    }
    catch (long time_duration)
    {
        cout << "Backup of " << time_duration << " microseconds saved\n" ;
    }
    cout << "Video Recording Ended" << endl ;
    return 0;
}