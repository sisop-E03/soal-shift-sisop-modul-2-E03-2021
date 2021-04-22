# soal-shift-sisop-modul-2-E03-2021

## Soal 1
### Penjelasan soal
Pada suatu masa, hiduplah seorang Steven yang hidupnya pas-pasan. Steven punya pacar, namun sudah putus sebelum pacaran. 
Ketika dia galau memikirkan mantan, ia selalu menonton https://www.youtube.com/watch?v=568DH_9CMKI untuk menghilangkan kesedihannya. 
Di lain hal Steven anak yang tidak amat sangat super membenci matkul sisop, beberapa jam setelah diputus oleh pacarnya dia menemukan wanita lain bernama Stevany, 
namun Stevany berkebalikan dengan Steven karena menyukai sisop. Steven ingin terlihat jago matkul sisop demi menarik perhatian Stevany.
Pada hari ulang tahun Stevany, Steven ingin memberikan Stevany zip berisikan hal-hal yang disukai Stevany. 
Steven ingin isi zipnya menjadi rapi dengan membuat folder masing-masing sesuai extensi. 

A. Dikarenakan Stevany sangat menyukai huruf Y, Steven ingin nama folder-foldernya adalah Musyik untuk mp3, Fylm untuk mp4, dan Pyoto untuk jpg 

B. untuk musik Steven mendownloadnya dari link di bawah, film dari link di bawah lagi, dan foto dari link dibawah juga :). 

C. Steven tidak ingin isi folder yang dibuatnya berisikan zip, sehingga perlu meng-extract-nya setelah didownload serta 

D. memindahkannya ke dalam folder yang telah dibuat (hanya file yang dimasukkan).

E. Untuk memudahkan Steven, ia ingin semua hal di atas berjalan otomatis 6 jam sebelum waktu ulang tahun Stevany).

F. Setelah itu pada waktu ulang tahunnya Stevany, semua folder akan di zip dengan nama Lopyu_Stevany.zip dan semua folder akan di delete(sehingga hanya menyisakan .zip).
   Kemudian Steven meminta bantuanmu yang memang sudah jago sisop untuk membantunya mendapatkan hati Stevany. Bantu Woy!!

