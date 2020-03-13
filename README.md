# SoalShiftSISOP20_modul2_F02
Praktikum Modul 2 Sisop 2020

Kelompok F02 (0099 &amp; 0142)

# Pembahasan Jawaban

## No 1 Program C untuk Crontab

### Source Code : [1](https://github.com/marsellaeve/SoalShiftSISOP20_modul2_F02/tree/master/soal1)

```C
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
 
int main(int argc, char** array){
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

  time_t rawtime;
	struct tm *timeinfo;
	int i,arr[10],j,temp,a,b;
	char str[35];
	for(i=1;i<argc;i++){ printf("%s",array[i]);}
	if(argc<5){
		printf("Program error masukkan argumen\n");
		return 0;
	}
	if(array[4][(strlen(array[4])-3)]!='.'|| array[4][(strlen(array[4])-2)]!='s'|| array[4][(strlen(array[4])-1)]!='h'){
		printf("Program error masukkan argumen\n");
		return 0;
	}
	for (i = 1; i < argc; ++i){		
		if(strcmp(array[i],"*")==0) { arr[i-1]=-1; continue; }
		else if(i==4){
			strcpy(str,array[i]);
			continue;
		}
		else if(strlen(array[i])>=2){
			a=(int)(array[i][0]);
			b=(int)(array[i][1]);
      a=a-48; b=b-48;
			if(a<0||a>9||b<0||a>9){
        printf("Program error, input harus angka\n");
        break;
			}
			temp=(a*10)+b;
		}
		else if(strlen(array[i])==1){
			temp=(int)(array[i][0])-48;
			if(temp<0||temp>9){
        printf("Program error, b input harus angka\n");
        break;
			}
		}
		arr[i-1]=temp;
    
		if((i==1||i==2)&&(arr[i-1]<0||arr[i-1]>59)){
      printf("Program error masukkan detik/menit(0-59)\n");
      break;
		}
		else if(i==3&&(arr[i-1]>23||arr[i-1]<0)){
      printf("c Program error masukkan jam (0-23)\n");
      break;
		}
	}
	
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
    }
    else {
      // this is parent
      while ((wait(&status)) > 0);
      sleep(1);
    }	
  }
}
```
#### A. Program menerima 4 argumen detik, menit, jam dan path file bash

