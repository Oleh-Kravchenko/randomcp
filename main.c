#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "conf.h"

/*------------------------------------------------------------------------*/

typedef struct file_info_s
{
	char file[0x100];

	off_t size;
} file_info_t;

const char usage[] = "-s SOURCE_DIR -d DEST_DIR -m MAX_DEST_DIR";

/*------------------------------------------------------------------------*/

conf_t conf;

file_info_t *fi = NULL;
size_t fi_cnt = 0;

/*------------------------------------------------------------------------*/

void scan_dir(const char* path)
{
	DIR *src;
	struct dirent *ent;
	char new_path[0x100];
	struct stat st;
	
	if(!(src = opendir(path)))
	{
		perror(path);
		return;
	}

	while(ent = readdir(src))
	{
		if(!strcmp(ent->d_name, "."))
			continue;

		if(!strcmp(ent->d_name, ".."))
			continue;

		if(ent->d_type == DT_REG)
		{
			snprintf(new_path, sizeof(new_path), "%s%s", path, ent->d_name);

			if(!stat(new_path, &st))
			{
				fi = realloc(fi, sizeof(file_info_t) * (fi_cnt + 1));

				strncpy(fi[fi_cnt].file, new_path, sizeof(fi[fi_cnt].file) - 1);
				fi[fi_cnt].file[sizeof(fi[fi_cnt].file) - 1] = 0;

				fi[fi_cnt].size = st.st_size;

				++ fi_cnt;
			}
			else
				perror(new_path);
		}
		else if(ent->d_type == DT_DIR)
		{
			snprintf(new_path, sizeof(new_path), "%s%s/", path, ent->d_name);

			scan_dir(new_path);
		}
	}

	closedir(src);
}

/*------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
	off_t total_size;
	size_t i, j;

	if(conf_parse_cmdline(argc, argv, &conf))
	{
		puts(usage);

		return(1);
	}

	scan_dir(conf.src_path);

	srand(time(NULL));

	total_size = 0;

	for(i = 0; i < fi_cnt && total_size < conf.dst_size; ++ i)
	{
		j = rand() % fi_cnt;

		total_size += fi[j].size;

		if(total_size > conf.dst_size)
			continue;

		puts(fi[j].file);
	}

	free(fi);

	return(0);
}
