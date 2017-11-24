#ifndef _CANNY_EDGE_H_
#define _CANNY_EDGE_H_

#include "OtsuThreshold.h"

void DetectCannyEdge(const unsigned char *inImage, unsigned char *outImage, const int imageWidth, const int imageHeight)
{
	int lowThreshold = 0;
	int highThreshold = 0;

#if APPLY_ADAPTIVE_THRESHOLDING
	highThreshold = GetOtsuThreshold(inImage, imageWidth, imageHeight);
	lowThreshold = highThreshold>>1;

	#if MAKE_BINARY_IMAGE
		unsigned char *binaryImage = (unsigned char *)malloc(sizeof(unsigned char)*imageWidth*imageHeight);
		IplImage *iplBinaryImage = cvCreateImage(cvSize(imageWidth, imageHeight), IPL_DEPTH_8U, 1);
		MakeBinaryImage(inImage, binaryImage, imageWidth, imageHeight, highThreshold);
		ConvertBufferToIplImage(binaryImage, iplBinaryImage, imageWidth, imageHeight);
		cvSaveImage("Otsu.bmp", iplBinaryImage);
		free(binaryImage);
		cvReleaseImage(&iplBinaryImage);
	#endif

#else
	lowThreshold = HYSTERESIS_LOW_THRESHOLD;
	highThreshold = HYSTERESIS_HIGHT_THRESHOLD;
#endif

	printf("< Canny Edge >\n");
	printf("Hysteresis low T: %d, Hysteresis high T: %d\n", lowThreshold, highThreshold);

	register int i, j; 

	int dx, dy, magnitude, slope, direction; 
	int index, index2; 

	const int fbit = 10; 
	const int tan225 = 424;  
	const int tan675 = 2472; 

	const int maximaMagnitude = 255; 
	const int nonMaximaMagnitude = 100; 
	const int imageSize = imageWidth*imageHeight;

	bool checkMaxima; 

	int *magnitudeTable = (int*)malloc(imageSize*sizeof(int));
	int *dxTable = (int*)malloc(imageSize*sizeof(int));
	int *dyTable = (int*)malloc(imageSize*sizeof(int));

	unsigned char **stackTOP = (unsigned char**)malloc(imageWidth*imageHeight*sizeof(unsigned char*)); 
	unsigned char **stackBOTTOM = stackTOP; 

#define CANNY_PUSH(p)    *(p) = maximaMagnitude, *(stackTOP++) = (p) 
#define CANNY_POP()      *(--stackTOP) 

	//Detect Sobel Edge
	for(i=1; i<imageHeight-1; i++) 
	{ 
		index = i*imageWidth; 

		for(j=1; j<imageWidth-1; j++) 
		{ 
			index2 = index+j; 

			// kernel
			// -1 0 1 
			// -2 0 2 
			// -1 0 1 
			dx = inImage[index2-imageWidth+1] + (inImage[index2+1]<<1) + inImage[index2+imageWidth+1] 
			-inImage[index2-imageWidth-1] - (inImage[index2-1]<<1) - inImage[index2+imageWidth-1]; 

			// kernel 
			// -1 -2 -1 
			//  0  0  0 
			//  1  2  1
			dy = -inImage[index2-imageWidth-1] - (inImage[index2-imageWidth]<<1) - inImage[index2-imageWidth+1] 
			+inImage[index2+imageWidth-1] + (inImage[index2+imageWidth]<<1) + inImage[index2+imageWidth+1]; 

			magnitude = abs(dx)+abs(dy); 

			dxTable[index2] = dx; 
			dyTable[index2] = dy; 
			magnitudeTable[index2] = magnitude;             

		}   
	}   

	// Run NMS(Non-Maxima Suppression)
	for(i=1; i<imageHeight-1; i++) 
	{ 
		index = i*imageWidth; 

		for(j=1; j<imageWidth-1; j++) 
		{ 
			index2 = index+j; 

			magnitude = magnitudeTable[index2];         

			//Hysteresis thresholding
			if(magnitude > lowThreshold) 
			{ 
				//edge direction search(range: 0~3)
				dx = dxTable[index2]; 
				dy = dyTable[index2]; 

				if(dx != 0) 
				{ 
					slope = (dy<<fbit)/dx; 

					if(slope > 0) 
					{ 

						if(slope < tan225)
						{
							direction = 0; 
						}


						else if(slope < tan675)
						{
							direction = 1; 
						}


						else
						{
							direction = 2; 
						}

					} 

					else 
					{ 
						if(-slope > tan675)
						{
							direction = 2; 
						}

						else if(-slope > tan225)
						{
							direction = 3; 
						}


						else
						{
							direction = 0; 
						}
					}

				} 

				else
				{
					direction = 2; 
				}

				checkMaxima = true; 

				//Suppress Non-Maxima
				if(direction == 0) 
				{ 

					if(magnitude < magnitudeTable[index2-1] || magnitude < magnitudeTable[index2+1])
					{
						checkMaxima = false; 
					}

				} 

				else if(direction == 1) 
				{ 

					if(magnitude < magnitudeTable[index2+imageWidth+1] || magnitude < magnitudeTable[index2-imageWidth-1])
					{
						checkMaxima = false; 
					}

				} 

				else if(direction == 2)
				{ 

					if(magnitude < magnitudeTable[index2+imageWidth] || magnitude < magnitudeTable[index2-imageWidth])
					{
						checkMaxima = false;  
					}
				}

				else
				{  
					if(magnitude < magnitudeTable[index2+imageWidth-1] || magnitude < magnitudeTable[index2-imageWidth+1])
					{
						checkMaxima = false; 
					}

				} 

				if(checkMaxima) 
				{ 
					//hysteresis thresholding 
					if(magnitude > highThreshold) 
					{ 
						outImage[index2] = maximaMagnitude;            
						*(stackTOP++) = (unsigned char*)(outImage+index2); 
					} 

					else
					{
						outImage[index2] = nonMaximaMagnitude;             
					}

				} 
			} 

		} 
	} 

	//Edge Linking 
	while(stackTOP != stackBOTTOM) 
	{ 
		unsigned char* p = CANNY_POP(); 

		if(*(p+1) == nonMaximaMagnitude)
		{
			CANNY_PUSH(p+1);
		}

		if(*(p-1) == nonMaximaMagnitude)
		{
			CANNY_PUSH(p-1);
		}

		if(*(p+imageWidth) == nonMaximaMagnitude)
		{
			CANNY_PUSH(p+imageWidth);
		}

		if(*(p-imageWidth) == nonMaximaMagnitude)
		{
			CANNY_PUSH(p-imageWidth);
		}

		if(*(p-imageWidth-1) == nonMaximaMagnitude)
		{
			CANNY_PUSH(p-imageWidth-1);
		}

		if(*(p-imageWidth+1) == nonMaximaMagnitude)
		{
			CANNY_PUSH(p-imageWidth+1);
		}

		if(*(p+imageWidth-1) == nonMaximaMagnitude)
		{
			CANNY_PUSH(p+imageWidth-1);
		}

		if(*(p+imageWidth+1) == nonMaximaMagnitude)
		{
			CANNY_PUSH(p+imageWidth+1);
		}
	} 


	for(i=0; i<imageSize; i++)
	{
		if(outImage[i] != maximaMagnitude)
		{
			outImage[i] = 0;
		}
	}

	free(stackBOTTOM);

	free(dxTable);
	free(dyTable);
	free(magnitudeTable);

}

#endif
