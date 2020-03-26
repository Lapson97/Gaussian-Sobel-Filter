#include<iostream>
#include<opencv2/opencv.hpp>
#include <math.h>

#include "Operations.h"

using namespace std;
using namespace cv;

int main()
{
	//load a colourful image for Gaussian filter
	Mat src = imread("ptak.jpeg", 1);
	Mat dst(src.size(), CV_8UC3);
	Mat ROIImage(src.size(), CV_8UC3);

	//load a gray image for Gaussian filter
	Mat src1 = imread("ptak.jpeg", 0);
	Mat dst1(src1.size(), CV_8UC1);
	Mat ROIImage1(src1.size(), CV_8UC1);

	//load a gray image for Sobel filter
	Mat srcSobel = imread("ptak.jpeg", 0);
	Mat dstSobel(srcSobel.size(), CV_8UC1);

	
	//in case of error
	if (!src.data)
	{
		return -1;
	}

	if (!src1.data)
	{
		return -1;
	}

	if (!srcSobel.data)
	{
		return -1;
	}

	//may be put in by the user, 3.0 is set as a default value
	double sigma = 3.0;

	//function where the user decides about the type of image processing
	chooseOperation(srcSobel, dstSobel);

	//userKernelSize();
	int kernelSize = userKernelSize();

	//creating a new 2d dynamic array
	float** kernelArray1 = new float* [kernelSize];
	for (int i = 0; i < kernelSize; i++)
		kernelArray1[i] = new float[kernelSize];
	
	//function where the user decides the whole action of processing the image
	decideAction(kernelArray1, sigma, kernelSize, src, dst, src1, dst1, ROIImage, ROIImage1);


	waitKey();

	return 0;
}