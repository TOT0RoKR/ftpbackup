#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILENAME "Pictures.tar"
int main()
{

	FILE *fd, *log;
	time_t ti;
	struct tm t;

	log = fopen("ftp.log", "a");

	while(1){
		char cmd[1024];
		char dir[1024];
		do {
			sleep(60);
			fd = fopen(FILENAME, "rb");
			//printf("작동중\n");
		} while(fd == NULL);
		fclose(fd);

		ti = time(NULL);
		t = *localtime(&ti);
		sprintf(dir, "%04d-%02d-%02d_%02d:%02d:%02d_%s"
			, t.tm_year + 1900, t.tm_mon + 1, t.tm_mday
			, t.tm_hour, t.tm_min, t.tm_sec, FILENAME);

		fprintf(log, "run : %s", dir);

		sprintf(cmd, "mkdir %s", dir);

		system(cmd);
		
		sprintf(cmd, "mv %s %s", FILENAME, dir);

		system(cmd);

		sprintf(cmd, "chown -R ftp:ftp %s", dir);

		system(cmd);
	}

	return 0;
}

