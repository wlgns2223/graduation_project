//
//  LaneDetection.hpp
//  opencv
//
//  Created by 심지훈 on 22/05/2019.
//  Copyright © 2019 Shim. All rights reserved.
//
#include <opencv2/opencv.hpp>


#ifndef LaneDetection_hpp
#define LaneDetection_hpp
using namespace cv;
using namespace std;

const int LEFT_LINE = 0;
const int RIGHT_LINE = 1;

void draw_houghLine(Mat src, Mat& dst, vector<Vec2f> left_lines, vector<Vec2f> right_lines);
void onMouse(int event, int x, int y, int flags, void* param);
void draw_trapezoid(Mat& dst, Point* pt, const Scalar& color );
void two_HoughLines(InputArray image, OutputArray left_lines ,OutputArray right_lines, double rho, double theta, int threshold);
Mat region_of_interest(Mat img, const Point* pt, int nPoints, const Scalar& color);


#endif /* LaneDetection_hpp */
