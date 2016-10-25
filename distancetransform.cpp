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
    // distanceTransform( edge, dist, labels, CV_DIST_L1, CV_DIST_MASK_3, DIST_LABEL_CCOMP);
    normalize(dist, dist, 0, 1., NORM_MINMAX);


    // std::cout << dist.at<int>(0,0) << "\n";
    // std::cout << labels.at<int>(0,0) << "\n";
    // std::cout << dist.at<int>(0,1) << "\n";
    // std::cout << labels.at<int>(0,1) << "\n";
    // std::cout << dist.rows << "," << dist.cols << "\n";
    // std::cout << labels.rows << "," << labels.cols << "\n";
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

    // for (int row = 0; row < edge.rows; ++row)
    //     std::cout << edge.at<int>(row,0) << "\n";

    // for (int i = 100000; i < 100100; i++) {
    for (int i = 99980; i < 100050; i++) {
        edge.data[i]=0; // mark what are we looking for

        idx = floor(i/dist.cols);
        idy = i%dist.cols;
        // edge.at<uchar>(idx,idy)=0; // mark what are we looking for

        tmp = labels.at<int>(idx,idy);
        // std::cout << "("<<idx<<","<<idy <<")" <<"-"<<labels.at<int>(idx,idy)<< "\n";
        std::cout << "("<<idx<<","<<idy <<":"<<
            (int)edge.at<uchar>(idx,idy)<<"|"<<dist.at<float>(idx,idy)<<")" <<"-"<<
            tmp<<"-"<< label_to_index[tmp]<< "\n";

        // jdx = floor(tmp/labels.cols);
        // jdy = tmp%labels.cols;
        // std::cout << "("<<idx<<","<<idy <<")" <<"-("<<jdx<<","<<jdy <<")"<< "\n";
        // edge.data[tmp]=1;
    }
    std::cout << "("<<0<<","<<0 <<")" <<"-"<<labels.at<int>(0,0)<<"-"<<label_to_index[labels.at<int>(0, 0)]<< "\n";
    std::cout << "("<<0<<","<<1 <<")" <<"-"<<labels.at<int>(0,1)<<"-"<<label_to_index[labels.at<int>(0, 1)]<< "\n";
    std::cout << "("<<1<<","<<0 <<")" <<"-"<<labels.at<int>(1,0)<<"-"<<label_to_index[labels.at<int>(1, 0)]<< "\n";


    imshow("Edges Map", edge );
    imshow("Distance Map", dist );
    // imshow("Who knows", labels );
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
