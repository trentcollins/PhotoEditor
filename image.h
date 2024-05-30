#ifndef _IMAGE_H_
#define _IMAGE_H_

typedef struct _pixel {
    int red;
    int green;
    int blue;
} Pixel;

typedef struct _imagePPM {
	char magic[3];  // magic identifier, "P3" for PPM
    int width;      // number of columns
    int height;     // number of rows
    int max_value;  // maximum intenrsity of RGB components
    Pixel **pixels; // the actual color pixel data
} ImagePPM;

// Given a filename of a ppm image, read in the image and
// store it in the ImagePPM structure. Return the address of
// the ImagePPM structure if the file can be opened or
// NULL otherwise. 
ImagePPM *readPPM(char *filename);

// Write out a ppm image stored in a ImagePPM structure into
// the specified file. Return 1 if writing is successful or
// 0 otherwise.
int writePPM(ImagePPM *pImagePPM, char *filename);

// Rotate a ppm image clockwise by 90 degrees, and return the new ppm image.
ImagePPM *rotatePPM(ImagePPM *pImagePPM);

// Flip a ppm image horizontally (from left to right), and return the new ppm image.
ImagePPM *flipPPM(ImagePPM *pImagePPM);

// Enlarge a ppm image by doubling its size (both width and height), and return the new ppm image.
ImagePPM *enlargePPM(ImagePPM *pImagePPM);

// Shrink a ppm image by reducing its size (both width and height) into the half, and return the new ppm image.
ImagePPM *shrinkPPM(ImagePPM *pImagePPM);

// Invert each color of a ppm image by subtracting its red/green/blue intensity
// from the maximum-value of the image, and return the new ppm image.
ImagePPM *invertPPM(ImagePPM *pImagePPM);

// Crop a ppm image. The crop window is specified by two corners (r1, c1) and (r2, c2), both inclusive.
ImagePPM *cropPPM(ImagePPM *pImagePPM, int r1, int c1, int r2, int c2);

// Free the space used by a ppm image.
void freeSpacePPM(ImagePPM *pImagePPM);

#endif
