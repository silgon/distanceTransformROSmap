// based on https://github.com/opencv/opencv/blob/2.4/samples/cpp/distrans.cpp
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>
#include <iostream>

#include <algorithm>
#include <vector>

using namespace cv;

int maskSize0 = CV_DIST_MASK_5;
int voronoiType = -1;
int edgeThresh = 240;
int distType0 = CV_DIST_L1;

// The output and temporary images
Mat gray;

std::vector<float> unique(const cv::Mat& input, bool sort = false)
{
    if (input.channels() > 1 || input.type() != CV_32F) 
    {
        std::cout<< "unique !!! Only works with CV_32F 1-channel Mat" << std::endl;
        return std::vector<float>();
    }

    std::vector<float> out;
    for (int y = 0; y < input.rows; ++y)
    {
        const float* row_ptr = input.ptr<float>(y);
        for (int x = 0; x < input.cols; ++x)
        {
            float value = row_ptr[x];

            if ( std::find(out.begin(), out.end(), value) == out.end() )
                out.push_back(value);
        }
    }

    if (sort)
        std::sort(out.begin(), out.end());

    return out;
}



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



    int maskSize = voronoiType >= 0 ? CV_DIST_MASK_5 : maskSize0;
    int distType = voronoiType >= 0 ? CV_DIST_L2 : distType0;

    Mat edge = gray >= edgeThresh, dist, labels, dist8u;
    // Mat edge = gray >= edgeThresh;


    distanceTransform( edge, dist, labels, distType, maskSize);
    // std::cout << dist << "\n";
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

    Mat planes[] = {dist8u1, dist8u2, dist8u2};
    merge(planes, 3, dist8u);

    imshow("Distance Map", dist8u );
    waitKey(0);

    return 0;
}
