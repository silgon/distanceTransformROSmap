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

    // Mat edge = gray >= edgeThresh, dist, labels, dist8u;
    Mat edge = gray >= edgeThresh, dist, labels;

    // distanceTransform( edge, dist, labels, CV_DIST_L1, CV_DIST_MASK_3);
    distanceTransform( edge, dist, labels, CV_DIST_L1, CV_DIST_MASK_3, DIST_LABEL_PIXEL);
    normalize(dist, dist, 0, 1., NORM_MINMAX);


    std::cout << dist.at<int>(0,0) << "\n";
    std::cout << labels.at<int>(0,0) << "\n";
    std::cout << dist.at<int>(0,1) << "\n";
    std::cout << labels.at<int>(0,1) << "\n";


    imshow("Edges Map", edge );
    imshow("Distance Map", dist );
    // imshow("Labels Map", label8u );
    // imshow("voronoi Map", labels*100 );
    // vector<Mat> channel;
    // split(labels, channel);
    // imshow("x", channel[0] );
    // imshow("y", channel[1] );
    // imshow("z", channel[2] );
    waitKey(0);

    return 0;
}