`int main(int argc, char** array)` untuk memasukkan inputan dengan argumen (contoh ./program \* 34 7 /home/somi/test.sh saat menjalankan program sekaligus input argumen. Kemudian argc untuk menghitung berapa banyak string nya, dan array of strings disimpan dalam char** array.

`if(strcmp(array[i],"*")==0) { arr[i-1]=-1; continue;}` artinya pada saat inputan * akan dilewati dan diberi nilai -1

`else if(i==4){	strcpy(str,array[i]);	continue;` artinya saat nama file yang sudah disimpan, dicopy ke variable string

`else if(strlen(array[i])>=2){a=(int)(array[i][0]);	b=(int)(array[i][1]);	a=a-48; b=b-48;	temp=(a*10)+b;}` artinya apabila inputan waktu (detik,menit,jam) selain tanda * dilihat jika panjang stringnya 2 maka puluhan, dari string diubah menjadi bilangan char kemudian dikurangi 48 karena angka 0 = ascii 48. Kemudian puluhannya dikali 10, agar menjadi bilangan puluhan.

`else if(strlen(array[i])==1){` artinya apabila dia hanya satu string, maka tidak perlu dikali 10.

 `arr[i-1]=temp;` semua angka waktu yang sudah diubah menjadi integer dimasukkan kedalam array.
 
 #### B. Program akan mengeluarkan pesan error jika argumen yang diberikan tidak sesuai
 
 `if(argc<5)` Error apabila argumen yang dimasukkan kurang dari argumen yang seharusnya.
 
 `if(array[4][(strlen(array[4])-3)]!='.'|| array[4][(strlen(array[4])-2)]!='s'|| array[4][(strlen(array[4])-1)]!='h'` error apabila file bukan berekstensi .sh
 
 `if(a<0||a>9||b<0||a>9)` error apabila input detik,menit atau jam bukan berisi angka, sehingga ascii-48 hasilnya bukan angka 0-9.
 
 `if((i==1||i==2)&&(arr[i-1]<0||arr[i-1]>59))` error apabila input detik atau menit selain angka 0-59.
 
 `else if(i==3&&(arr[i-1]>23||arr[i-1]<0))` error apabila input jam selain 0-23.
 
 #### C. Program hanya menerima 1 config cron
 
 `if((timeinfo->tm_sec==arr[0] || arr[0]==-1) && (timeinfo->tm_min==arr[1] || arr[1]==-1) && (timeinfo->tm_hour==arr[2]|| arr[2]==-1))` hanya memproses 1 config cron sesuai urutan argumen yang telah diinput.
 
 #### D. Program berjalan di background (daemon)
 
 ```
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
```
Menggunakan Daemon dengan template sesuai modul

#### C. Tidak boleh menggunakan fungsi system()

Tidak menggunakan fungsi system() pada codingan, hanya menggunakan daemon, fork, exec, dan lain-lain yang diperbolehkan.

## No 3 Program C untuk Daemon

### Source Code : [2](https://github.com/marsellaeve/SoalShiftSISOP20_modul2_F02/tree/master/Soal2)
```
#include<stdio.h>
#include<time.h>
#include <sys/stat.h> 
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <sys/prctl.h>

void writeKillA();
void writeKillB();

int main(int argm, char **argmod)
{
    if(strcmp(argmod[1], "-a") == 0){
    writeKillA();
  }
    else if(strcmp(argmod[1], "-b") == 0){
    writeKillB();
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


void writeKillA() //program utama akan langsung menghentikan semua operasinya ketika program killer dijalankan.
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

void writeKillB() //program utama akan berhenti tapi membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua folder terisi gambar, terzip lalu di delete).
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

```

#### A. Membuat File tiap 30 detik dengan format timestamp [YYYY-mm-dd_HH:ii:ss].
```
    strftime(date, 26, "%Y-%m-%d_%H:%M:%S", tm_info);
    sprintf(namafile, "/home/rayroyy/%s", date);
    char *argv[] = {"mkdir", "-p", namafile, NULL};
```

#### B. Tiap-tiap folder lalu diisi dengan 20 gambar yang di download darihttps://picsum.photos/, dimana tiap gambar di download setiap 5 detik. Tiap gambar berbentuk persegi dengan ukuran (t%1000)+100 piksel dimana t adalah detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYY-mm-dd_HH:ii:ss].

```
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
```
### C. Setelah Folder terisi oleh 20 gambar, Folder akan di zip, dan folder asli akan dihapus dan menyisakan hanya folder zip
```
char namazip[150];
			sprintf(namazip, "%s.zip", namafile);

			pid_t child3 = fork();
			if (child3 == 0) {
				execl("/usr/bin/zip", "zip", "-r", namazip, namafile, NULL);
			}
			int status3;
			while (wait(&status3) > 0);	
			execl("/bin/rm", "rm", "-rf", namafile, NULL);
```

### D&E membuat sebuah program killer dimana memiliki 2 mode, yaitu mode A dan mode B

```
void writeKillA() //program utama akan langsung menghentikan semua operasinya ketika program killer dijalankan.
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

void writeKillB() //program utama akan berhenti tapi membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua folder terisi gambar, terzip lalu di delete).
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
```

## No 3 Program C untuk multiprocessing

### Source Code : [3](https://github.com/marsellaeve/SoalShiftSISOP20_modul2_F02/tree/master/soal3)

```
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
    while ((ent = readdir (dir)) != NULL) {
      int flag=0;
      if(strcmp(ent->d_name,".")==0) continue;
      if(strcmp(ent->d_name,"..")==0) continue;
      FILE *fptr1;
      char filename[]="/coba1.txt",filenamee[]="/coba2.txt";
      char text1[50]="/home/evelyn/modul2/jpg/";	
      char text2[50]="/home/evelyn/modul2/indomie/";
      strcat(text1,ent->d_name);
      strcat(text2,ent->d_name);
      if(ent->d_type == 4){
        flag=1;
      }
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
          execv("/bin/mv", argv);
        } 
      } else {
        // this is parent
        while ((wait(&status)) > 0);
        if(flag==1){
          strcat(text2,filename);
          fptr1=fopen(text2,"w");
          fprintf(fptr1," ");
          fclose(fptr1);
          sleep(3);	
          char text2[]="/home/evelyn/modul2/indomie/";
          strcat(text2,ent->d_name);
          strcat(text2,filenamee);
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
```

#### A. Buat 2 directory "indomie" kemudian 5 detik setelahnya "sedaap"

`char *argv[] = {"mkdir", "/home/evelyn/modul2/indomie", NULL};	execv("/bin/mkdir", argv);` fork pertama pada child mengeksekusi pembuatan directory indomie dengan mkdir, kemudian pada parentnya diberi sleep 5 detik sebelum fork berikutnya. Penggunaan fork karena setelah di exec proses tersebut akan mati, sehingga membutuhkan proses baru untuk melanjutkan pekerjaannya.

`char *argv[] = {"mkdir", "/home/evelyn/modul2/sedaap", NULL}; execv("/bin/mkdir", argv);` fork kedua pada child, melakukan eksekusi pembuatan directory sedaap dengan mkdir.

#### B. Mengekstrak jpg.zip pada modul 2

`char *argv[] = {"unzip", "/home/evelyn/Praktikum2/jpg.zip","-d","/home/evelyn/modul2/", NULL}; execv("/usr/bin/unzip", argv);` fork ketiga melakukan unzip untuk mengekstrak jpg.zip dengan exec. Secara otomatis akan terbuat folder dengan nama jpg.

#### C. Hasil esktrak, semua file dipindah ke sedaap dan semua direktori dipindah ke indomie

 ```
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
    char text1[50]="/home/evelyn/modul2/jpg/";	
    char text2[50]="/home/evelyn/modul2/indomie/";
    strcat(text1,ent->d_name);
    strcat(text2,ent->d_name);
    if(ent->d_type == 4){
    flag=1; }
  ```
Menggunakan dirent untuk membuat list isi dari folder jpg. Kemudian dari dirent, dapat diketahui type nya apabila folder typenya 4 dan bila file typenya 8. Karena dalam list dirent terdapat folder itu sendiri dan folder sebelumnya, maka di continue.

`if(ent->d_type == 4){char *argv[] = {"mv", text1 ,"/home/evelyn/modul2/indomie/", NULL}; execv("/bin/mv", argv);` apabila type nya 4 maka itu adalah directory sehingga dipindahkan ke directory indomie, begitu juga dengan file dipindah ke sedaap.

#### D. Membuat 2 file kosong pada setiap folder yang dipindahkan ke indomie setiap 3 detik dengan nama coba1.txt dan coba2.txt

```
char filename[]="/coba1.txt",filenamee[]="/coba2.txt";
char text2[50]="/home/evelyn/modul2/indomie/";
// this is parent
while ((wait(&status)) > 0);
if(flag==1){
strcat(text2,filename);
fptr1=fopen(text2,"w");
fprintf(fptr1," ");
fclose(fptr1);
sleep(3);	
char text2[]="/home/evelyn/modul2/indomie/";
strcat(text2,ent->d_name);
strcat(text2,filenamee);
fptr1=fopen(text2,"w");
fprintf(fptr1," ");
fclose(fptr1);
```
membuat file baru dengan fopen write, untuk nama text2 adalah directory indomie kemudian digabungkan dengan string nama filenya coba1.txt dan coba2.txt. Kemudian sebelum membuat coba2.txt, proses di slepp selama 3 detik. Process ini ada di parent fork pemindahan file dan directory sebelumnya.

## Authors

Created by:

[Rifki Aulia Irawan 142] (https://github.com/rifkiirawan)

[Evelyn Tjitrodjojo 99] (https://github.com/marsellaeve)
