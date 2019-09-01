//
//  AdvancedLaneDetection.hpp
//  opencv
//
//  Created by 심지훈 on 17/07/2019.
//  Copyright © 2019 Shim. All rights reserved.
//

#ifndef AdvancedLaneDetection_hpp
#define AdvancedLaneDetection_hpp
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

#define BIRDEYE_VIEW 0
#define NORMAL_VIEW  1

class AdvnacedLaneDetection {
    
private:
    Mat img;
    
    //Trapezoid_vertices coordinates
    const vector<Point2f> srcPts = {
        Point2f(130,340), Point2f(290,230) ,
        Point2f(355, 230), Point2f(530,340)
        
    };
    
    //Points For Warping
    const vector<Point2f> destPts = {
        Point2f(115, 340), Point2f(115,0),
        Point2f(545, 0), Point2f(545, 340)
    };
    
    vector<Point> leftFitPt, rightFitPt;
    vector<Point> leftFit_windowLine1,leftFit_windowLine2;
    vector<Point> rightFit_windowLine1, rightFit_windowLine2;
    
    Size size = Size(640, 360);
    
    Scalar GREEN = Scalar(0,255,0);
    Scalar BLUE = Scalar(255,0,0);
    Scalar RED = Scalar(0,0,255);
    Scalar YELLOW = Scalar(0,255,255);

    
    const int sobelMinThreshold = 35;
    const int sobelMaxThreshold = 220;
    
    const int colorMinThreshold = 100;
    const int colorMaxThreshold = 255;
    const int nwindows = 9;
    const int margin = 50;
    
    Mat loadImgAndResize(String path, int flag = IMREAD_COLOR);
    Mat sobelThresholding(Mat input, string dir = "x");
    Mat colorThresholding(Mat input);
    
    void changeSize(Mat input);
    void drawCurveline(Mat input, vector<Point> leftPt, vector<Point> rightPt);
    void drawWindowLine(Mat input);
    void clearFitPtVec();

public:
    AdvnacedLaneDetection();
    AdvnacedLaneDetection(String path, int flag = IMREAD_COLOR);
    
    Mat getImg();
    
    //Change view perspective from normal to bird eye view
    Mat transformingView(Mat input, const int flag);
    
    //apply sobel edge and thresholding
    Mat sobelColorThresholding(Mat input);
    
    //find out pixels(Point) in windows and draw left and right curve fitting to lanes
    Mat windowSearch(Mat input);
    
    Mat drawPolyArea(Mat input);
        
};

void polyfit(const Mat& src_x, const Mat& src_y, Mat& dst, int order);
#endif /* AdvancedLaneDetection_hpp */
