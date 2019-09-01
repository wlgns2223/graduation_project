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

string filePath1  = "/Users/shimjihoon/Desktop/programming/study/opencv/sample/test_images/straight_lines1.jpg";
string filePath2  = "/Users/shimjihoon/Desktop/programming/study/opencv/sample/test_images/straight_lines2.jpg";
string filePath3 = "/Users/shimjihoon/Desktop/programming/study/opencv/Advanced-Lane-Lines-master/test_images/test2.jpg";
string filePath4 = "/Users/shimjihoon/Desktop/programming/study/opencv/Advanced-Lane-Lines-master/test_images/test3.jpg";

string curvePath1 = "/Users/shimjihoon/Desktop/programming/study/opencv/Advanced-Lane-Lines-master/test_images/test5.jpg";
string curvePath2 =  "/Users/shimjihoon/Desktop/programming/study/opencv/Advanced-Lane-Lines-master/test_images/test4.jpg";

string chessboard = "/Users/shimjihoon/Desktop/programming/study/opencv/Advanced-Lane-Lines-master/camera_cal/calibration";

int main(int argc, const char * argv[])
{
    
    string xmlPath = "/Users/shimjihoon/Desktop/programming/study/opencv/opencv/";
    string videoPath = "/Users/shimjihoon/Desktop/programming/study/opencv/Advanced-Lane-Lines-master/project_video.mp4";
    
    VideoCapture capture(videoPath);
    CV_Assert(capture.isOpened());
    
    Size size = Size(640,360);
    Mat frame;
    
//    Mat img = imread(filePath4);
//    resize(img, img, Size(640,360));
    
    AdvnacedLaneDetection detector;
    Calibration cali;
    cali.loadCameraMatrix(xmlPath, "cameraMatrix.xml");
    
    double frame_rate = capture.get(CAP_PROP_FPS);
    int delay = 1000 / frame_rate;

    while(capture.read(frame)){
        
        if(waitKey(delay) >= 0) break;
        resize(frame, frame, size);
        cout<<"frame read\n";
        Mat undis = cali.getUndistortedImg(frame);
        Mat trans = detector.transformingView(undis, BIRDEYE_VIEW);
        Mat sobel = detector.sobelColorThresholding(trans);
        Mat curve = detector.windowSearch(sobel);
        Mat temp = detector.transformingView(curve, NORMAL_VIEW);
        Mat output = detector.drawPolyArea(frame);
        
        imshow("video", output);
    }
    
    
    
    
}
