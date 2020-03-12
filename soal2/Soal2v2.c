#include<stdio.h>
#include<time.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <sys/prctl.h>

void killer(int mode){
    FILE* killer;
	killer = fopen("killer.sh", "w");
	fprintf(killer, "#!/bin/bash\nkill %d\necho \'Killed program.\'\nrm \"$0\"", getpid());
	fclose(killer);
	pid_t child = fork();
	if (child == 0) {
		execl("/bin/chmod", "chmod", "u+x", "killer.sh", NULL);
	}
	int status;
	while(wait(&status) > 0);
}

int main(int argm, char* argmod[])
{
    if(argm != 2) exit(EXIT_FAILURE); //kalo argumennya salah
    int mode;
    if(argmod[1][1]=='a') mode = 1;
    else if (argmod[1][1]=='b') mode = 0;


  pid_t pid, sid;        // Variabel untuk menyimpan PID
  pid = fork();     // Menyimpan PID dari Child Process
  /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }
  /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
  if (pid < 0) {
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

  killer(mode);
  while(1)
  {
    int status;
    time_t waktufile;
    char date[26], namafile[26];
    struct tm* tm_info = localtime(&waktufile);

    waktufile = time(NULL);
    // tm_info = localtime(&waktufile);
    strftime(date, 26, "%Y-%m-%d_%H:%M:%S", tm_info);
    // puts(date);
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
        if (mode) prctl(PR_SET_PDEATHSIG, SIGHUP);
        int i=0;
        chdir(namafile);
        while(i<20){
            char link[100],date2[26];
            time_t waktugambar;
            time(&waktugambar);
            // waktugambar = time(NULL);
            struct tm* tm_info2 = localtime(&waktugambar);
            strftime(date2, 26, "%Y-%m-%d_%H:%M:%S", tm_info2);
            sprintf(link, "https://picsum.photos/%ld", waktugambar % 1000); //download + set pixelnya
            pid_t child2 = fork();
            if (child2==0){
                // char *argv[] = {"wget", "-O", "link", NULL};
                // char *argv2[] = {"-o", "date2", NULL};
                execl("/usr/bin/wget" , "wget", link, "-O", date2, "-o", "/dev/null", NULL); //-O untuk write documents to file , -o untuk log message to file
                exit(EXIT_SUCCESS);
            }
            sleep(5);
            i++;
        }
        int status2;
			while(wait(&status2) > 0);
            char *argv[] = {"zip", "-r", namafile, NULL}; //-r recurse into directories
            execv("usr/bin/zip", argv);

            pid_t child3 = fork();
            if (child3 == 0) {
                int status3;
                while (wait(&status3) > 0);	
                char *argv[] = {"rm", "-rf", "namafile", NULL};
			    execv("/bin/rm", argv);
            }

			// chdir("..");
			// char namazip[150];
			// sprintf(namazip, "%s.zip", namafile);

			// pid_t child3 = fork();
			// if (child3 == 0) {
			// 	execl("/usr/bin/zip", "zip", "-r", namazip, namafile, NULL);
			// }
			// int status3;
			// while (wait(&status3) > 0);	
			// execl("/bin/rm", "rm", "-rf", namafile, NULL);
    }
    // else
    // {
    //     // this is parent
    //     int ukuran,t;
    //     char ukuranfoto[50];
    //     time_t seconds;
    //     while ((wait(&status)) > 0);
    //     seconds =time (NULL);
    //     t=(int)seconds;
    //     ukuran=(t%1000)+100;
    //     sprintf(ukuranfoto , "%d", ukuran);
    //     char *argv[] = {"touch", "namafile/", NULL};
    //     execl("/bin/wget/https://picsum.photos/ukuranfoto", argv);
    // }
    sleep(30);
  }
}