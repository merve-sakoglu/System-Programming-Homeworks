
/******************************************/
/*      121044055                         */
/*    Merve SAKOĞLU                       */
/*  HW01_121044055_merve_sakoğlu          */
/*        grep                            */
/******************************************/

#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>  
#include <errno.h> 
#include "grepFromFile.h"

int main(int argc, char *argv[])
{
	FILE *inputFile;
	FILE *outputFile;

	int TotalWord = 0 , size = 0;

	inputFile = fopen(argv[1],"a");
	outputFile = fopen("gfF.log","a");

	if ((argv[1] == NULL) || (argv[2] == NULL) || argc!=3) 
 	{ 
		if(argc > 3)
			printf("Fazla argüman girdiniz\n");
		else if(argc < 3)
			printf("Az argüman girdiniz\n");

		usage(argv[1]);

	        return -1; 
	} 
	
	if(inputFile == NULL){
		printf("Dosya açılamadı\n");
		exit (0);
	}
	

	size = findSize(inputFile,argv[1]);

	TotalWord = myGrep(inputFile,outputFile, size,argv[1],argv[2]);
	
	if(TotalWord == 0)
		printf("Dosyada kelime bulunmamaktadır\n");
	else
		printf("\nNumber of times %d\n\n",TotalWord);
	fclose(inputFile);
        fclose(outputFile);
	return 0;
}
/*Dosyadaki karakter sayisini sayar*/
int findSize(FILE *inputFile,char *fileName){

	char character;
	int count ;
	inputFile = fopen(fileName,"r");

	for(count =0 ; fscanf(inputFile,"%c",&character) != EOF ; ++count){
	
	}
  
   	fclose(inputFile);

	return (count);

}
int myGrep(FILE *inputFile,FILE *outputFile,int size,char *fileName,char *argv){
	
	int coloumnCount = 0 , totalWord = 0;
	int  i , j,row = 0;
	char *string=(char*)malloc(size*sizeof(char));

	inputFile = fopen(fileName,"r");
	outputFile = fopen("gfF.log","a");

	if (string == NULL) 
 	{ 
 	    printf("Memory not allocated\n"); 
	    free(string);
	    exit (0); 
 	} 

	fprintf(outputFile,"----------%s----------\n\n",fileName);

	/*Dosyadan ilk satiri okur*/
	while(fgets(string ,size, inputFile) != 0){

		/*Satir sonuna kadar kontrol eder*/
		for(i=0 ; i<strlen(string) ; ++i){
			/*Karakter aranan kelimenin ilk karakterine eşit ise o satiri row da tutar*/
			if(string[i] == argv[0]){
				row = 0;  row = i;
				
				j = 1;
				/*aranan kelimenin butun  karakterleri esit oldugu surece j i artirir */
				while((string[i+j] == argv[j]) && (j<strlen(argv))){
					++j;

				}
				/*kelimenin butun karakterleri esit ise kelime sayisisni artirir ve dosyaya bastirir*/
				if(strlen(argv) == j){

					++totalWord;
					fprintf(outputFile,"-->%d. Line # %d. Column\n",coloumnCount,row);
				}

			}

		}
	++coloumnCount;
	}

	fclose(inputFile);
	fclose(outputFile);

	free(string);

	return totalWord;
}
void usage(char *argPointer){
	printf("\n------------\n");
	printf("Usage : \n");
	printf("./grepFromFile %s *(aranmak istenen karakter)",argPointer);
	printf("\n------------\n");

}
