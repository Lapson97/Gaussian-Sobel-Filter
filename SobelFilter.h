#pragma once

#define _SobelFilter_H_

#include <iostream>
#include<opencv2/opencv.hpp>

#include "GaussFilter.h"

using namespace std;
using namespace cv;

int xGradient(Mat src, int x, int y);
int yGradient(Mat src, int x, int y);
void performSobelFilter(Mat& src, Mat& dst);
