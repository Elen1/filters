/*

 * To change this license header, choose License Headers in Project Properties.

 * To change this template file, choose Tools | Templates

 * and open the template in the editor.

 */

#include "opencv2/core/core.hpp"
#include  "opencv2/highgui.hpp"

#include <iostream>
#include <vector>
#include <cmath>

//#define filterWidth 3
//#define filterHeight 3

typedef std::vector<double> Array;
typedef std::vector<Array> Matrix;

Matrix getBoxBlur(const int filterWidth, const int filterHeight)
{

    Matrix kernel(filterHeight, Array(filterWidth));
    for(int i = 0; i < filterHeight; i++)
    {
        for(int j = 0; j < filterWidth; j++)
        {
            kernel[i][j] = (double)1/9;
        }
    }
    return kernel;
}
Matrix getGaussian(int height, int width, double sigma)
{
    Matrix kernel(height, Array(width));
    double sum=0.0;

    for (int y=0 ; y<height ; y++)
    {
        for (int x=0 ; x<width ; x++) 
        {
            kernel[y][x] = exp(-(y*y+x*x)/(2*sigma*sigma))/(2*M_PI*sigma*sigma);
            sum += kernel[y][x];
        }
    }

    for (int i=0 ; i<height ; i++)
    {
        for (int j=0 ; j<width ; j++) 
        {
            kernel[i][j] /= sum;
        }
    }

    return kernel;
}


cv::Mat applyFilter(cv::Mat image, Matrix filter)

{
    int height = image.rows;
    int width = image.cols;
    int filterHeight = filter.size();
    int filterWidth = filter[0].size();
    int newImageHeight = height-filterHeight+1;
    int newImageWidth = width-filterWidth+1;
    

    cv::Mat newImage(image.rows, image.cols, CV_8UC3, cv::Scalar(0,0,0));

    for (int i=0 ; i<newImageHeight ; i++)
    {
        for (int j=0 ; j<newImageWidth ; j++) 
        {
            for (int h=i ; h<i+filterHeight ; h++) 
            {
                for (int w=j ; w<j+filterWidth ; w++)
                {
                   newImage.at<uchar>(i,j) += filter[h-i][w-j]*image.at<uchar>(h,w);

                }
            }
        }
    }

    return newImage;
}
/*
cv::Mat getSharpening(cv::Mat image)
{
    //Matrix kernel(3, Array(3));
    std::vector<std::vector<double>> ker = { {0,-1,0},{-1,5,-1},{0,-1,0} };
    cv::Mat result = applyFilter(image,ker);
    return result;

}*/

cv::Mat getMedian(cv::Mat image)
{
    std::vector<int> v(9);
    cv::Mat dst(image.rows, image.cols, CV_8UC3, cv::Scalar(0,0,0));

    for(int y = 1; y < image.rows - 1; y++)
    {
        for(int x = 1; x < image.cols - 1; x++)
        {
            v[0] = image.at<uchar>(y - 1 ,x - 1);
            v[1] = image.at<uchar>(y, x - 1);
            v[2] = image.at<uchar>(y + 1, x - 1);
            v[3] = image.at<uchar>(y - 1, x);
            v[4] = image.at<uchar>(y, x);
            v[5] = image.at<uchar>(y + 1, x);
            v[6] = image.at<uchar>(y - 1, x + 1);
            v[7] = image.at<uchar>(y, x + 1);
            v[8] = image.at<uchar>(y + 1, x + 1);

            std::sort(v.begin(), v.end());
            dst.at<uchar>(y,x) = v[4];
            //dst.at<uchar>(y,x) = v[0]; # for min filter
            //dst.at<uchar>(y,x) = v[8]; # for max filter

        }
            
    }
}


int main()

{
	cv::Mat image = cv::imread("/home/elen/Pictures/car.png",cv::IMREAD_GRAYSCALE);
        if (!image.data)
        {
             std::cout << "Image not loaded";
            return -1;
        }

        
	cv::namedWindow("image_before", cv::WINDOW_NORMAL);
	cv::imshow("image_before", image);

	Matrix kernel = getBoxBlur(3,3);

        cv::Mat result = applyFilter(image,kernel);
        //cv::Mat result = getMedian(image);
        
        cv::namedWindow("blured", cv::WINDOW_NORMAL);
        cv::imshow("blured", result);
        
	cv::waitKey(0);

	return 0;

}







