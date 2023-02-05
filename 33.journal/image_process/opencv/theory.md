### Matrix
```c
#if 0
        Column0 Column1 Column2
    Row0 xxx     xxx     xxx
    Row1 xxx     xxx     xxx
    Row2 xxx     xxx     xxx
    Row3 xxx     xxx     xxx
    Row4 xxx     xxx     xxx

number of row is 5
number of column is 3
#endif
```

### pixel format
+ RGB: display-device,
    like human-eyes
+ HSV/HLS: Hue, Saturation, Value/Lightness
    most natural way to descibe color
+ YCrCb: common used in jpeg
+ CIE L\*a\*b\*:
+ cv::Mat, pixels are store in BGR format

#####
+ difference between Mat pointer and its memory space
+ addWeighted()
    dst = src1[I]*alpah + src2[I]*beta + gamma;
+ `split(srcImage, channels);`
    + channels.at(0) blue;
    + channels.at(1) green;
    + channels.at(2) red;
+ `merge(channels, dstImage);`

##### pointoperators
+ contrast and brightness
    dst(col, row)  = a * src(col, row) + b
    a, control contrast, [a > 0] means gain
    b, control brightness, so-called bias
```cpp
Mat img;
uchar pixel = img.at<Vec3b>(row, col)[channel];
pixel = saturate_cast<uchar>(a * pixel + b);
```

##### DFT
+ Discrete Fourier Transform
+ FT is a function divide toolkit
+ transformit image from spatial-domain to frequency-domain
+ frequency-domain value:
    real-image + complex-image
    magitude-image + phase-image

### image process
+ smoothing, so-called bluring, denoise and reduce distortion
+ spatial-domain smoothing-filter, low-frequency promote
+ BoxFilter, MeanFilter, GaussianFilter, MedianFilter, BilateralFilter
+ MedianFilter, useful for SpeckleNoise, Salt-and-PepperNoise
+ dilate and erode
    + blight-area is the target
    + denoise
    + isolate single image element
    + join adjacent image element
    + find the obvious Max/Min-area
    + solve the image gradient

##### Morpholog (mess of dilate and erode)
+ open operatioyn
+ close operation
+ gradient
+ top-hat
+ black-hat
+ erode
+ dilate

##### flood fill
+ like magic-stick in Photoshop
+ one version with **mask**
+ another version without **mask**

##### image pyramid
+ a filtering, but just like resize()
+ Gaussian pyramid
+ Laplacian pyramid

##### miscellaneous
+ LUT, table-refers is faster than calculate
+ saturate_cast<uchar>, to avoid overflow
    if(overflow_flag == 1 && value < 0)
        value = uchar_max;
