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

int main(int argc, const char * argv[])
{
    string filePath1  = "/Users/shimjihoon/Desktop/programming/study/opencv/sample/test_images/straight_lines1.jpg";
    string filePath2  = "/Users/shimjihoon/Desktop/programming/study/opencv/sample/test_images/straight_lines2.jpg";
    string curvePath1 = "/Users/shimjihoon/Desktop/programming/study/opencv/Advanced-Lane-Lines-master/test_images/test5.jpg";
    string curvePath2 =  "/Users/shimjihoon/Desktop/programming/study/opencv/Advanced-Lane-Lines-master/test_images/test4.jpg";
    
    string chessboard = "/Users/shimjihoon/Desktop/programming/study/opencv/Advanced-Lane-Lines-master/camera_cal/calibration";
    string xmlPath = "/Users/shimjihoon/Desktop/programming/study/opencv/opencv/";
    
    Calibration cal;
    cal.cameraCalibration(chessboard);
    cal.saveCameraMatrix(xmlPath);
    cal.loadCameraMatrix(xmlPath, "cameraMatrix.xml");
    
    Mat img = imread(filePath1);
    Mat un = cal.getUndistortedImg(img);
    imshow("img", img);
    imshow("un", un);
    waitKey();
    
    
    
}

//AdvnacedLaneDetection lane = AdvnacedLaneDetection(filePath1);
//Mat img = lane.getImg();
//Mat output;
//
////Trapezoid_vertices coordinates
//vector<Point2f> srcPts = {
//    Point2f(100,350), Point2f(300,220) ,
//    Point2f(340, 220), Point2f(560,350)
//
//};
//
////Points For Warping
//vector<Point2f> destPts = {
//    Point2f(80, 350), Point2f(80,0),
//    Point2f(580, 0), Point2f(580, 350)
//};
//
//line(img, srcPts[0], srcPts[1], Scalar(0,0,255));
//line(img, srcPts[2], srcPts[3], Scalar(0,0,255));
//
//Mat transMat = getPerspectiveTransform(srcPts, destPts);
//warpPerspective(img, output, transMat, img.size());
//
//imshow("img", img);
//imshow("output", output);
