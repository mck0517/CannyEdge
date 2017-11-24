#ifndef _OTSU_THRESHOLD_H_
#define _OTSU_THRESHOLD_H_


void MakeBinaryImage(const unsigned char *inImage, unsigned char *outImage, const int imageWidth, const int imageHeight, const int threshold)
{
	register int k;

	int imageSize = imageWidth*imageHeight;

	for(k=0; k<imageSize; k++)
	{
		outImage[k] = (inImage[k] > threshold) ? 255 : 0;
	}

}

int GetOtsuThreshold(const unsigned char *image, const int imageWidth, const int imageHeight)
{
	register int m;

	int totalSize = imageWidth*imageHeight;

	float cumulativeHistogram[256] = {0, };

	for(m=0; m<totalSize; m++)
	{
		cumulativeHistogram[image[m]]++;
	}

	register int i;

	float sum = 0.0f;

	for(i=0; i<256; i++)
	{
		sum += cumulativeHistogram[i];
	}

	float mu = 0.0f; 
	float scale = 1.f/float(sum);

	for(i=0; i<256; i++)
	{
		mu += float(i)*float(cumulativeHistogram[i]);
	}

	mu *= scale;
	float mu1 = 0.0f;
	float q1 = 0.0f;
	float maxSigma = 0.0f;
	int findThreshold = 0;

#define MY_FLT_EPSILON     1.192092896e-07F
#define GET_MAX(a, b)	   (((a) > (b)) ? (a) : (b))
#define GET_MIN(a, b)	   (((a) < (b)) ? (a) : (b))

	for(i=0; i<256; i++) 
	{
		float pI, q2, mu2, sigma;

		pI = float(cumulativeHistogram[i])*scale;

		mu1 *= q1;
		q1 += pI;
		q2 = 1.f - q1;

		if( GET_MIN(q1, q2) < MY_FLT_EPSILON || GET_MAX(q1, q2) > 1.f - MY_FLT_EPSILON )
		{
			continue;
		}

		mu1 = (mu1 + i*pI)/q1;
		mu2 = (mu  - q1*mu1)/q2;

		sigma = q1*q2*(mu1-mu2)*(mu1-mu2);

		if(sigma > maxSigma)
		{
			maxSigma = sigma;
			findThreshold = i;
		}	
	}

	return findThreshold;
}

#endif