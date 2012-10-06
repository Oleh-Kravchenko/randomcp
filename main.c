#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

const char usage[] = "-s SOURCE_DIR -d DEST_DIR -m MAX_DEST_DIR";

int main(int argc, char **argv)
{
	char src_path[0x100];
	char dst_path[0x100];
	off_t max_size;
	int param;

	*src_path = 0;
	*dst_path = 0;
	max_size = 0;

	while((param = getopt(argc, argv, "s:d:m:")) != -1)
	{
		switch(param)
		{
			case 's':
				strncpy(src_path, optarg, sizeof(src_path) - 1);
				src_path[sizeof(src_path) - 1] = 0;
				break;

			case 'd':
				strncpy(dst_path, optarg, sizeof(dst_path) - 1);
				dst_path[sizeof(dst_path) - 1] = 0;
				break;

			case 'm':
				max_size = atoi(optarg);
				break;

			default:
				printf("%s\n\t%s\n", argv[0], usage);

				return(1);
		}
	}

	printf("src_path = %s, dst_path = %s, max_size of dst_path = %zd\n", src_path, dst_path, max_size);

	if(!*src_path || !*dst_path || !max_size)
	{
		printf("%s\n\t%s\n", argv[0], usage);

		return(1);
	}

	return(0);
}
