//
//  main.cpp
//  opencv
//
//  Created by 심지훈 on 31/03/2019.
//  Copyright © 2019 Shim. All rights reserved.
//

/*
 
1. Gaussian Blurring
2. Binary Threshhold
3. Edge Detection
4. Hough Transformation
 
 
 
 
 
 */

#include <iostream>
#include <string>
#include "LaneDetection.hpp" // Lane Detection functions


int main(int argc, const char * argv[])
{
    
    //800 x 450 size , Image load
    string carPath = "/Users/shimjihoon/Desktop/programming/study/opencv/sample/porsche.jpg";
    string testPath = "/Users/shimjihoon/Desktop/programming/study/opencv/sample/test.png";
    string videoPath = "/Users/shimjihoon/Desktop/programming/study/opencv/sample/solidWhiteRight.mp4";
    Mat image = imread(testPath,IMREAD_COLOR);
    CV_Assert(image.data);
    
    Mat gray_image;
    Mat gaussian_img;
    Mat binary_img;
    Mat canny_img;
    Mat hough_img;
    Mat roi;
    vector<Vec4i> lines;
    vector<Point> line_pt;
    vector< vector<Vec4i>> left_right_lines;
    
    Point roi_pts[4] = {
        Point(119,339), // left bottom
        Point(254,235), // left top
        Point(390,235), // right top
        Point(593,339) // right bottom
    };
    
    
    cvtColor(image, gray_image, COLOR_BGR2GRAY);
    
    // 1. Gaussian blurring
    GaussianBlur(gray_image, gaussian_img, Size(3,3), 0);
    
    //2. Binary Image
    threshold(gaussian_img, binary_img, 140, 255, THRESH_BINARY);
    //draw_trapezoid(binary_img, roi_pts, Scalar(0,255,0));
    
    
    // Set Region of Interest
    roi = region_of_interest(binary_img, roi_pts, 4, Scalar(255,255,255));
    
    //3. Edge detection
    Canny(roi, canny_img, (roi.rows + roi.cols) / 4 , (roi.rows + roi.cols) / 2);
    
    
    // 4. Hough Transformation
    HoughLinesP(canny_img, lines, 1, CV_PI / 180 , 10);
    
    
    
    
    if(!lines.empty()) {
        
        
        left_right_lines = lineSeperator(lines, canny_img);
        line_pt = regression(left_right_lines, image);
        
    
        plotLane(image, line_pt);
        
    } else {
        cout<<"empty line"<<endl;
    }
    
    
//    imshow("image", image);
//    imshow("gray_image", gray_image);
//    imshow("gaussian_img", gaussian_img);
//    imshow("binary_img", binary_img);
//    imshow("roi", roi);
//    imshow("canny_img", canny_img);
//    imshow("hough", hough_img);

    
    
    
}