### Penyelesaian
```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
```
Di awal, dilakukan include header files yang diperlukan.
```c
int main()
{
    pid_t pid, sid;

    pid = fork();

    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    if ((chdir("/tmp")) < 0)
    {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

	...

}
```
Di dalam fungsi main, program dibuat menjadi daemon process sehingga bisa berjalan di background.
```c
int bdayMday = 9;
int bdayMon = 4 - 1;
int bdayHour = 22;
int bdayMin = 22;
int bdaySec = 0;

char *filmPath = "./Fylm";
char *photoPath = "./Pyoto";
char *musicPath = "./Musyik";
char *filmZip = "film.zip";
char *photoZip = "photo.zip";
char *musicZip = "music.zip";
```
Variabel yang digunakan untuk menyimpan ulang tahun, nama folder, dan nama file zip dibuat.
```c
while (1)
    {
        time_t t = time(NULL);
        struct tm now = *localtime(&t);

        if (now.tm_mday == bdayMday && now.tm_mon == bdayMon && now.tm_hour == bdayHour - 6 &&
            now.tm_min == bdayMin && now.tm_sec == bdaySec)
        {
            ...
        }

        if (now.tm_mday == bdayMday && now.tm_mon == bdayMon && now.tm_hour == bdayHour &&
            now.tm_min == bdayMin && now.tm_sec == bdaySec)
        {
            ...
        }

        sleep(1);
    }
```
Di dalam loop utama, pertama waktu saat ini diambil dan disimpan ke variable `now`. Kemudian dilakukan pengecekan apakah waktu saat ini 6 jam sebelum ulang tahun atau sama dengan ulang tahun. Loop utama ini dijalankan setiap satu detik.
```c
pid_t pid1, pid2;

pid1 = fork();
if (pid1 < 0)
{
    exit(EXIT_FAILURE);
}

pid2 = fork();
if (pid2 < 0)
{
    exit(EXIT_FAILURE);
}

if (pid1 == 0 && pid2 > 0)
{
    while (wait(NULL) > 0);
    
    char *argv[] = {"wget", "--no-check-certificate", "-o", "/dev/null",
                    "https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download",
                    "-O", musicZip, NULL};
    execv("/bin/wget", argv);
}
else if (pid1 > 0 && pid2 == 0)
{
    char *argv[] = {"wget", "--no-check-certificate", "-o", "/dev/null",
                    "https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download",
                    "-O", photoZip, NULL};
    execv("/bin/wget", argv);
}
else if (pid1 == 0 && pid2 == 0)
{
    char *argv[] = {"wget", "--no-check-certificate", "-o", "/dev/null",
                    "https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download",
                    "-O", filmZip, NULL};
    execv("/bin/wget", argv);
}

while (wait(NULL) > 0);
```
Jika waktu saat ini 6 jam kurang dari ulang tahun, maka program akan di-fork 2 kali sehingga mempunyai 2 child dan 1 grandchild. Di masing-masing process tersebut dilakukan download file zip film, foto dan musik yang akan disimpan dengan nama sesuai dengan variable `filmZip`, `photoZip`, dan `musicZip`. Tetapi di child yang pertama, akan menunggu process grandchild selesai sebelum menjalankan download. Parent process juga akan menunggu semua process child selesai sebelum melanjutkan eksekusi.
```c
pid1 = fork();
if (pid1 < 0)
{
    exit(EXIT_FAILURE);
}

pid2 = fork();
if (pid2 < 0)
{
    exit(EXIT_FAILURE);
}

if (pid1 == 0 && pid2 > 0)
{
    while (wait(NULL) > 0);

    char *argv[] = {"unzip", "-j", musicZip, "*.mp3", "-d", musicPath, NULL};
    execv("/bin/unzip", argv);
}
else if (pid1 > 0 && pid2 == 0)
{
    char *argv[] = {"unzip", "-j", photoZip, "*.jpg", "-d", photoPath, NULL};
    execv("/bin/unzip", argv);
}
else if (pid1 == 0 && pid2 == 0)
{
    char *argv[] = {"unzip", "-j", filmZip, "*.mp4", "-d", filmPath, NULL};
    execv("/bin/unzip", argv);
}
```
Setelah download selesai, maka akan parent akan kembali di-fork 2 kali. Di masing-masing keturunannya, akan dilakukan unzip file-file zip ke dalam folder yang disimpan di variable `filmPath`, `photoPath`, `musicPath`. Dalam kasus dimana folder belum ada, maka unzip akan membuat folder-folder tersebut. Sama seperti sebelumnya, child pertama akan menunggu process grandchild selesai dulu sebelum menjalankan unzip.
```c
pid_t pid1;

pid1 = fork();
if (pid1 < 0)
{
    exit(EXIT_FAILURE);
}

if (pid1 == 0)
{
    char *username = malloc(33 * sizeof(char));
    cuserid(username);

	 char *targetPath = malloc(50 * sizeof(char));
	 sprintf(targetPath, "/home/%s/Lopyu_Stevany.zip", username);

    char *argv[] = {"zip", "-r", targetPath, filmPath, photoPath, musicPath, NULL};
    execv("/bin/zip", argv);
}

while (wait(NULL) > 0);
```
Apabila waktu saat ini sama dengan waktu ulang tahun, program akan di-fork. Di dalam process anaknya, pertama username dari user yang menjalankan program diambil kemudian digabungkan dengan nama zip akhirnya dan disimpan di `targetPath`. Kemudian folder film, foto, dan musik di-zip dengan nama yang sesuai dengan `targetPath`. Parent process akan menunggu hingga proses zip selesai.
```c
pid1 = fork();
if (pid1 < 0)
{
    exit(EXIT_FAILURE);
}

if (pid1 == 0)
{
    char *argv[] = {"rm", "-r", filmPath, photoPath, musicPath,
                    filmZip, photoZip, musicZip, NULL};
    execv("/bin/rm", argv);
}
```
Setelah zip selesai, maka program kembali di-fork. folder dan file zip yang dihasilkan process sebelumnya kecuali file zip akhir akan dihapus di process anaknya. 

