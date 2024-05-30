#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "image.h"

ImagePPM *readPPM(char *filename)
{
    ImagePPM *image = malloc(sizeof(ImagePPM));
    FILE *fr = fopen(filename, "r");
    if(!fr){
        return NULL;
    }
    fscanf(fr, "%s", image->magic);
    fscanf(fr, "%d", &image->width );
    fscanf(fr, "%d", &image->height);
    fscanf(fr, "%d", &image->max_value);

    image->pixels = malloc(sizeof(Pixel*)* image->height);
    for(int i=0; i<image->height; i++){
        image->pixels[i] = malloc(sizeof(Pixel)*image->width);
    }
    for(int i=0; i< image->height; i++){
        for(int j=0; j< image->width; j++ ){
            fscanf(fr, "%d", &image->pixels[i][j].red);
            fscanf(fr, "%d", &image->pixels[i][j].green);
            fscanf(fr, "%d", &image->pixels[i][j].blue);
        }
    }
    fclose(fr); //is this necissary? i cant spell 

	return image;
}

int writePPM(ImagePPM *pImagePPM, char *filename) //what format shall we write out? End in a new line, can we have 5 spaces after before drop down?
{

    FILE *fw = fopen(filename, "w");
    if(!fw){
        return 0;
    }
    fprintf(fw,"%s\n", pImagePPM->magic);
    fprintf(fw,"%d     %d \n", pImagePPM->width, pImagePPM->height);
    fprintf(fw,"%d\n", pImagePPM->max_value);
    for(int i=0; i< pImagePPM->height; i++){
        for(int j=0; j< pImagePPM->width; j++ ){
            fprintf(fw, "%d ", pImagePPM->pixels[i][j].red);
            fprintf(fw, "%d ", pImagePPM->pixels[i][j].green);
            fprintf(fw, "%d ", pImagePPM->pixels[i][j].blue);
            if(j<pImagePPM->width-1){
                fprintf(fw, "\t");
            }
        }
        fprintf(fw, "\n");
    }

    fclose(fw); //is this neccissary? sorry i cant spell
	return 1;
}

ImagePPM *rotatePPM(ImagePPM *pImagePPM) //seems like the write function isn't working for this but I'm not sure why
{
//rotate corners first? no.
  


    ImagePPM *newImage = malloc(sizeof(ImagePPM));
    newImage->pixels = malloc(sizeof(Pixel *) * pImagePPM->width);
    for(int i=0; i<pImagePPM->width; i++){
        newImage->pixels[i] = malloc(sizeof(Pixel)* pImagePPM->height);
    }
    strcpy(newImage->magic,pImagePPM->magic);
    newImage->height = pImagePPM->width;
    newImage->width = pImagePPM->height;
    newImage->max_value = pImagePPM->max_value;

    int Oldwidth = pImagePPM->width;
    int Oldheight = pImagePPM->height;


    for(int rows=0; rows < Oldheight; rows++){
        for (int cols = 0; cols< Oldwidth; cols++){
           newImage->pixels[cols][Oldheight-1-rows] = pImagePPM->pixels[rows][cols];
           


        }

    }

    
	return newImage;
    
}

ImagePPM *flipPPM(ImagePPM *pImagePPM) //do we create these on the heap or stack, also i think complete lol
{
    
    ImagePPM *newImage = malloc(sizeof(ImagePPM));
    newImage->pixels = malloc(sizeof(Pixel *) * pImagePPM->height);
    for(int i=0; i<pImagePPM->height; i++){
        newImage->pixels[i] = malloc(sizeof(Pixel)*pImagePPM->width);
    }
    strcpy(newImage->magic,pImagePPM->magic);
    newImage->height = pImagePPM->height;
    newImage->width = pImagePPM->width;
    newImage->max_value = pImagePPM->max_value;
    int k; //controlls the new image asf
    for (int i = 0; i<pImagePPM->height; i++){
        k = (pImagePPM->width)-1;
        for( int j = 0; j < pImagePPM->width; j++){
            newImage->pixels[i][k] = pImagePPM->pixels[i][j]; //does this update all info? Yeah it looks like it does lol
            k--;
        }
    }


    return newImage;

}

ImagePPM *enlargePPM(ImagePPM *pImagePPM) //this one isn't writing the new struct FIXED
{
   ImagePPM *newImage = malloc(sizeof(ImagePPM));
    newImage->pixels = malloc(sizeof(Pixel *) * (pImagePPM->height) *2);
    for(int i=0; i< (pImagePPM->height * 2); i++){
        newImage->pixels[i] = malloc(sizeof(Pixel)*(pImagePPM->width)*2);
    }
    strcpy(newImage->magic,pImagePPM->magic);
    int tempH = (pImagePPM->height)*2;
    newImage->height = tempH;
    int tempW = (pImagePPM->width)*2;
    newImage->width = tempW;

    newImage->max_value = pImagePPM->max_value;


    for(int rows=0; rows<pImagePPM->height;rows++){
        for (int cols=0; cols<pImagePPM->width; cols++){
            newImage->pixels[2*rows][2*cols] = pImagePPM->pixels[rows][cols];
            newImage->pixels[(2*rows)+1][(2*cols)+1] = pImagePPM->pixels[rows][cols];
            newImage->pixels[(2*rows)][(2*cols)+1] = pImagePPM->pixels[rows][cols];
            newImage->pixels[(2*rows)+1][(2*cols)] = pImagePPM->pixels[rows][cols];
        }
    }





    return newImage;
}

