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

class AdvnacedLaneDetection {
    
private:
    Mat img;
    
    //Trapezoid_vertices coordinates
    const vector<Point2f> srcPts = {
        Point2f(80,350), Point2f(290,220) ,
        Point2f(350, 220), Point2f(580,350)
        
    };
    
    //Points For Warping
    const vector<Point2f> destPts = {
        Point2f(160, 350), Point2f(160,0),
        Point2f(500, 0), Point2f(500, 350)
    };

    
    const int sobelMinThreshold = 60;
    const int sobelMaxThreshold = 200;
    
    const int colorMinThreshold = 160;
    const int colorMaxThreshold = 235;
    
    Mat loadImgAndResize(String path, int flag = IMREAD_COLOR);
    Mat sobelThresholding(Mat input, string dir = "x");
    Mat colorThresholding(Mat input);
    
    
    
public:
    AdvnacedLaneDetection();
    AdvnacedLaneDetection(String path, int flag = IMREAD_COLOR);
    
    Mat getImg();
    Mat transformingToSkyview(Mat input);
    Mat sobelColorThresholding(Mat input);
    
    
};




#endif /* AdvancedLaneDetection_hpp */
