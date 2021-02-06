Generate filtered output image from input image.

Designed for bitmaps.

This program was written to work with a distro.  In the distro the main function was already written and a pixel was already defined for use by the filter.c file, which I implemented.

### Usage: filter [flag] infile outfile ###

### Filters ###
Grayscale -g, 
Reflect -r, 
Edges -e, 
Blur -b, 

Grayscale - render image in black and white, 

Reflect - flip image, 

Edges - edge detection algorithm, 

Blur - smooth edges



A program based on [specifications](https://cs50.harvard.edu/x/2020/psets/4/filter/more/ "Filter") given by HarvardX's online cs50 course.