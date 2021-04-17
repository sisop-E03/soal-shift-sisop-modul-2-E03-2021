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

    while (1)
    {
        time_t t = time(NULL);
        struct tm now = *localtime(&t);

        if (now.tm_mday == bdayMday && now.tm_mon == bdayMon && now.tm_hour == bdayHour - 6 &&
            now.tm_min == bdayMin && now.tm_sec == bdaySec)
        {
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
        }

        if (now.tm_mday == bdayMday && now.tm_mon == bdayMon && now.tm_hour == bdayHour &&
            now.tm_min == bdayMin && now.tm_sec == bdaySec)
        {
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
        }

        sleep(1);
    }
}
