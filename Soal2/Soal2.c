#include<stdio.h>
#include<time.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <sys/prctl.h>

void writeKillerA();
void writeKillerB();

int main(int argm, char **argmod)
{
    if(strcmp(argmod[1], "-a") == 0){
    writeKillerA();
  }
    else if(strcmp(argmod[1], "-b") == 0){
    writeKillerB();
  }

  pid_t pid, sid;        // Variabel untuk menyimpan PID
  pid = fork();     // Menyimpan PID dari Child Process
  if (pid > 0) { // Keluar saat fork berhasil
    exit(EXIT_SUCCESS);
  }
  if (pid < 0) {   //Keluar saat fork gagal
    exit(EXIT_FAILURE);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1)
  {
    int status;
    time_t waktufile;
    char date[26], namafile[26];
    struct tm* tm_info = localtime(&waktufile);
    waktufile = time(NULL);
    strftime(date, 26, "%Y-%m-%d_%H:%M:%S", tm_info);
    sprintf(namafile, "/home/rayroyy/%s", date);
    char *argv[] = {"mkdir", "-p", namafile, NULL};

     pid_t child_id = fork();

    if (child_id == 0)
    {
        execv("/bin/mkdir", argv); // buat bikin nama file
    }

    wait(&status);

    child_id=fork();
    if (child_id == 0)
    {
        int i=0;
        chdir(namafile);
        while(i<20){
            char link[100],date2[26];
            time_t waktugambar;
            time(&waktugambar);
            struct tm* tm_info2 = localtime(&waktugambar);
            // long int sec = time(NULL)%1000 + 100;
            strftime(date2, 26, "%Y-%m-%d_%H:%M:%S", tm_info2);
            sprintf(link, "https://picsum.photos/%ld", waktugambar % 1000 + 100); //download + set pixelnya
            pid_t child2 = fork();
            if (child2==0){
                // char *argv[] = {"wget", "-O", date2, link, NULL};
                // execv("/usr/bin/wget" , argv);
                execl("/usr/bin/wget", "wget", link, "-O", date2, "-o", "/dev/null", NULL);
                exit(EXIT_SUCCESS);
            }
            sleep(5);
            i++;
        }
        pid_t child_id2;
        child_id2 = fork();
      //   int status2;
      // if(child_id2==0){
      //       char namazip[100];
      //       sprintf(namazip, "%s.zip", namafile);
      //       char *argv[] = {"zip", "-r", namazip, namafile, NULL}; //-r recurse into directories
      //       execv("usr/bin/zip", argv);
      //     } else {
      //         while((wait(&status2)) > 0); 
      //         char *argv[] = {"rm", "-rf", namafile, NULL};
			//         execv("/bin/rm", argv);
      int status2;
			while(wait(&status2) > 0);
			chdir("..");
			char namazip[150];
			sprintf(namazip, "%s.zip", namafile);

			pid_t child3 = fork();
			if (child3 == 0) {
				execl("/usr/bin/zip", "zip", "-r", namazip, namafile, NULL);
			}
			int status3;
			while (wait(&status3) > 0);	
			execl("/bin/rm", "rm", "-rf", namafile, NULL);
            }
    sleep(30);
  }
 }


void writeKillerA() //program utama akan langsung menghentikan semua operasinya ketika program killer dijalankan.
{
  FILE *temp;
  temp = fopen("killer.sh", "w");
  fputs("#!/bin/bash\n", temp);
  fputs("killOrder=$(echo $(pidof soal2))\n", temp);
  fputs("kill -9 $killOrder\n", temp);
  fputs("rm $0\n", temp); //buat matiin temp
  fclose(temp);

  pid_t temp_idchild;
  temp_idchild = fork();

  if(temp_idchild == 0){
    char *argv[]={"chmod", "+x", "killer.sh", NULL}; //+x execute killer.sh nya
    execv("/bin/chmod", argv);
  }
}

void writeKillerB() //program utama akan berhenti tapi membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua folder terisi gambar, terzip lalu di delete).
{
  FILE *temp;
  temp = fopen("killer.sh", "w");
  fputs("#!/bin/bash\n", temp);
  fputs("killOrder=$(echo $(pidof soal2))\n", temp);
  fputs("killOrder=${killOrder##* }\n", temp);
  fputs("kill -9 $killOrder\n", temp);
  fputs("rm $0\n", temp);
  fclose(temp);

  pid_t temp_idchild;
  temp_idchild = fork();

  if(temp_idchild == 0){
    char *argv[]={"chmod", "+x", "killer.sh", NULL};
    execv("/bin/chmod", argv);
  }
}