#pragma once

#define _GaussFilter_H_

#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>


using namespace std;
using namespace cv;

int userKernelSize();
float** getKernelArray(int kernelWidth, int kernelHeight, double sigma);
void performColorGaussianFilter(float** kernelArray, int kernelWidth, int kernelHeight, Mat& src, Mat& dst, Mat& ROIImage);
void performGaussianFilter(float** kernelArray, int kernelWidth, int kernelHeight, Mat& src1, Mat& dst1, Mat& ROIImage1);
void deleteArray(float** kernelArray, int kernelWidth, int kernelHeight);
void deleteArray1(float** kernelArray, int kernelWidth, int kernelHeight);
void decideAction(float** kernelArray1, double sigma, int kernelSize, Mat& src, Mat& dst, Mat& src1, Mat& dst1, Mat& ROIImage, Mat& ROIImage1);
void chooseOperation(Mat& srcSobel, Mat& dstSobel);
int xGradient(Mat src, int x, int y);
int yGradient(Mat src, int x, int y);
void performSobelFilter(Mat& src, Mat& dst);