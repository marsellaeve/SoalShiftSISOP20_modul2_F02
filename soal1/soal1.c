#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <wait.h>
#include<dirent.h>
#include <string.h>
 
int main(int argc, char** array){
	
   	time_t rawtime;
	struct tm *timeinfo;
	int i,arr[10],j,temp,a,b;
	char str[35];
	for(i=1;i<argc;i++){ printf("%s",array[i]);}
	DIR *dir;
	struct dirent *ent;
	char text[200]="/home/evelyn/Praktikum2/";
	int flags=0;
	if ((dir = opendir (text)) != NULL) {
	  while ((ent = readdir (dir)) != NULL) {
		if(strcmp(ent->d_name,array[4])==0){flags=1; break;}
	  }
	if(flags==0){
		printf("file tidak ditemukan\n");
		return 0;
	}		
	} 
	if(argc<5||argc>5){
		printf("a Program error masukkan argumen\n");
		return 0;
	}
	if(array[4][(strlen(array[4])-3)]!='.'|| array[4][(strlen(array[4])-2)]!='s'|| array[4][(strlen(array[4])-1)]!='h'){
		printf("Program error masukkan argumen\n");
		return 0;
	}
	int flagss=0;
	for (i = 1; i < argc; ++i){		
		if(strcmp(array[i],"*")==0) {
			flagss++; if(flagss==3) return 0;
			arr[i-1]=-1; continue;}
		else if(i==4){
			strcpy(str,array[i]);
			continue;
		}
		else if(strlen(array[i])>=2){
			a=(int)(array[i][0]);
			b=(int)(array[i][1]);//printf("%d %d\n",a,b);
			a=a-48; b=b-48;  //printf("%d %d\n",a,b);
			if(a<0||a>9||b<0||a>9){
			printf("Program error, input harus angka\n");
			return 0;
			}
			temp=(a*10)+b;
		}
		else if(strlen(array[i])==1){
			temp=(int)(array[i][0])-48;
			if(temp<0||temp>9){
			printf("Program error, b input harus angka\n");
			return 0;
			}
		}
		arr[i-1]=temp;
		//printf("\n%d\n",arr[i-1]);
		
		if((i==1||i==2)&&(arr[i-1]<0||arr[i-1]>59)){
		printf("Program error masukkan detik/menit(0-59)\n");
		return 0;
		}
		
		else if(i==3&&(arr[i-1]>23||arr[i-1]<0)){
		printf("c Program error masukkan jam (0-23)\n");
		return 0;
		}
	}	
  pid_t pid, sid;        // Variabel untuk menyimpan PID

  pid = fork();     // Menyimpan PID dari Child Process

  /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/home/evelyn/Praktikum2")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);


	
	while(1){
	time(&rawtime);
	timeinfo = localtime(&rawtime);
printf("%d:%d:%d skrg: %d:%d:%d\n",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec,arr[2],arr[1],arr[0]);

 pid_t child_id;
  int status;
  child_id = fork();
  if (child_id < 0) {
    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
	
  }

  if (child_id == 0) {
    // this is child
	if((timeinfo->tm_sec==arr[0] || arr[0]==-1) && (timeinfo->tm_min==arr[1] || arr[1]==-1) && (timeinfo->tm_hour==arr[2]|| arr[2]==-1)){
printf("masuk %d:%d:%d skrg: %d:%d:%d\n",timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec,arr[2],arr[1],arr[0]);
		
		char *argv[] = {"bash",array[4], NULL};
		execv("/bin/bash", argv);
	}
	else {printf("blm msk"); exit(0);}
	
  } else {
    // this is parent
    while ((wait(&status)) > 0);
	sleep(1);
  }	
	
  }
}

