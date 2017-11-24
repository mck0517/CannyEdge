#ifndef _GAUSSIAN_BLUR_H_
#define _GAUSSIAN_BLUR_H_

static int HX_CONVOLUTION[IMAGE_WIDTH*IMAGE_HEIGHT] = {0, };

void BlurUsing3by3Kernel(const unsigned char *inImage, unsigned char *outImage, const int imageWidth, const int imageHeight)
{
	memset(HX_CONVOLUTION, 0, sizeof(HX_CONVOLUTION));

	int hx[3] = {1, 2, 1};
	int hy[3] = {1, 2, 1};

	register int i, j, i2, j2;

	int index, index2, convolution;

	for(i=0; i<imageHeight; i++) 
	{
		index = i*imageWidth;

		for(j=0; j<imageWidth; j++) 
		{
			index2 = index+j;

			convolution = 0;

			for(j2=-1; j2<=1; j2++) 
			{

				if(j+j2 < 0 || j+j2 >= imageWidth)
				{
					continue;
				}

				convolution += hx[1+j2]*inImage[index2+j2];
			}

			HX_CONVOLUTION[index2] = convolution;
		}
	}

	for(i=0; i<imageHeight; i++) 
	{
		index = i*imageWidth;

		for(j=0; j<imageWidth; j++) 
		{
			index2 = index+j;

			convolution = 0;

			for(i2=-1; i2<=1; i2++) 
			{

				if(i+i2 < 0 || i+i2 >= imageHeight)
				{
					continue;
				}

				convolution += hy[i2+1]*HX_CONVOLUTION[index2+i2*imageWidth];
			}

			outImage[index2] = convolution>>4; // convolution/16;
		}
	}

}

void BlurUsing5by5Kernel(const unsigned char *inImage, unsigned char *outImage, const int imageWidth, const int imageHeight)
{

	memset(HX_CONVOLUTION, 0, sizeof(HX_CONVOLUTION));

	int hx[5] = {1, 3, 4, 3, 1};
	int hy[5] = {1, 3, 4, 3, 1};

	register int i, j, i2, j2;

	int index, index2, convolution;

	for(i=0; i<imageHeight; i++) 
	{
		index = i*imageWidth;

		for(j=0; j<imageWidth; j++) 
		{
			index2 = index+j;

			convolution = 0;

			for(j2=-2; j2<=2; j2++) 
			{

				if(j+j2 < 0 || j+j2 >= imageWidth)
				{
					continue;
				}

				convolution += hx[2+j2]*inImage[index2+j2];
			}

			HX_CONVOLUTION[index2] = convolution;
		}
	}

	for(i=0; i<imageHeight; i++) 
	{
		index = i*imageWidth;

		for(j=0; j<imageWidth; j++) 
		{
			index2 = index+j;

			convolution = 0;

			for(i2=-2; i2<=2; i2++) 
			{

				if(i+i2 < 0 || i+i2 >= imageHeight)
				{
					continue;
				}

				convolution += hy[i2+2]*HX_CONVOLUTION[index2+i2*imageWidth];
			}

			outImage[index2] = convolution/144;
		}
	}

}

#if APPLY_GAUSSIAN_BLUR
void GaussianBlur(unsigned char* inImage, unsigned char* outImage, const int imageWidth, const int imageHeight, int type)
{
	if(type == KERNEL_3_BY_3)
	{
		BlurUsing3by3Kernel(inImage, outImage, imageWidth, imageHeight);

	}
	
	else if(type == KERNEL_5_BY_5)
	{
		BlurUsing5by5Kernel(inImage, outImage, imageWidth, imageHeight);
	}
}
#endif

#endif