## Soal 2
### Penjelasan soal
Loba bekerja di sebuah petshop terkenal, suatu saat dia mendapatkan zip yang berisi banyak sekali foto peliharaan dan 
Ia diperintahkan untuk mengkategorikan foto-foto peliharaan tersebut. Loba merasa kesusahan melakukan pekerjaanya secara manual, 
apalagi ada kemungkinan ia akan diperintahkan untuk melakukan hal yang sama. Kamu adalah teman baik Loba dan Ia meminta bantuanmu untuk membantu pekerjaannya.

A. Pertama-tama program perlu mengextract zip yang diberikan ke dalam folder “/home/[user]/modul2/petshop”. Karena bos Loba teledor, 
   dalam zip tersebut bisa berisi folder-folder yang tidak penting, 
   maka program harus bisa membedakan file dan folder sehingga dapat memproses file yang seharusnya dikerjakan dan menghapus folder-folder yang tidak dibutuhkan.

B. Foto peliharaan perlu dikategorikan sesuai jenis peliharaan, maka kamu harus membuat folder untuk setiap jenis peliharaan yang ada dalam zip.
   Karena kamu tidak mungkin memeriksa satu-persatu, maka program harus membuatkan folder-folder yang dibutuhkan sesuai dengan isi zip.
   Contoh: Jenis peliharaan kucing akan disimpan dalam “/petshop/cat”, jenis peliharaan kura-kura akan disimpan dalam “/petshop/turtle”.

C. Setelah folder kategori berhasil dibuat, programmu akan memindahkan foto ke folder dengan kategori yang sesuai dan di rename dengan nama peliharaan.
   Contoh: “/petshop/cat/joni.jpg”. 

D. Karena dalam satu foto bisa terdapat lebih dari satu peliharaan maka foto harus di pindah ke masing-masing kategori yang sesuai.
   Contoh: foto dengan nama “dog;baro;1_cat;joni;2.jpg” dipindah ke folder “/petshop/cat/joni.jpg” dan “/petshop/dog/baro.jpg”.

E. Di setiap folder buatlah sebuah file "keterangan.txt" yang berisi nama dan umur semua peliharaan dalam folder tersebut. Format harus sesuai contoh.

### Penyelesaian
Pertama mendownload zip pets.zip terlebih dahulu dari drive sisop21-modul 2

