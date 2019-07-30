//
//  main.cpp
//  opencv
//
//  Created by 심지훈 on 31/03/2019.
//  Copyright © 2019 Shim. All rights reserved.
//
#include <iostream>
#include <string>
#include "AdvancedLaneDetection.hpp"



void mouseOnClick(int event, int x, int y, int flags, void* userdata){
    if(event == EVENT_LBUTTONDOWN)
        cout<<"( "<<x<<", "<<y<<" )\n";
}

int main(int argc, const char * argv[])
{
    string filePath1  = "/Users/shimjihoon/Desktop/programming/study/opencv/sample/test_images/straight_lines1.jpg";
    string filePath2  = "/Users/shimjihoon/Desktop/programming/study/opencv/sample/test_images/straight_lines2.jpg";
    string curvePath1 = "/Users/shimjihoon/Desktop/programming/study/opencv/Advanced-Lane-Lines-master/test_images/test5.jpg";
    string curvePath2 =  "/Users/shimjihoon/Desktop/programming/study/opencv/Advanced-Lane-Lines-master/test_images/test4.jpg";
    
    
    AdvnacedLaneDetection lane = AdvnacedLaneDetection(curvePath1);
    Mat img = lane.getImg();
    Mat output;
    output = lane.sobelColorThresholding(img);
    
    imshow("output", output);
    waitKey();
    
    
    
    
    
    
//    Mat hlsImg;
//    Mat hls[3];
//    Mat sobelx;
//
//
//    cvtColor(img, hlsImg, COLOR_BGR2HLS_FULL);
//    split(hlsImg, hls);
//    Mat l_channel = hls[1];
//    Mat s_channel = hls[2];
//
//    // Sobel X Gradient Thresholding
//    Sobel(l_channel, sobelx, CV_64F, 1, 0);
//    convertScaleAbs(sobelx, sobelx);
//    Mat sxbinary = Mat::zeros(sobelx.size(), sobelx.type());
//    inRange(sobelx, Scalar(60), Scalar(180), sxbinary);
//
//
//    // Threshold Color Channel
//    Mat s_binary = Mat::zeros(s_channel.size(), s_channel.type());
//    inRange(s_channel, Scalar(170), Scalar(255), s_binary);
//
//    Mat temp = Mat::zeros(sxbinary.size(), sxbinary.type());
//    hls[0] = temp.clone();
//    hls[1] = sxbinary.clone();
//    hls[2] = s_binary.clone();
//
//
//
//
//    merge(hls, 3, hlsImg);
//
//    Mat combined = Mat::zeros(s_channel.size(), s_channel.type());
//
//    bitwise_or(sxbinary, s_binary, combined);
//
//    imshow("sxbinary", sxbinary);
//    imshow("s_binary", s_binary);
//    imshow("color", hlsImg);
//    imshow("merged", combined);

}
