#ifndef _UTIL_H_
#define _UTIL_H_

void ConvertIplImageToBuffer(IplImage *image, unsigned char *bufferImage)
{
	int imageWidth = image->width;
	int imageHeight = image->height;

	register int i, j;

	int index = 0;

	for(i=0; i<imageHeight; i++)
	{
		index = i*imageWidth;

		for(j=0; j<imageWidth; j++)
		{
			bufferImage[index+j] = (unsigned char)(image->imageData[i*image->widthStep+j]);
		}
	}

}

void ConvertBufferToIplImage(unsigned char *bufferImage, IplImage *image, int imageWidth, int imageHeight)
{
	register int i, j;

	for(i=0; i<imageHeight; i++)
	{
		for(j=0; j<imageWidth; j++)
		{
			image->imageData[i*image->widthStep+j] = bufferImage[i*imageWidth+j];
		}
	}

}

#endif