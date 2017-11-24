// Vision_Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include "Param.h"
#include "Util.h"
#include "GaussianBlur.h"
#include "CannyEdge.h"

int _tmain(int argc, _TCHAR* argv[])
{
	IplImage *iplInputImage = cvLoadImage("lenna.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	const int imageWidth = iplInputImage->width;
	const int imageHeight = iplInputImage->height;
	assert(imageWidth == IMAGE_WIDTH || imageHeight == IMAGE_HEIGHT);

	unsigned char *inputImage = (unsigned char *)malloc(sizeof(unsigned char)*imageWidth*imageHeight);
	ConvertIplImageToBuffer(iplInputImage, inputImage);

	unsigned char *outputImage = (unsigned char *)malloc(sizeof(unsigned char)*imageWidth*imageHeight);
	IplImage *iplOutputImage = cvCreateImage(cvSize(imageWidth, imageHeight), IPL_DEPTH_8U, 1);

	//////////////////////////////////////////////////////////////////////////
	//Run Algorithm
#if APPLY_GAUSSIAN_BLUR
	unsigned char *blurImage = (unsigned char *)malloc(sizeof(unsigned char)*imageWidth*imageHeight);
	IplImage *ipBlurImage = cvCreateImage(cvSize(imageWidth, imageHeight), IPL_DEPTH_8U, 1);
	GaussianBlur(inputImage, blurImage, imageWidth, imageHeight, KERNEL_SIZE);
	ConvertBufferToIplImage(blurImage, ipBlurImage, imageWidth, imageHeight);
	cvSaveImage("BlurGaussian.bmp", ipBlurImage);
	DetectCannyEdge(blurImage, outputImage, imageWidth, imageHeight);
	free(blurImage);
	cvReleaseImage(&ipBlurImage);
#else
	DetectCannyEdge(inputImage, outputImage, imageWidth, imageHeight);
#endif

	//Save result image
	ConvertBufferToIplImage(outputImage, iplOutputImage, imageWidth, imageHeight);
	cvSaveImage("CannyEdge.bmp", iplOutputImage);
	//////////////////////////////////////////////////////////////////////////
	
	cvReleaseImage(&iplInputImage);
	cvReleaseImage(&iplOutputImage);
	free(inputImage);
	free(outputImage);

	printf("\n");
	printf("Press enter for exit!\n");

	getchar();

	return 0;
}

