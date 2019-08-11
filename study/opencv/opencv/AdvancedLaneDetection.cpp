//
//  AdvancedLaneDetection.cpp
//  opencv
//
//  Created by 심지훈 on 17/07/2019.
//  Copyright © 2019 Shim. All rights reserved.
//

#include "AdvancedLaneDetection.hpp"



Mat AdvnacedLaneDetection::loadImgAndResize(String path, int flag) {
    
    Mat temp = imread(path,flag);
    resize(temp, temp, Size(640,360));
    return temp;
}

AdvnacedLaneDetection::AdvnacedLaneDetection(){
    
}

AdvnacedLaneDetection::AdvnacedLaneDetection(String path, int flag) {
    img = loadImgAndResize(path,flag);
}

Mat AdvnacedLaneDetection::getImg(){
    return img;
}


Mat AdvnacedLaneDetection::transformingToSkyview(Mat input) {
    
    Size warpSize(input.size());
    Mat output(warpSize, input.type());
    Mat transformationMatrix;
    
    // Get Transformation Matrix
    transformationMatrix = getPerspectiveTransform(srcPts, destPts);
    
    //Warping perspective
    warpPerspective(input, output, transformationMatrix, warpSize);
    
    return output;
}

Mat AdvnacedLaneDetection::sobelThresholding(Mat input, string dir){
    // Sobel Direction (x) and Thresholding
    
    
    // Params
    // Input : L Channel of HLS Img
    // Dir : Sobel Thresholding Direction . {X, Y}
    
    //Output
    //A Mat Img processed by Sobel direction thresholding between
    //range sobelMinThreshold to sobelMaxThreshold
    
    Mat l_channel, sobel;
    l_channel = input;
    Mat sxbinary = Mat::zeros(sobel.size(), sobel.type());
    
    if(dir == "x")
        Sobel(l_channel, sobel, CV_64F, 1, 0);
    if(dir == "y")
        Sobel(l_channel, sobel, CV_64F, 0, 1);
    
    // Sobel X Gradient Thresholding
    Sobel(l_channel, sobel, CV_64F, 1, 0);
    convertScaleAbs(sobel, sobel);
    inRange(sobel, Scalar(sobelMinThreshold), Scalar(sobelMaxThreshold), sxbinary);
    
    return sxbinary;
        
}

Mat AdvnacedLaneDetection::colorThresholding(Mat input){
    // Color Channel Thresholding
    
    //Param
    //Input : S_Channel of HLS Img.
    
    //Output
    //A Mat img processed by Color Thresholding Between
    //colorMinThreshold To colorMaxThreshold
    
    Mat s_binary = Mat::zeros(input.size(), input.type());
    inRange(input, Scalar(colorMinThreshold), Scalar(colorMaxThreshold), s_binary);
    return s_binary;
    
}

Mat AdvnacedLaneDetection::sobelColorThresholding(Mat input){
    // Bitwise operation of sobel threshold img and color threshold img
    
    // Param
    // input :  Mat BGR Color Img
    
    // output
    // A Mat img with bitwise operation of
    // sobel threshold img and color threshold img
    
    
    
    Mat hlsImg;
    Mat sxbinary;
    Mat s_binary;
    Mat output;
    Mat hls[3];
    cvtColor(input, hlsImg, COLOR_BGR2HLS_FULL);
    split(hlsImg, hls);
    
    Mat l_channel = hls[1].clone();
    Mat s_channel = hls[2].clone();
    
    sxbinary = sobelThresholding(l_channel);
    s_binary = colorThresholding(s_channel);
    
    bitwise_or(sxbinary, s_binary, output);
    
    return output;
    
}
