#include <getopt.h>
#include <string.h>

#include "re.h"
#include "conf.h"

/*------------------------------------------------------------------------*/

off_t filesize_atoi(const char* s)
{
	regmatch_t* pmatch;
	size_t nmatch;
	off_t result;

	if(re_parse(s, "([0-9]+)([kmgKMG]?)", &nmatch, &pmatch))
		return(0);

	result = re_atoi(s, pmatch + 1);

	/* suffix for size */
	switch(re_char(s, pmatch + 2))
	{
		/* 10 ^ 3 */
		case 'k':
			result *= 1000;
			break;

		/* 10 ^ 6 */
		case 'm':
			result *= 1000000;
			break;

		/* 10 ^ 9 */
		case 'g':
			result *= 1000000000;
			break;

		/* 2 ^ 10 */
		case 'K':
			result *= 0x400;
			break;

		/* 2 ^ 20 */
		case 'M':
			result *= 0x100000;
			break;

		/* 2 ^ 30 */
		case 'G':
			result *= 0x40000000;
			break;
	}

	return(result);
}

/*------------------------------------------------------------------------*/

int conf_parse_cmdline(int argc, char **argv, conf_t* conf)
{
	int key;

	memset(conf, 0, sizeof(*conf));

	while((key = getopt(argc, argv, "s:d:m:")) != -1)
	{
		switch(key)
		{
			case 's':
				strncpy(conf->src_path, optarg, sizeof(conf->src_path) - 1);
				conf->src_path[sizeof(conf->src_path) - 1] = 0;
				break;

			case 'd':
				strncpy(conf->dst_path, optarg, sizeof(conf->dst_path) - 1);
				conf->dst_path[sizeof(conf->dst_path) - 1] = 0;
				break;

			case 'm':
				conf->dst_size = filesize_atoi(optarg);
				break;

			default:
				return(-1);
		}
	}

	/* checking requirements */
	if(!conf->src_path[0] || !conf->dst_path[0] || !conf->dst_size)
		return(-1);

	return(0);
}
