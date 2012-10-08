/*
Copyright (C) 2012  Oleh Kravchenko

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

	/* add slash if need */
	if(conf->src_path[strlen(conf->src_path) - 1] != '/')
	{
		strncat(conf->src_path, "/", sizeof(conf->src_path) - 1);
		conf->src_path[sizeof(conf->src_path) - 1] = 0;
	}

	if(conf->dst_path[strlen(conf->dst_path) - 1] != '/')
	{
		strncat(conf->dst_path, "/", sizeof(conf->dst_path) - 1);
		conf->dst_path[sizeof(conf->dst_path) - 1] = 0;
	}

	return(0);
}
