#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"

char *operations[6] = { "rotate", "flip", "enlarge", "shrink", "invert", "crop"};

// return -1 for an invalid operation
int getOperationID(char *op)
{
	for (int i=0; i<6; i++) {
		if (strcmp(op, operations[i])==0) return i;
	}
	return -1;
}

int main(int argc, char **argv) {
    if (argc != 4 && argc != 8) {
        printf("Incorrect number of arguments\n");
        return 1;
    }
	int op_id=getOperationID(argv[3]);
	if (op_id<0) {
        printf("Invalid operation: %s\n", argv[3]);
        printf("Valid operations:\n");
		for (int i=0; i<6; i++) {
			printf("\t%s\n", operations[i]);
		}
		return 2;
	}
    if (strcmp(argv[3], "crop")==0) {
        if (argc!=8) {
            printf("Usage: %s in_ppm_filename out_ppm_filename crop r1 c1 r2 c2\n", argv[0]);
		    return 3;
        }
    }
    else {
        if (argc!=4) {
            printf("Usage: %s in_ppm_filename out_ppm_filename operation\n", argv[0]);
		    return 3;
        }
    }

    ImagePPM *pImagePPM;
    pImagePPM = readPPM(argv[1]);
    if (pImagePPM == NULL) {
        printf("Unable to read file: %s\n", argv[1]);
        return 4;
    }

    ImagePPM *pImagePPM2;
	switch (op_id) {
	case 0: // rotate
        pImagePPM2 = rotatePPM(pImagePPM);
		break;
	case 1: // flip
        pImagePPM2 = flipPPM(pImagePPM);
		break;
	case 2: // enlarge
        pImagePPM2 = enlargePPM(pImagePPM);
		break;
	case 3: // shrink
        pImagePPM2 = shrinkPPM(pImagePPM);
		break;
	case 4: // invert
        pImagePPM2 = invertPPM(pImagePPM);
		break;
	case 5: // crop
        {
            int r1=atoi(argv[4]);
            int c1=atoi(argv[5]);
            int r2=atoi(argv[6]);
            int c2=atoi(argv[7]);
            if (r1<0 || r1>=pImagePPM->height ||
                c1<0 || c1>=pImagePPM->width ||
                r2<0 || r2>=pImagePPM->height ||
                c2<0 || c2>=pImagePPM->width) {
                printf("The crop window, (%d, %d) to (%d, %d), is not inside the image.\n", r1, c1, r2, c2);
	            freeSpacePPM(pImagePPM);
                return -1;
            }
            pImagePPM2 = cropPPM(pImagePPM, r1, c1, r2, c2);
        }
		break;
    }

    int result = writePPM(pImagePPM2, argv[2]);
	if (result==0) {
        printf("Unable to write file: %s\n", argv[2]);
	}

	freeSpacePPM(pImagePPM);
	freeSpacePPM(pImagePPM2);

    return 0;
}