Lalu include header files
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <wait.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
```
membuat fungsi jalan untuk menjalankan perintah nantinya ( menggabungkan fork dan exec)
```c
void jalan(char perintah[], char *argv[]){
    pid_t child_id;
    int status;
    child_id = fork();
    if(child_id == 0){
        execv(perintah, argv);}
    else{
        while ((wait(&status)) > 0);}
}
```
lalu membuat folder petshop di modul 2 sebagai directory pets.zip yang diextract nantinya. Setelah folder dibuat, maka pets.zip di unzip.
Menggunakan command ( "-x", "*/*" )Karena  yang ingin di unzip adalah yang berupa file saja (yg berupa folder tidak di unzip).
```c
void duaa()
{
    int status;
    pid_t child;
    child = fork();
    if(child == 0)
    {
        char *argv[] = {"mkdir","-p", "/home/vika/modul2/petshop", NULL};
        jalan("/bin/mkdir", argv);
    }
    else
    {
        while ((wait(&status)) > 0);
        
        char *argv2[] = {"unzip","-q","/home/vika/modul2/pets.zip","-x","*/*","-d" ,"/home/vika/modul2/petshop",NULL};
        jalan("/bin/unzip", argv2);
    }
}
```
setelah di unzip, file-file ini pun di kelompokkan berdasarkan jenisnya ( cat,dog,turtle, dll ). maka harus mengecek terlebih dahulu
jenis hewannya berdasarkan nama filenya. Memisahkan terlebih dahulu nama file dari .jpg mengunakan fungsi cutnama
```c
char* cutnama (char* s){
    int i;
    char* new;
    for (i = 0; s[i] != '\0'; i++);
    // lenght of the new string
    int n;
    n = i - 4 + 1;
    if (n < 1)
        return NULL;
    new = (char*) malloc (n * sizeof(char));
    for (i = 0; i < n - 1; i++)
        new[i] = s[i];
    new[i] = '\0';
    return new;
}
```
Karena ada 1 foto memiliki 2 jenis dan nama (dipisahkan oleh ``_``) maka harus dipisahkan terlebih dahulu.
Setelah itu antara jenis hewan; nama; umur juga dipisahkan menggunakan strtok. Setelah itu membuat folder berdasarkan jenis
hewan yang sudah terdeteksi.
```c
void duabcde()
{
    int status;
    pid_t child_id;
    
    child_id = fork();
    if(child_id == 0){
        DIR *dir;
        dir=opendir("/home/vika/modul2/petshop");
        struct dirent *infol;
        if(dir != NULL){
            while((infol = readdir(dir)) != NULL){
                if(infol->d_type == DT_REG){
                    int i;
                    char* hapus = cutnama(infol->d_name) ;// ini yg delete .jpg
                    char *s1,*x;
                    
                    while ((s1=strtok_r(hapus, "_", &hapus))!=NULL) {//misahin yg ngandung 2 nama
                        char base[100] = "/home/vika/modul2/petshop/";
                        
                        char hewan[20], nama[20], umur[20];
                        char *s2;
                        char *x = s1;
                        i=0;
                        //misahin antara id(jenis hewan,nama,umur)
                            while((s2=strtok_r(s1, ";", &s1))!=NULL) {
                                if (i == 0){
                                    strcpy(hewan, s2);}
                                if (i == 1){
                                    strcpy(nama, s2);}
                                if (i == 2) {
                                    strcpy(umur, s2);}
                                i++;
                            }

                        char temp[100], temp2[100], temp3[100];
                        char base2[100], base3[100];
                        
                        //pencadangan
                        strcpy(base2, base);
                        strcpy(base3, base);
                        strcpy(temp, infol->d_name);
                        strcpy(temp2, infol->d_name);
                        strcpy(temp3, infol->d_name);
                        
                        //Membuat folder sesuai hewan
                        strcat(base, hewan);
                        char *newfolder[] = {"mkfolder", "-p", base, NULL};
                        jalan("/bin/mkdir", newfolder);
```
Setelah folder sudah dibuat,foto tersebut dipindahkan sesuai folder jenisnya dan diberinama [namahewan].jpg.
Karena ada 1 foto untuk 2 nama, maka kita harus mengcopynya terlebih dahulu. Foto-foto yang nantinya tidak digunakan akan dihapus di akhir.
```c
char namahewan[20], id[10];
                        strcpy(namahewan, nama);
                        
                        //ngopy file ke folder masing" karena kan ada 1 foto 2 hewan
                        strcat(base2, temp2);
                        char *masukfolder[] = {"copy", base2, base, NULL};
                        jalan("/bin/cp", masukfolder);
                        
                        //trus ini rename nama jadi jpg
                        strcat(base, "/");
                        strcpy(base3, base);
                        strcat(base3, temp2);
                        strcat(nama,".jpg");
                        strcat(base, nama);
                        
                        //ini baru dipindahin sesuai nama.jpg
                        char *newnama[] = {"mv", base3, base, NULL};
                        jalan("/bin/mv", newnama);
```

Membuat file keterangan.txt di tiap folder yang nantinya berisi data-data hewan yaitu nama dan umur hewan tersebut sesuai format yang ditentukan
di soal.
```c
char txtLoc[100], ket[100];
                        strcpy(txtLoc, base);
                        stpcpy(ket, txtLoc);
			
//Memasukkan elemen file ke dalam txt
                        strcat(ket, "/keterangan.txt");
                        strcpy(id, "nama : ");
                        strcat(id, namahewan);
                        strcat(id, "\numur : ");
                        strcat(id, umur);
                        strcat(id, " tahun\n\n");

                        FILE *fPtr = fopen(ket, "a");
                        fputs(id, fPtr);
                        fclose(fPtr);
                    }       
                }
```

Lalu file-file yang tidak digunakan dihapus dari folder petshop.
```c
//hapus file gak butuh
                if (infol->d_type == DT_REG){
                    char base[99] = "/home/vika/modul2/petshop/";
                    strcat(base, infol->d_name);
                 char *sisa[] = {"remove", base, NULL};
                 jalan("/bin/rm", sisa);
                }
            }
        }
        closedir(dir);
    }
    else{
        while ((wait(&status)) > 0);}
}
```
### Dokumentasi
isi folder petshop yang berisi folder jenis-jenis hewan.
![alt text](https://github.com/sisop-E03/soal-shift-sisop-modul-2-E03-2021/blob/94aba64921ebffbbffd996bbd5a223803513e3ff/soal2/isi%20petshop.jpg)

salah satu isi folder jenis hewan (folder dog) yang berisi foto-foto dog dan keterangannya.
![alt text](https://github.com/sisop-E03/soal-shift-sisop-modul-2-E03-2021/blob/94aba64921ebffbbffd996bbd5a223803513e3ff/soal2/isi%20dog.jpg)

### Kendala
1. Sempat error untuk bikin folder hewan (hanya bikin satu folder atau tidak sama sekali)
2. Foto tidak pindah sesuai jenis hewan
3. Jumlah nama hewan di keterangan tidak sesuai

## Soal 3
### Penjelasan soal
Ranora adalah mahasiswa Teknik Informatika yang saat ini sedang menjalani magang di perusahan ternama yang bernama “FakeKos Corp.”, 
perusahaan yang bergerak dibidang keamanan data. Karena Ranora masih magang, maka beban tugasnya tidak sebesar beban tugas pekerja tetap perusahaan. 
Di hari pertama Ranora bekerja, pembimbing magang Ranora memberi tugas pertamanya untuk membuat sebuah program.

A. Ranora harus membuat sebuah program C yang dimana setiap 40 detik membuat sebuah direktori dengan nama sesuai timestamp [YYYY-mm-dd_HH:ii:ss].

B. Setiap direktori yang sudah dibuat diisi dengan 10 gambar yang didownload dari https://picsum.photos/, dimana setiap gambar akan didownload setiap 5 detik. 
   Setiap gambar yang didownload akan diberi nama dengan format timestamp [YYYY-mm-dd_HH:ii:ss] dan gambar tersebut berbentuk persegi dengan ukuran (n%1000) + 50 pixel 
   dimana n adalah detik Epoch Unix.
   
C.  Setelah direktori telah terisi dengan 10 gambar, program tersebut akan membuat sebuah file “status.txt”, dimana didalamnya berisi pesan “Download Success” 
	yang terenkripsi dengan teknik Caesar Cipher dan dengan shift 5. Caesar Cipher adalah Teknik enkripsi sederhana yang dimana dapat melakukan enkripsi string 
	sesuai dengan shift/key yang kita tentukan. Misal huruf “A” akan dienkripsi dengan shift 4 maka akan menjadi “E”. 
	Karena Ranora orangnya perfeksionis dan rapi, dia ingin setelah file tersebut dibuat, direktori akan di zip dan direktori akan didelete, sehingga menyisakan 
	hanya file zip saja.
	
D.  Untuk mempermudah pengendalian program, pembimbing magang Ranora ingin program tersebut akan men-generate sebuah program “Killer” yang executable, 
	dimana program tersebut akan menterminasi semua proses program yang sedang berjalan dan akan menghapus dirinya sendiri setelah program dijalankan. 
	Karena Ranora menyukai sesuatu hal yang baru, maka Ranora memiliki ide untuk program “Killer” yang dibuat nantinya harus merupakan program bash.

E.  Pembimbing magang Ranora juga ingin nantinya program utama yang dibuat Ranora dapat dijalankan di dalam dua mode. Untuk mengaktifkan mode pertama, program harus dijalankan 	dengan argumen -z, dan Ketika dijalankan dalam mode pertama, program utama akan langsung menghentikan semua operasinya Ketika program Killer dijalankan. 
	Sedangkan untuk mengaktifkan mode kedua, program harus dijalankan dengan argumen -x, dan Ketika dijalankan dalam mode kedua, program utama akan berhenti namun membiarkan 		proses di setiap direktori yang masih berjalan hingga selesai (Direktori yang sudah dibuat akan mendownload gambar sampai selesai dan membuat file txt, lalu zip dan delete 	direktori).

