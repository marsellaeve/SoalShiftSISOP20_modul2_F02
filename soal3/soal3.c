#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include<time.h>
#include<dirent.h>
#include <string.h>

int main() {
  pid_t child_id;
  int status,i;
  child_id = fork();

  if (child_id < 0) {
    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
  }

  if (child_id == 0) {
    // this is child
	char *argv[] = {"mkdir", "/home/evelyn/modul2/indomie", NULL};
	for(i=0;i<3;i++) printf("%s ",argv[i]);  
	execv("/bin/mkdir", argv);
    
	
  } else {
    // this is parent
    	while ((wait(&status)) > 0);
	sleep(5);
  }

  child_id = fork();

  if (child_id < 0) {
    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
  }

  if (child_id == 0) {
    // this is child
	char *argv[] = {"mkdir", "/home/evelyn/modul2/sedaap", NULL};
	for(i=0;i<3;i++) printf("%s ",argv[i]);  
	execv("/bin/mkdir", argv); 
	
  } else {
    // this is parent
    	while ((wait(&status)) > 0);
  }

  child_id = fork();

  if (child_id < 0) {
    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
  }

  if (child_id == 0) {
    // this is child
	char *argv[] = {"unzip", "/home/evelyn/Praktikum2/jpg.zip","-d","/home/evelyn/modul2/", NULL};
	//for(i=0;i<3;i++) printf("%s ",argv[i]);  
	execv("/usr/bin/unzip", argv); 	
  } else {
    // this is parent
    	while ((wait(&status)) > 0);
  }

DIR *dir;
struct dirent *ent;
if ((dir = opendir ("/home/evelyn/modul2/jpg/")) != NULL) {
  /* print all the files and directories within directory */	
	
  while ((ent = readdir (dir)) != NULL) {
	int flag=0;
	if(strcmp(ent->d_name,".")==0) continue;
	if(strcmp(ent->d_name,"..")==0) continue;
	FILE *fptr1;
	char filename[]="/coba1.txt",filenamee[]="/coba2.txt";
//    printf ("%s %d\n",ent->d_name, ent->d_type);
    char text1[50]="/home/evelyn/modul2/jpg/";	
    char text2[50]="/home/evelyn/modul2/indomie/";
	strcat(text1,ent->d_name);
	strcat(text2,ent->d_name);
	if(ent->d_type == 4){
	flag=1; }

    child_id = fork();

  if (child_id < 0) {
    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
  }

  if (child_id == 0) {
    // this is child
	if(ent->d_type == 4){
	char *argv[] = {"mv", text1 ,"/home/evelyn/modul2/indomie/", NULL};
	//for(i=0;i<3;i++) printf("%s ",argv[i]);
	execv("/bin/mv", argv);
	}
	else if(ent->d_type == 8){
	char *argv[] = {"mv", text1 ,"/home/evelyn/modul2/sedaap/", NULL};
	//for(i=0;i<3;i++) printf("%s ",argv[i]);  
	execv("/bin/mv", argv);
	} 
  } else {
    // this is parent
    	while ((wait(&status)) > 0);
	if(flag==1){
	strcat(text2,filename);
//	printf("%s\n",text2);
    	fptr1=fopen(text2,"w");
    	fprintf(fptr1," ");
    	fclose(fptr1);
	sleep(3);	
    	char text2[]="/home/evelyn/modul2/indomie/";
	strcat(text2,ent->d_name);
	strcat(text2,filenamee);
//	printf("%s\n",text2);
    	fptr1=fopen(text2,"w");
    	fprintf(fptr1," ");
    	fclose(fptr1);
	}
  }
  }
  closedir (dir);
} else {
  /* could not open directory */
  perror ("");
  return EXIT_FAILURE;
}


}
