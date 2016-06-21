/******************************************/
/*      121044055                         */
/*    Merve SAKOĞLU                       */
/*  HW03_121044055_merve_sakoğlu          */
/*      grepFromDirParalel.c              */
/******************************************/

#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
#include<dirent.h> 
#include<errno.h> 
#include<time.h> 
#include<sys/stat.h> 
#include<sys/types.h> 
#include<sys/param.h> 
#include<sys/mman.h> 
#include<unistd.h> 
#include<sys/wait.h> 
#include<termios.h> 
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include <pthread.h>

#undef MAX
#define MAX 1024
#define REQ_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
/*Dosyalardaki toplam kelime sayisini tutar*/
static int totalW = 0;
/*Toplam file sayisini tutar*/
static int totalFile = 0;
/*Toplam child sayisini tutar*/
pid_t  pid[MAX];
static int totalChild = 0;
/*Ana directory ile birlikte toplam directory sayisini tutar*/
static int totalDirectory = 1;
char myfifo[MAX] = "/home/user/Desktop/myFifo";
/*
*Bu fonksiyon dosya icinde aranan kelimenin kac kez gectigini bulur
*kacıncı satir ve sütunda gectigini dosyaya yazdirir
*/
int myGrep(FILE *inputFile,FILE *outputFile,char* strPipe,int* fd,int size,char *fileName,char *argv);
/*
*Bu fonksiyon dosyanin size ini bulur
*/
int findSize(FILE *inputFile,char *fileName);
/*
*Asil isi yapan fonksiyon static olarak tanimladigim toplam kelime,
*file,directory ve child degiskenlerimi burada buldum
*myGrep() fonksiyonumu burada cagirarak dosyalardaki tum kelime
*sayisini burada buldum
*/
int fileOperations(FILE *outputFile,int* fd,char *directoryName,char *findString);
void ctrlcSignal(int ctrl);
void usage();
pid_t r_wait(int *stat_loc);

FILE *outputFile;

void ctrlcSignal(int signum){

	char fileName[] = "gfd.log";
	int i;
	
	printf("CTR^C ile sonlandirildi. Tid: %ld\n",pthread_self());
	outputFile=fopen(fileName,"a");
	if(outputFile==NULL){
	    	fprintf(stderr,"Cannot open File: %s\n",fileName);
	    	exit(1);
	}
   
	fprintf(outputFile, "Ctrl-C pressed and quit\n");
	fclose(outputFile);
	  for(i=0;i<totalChild;++i)
    	    kill(getppid(),SIGINT);
    	
	unlink("myfifo");
	remove(myfifo);
	exit(signum);
}

int main(int argc, char *argv[]){

	
	int fd[2];

	long  iPathLength;
	outputFile = fopen("gfd.log","w");
	
	/*
	*sinyal
	*/
	
	struct sigaction sinyal;
	sinyal.sa_handler = ctrlcSignal;
        sinyal.sa_flags = 0;
	if((sigemptyset(&sinyal.sa_mask) || sigaction(SIGINT, &sinyal, NULL)) == -1){
		perror("Couldn't set signal handler for SIGINT");
		return 1;
	}

   

	if ((argv[1] == NULL) || (argv[2] == NULL) || argc!=3) 
 	{ 
		if(argc > 3)
			printf("Fazla argüman girdiniz\n");
		else if(argc < 3)
			printf("Az argüman girdiniz\n");

		usage();
		fclose(outputFile);
	        return -1; 
	} 
	/*
	if(outputFile == NULL){
		printf("Dosya açılamadı\n");
		fclose(outputFile);
		exit (0);
	}*/
	if(pipe(fd)==-1)
	{
		perror("Failed to create pipe");
		return 0;
	}
	
		/* Kitabin kodlarini kullandim */
	if ((iPathLength = pathconf(".", _PC_PATH_MAX)) == -1) {
	      perror("Failed to determine the pathname length");
	      return 1;
	}
	 
	 
	if (getcwd(myfifo, iPathLength) == NULL) {
	      perror("Failed to get current working directory");
	      return 1;
   	}
   	strcat(myfifo,"/MyFifo");
   	
	if ((mkfifo(myfifo, 0777) == -1) && (errno != EEXIST)) {
       		perror("Server failed to create request FIFO");
      		 return 1; 
  	}
	//signal(SIGINT, &ctrlcSignal);
        fileOperations(outputFile,fd,argv[1],argv[2]);

	printf("******************************************************************\n");
	printf("Total Words  *  Total Directory  *  Total File  *  Total Child  \n");
	printf("******************************************************************\n");
	printf("    %d               %d                  %d             %d        \n",totalW,totalDirectory,totalFile,totalChild);
	printf("******************************************************************\n");
	
	//unlink(myfifo);
	fclose(outputFile);
	return 0;
} 

