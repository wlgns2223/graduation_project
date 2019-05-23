//
//  LaneDetection.cpp
//  opencv
//
//  Created by 심지훈 on 22/05/2019.
//  Copyright © 2019 Shim. All rights reserved.
//

#include "LaneDetection.hpp"

void draw_houghLine(Mat src, Mat& dst, vector<Vec2f> left_lines, vector<Vec2f> right_lines)
{
    cvtColor(src, dst, COLOR_GRAY2BGR);
    float rho[2] = { left_lines[0][0], right_lines[0][0] };
    float theta[2] = { left_lines[0][1], right_lines[0][1] };
    
    double a[2] = { cos(theta[LEFT_LINE]), cos(theta[RIGHT_LINE])};
    double b[2] = { sin(theta[LEFT_LINE]), sin(theta[RIGHT_LINE])};
    
    Point2d pt[2] = {
        Point2d(a[LEFT_LINE] * rho[LEFT_LINE], b[LEFT_LINE] * rho[LEFT_LINE]),
        Point2d(a[RIGHT_LINE] * rho[RIGHT_LINE], b[RIGHT_LINE] * rho[RIGHT_LINE])
    };
    
    Point2d delta[2] = {
        Point2d( 1000 * (-b[LEFT_LINE]), 1000 * a[LEFT_LINE]),
        Point2d( 1000 * (-b[RIGHT_LINE]), 1000 * a[RIGHT_LINE])
    };
    
    
    cout<<left_lines[0]<<"  "<<left_lines[1]<<endl;
    
    
//    line(dst, pt[LEFT_LINE] + delta[LEFT_LINE], pt[LEFT_LINE] - delta[LEFT_LINE], Scalar(0,255,0),1 , LINE_AA);
//    line(dst, pt[RIGHT_LINE] + delta[RIGHT_LINE], pt[RIGHT_LINE] - delta[RIGHT_LINE], Scalar(0,255,0),1 , LINE_AA);
    
    
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
    Mat output;
    Mat mask(img.rows, img.cols, CV_8U,Scalar(0));
    fillConvexPoly(mask, pt, 4, color);
    bitwise_and(img, mask, output);
    return output;
}

void two_HoughLines(InputArray image, OutputArray left_lines, OutputArray right_lines,
                    double rho, double theta,
                    int threshold)
{

    
    HoughLines(image, left_lines, rho, theta, 10,0,0,0, CV_PI /2 );
    HoughLines(image, right_lines, rho, theta, 10,0,0,CV_PI /2 , CV_PI);
    
}
