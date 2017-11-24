* Canny Edge Detection Implementation using C language 
- Implementation of Canny Edge Detector(https://en.wikipedia.org/wiki/Canny_edge_detector)  
- Some modifications for optimization(ex: separable gaussian kernel, otsu thresholding(https://en.wikipedia.org/wiki/Otsu%27s_method)) 
- No 3rd party lib dependency, OpenCV was used only for image I/O handling.

* File Description
- Param.h: gaussian blurring setting flag, hysteresis threshold calculation methods(manual setting or using otsu).  
- Vision_Test.cpp: main fuction
- GaussianBlur.h: 3 by 3 or 5 by 5 separable gaussian kernel fuction.
- OtsuThreshold.h: otsu`s method.
- Util.h: image I/O handling for OpenCV.
- CannyEdge.h: canny edge detection. 

* Program Description
- You can use any kind of OpenCV Version.
- For the minimization of dynamic memory allocation, I sometimes use array buffer. Thus, You must set the width and height(IMAGE_WIDTH, IMAGE_HEIGHT in Param.h) of the image in advance. 
- OpenCV was used only for image reading fuction(ex: cvLoadImage("lenna.bmp", CV_LOAD_IMAGE_GRAYSCALE)).
- This program use only gray scale image format But you can use color image format using above opencv code.  
- If you run the main fuction(Vision_Test.cpp), you can see the results of the canny edge detector. 
- Otsu.bmp: Otsu binarization result image, BlurGaussian.bmp: Blurring result image, CannyEdge.bmp: Canny edge result image.

 
 

