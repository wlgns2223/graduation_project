//
//  AdvancedLaneDetection.cpp
//  opencv
//
//  Created by 심지훈 on 17/07/2019.
//  Copyright © 2019 Shim. All rights reserved.
//

#include "AdvancedLaneDetection.hpp"

Mat AdvnacedLaneDetection::changeSize(Mat input) {
    if(input.size() != this->size)
        resize(input, input, this->size);
    
    return input;
}

Mat AdvnacedLaneDetection::loadImgAndResize(String path, int flag) {
    
    Mat temp = imread(path,flag);
    changeSize(temp);
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
    
    changeSize(input);
    
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
    
    changeSize(input);
    
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

void AdvnacedLaneDetection::drawCurveline(Mat input, vector<Point> leftPt, vector<Point> rightPt){
    vector<float> leftx, lefty, rightx, righty;
    for(auto _good_left : leftPt){
        leftx.push_back(_good_left.x);
        lefty.push_back(_good_left.y);
    }
    
    for(auto _good_right : rightPt){
        rightx.push_back(_good_right.x);
        righty.push_back(_good_right.y);
    }
    
    int leftRow = (int)leftx.size();
    Mat leftxMat(leftRow, 1 , CV_32FC1, leftx.data());
    Mat leftyMat(leftRow, 1, CV_32FC1, lefty.data());
    Mat leftDst(3,1, CV_32FC1 );
    polyfit(leftyMat, leftxMat, leftDst, 2);
    
    int rightRow = (int)rightx.size();
    Mat rightxMat(rightRow,1, CV_32FC1, rightx.data());
    Mat rightyMat(rightRow, 1, CV_32FC1, righty.data());
    Mat rightDst(3,1, CV_32FC1 );
    polyfit(rightyMat, rightxMat, rightDst, 2);

    
    float leftCoef[3] = {leftDst.at<float>(2,0), leftDst.at<float>(1,0), leftDst.at<float>(0,0)};
    float rightCoef[3] = {rightDst.at<float>(2,0), rightDst.at<float>(1, 0), rightDst.at<float>(0,0) };
    float leftFitx,rightFitx;
    vector<Point> leftFitPt, rightFitPt;
    for(int i=0; i<input.size().height; ++i){
        leftFitx = leftCoef[0] * i * i + leftCoef[1] * i + leftCoef[2];
        rightFitx = rightCoef[0] * i * i + rightCoef[1] * i + rightCoef[2];
        leftFitPt.push_back(Point(leftFitx, i));
        rightFitPt.push_back(Point(rightFitx, i));
    }
    polylines(input, leftFitPt, false, Scalar(0,255,255));
    polylines(input, rightFitPt, false, Scalar(0,255,255));
}

Mat AdvnacedLaneDetection::windowSearch(Mat input){
    
    Mat bottom_half_y = input.rowRange(input.size().height / 2, input.size().height );
    Mat hist;
    reduce(bottom_half_y, hist, 0, REDUCE_SUM, CV_32FC1);
    
    int midPoint = (int)hist.cols / 2;
    Point leftx_base, rightx_base;
    Mat bottomLeft = hist.colRange(0, midPoint);
    Mat bottomRight = hist.colRange(midPoint, hist.cols);
    
    minMaxLoc(bottomLeft, nullptr, nullptr, nullptr, &leftx_base);
    minMaxLoc(bottomRight, nullptr, nullptr, nullptr, &rightx_base);
    
    Point leftx_current = leftx_base;
    rightx_base.x += midPoint;
    Point rightx_current = rightx_base;
    
    vector<Point> good_left_idx, good_right_idx;
    Scalar left_mean, right_mean;
    Mat temp[3] = {input, input, input};
    Mat out;
    cv::merge(temp, 3, out);
    
    int windowHeight = (int)input.size().height / nwindows;
    vector<Point> nonzero;
    findNonZero(input, nonzero);
    
    for(int window = 0; window < this->nwindows; ++window){
        int win_y_low = input.size().height - ((window + 1 ) * windowHeight);
        int win_y_high = input.size().height - ( window * windowHeight);
        
        int win_xleft_low = leftx_current.x - margin;
        int win_xleft_high = leftx_current.x + margin;
        
        int win_xright_low = rightx_current.x - margin;
        int win_xright_high = rightx_current.x + margin;
        
        rectangle(out, Point(win_xleft_low, win_y_low), Point(win_xleft_high, win_y_high), Scalar(0,255,0));
        rectangle(out, Point(win_xright_low, win_y_low), Point(win_xright_high, win_y_high), Scalar(0,255,0));
        
        vector<Point>leftIdx, rightIdx;
        for(auto _nonzero : nonzero){
            if((_nonzero.y >= win_y_low) && (_nonzero.y < win_y_high) && (_nonzero.x >= win_xleft_low)
               && ( _nonzero.x < win_xleft_high)){
                leftIdx.push_back(_nonzero);
                good_left_idx.push_back(_nonzero);
            }
            
            if((_nonzero.y >= win_y_low) && (_nonzero.y < win_y_high) && (_nonzero.x >= win_xright_low)
               && ( _nonzero.x < win_xright_high)){
                rightIdx.push_back(_nonzero);
                good_right_idx.push_back(_nonzero);
            }
        }
        left_mean = cv::mean(leftIdx);
        right_mean = cv::mean(rightIdx);
        
        if( left_mean != Scalar(0,0,0,0))
            leftx_current = Point(left_mean[0], left_mean[1] );
        
        if( right_mean != Scalar(0,0,0,0) )
            rightx_current = Point(right_mean[0], right_mean[1]);
    }
    drawCurveline(out, good_left_idx, good_right_idx);
    
    return out;
}

void polyfit(const Mat& src_x, const Mat& src_y, Mat& dst, int order)
{
    CV_Assert((src_x.rows>0)&&(src_y.rows>0)&&(src_x.cols==1)&&(src_y.cols==1)
              &&(dst.cols==1)&&(dst.rows==(order+1))&&(order>=1));
    Mat X;
    X = Mat::zeros(src_x.rows, order+1,CV_32FC1);
    Mat copy;
    for(int i = 0; i <=order;i++)
    {
        copy = src_x.clone();
        pow(copy,i,copy);
        Mat M1 = X.col(i);
        copy.col(0).copyTo(M1);
    }
    Mat X_t, X_inv;
    transpose(X,X_t);
    Mat temp = X_t*X;
    Mat temp2;
    invert (temp,temp2);
    Mat temp3 = temp2 * X_t;
    Mat W = temp3 * src_y;
    W.copyTo(dst);
}

