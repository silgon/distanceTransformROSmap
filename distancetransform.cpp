// based on https://github.com/opencv/opencv/blob/2.4/samples/cpp/distrans.cpp
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>
#include <iostream>

#include <algorithm>
#include <vector>

using namespace cv;

int edgeThresh = 240;

// The output and temporary images
Mat gray;


const char* keys =
{
    "{1| |stuff.jpg|input image file}"
};

int main( int argc, const char** argv )
{
    CommandLineParser parser(argc, argv, keys);
    string filename = parser.get<string>("1");
    gray = imread(filename.c_str(), 0);
    if(gray.empty())
    {
        printf("Cannot read image file: %s\n", filename.c_str());
        return -1;
    }

    Mat edge = gray >= edgeThresh, dist, labels, dist8u;
    // Mat edge = gray >= edgeThresh;


    distanceTransform( edge, dist, labels, CV_DIST_L1, CV_DIST_MASK_3);

    // begin "painting" the distance transform result
    dist *= 5000;
    pow(dist, 0.5, dist);

    Mat dist32s, dist8u1, dist8u2;

    dist.convertTo(dist32s, CV_32S, 1, 0.5);
    dist32s &= Scalar::all(255);

    dist32s.convertTo(dist8u1, CV_8U, 1, 0);
    dist32s *= -1;

    dist32s += Scalar::all(255);
    dist32s.convertTo(dist8u2, CV_8U);

    Mat planes[] = {dist8u1, dist8u1, dist8u1};
    merge(planes, 3, dist8u);

    imshow("Distance Map", dist8u1 );
    waitKey(0);

    return 0;
}
