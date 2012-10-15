#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "conf.h"

/*------------------------------------------------------------------------*/

typedef struct file_item_s
{
	char file[0x100];

	off_t size;
} file_item_t;

/*------------------------------------------------------------------------*/

file_item_t *files_list = NULL;

size_t files_list_cnt = 0;

conf_t conf;

/*------------------------------------------------------------------------*/

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

void scan_directory(const char* path)
{
	file_item_t *files_list_new;
	struct dirent *item;
	struct stat st;
	char s[0x100];
	DIR *dir;
	
	if(!(dir = opendir(path)))
		return;

	while(item = readdir(dir))
	{
		/* file */
		if(item->d_type == DT_REG)
		{
			snprintf(s, sizeof(s), "%s%s", path, item->d_name);

			/* file size */
			if(stat(s, &st))
				continue;

			if(!(files_list_new = realloc(files_list, sizeof(file_item_t) * (files_list_cnt + 1))))
				goto err;

			files_list = files_list_new;

			/* adding new file info */
			strncpy(files_list[files_list_cnt].file, s, sizeof(files_list[files_list_cnt].file) - 1);
			files_list[files_list_cnt].file[sizeof(files_list[files_list_cnt].file) - 1] = 0;

			files_list[files_list_cnt].size = st.st_size;

			++ files_list_cnt;
		}
		/* directory */
		else if(item->d_type == DT_DIR)
		{
			if(!strcmp(item->d_name, "."))
				continue;

			if(!strcmp(item->d_name, ".."))
				continue;

			snprintf(s, sizeof(s), "%s%s/", path, item->d_name);

			/* recursive call */
			scan_directory(s);
		}
	}

err:
	closedir(dir);
}

/*------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
	char s[0x100], path[0x100];
	off_t total_size;
	size_t i;

	puts(license);

	if(conf_parse_cmdline(argc, argv, &conf))
	{
		printf("Usage:\n%s %s\n", argv[0], usage);

		return(1);
	}

	scan_directory(conf.src_path);

	/* initialize random generator */
	srand(time(NULL));

	total_size = 0;

	while(files_list_cnt && total_size < conf.dst_size)
	{
		i = rand() % files_list_cnt;

		total_size += files_list[i].size;

		if(total_size > conf.dst_size)
			continue;

		/* file name for destination */
		strncpy(s, files_list[i].file, sizeof(s) - 1);
		s[sizeof(s) - 1] = 0;

		snprintf(path, sizeof(path), "%s%s", conf.dst_path, basename(s));

		/* coping file */
		printf("%s\t- %s\n", fcopy(files_list[i].file, path) ? "Failed" : "Successful", files_list[i].file);

		/* removing file info from the list */
		if(files_list_cnt > 1)
			memmove(files_list + i, files_list + i + 1, files_list_cnt - 1);

		-- files_list_cnt;
	}

	free(files_list);

	return(0);
}
