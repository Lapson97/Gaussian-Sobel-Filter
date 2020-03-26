#include "Operations.h"

int userKernelSize()
{
	int kernelSize;
	cout << "Put in the value of the array you want to use to filter the image." << endl;
	cout << "The value must be indivisible by 2." << endl;

	cin >> kernelSize;

	while (kernelSize % 2 == 0)
	{
		cout << "Wrong input!" << endl;
		cout << "The value must be indivisible by 2." << endl;
		cin >> kernelSize;
	}

	return kernelSize;
}


//function which creates a 2d array, performs calculations of the values in the array and divides every value by the sum of all values
float** getKernelArray(/*float** kernelArray*/ int kernelWidth, int kernelHeight, double sigma)
{
	float** kernelArray = new float* [kernelWidth];
	for (int i = 0; i < kernelWidth; ++i)
		kernelArray[i] = new float[kernelHeight];

	const double PI = 4.0 * atan(1.0);

	float total = 0;

	//calculating kernel values inside the 2d array
	for (int row = 0; row < kernelWidth; row++)
	{
		for (int col = 0; col < kernelHeight; col++)
		{
			float value = (1 / (2 * PI * pow(sigma, 2))) * exp(-(pow(row - kernelWidth / 2, 2) + pow(col - kernelHeight / 2, 2)) / (2 * pow(sigma, 2)));

			kernelArray[row][col] = value;

			total += value;
		}
	}

	cout << "Coefficients array:" << endl;

	//calculating a value of the coefficients of a gaussian filter
	for (int row = 0; row < kernelWidth; row++)
	{
		for (int col = 0; col < kernelHeight; col++)
		{
			kernelArray[row][col] /= total;
			cout << kernelArray[row][col] << " ";
		}
		cout << endl;
	}

	return kernelArray;
}

//function which perform the calculations on specific pixels of the image multiplying them by the values stored in the kernelArray - function only for coloured images
void performColorGaussianFilter(float** kernelArray, int kernelWidth, int kernelHeight, Mat& src, Mat& dst, int xROIU, int xROID, int yROIL, int yROIR, Mat& ROIImage)
{
	dst = src.clone();
	int verticalImageBound = (kernelHeight - 1) / 2;
	int horizontalImageBound = (kernelWidth - 1) / 2;

	for (int row = 0 + verticalImageBound + xROIU; row < src.rows - verticalImageBound - xROID; row++)
	{
		for (int col = 0 + horizontalImageBound + yROIL; col < src.cols - horizontalImageBound - yROIR; col++)
		{
			float value1 = 0.0;
			float value2 = 0.0;
			float value3 = 0.0;

			for (int kRow = 0; kRow < kernelHeight; kRow++)
			{
				for (int kCol = 0; kCol < kernelWidth; kCol++)
				{
					float pixel1 = src.data[(kRow + row - verticalImageBound + xROIU) * src.step + (kCol + col - horizontalImageBound + yROIL) * src.elemSize() + 0] * kernelArray[kRow][kCol];
					float pixel2 = src.data[(kRow + row - verticalImageBound + xROIU) * src.step + (kCol + col - horizontalImageBound + yROIL) * src.elemSize() + 1] * kernelArray[kRow][kCol];
					float pixel3 = src.data[(kRow + row - verticalImageBound + xROIU) * src.step + (kCol + col - horizontalImageBound + yROIL) * src.elemSize() + 2] * kernelArray[kRow][kCol];

					value1 += pixel1;
					value2 += pixel2;
					value3 += pixel3;
				}
			}

			dst.data[(row + xROIU) * dst.step + (col + yROIL) * dst.elemSize() + 0] = value1;
			dst.data[(row + xROIU) * dst.step + (col + yROIL) * dst.elemSize() + 1] = value2;
			dst.data[(row + xROIU) * dst.step + (col + yROIL) * dst.elemSize() + 2] = value3;

			if (xROIU != 0 || xROID != 0 || yROIL != 0 || yROIR != 0)
			{
				ROIImage.data[(row + xROIU) * ROIImage.step + (col + yROIL) * ROIImage.elemSize() + 0] = value1;
				ROIImage.data[(row + xROIU) * ROIImage.step + (col + yROIL) * ROIImage.elemSize() + 1] = value2;
				ROIImage.data[(row + xROIU) * ROIImage.step + (col + yROIL) * ROIImage.elemSize() + 2] = value3;
			}
		}
	}
}

