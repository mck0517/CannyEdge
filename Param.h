#ifndef _PARAM_H_
#define _PARAM_H_

//If you want to apply blurring to the input image, enable APPLY_GAUSSIAN_BLUR to 1. 
#define APPLY_GAUSSIAN_BLUR   1

//Blurring Option: Choose 3x3 or 5x5 kernel for blurring.
//I use separable kernel for the fast processing speed. 
#if APPLY_GAUSSIAN_BLUR
	#define KERNEL_3_BY_3		0
	#define KERNEL_5_BY_5		1	
	#define KERNEL_SIZE			KERNEL_5_BY_5		
#endif

//Hysteresis threshold(low and high) for canny edge
//If you want to apply adaptive thresholding for hysteresis, enable APPLY_ADAPTIVE_THRESHOLDING to 1.
//High threshold is calculated by Otsu thresholding and low threshold is half value of high threshold.
#define APPLY_ADAPTIVE_THRESHOLDING   1

#if APPLY_ADAPTIVE_THRESHOLDING
#else 
	//Manual Setting 
	static const int HYSTERESIS_LOW_THRESHOLD = 50;
	static const int HYSTERESIS_HIGHT_THRESHOLD = 100;
#endif

#if APPLY_ADAPTIVE_THRESHOLDING
	//If you want to make binary image using otsu`s method, enable MAKE_BINARY_IMAGE to 1.
	#define MAKE_BINARY_IMAGE		1
#endif

//Input image size 
static const int IMAGE_WIDTH = 256;
static const int IMAGE_HEIGHT = 256;


#endif