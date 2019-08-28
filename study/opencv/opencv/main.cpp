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
#include "Calibration.hpp"



void mouseOnClick(int event, int x, int y, int flags, void* userdata){
    if(event == EVENT_LBUTTONDOWN)
        cout<<"( "<<x<<", "<<y<<" )\n";
}

void draw_histo(Mat hist, Mat &hist_img,int channel, Size size = Size(256,200)){
    hist_img = Mat(size, channel, Scalar(255));
    float bin = (float)hist_img.cols / hist.rows;
    normalize(hist, hist, 0 ,hist_img.rows, NORM_MINMAX);
    
    for(int i=0; i<hist.rows; ++i){
        float start_x = i * bin;
        float end_x = (i+1) * bin;
        Point2f pt1(start_x,0);
        Point2f pt2(end_x, hist.at<float>(i));
        
        if(pt2.y > 0)
            rectangle(hist_img,pt1, pt2, Scalar(0), -1);
    }
    
    flip(hist_img, hist_img, 0);
}

int main(int argc, const char * argv[])
{
    string filePath1  = "/Users/shimjihoon/Desktop/programming/study/opencv/sample/test_images/straight_lines1.jpg";
    string filePath2  = "/Users/shimjihoon/Desktop/programming/study/opencv/sample/test_images/straight_lines2.jpg";
    string filePath3 = "/Users/shimjihoon/Desktop/programming/study/opencv/Advanced-Lane-Lines-master/test_images/test2.jpg";
    string filePath4 = "/Users/shimjihoon/Desktop/programming/study/opencv/Advanced-Lane-Lines-master/test_images/test3.jpg";
    
    
    string curvePath1 = "/Users/shimjihoon/Desktop/programming/study/opencv/Advanced-Lane-Lines-master/test_images/test5.jpg";
    string curvePath2 =  "/Users/shimjihoon/Desktop/programming/study/opencv/Advanced-Lane-Lines-master/test_images/test4.jpg";
    
    string chessboard = "/Users/shimjihoon/Desktop/programming/study/opencv/Advanced-Lane-Lines-master/camera_cal/calibration";
    string xmlPath = "/Users/shimjihoon/Desktop/programming/study/opencv/opencv/";
    
    Mat img = imread(filePath4);
    AdvnacedLaneDetection detector;
    Calibration cali;
    cali.loadCameraMatrix(xmlPath, "cameraMatrix.xml");
    Mat undis = cali.getUndistortedImg(img);
    Mat trans = detector.transformingToSkyview(undis);
    Mat sobel = detector.sobelColorThresholding(trans);
    Mat curve = detector.windowSearch(sobel);
    
    imshow("test", curve);
    waitKey();


}
