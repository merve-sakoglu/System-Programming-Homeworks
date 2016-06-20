/******************************************/
/*      121044055                         */
/*    Merve SAKOĞLU                       */
/*  HW01_121044055_merve_sakoğlu.h        */
/*        grep                            */
/******************************************/


#include <stdio.h>
#include <stdlib.h>

int myGrep(FILE *inputFile,FILE *outputFile,int size,char *fileName,char *argv);
int findSize(FILE *inputFile,char *fileName);
void usage(char *argPointer);

