#ifndef __CONF_H
#define __CONF_H

#include <unistd.h>

typedef struct conf_s
{
	char src_path[0x100];

	char dst_path[0x100];

	off_t dst_size;

	size_t cp_buf_len;
} conf_t;

int conf_parse_cmdline(int argc, char **argv, conf_t* conf);

#endif /* __CONF_H */
