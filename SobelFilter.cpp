#include "SobelFilter.h"


int xGradient(Mat src, int x, int y)
{
	return src.at<uchar>(y - 1, x - 1) + 2 * src.at<uchar>(y, x - 1) + src.at<uchar>(y + 1, x - 1) - src.at<uchar>(y - 1, x + 1) - 2 * src.at<uchar>(y, x + 1) - src.at<uchar>(y + 1, x + 1);
}

int yGradient(Mat src, int x, int y)
{
	return src.at<uchar>(y - 1, x - 1) + 2 * src.at<uchar>(y - 1, x) + src.at<uchar>(y - 1, x + 1) - src.at<uchar>(y + 1, x - 1) - 2 * src.at<uchar>(y + 1, x) - src.at<uchar>(y + 1, x + 1);
}

void performSobelFilter(Mat& src, Mat& dst)
{
	int sum = 0;
	int gx, gy;

	for (int y = 0; y < src.rows; y++)
	{ 
		for (int x = 0; x < src.cols; x++)
		{
			dst.at<uchar>(y, x) = 0.0;
		}
	}

	for (int y = 1; y < src.rows - 1; y++) 
	{
		for (int x = 1; x < src.cols - 1; x++) 
		{
			gx = xGradient(src, x, y);
			gy = yGradient(src, x, y);
			sum = abs(gx) + abs(gy);
			if (sum > 255)
			{
				sum = 255;
			}
			else if (sum < 0)
			{
				sum = 0;
			}
			dst.at<uchar>(y, x) = sum;
		}
	}
}