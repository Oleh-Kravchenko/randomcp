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

#ifndef __CONF_H
#define __CONF_H

#include <unistd.h>

typedef struct conf_s
{
	char src_path[0x100];

	char dst_path[0x100];

	off_t dst_size;
} conf_t;

/*------------------------------------------------------------------------*/

int conf_parse_cmdline(int argc, char **argv, conf_t* conf);

#endif /* __CONF_H */
