#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "conf.h"

/*------------------------------------------------------------------------*/

typedef struct file_info_s
{
	char file[0x100];

	off_t size;
} file_info_t;

const char license[] =
	"randomcp Copyright (C) 2012 Oleh Kravchenko\n"
	"This program comes with ABSOLUTELY NO WARRANTY\n"
	"This is free software, and you are welcome to redistribute it "
	"under certain conditions\n";

const char usage[] =
	"-s SOURCE_DIR -d DEST_DIR -m MAX_DEST_DIR\n"
	"-s - source directoty\n"
	"-d - destination directory\n"
	"-m - maximum size of destination directory\n";

/*------------------------------------------------------------------------*/

conf_t conf;

file_info_t *fi = NULL;
size_t fi_cnt = 0;
size_t fi_size = 0;

/*------------------------------------------------------------------------*/

void scan_directory(const char* path)
{
	struct dirent *item;
	struct stat st;
	char s[0x100];
	DIR *dir;
	
	if(!(dir = opendir(path)))
		return;

	while(item = readdir(dir))
	{
		if(item->d_type == DT_REG)
		{
			snprintf(s, sizeof(s), "%s%s", path, item->d_name);

			if(stat(s, &st))
				continue;

			fi = realloc(fi, sizeof(file_info_t) * (fi_cnt + 1));

			strncpy(fi[fi_cnt].file, s, sizeof(fi[fi_cnt].file) - 1);
			fi[fi_cnt].file[sizeof(fi[fi_cnt].file) - 1] = 0;

			fi[fi_cnt].size = st.st_size;

			++ fi_cnt;
		}
		else if(item->d_type == DT_DIR)
		{
			if(!strcmp(item->d_name, "."))
				continue;

			if(!strcmp(item->d_name, ".."))
				continue;

			snprintf(s, sizeof(s), "%s%s/", path, item->d_name);

			scan_directory(s);
		}
	}

	closedir(dir);
}

/*------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
	off_t total_size;
	size_t i, j;
	char s[0x100], path[0x100];

	puts(license);

	if(conf_parse_cmdline(argc, argv, &conf))
	{
		printf("Usage:\n%s %s\n", argv[0], usage);

		return(1);
	}

	/* TODO: code cleanup */

	scan_directory(conf.src_path);

	/* initialize random generator */
	srand(time(NULL));

	total_size = 0;

	for(i = 0; i < fi_cnt && total_size < conf.dst_size; ++ i)
	{
		j = rand() % fi_cnt;

		total_size += fi[j].size;

		if(total_size > conf.dst_size)
			continue;

		strncpy(s, fi[j].file, sizeof(s) - 1);
		s[sizeof(s) - 1] = 0;

		snprintf(path, sizeof(path), "%s%s", conf.dst_path, basename(s));

		printf("%s\t- %s\n", fcopy(fi[j].file, path) ? "Failed" : "Successful", fi[j].file);
	}

	free(fi);

	return(0);
}
