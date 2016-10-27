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

    Mat edge = gray >= edgeThresh, dist, labels, dist_0_1;

    distanceTransform( edge, dist, labels, CV_DIST_L1, CV_DIST_MASK_3, DIST_LABEL_PIXEL);
    // distanceTransform( edge, dist, labels, CV_DIST_L1, CV_DIST_MASK_3, DIST_LABEL_CCOMP);

    // get max and min values of distance
    double min, max;
    cv::minMaxLoc(dist, &min, &max);
    std::cout << "min: "<<min <<"\tmax:" <<max << "\n";
    

    std::cout << labels.size() << "\n";
    int idx, idy, jdx, jdy;
    int tmp;
    uint8_t *myData = labels.data;
    // extracted code
    std::vector<cv::Vec2i> label_to_index;
    label_to_index.push_back(-1);
    for (int row = 0; row < edge.rows; ++row)
        for (int col = 0; col < edge.cols; ++col)
            if(edge.at<uchar>(row,col)==0)
                label_to_index.push_back(cv::Vec2i(row,col));

    // check 20 values as sanity check that the detection of
    // the closest point is working
    srand (time(NULL));
    for (int i = 0; i < 20; i++) {
        idx = rand()%dist.cols;
        idy = rand()%dist.rows;
        tmp = labels.at<int>(idy, idx);

        std::cout << "("<<idx<<","<<idy <<":"<<
            (int)edge.at<uchar>(idy, idx)<<"|"<<dist.at<float>(idy, idx)<<")"
             <<"-"<< tmp <<"-"<< label_to_index[tmp]<< "\n";
    }

    // plot edges map
    imshow("Edges Map", edge );
    // plot normalized distance
    normalize(dist, dist_0_1, 0, 1., NORM_MINMAX);
    imshow("Distance Map", dist_0_1 );
    // wait
    waitKey(0);

    return 0;
}