/*
*Dosyadaki karakter sayisini sayar
*/
int findSize(FILE *inputFile,char *fileName){

	char character;
	int count ;
	inputFile = fopen(fileName,"r");

	for(count =0 ; fscanf(inputFile,"%c",&character) != EOF ; ++count){
	
	}
  
   	fclose(inputFile);

	return (count);

}
int myGrep(FILE *inputFile,FILE *outputFile,char* strPipe,int* fd,int size,char *fileName,char *argv){
	
	int coloumnCount = 0, totalWord = 0;
	int  i , j,row = 0;
	char *string=(char*)malloc(size*sizeof(char));
	
	inputFile = fopen(fileName,"r");

	if (string == NULL) 
 	{ 
 	    printf("Memory not allocated\n");
	    fclose(inputFile); 
	    free(string);
	    exit (0); 
 	}

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
					sprintf(strPipe,"%d. Satir # %d. Sutun  %s\n",coloumnCount,row,fileName);
					//close(fd[0]);
					if(write(fd[1],strPipe,MAX) <=0){
						perror("write error \n");
						exit(1);
					
					}
					
						
				}

			}

		}
	++coloumnCount;
	}
	
        close(fd[1]);
	fclose(inputFile);
	free(string);

	return totalWord;
}
int fileOperations(FILE *outputFile,int* fd,char *directoryName,char *findString){

	DIR *openFile;
	FILE *inputFile=NULL;
	struct dirent *input;
	char directory[MAX];
	char fullDirectory[MAX];
	struct stat statFile;
	pid_t  pid;
	char strPipe[MAX];
	char readpipe[MAX];
	char arr[MAX];
	int stat = 0;
	int size = 0;
	int fifo2;
	int in;
	openFile = opendir(directoryName);

	if(openFile == NULL){
		printf("Dosya acilamadi\n");
		closedir(openFile);
		exit (1);
	}
	
	strncpy(directory,directoryName, MAX - 1);

	//signal(SIGINT, &ctrlcSignal);
        /*Directory okudum*/
	while((input = readdir(openFile)) != NULL){
		
		if(strcmp(input->d_name,".")!=0 && strcmp(input->d_name,"..")!=0){
			
			/*Directory ve altindaki dosyayi bir arraye attim*/
			sprintf(fullDirectory, "%s/%s",directory, input -> d_name );

			if (lstat(fullDirectory,&statFile) < 0){
          			 perror(fullDirectory);
			}
			/*Dosya mi directory mi oldugunu kontrol ettim dosya ise*/
			if (S_ISDIR(statFile.st_mode) == 0){

				/*Dosya sayimi artirdim*/
				totalFile = totalFile + 1;
				if((pid = fork())== -1){
					perror("Not fork\n");
					exit (1);
				}if(pid == 0){
					size = findSize(inputFile,fullDirectory);
					
					totalW += myGrep(inputFile,outputFile,strPipe,fd,size,fullDirectory,findString);
					
					exit (totalW);
				}else{
						// while (r_wait(NULL) > 0);
					/*Asagidaki fonksiyonlari http://www.kaanaslan.com/resource/article/display_article.php?id=87
					 * sitesinden referans alarak kullandim */
					wait(&stat);
					totalW = WEXITSTATUS(stat);
					
				}
				/*Childlari topladim*/
				totalChild = totalChild + 1;
			}else{/*Dosya degil ise recursive olarak fonksiyonumu tekrar cagirdim ki dosyayi bulsun*/
				
				/*Directory sayimi buldum*/
				totalDirectory = totalDirectory + 1;

				if((pid = fork())== -1){
					perror("Not fork\n");
					exit (1);
			
				}
				if(pid == 0){
					
					fileOperations(outputFile,fd,fullDirectory,findString);
				
					if((in= read(fd[0], readpipe,MAX)> 0)){
						
						if(in<=0){
					
							perror("read error \n");
							exit(1);
					        }
					
				
						//close(fd[0]);
					
						if((fifo2 = open(myfifo, O_RDWR ))==-1){
							perror("Not fifo");
						        exit(1);
						}
							
					
						if(write(fifo2,readpipe,sizeof(char)*MAX)<0)
						{
						    	
						    	perror("Failed to write");
						    	exit(1);
					        }
						    	
						close(fifo2);
						if((fifo2 = open(myfifo,O_RDONLY))==-1){
							perror("Not fifo");
						        exit(1);
						}
						in = read( fifo2, arr,sizeof(char)*MAX);
						if(in<=0)
				                {
							perror("not read");
							exit(1);
		
						}
					
						fprintf(outputFile,"%s\n",arr);
						exit(1);
						
				
				     }
				}else{
					
					//while (r_wait(NULL) > 0);
					//close(fd[1]);
					//unlink("myfifo");
					
					
				}	
			}
			
		}
	}
	
	//close(fifo2);
	closedir(openFile);
	return 0;
}

void usage(){
	printf("\n------------\n");
	printf("Usage : \n");
	printf("./grepFromDirParalel dirname *(aranmak istenen karakter)");
	printf("\n------------\n");
}