//function which perform the calculations on specific pixels of the image multiplying them by the values stored in the kernelArray - function only for gray coloured images
void performGaussianFilter(float** kernelArray, int kernelWidth, int kernelHeight, Mat& src1, Mat& dst1, int xROIU, int xROID, int yROIL, int yROIR, Mat &ROIImage1)
{
	dst1 = src1.clone();

	int verticalImageBound = (kernelHeight - 1) / 2;
	int horizontalImageBound = (kernelWidth - 1) / 2;

	for (int row = 0 + verticalImageBound + xROIU; row < src1.rows - verticalImageBound - xROID; row++)
	{
		for (int col = 0 + horizontalImageBound + yROIL; col < src1.cols - horizontalImageBound - yROIR; col++)
		{
			float value = 0.0;

			for (int kRow = 0; kRow < kernelHeight; kRow++)
			{
				for (int kCol = 0; kCol < kernelWidth; kCol++)
				{
					float pixel = src1.at<uchar>(kRow + row - verticalImageBound, kCol + col - horizontalImageBound) * kernelArray[kRow][kCol];
					value += pixel;
				}
			}

			dst1.at<uchar>(row, col) = value;
			ROIImage1.at<uchar>(row, col) = value;
		}
	}
}

//function to delete the dynamically created 2d array
void deleteArray(float** kernelArray, int kernelWidth, int kernelHeight)
{
	for (int row = 0; row < kernelWidth; row++)
	{
		delete[] kernelArray[row];
	}
	delete[]kernelArray;
}

void decideAction(float** kernelArray1, double sigma, int kernelSize, Mat& src, Mat& dst, Mat& src1, Mat& dst1, Mat& ROIImage, Mat& ROIImage1)
{
	cout << "Decide what image you want to process: " << endl;
	cout << "1. Colourful Image" << endl;
	cout << "2. Gray image" << endl;

	int userDecision;

	cin >> userDecision;

	switch (userDecision)
	{
	case 1:
		//operations performed on a colourful image
		kernelArray1 = getKernelArray(kernelSize, kernelSize, sigma);
		performColorGaussianFilter(kernelArray1, kernelSize, kernelSize, src, dst, 10, 110, 10, 110, ROIImage);
		deleteArray(kernelArray1, kernelSize, kernelSize);

		namedWindow("Original Image");
		imshow("Original Image", src);
		namedWindow("Filtered Image");
		imshow("Filtered Image", dst);
		namedWindow("ROI");
		imshow("ROI", ROIImage);

		break;

	case 2:
		//operations performed on a gray image
		kernelArray1 = getKernelArray(kernelSize, kernelSize, sigma);
		performGaussianFilter(kernelArray1, kernelSize, kernelSize, src1, dst1, 30, 120, 10, 150, ROIImage1);
		deleteArray(kernelArray1, kernelSize, kernelSize);

		namedWindow("Gray Original Image");
		imshow("Gray Original Image", src1);
		namedWindow("Gray Filtered Image");
		imshow("Gray Filtered Image", dst1);
		namedWindow("ROI");
		imshow("ROI", ROIImage1);

		break;

	default:
		cout << "Wrong input!" << endl;
		decideAction(kernelArray1, sigma, kernelSize, src, dst, src1, dst1, ROIImage, ROIImage1);
	}
}

//Sobel kernels are defined - for the x direction: [-1 0 1; -2 0 2; -1 0 1] and for the y direction: [-1 -2 -1; 0 0 0; 1 2 1]
void performSobelFilter(Mat& src, Mat& dst)
{
	int magnitude = 0;
	int gx, gy;

	for (int x = 0; x < src.rows; x++)
	{
		for (int y = 0; y < src.cols; y++)
		{
			dst.at<uchar>(x, y) = 0.0;
		}
	}

	for (int x = 1; x < src.rows - 1; x++)
	{
		for (int y = 1; y < src.cols - 1; y++)
		{
			gx = src.at<uchar>(x - 1, y + 1) + 2 * src.at<uchar>(x, y + 1) + src.at<uchar>(x + 1, y + 1) - src.at<uchar>(x - 1, y - 1) - 2 * src.at<uchar>(x, y - 1) - src.at<uchar>(x + 1, y - 1);
			gy = src.at<uchar>(x + 1, y - 1) + 2 * src.at<uchar>(x + 1, y) + src.at<uchar>(x + 1, y + 1) - src.at<uchar>(x - 1, y - 1) - 2 * src.at<uchar>(x - 1, y) - src.at<uchar>(x - 1, y + 1);

			magnitude = abs(gx) + abs(gy);

			if (magnitude > 255)
			{
				magnitude = 255;
			}

			dst.at<uchar>(x, y) = magnitude;
		}
	}
}

void chooseOperation(Mat& srcSobel, Mat& dstSobel)
{
	int userDecision;

	cout << "Welcome to the Gaussian/Sobel filtering programme!" << endl;
	cout << "Choose the operation you want to perform." << endl;
	cout << "1. Gaussian filtering" << endl;
	cout << "2. Sobel filtering" << endl;

	cin >> userDecision;

	switch (userDecision)
	{
	case 1:
		break;

	case 2:
		performSobelFilter(srcSobel, dstSobel);

		namedWindow("Original Image");
		imshow("Original Image", srcSobel);

		namedWindow("Filtered Image");
		imshow("Filtered Image", dstSobel);
		waitKey(0);
		break;

	default:
		cout << "Wrong input!" << endl;
		chooseOperation(srcSobel, dstSobel);
	}
}