ImagePPM *shrinkPPM(ImagePPM *pImagePPM)
{
   ImagePPM *newImage = malloc(sizeof(ImagePPM));
    newImage->pixels = malloc(sizeof(Pixel *) * pImagePPM->height);
    for(int i=0; i<pImagePPM->height; i++){
        newImage->pixels[i] = malloc(sizeof(Pixel)*pImagePPM->width);
    }
    strcpy(newImage->magic,pImagePPM->magic);
    newImage->height = (pImagePPM->height)/2;
    newImage->width = (pImagePPM->width)/2;
    newImage->max_value = pImagePPM->max_value;
    double redAvg, greenAvg, blueAvg;

        for(int rows=0; rows<newImage->height;rows++){
        for (int cols=0; cols<newImage->width; cols++){
            redAvg = (pImagePPM->pixels[2*rows][2*cols].red + pImagePPM->pixels[(2*rows)+1][(2*cols)+1].red + pImagePPM->pixels[2*rows][(2*cols)+1].red + pImagePPM->pixels[(2*rows)+1][2*cols].red)/4.0;
            greenAvg = (pImagePPM->pixels[2*rows][2*cols].green + pImagePPM->pixels[(2*rows)+1][(2*cols)+1].green + pImagePPM->pixels[2*rows][(2*cols)+1].green + pImagePPM->pixels[(2*rows)+1][2*cols].green)/4.0;
            blueAvg = (pImagePPM->pixels[2*rows][2*cols].blue + pImagePPM->pixels[(2*rows)+1][(2*cols)+1].blue + pImagePPM->pixels[2*rows][(2*cols)+1].blue + pImagePPM->pixels[(2*rows)+1][2*cols].blue)/4.0;
            newImage->pixels[rows][cols].red = round(redAvg);
            newImage->pixels[rows][cols].green = round(greenAvg);
            newImage->pixels[rows][cols].blue = round(blueAvg);

        }
    }



    return newImage;
}

ImagePPM *invertPPM(ImagePPM *pImagePPM)
{
    ImagePPM *newImage = malloc(sizeof(ImagePPM)); //create "new image"
    newImage->pixels = malloc(sizeof(Pixel *) * pImagePPM->height);
    for(int i=0; i<pImagePPM->height; i++){
        newImage->pixels[i] = malloc(sizeof(Pixel)*pImagePPM->width);
    }
    strcpy(newImage->magic,pImagePPM->magic);
    newImage->height = pImagePPM->height;
    newImage->width = pImagePPM->width;
    newImage->max_value = pImagePPM->max_value;


    for(int rows=0; rows<newImage->height;rows++){
        for (int cols=0; cols<newImage->width; cols++){
            newImage->pixels[rows][cols].red = pImagePPM->max_value - pImagePPM->pixels[rows][cols].red;
            newImage->pixels[rows][cols].green = pImagePPM->max_value - pImagePPM->pixels[rows][cols].green;
            newImage->pixels[rows][cols].blue = pImagePPM->max_value - pImagePPM->pixels[rows][cols].blue;
        }
    }



    return newImage;
}

ImagePPM *cropPPM(ImagePPM *pImagePPM, int r1, int c1, int r2, int c2)
{

    ImagePPM *newImage = malloc(sizeof(ImagePPM)); //create "new image"
    newImage->pixels = malloc(sizeof(Pixel *) * (abs(r2-r1)+1));
    for(int i=0; i<(abs(r2-r1)+1); i++){
        newImage->pixels[i] = malloc(sizeof(Pixel)*(abs(c2-c1)+1));
    }
    strcpy(newImage->magic,pImagePPM->magic);
    newImage->height = abs(r2-r1)+1; //check
    newImage->width = abs(c2-c1)+1; //check 
    newImage->max_value = pImagePPM->max_value;

    int currRow;
    int upperRowBound;
    int currCol;
    int upperColBound;


    if(r1<r2){
         currRow=(r1);
         upperRowBound = (r2);
    }else{
    
         currRow=(r2);
         upperRowBound = (r1);
    }
    if(c1<c2){
     currCol=(c1);  
     upperColBound = (c2);    
    }else{
     currCol=(c2);  
     upperColBound = (c1);  
    }

    int r=0;
    int c=0;
 
 
    for(; currRow <= upperRowBound ;currRow++){
        c=0;
        for (int oldCol = currCol; oldCol<=upperColBound; oldCol++){
            newImage->pixels[r][c] = pImagePPM->pixels[currRow][oldCol];
            c++;
        }
        
        r++;
    }


    return newImage;
}

void freeSpacePPM(ImagePPM *pImagePPM) //free both images or jst one?
{

    //every time you call malloc is everytime you call free (3 frees)
    //1. 
        for(int l=0; l<pImagePPM->height; l++){
        free(pImagePPM->pixels[l]);
    }
    free(pImagePPM->pixels);

    free(pImagePPM);



}
