//
//  main.cpp
//  opencv
//
//  Created by 심지훈 on 31/03/2019.
//  Copyright © 2019 Shim. All rights reserved.
//

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;



void draw_houghLine(Mat src, Mat& dst, vector<Vec2f> left_line, vector<Vec2f> right_line)
{
    cvtColor(src, dst, COLOR_GRAY2BGR);
    
    float left_rho = left_line[0][0] ;
    float left_theta = left_line[0][1];
    
    float right_rho = right_line[0][0] ;
    float right_theta = right_line[0][1];
        
    double left_a = cos(left_theta), left_b = sin(left_theta);
    Point2d left_pt(left_a * left_rho, left_b * left_rho);
    Point2d left_delta(1000* (-left_b), 1000*left_a);
    
    double right_a = cos(right_theta), right_b = sin(right_theta);
    Point2d right_pt(right_a * right_rho, right_b * right_rho);
    Point2d right_delta(1000* (-right_b), 1000*right_a);
    
    line(dst, left_pt+left_delta, left_pt-left_delta, Scalar(0,255,0),1,LINE_AA);
    line(dst, right_pt+right_delta, right_pt-right_delta, Scalar(0,255,0),1,LINE_AA);
    
}

void onMouse(int event, int x, int y, int flags, void* param)
{
    switch(event)
    {
        case EVENT_LBUTTONDOWN:
            cout<<"(x, y ) = ("<<x<<","<<y<<")"<<endl;
            break;
    }
}

void draw_trapezoid(Mat& dst, Point* pt, const Scalar& color )
{
    cvtColor(dst, dst, COLOR_GRAY2BGR);
    line(dst, pt[0], pt[1], color);
    line(dst, pt[1], pt[2], color);
    line(dst, pt[2], pt[3], color);
    line(dst, pt[3], pt[0], color);
    
}

Mat region_of_interest(Mat img, const Point* pt, int nPoints, const Scalar& color)
{
    Mat mask(img.rows, img.cols, CV_8U,Scalar(0));
    fillConvexPoly(mask, pt, 4, color);
    return mask;
}


int main(int argc, const char * argv[])
{
    
    //800 x 450 size
    string carPath = "/Users/shimjihoon/Desktop/programming/study/opencv/sample/porsche.jpg";
    string testPath = "/Users/shimjihoon/Desktop/programming/study/opencv/sample/test.png";
    string videoPath = "/Users/shimjihoon/Desktop/programming/study/opencv/sample/solidWhiteRight.mp4";
    Mat image = imread(testPath,IMREAD_COLOR);
    CV_Assert(image.data);
    

    
    Range roi_range(image.rows/2, image.rows);
    Mat result;
    Mat roi;
    Mat mask(1,3,CV_32F);
    mask.at<float>(0,0) = -1;
    mask.at<float>(0,1) = 0;
    mask.at<float>(0,2) = 1;
    vector<Vec2f> left_line;
    vector<Vec2f> right_line;
    double rho = 1;
    
    //0 ~ 120 도
    double theta = CV_PI / 180;
    
    cv::GaussianBlur(image, result, Size(3,3), 0);
    
    cvtColor(result, result, COLOR_BGR2GRAY);
    cv::threshold(result, result, 140, 255, THRESH_BINARY);
    cv::filter2D(result, result, -1, mask, Point(-1,-1),0, BORDER_DEFAULT);
    result.rowRange(roi_range).copyTo(roi);
    
    cv::Canny(roi, roi, (roi.rows+ roi.cols) /4 , (roi.rows + roi.cols) /2);
    
    
    
    cv::HoughLines(roi, left_line, rho, theta, 10, 0 , 0 , 0  ,CV_PI / 2 );
    cv::HoughLines(roi, right_line, rho, theta, 10, 0 , 0 ,CV_PI /2  ,CV_PI );
    
    draw_houghLine(roi, roi, left_line, right_line);
    
    Point pt[4] = { Point(109,result.rows), Point(244,235), Point(390,235), Point(result.cols-10,result.rows) };
    
    draw_trapezoid(result , pt, Scalar(0,255,0) );
    namedWindow("image",WINDOW_AUTOSIZE);
    namedWindow("result",WINDOW_AUTOSIZE);
    
    Mat roi_mask = region_of_interest(result, pt, 4, Scalar(255,255,255));
    Mat real_roi;
    
    cout<<result.channels()<<","<<roi_mask.channels()<<endl;
//    bitwise_and(roi_mask, image, real_roi);
    /*
        Result가 3채널
        Roi_mask가 1채널
        bitwise연산이 불가능하다.
        Result의 채널을 바꾸도록
     
     */
    
    
    
    
    
    imshow("roi", roi);
    imshow("result", result);
    imshow("image",image);
    imshow("roi2", roi_mask);
    waitKey();
    
    
    
    
}

