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

void jalan(char perintah[], char *argv[]){
    pid_t child_id;
    int status;
    child_id = fork();
    if(child_id == 0){
        execv(perintah, argv);}
    else{
        while ((wait(&status)) > 0);}
}

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
        //char *ar[] = {"rm", "/home/vika/modul2/pets.zip",NULL};
        //jalan("/bin/rm", ar);
    }
}

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
                        
                        char txtLoc[100], ket[100];
                        strcpy(txtLoc, base);
                        stpcpy(ket, txtLoc);

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

int main(){
    pid_t child_id;
    int status;
    child_id = fork();
    if (child_id < 0){
        exit(EXIT_FAILURE);
        }
    if (child_id == 0){
        duaa();}
    else{
        while ((wait(&status)) > 0);
        duabcde();}
}