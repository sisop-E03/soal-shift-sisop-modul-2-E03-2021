#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <wait.h>
#include <time.h>
#include <sys/prctl.h>
#include <signal.h>

void task(char dirname[]);

int main(int argc, char *argv[]) {

	if (argc != 2 || (argv[1][1] != 'x' && argv[1][1] != 'z')) {
		printf("argumen tidak valid.\npilih -x atau -z\n\n");
		exit(0);
	}

	FILE *killer;
	killer = fopen("killer.sh", "w");
	if (argv[1][1] == 'x') {
//		prctl(PR_SET_PDEATHSIG, SIGHUP);
		fprintf(killer, "#!/bin/bash\nps -ef | grep soal3 | grep -v grep | awk 'NR==1{print $2}' | xargs kill");
	} else {
		fprintf(killer, "#!/bin/bash\nps -ef | grep soal3 | grep -v grep | awk '{print $2}' | xargs kill");
	}

	fclose(killer);

	int status_killer;
	pid_t cid_killer;
	cid_killer = fork();
	if (cid_killer < 0) {
		exit(EXIT_FAILURE);
	}
	if (cid_killer == 0) {
		char *args[] = {"chmod", "+x", "killer.sh", NULL};
		execv("/bin/chmod", args);
	}
	while(wait(&status_killer) > 0);

	pid_t pid, sid;
	
	pid = fork();

	if (pid < 0) {
		exit(EXIT_FAILURE);
	}

	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	umask(0);

	sid = setsid();
	if (sid < 0) {
		exit(EXIT_FAILURE);
	}

	/*
	if ((chdir("/")) < 0) {
		exit(EXIT_FAILURE);
	}
	*/

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	
	int n = 5, i;

	while (n--) {
		pid_t cid_1, cid_mode;
		int status_1, status_mode;

		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		char dirname[50];
		sprintf(dirname, "%d-%02d-%02d_%02d:%02d:%02d",
				tm.tm_year + 1900, 
				tm.tm_mon + 1,
				tm.tm_mday,
				tm.tm_hour,
				tm.tm_min,
				tm.tm_sec);
		
		// 3a
		cid_1 = fork();

		if (cid_1 < 0) {
			exit(EXIT_FAILURE);
		}

		if (cid_1 == 0) {
			char *args[] = {"mkdir", dirname, NULL};
			execv("/bin/mkdir", args);
		}

		while(wait(&status_1) > 0);

		cid_mode = fork();
		
		if (cid_mode < 0) {
			exit(EXIT_FAILURE);
		}

		if (cid_mode == 0) {
			task(dirname);
		}
		sleep(40);
	}	
}

void task(char dirname[]) {

	pid_t cid_2, cid_3, cid_4, cid_5;
	int status_2, status_3;

	int i;
	// 3b

	chdir(dirname);
	for (i=0; i<10; i++) {
		time_t t_2 = time(NULL);
		struct tm tm_2 = *localtime(&t_2);
		char filename[50], link[50];
		sprintf(filename, "%d-%02d-%02d_%02d:%02d:%02d",
				tm_2.tm_year + 1900, 
				tm_2.tm_mon + 1,
				tm_2.tm_mday,
				tm_2.tm_hour,
				tm_2.tm_min,
				tm_2.tm_sec);
		sprintf(link, "https://picsum.photos/%ld", (t_2%1000)+50);

		cid_2 = fork();

		if (cid_2 < 0) {
			exit(EXIT_FAILURE);
		}
		
		if (cid_2 == 0) {
			char *args[] = {"wget", link, "-O", filename, NULL};
			execv("/usr/bin/wget", args);
		}
		sleep(5);

	}

	while(wait(&status_2) > 0);

	// 3c
	FILE *fp;
	int key;
	char ch, zipname[50];

	fp = fopen("status.txt", "w");
	
	if (fp == NULL) {
		exit(EXIT_FAILURE);
	}

	char text_status[50] = "Download Success";
	key = 5;
	// Caesar chiper
	for(i = 0; text_status[i] != '\0'; ++i){
		ch = text_status[i];

		if(ch >= 'a' && ch <= 'z'){
			ch = ch + key;

			if(ch > 'z'){
				ch = ch - 'z' + 'a' - 1;
			}

			text_status[i] = ch;
		}
		else if(ch >= 'A' && ch <= 'Z'){
			ch = ch + key;

			if(ch > 'Z'){
				ch = ch - 'Z' + 'A' - 1;
			}

			text_status[i] = ch;
		}
	}

	
	fputs(text_status, fp);
	fclose(fp);

	chdir("..");

	cid_3 = fork();

	strcpy(zipname, dirname);
	strcat(zipname, ".zip");
	if (cid_3 < 0) {
		exit(EXIT_FAILURE);
	}

	if (cid_3 == 0) {
		char *args[] = {"zip", "-r", zipname, dirname, NULL};
		execv("/usr/bin/zip", args);
	}
	if (cid_3 > 0) {
		while(wait(&status_3) > 0);
		cid_4 = fork();
		if (cid_4 < 0) {
			exit(EXIT_FAILURE);
		}
		if (cid_4 == 0) {
			char *args[] = {"rm", "-r", dirname, NULL};
			execv("/usr/bin/rm", args);
		}
	}
	exit(0);
}
