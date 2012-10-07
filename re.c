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

#include <stdlib.h>
#include <string.h>

#include "re.h"

/*------------------------------------------------------------------------*/

size_t re_strlen(const regmatch_t* re_subs)
{
	return(re_subs->rm_eo - re_subs->rm_so);
}

/*------------------------------------------------------------------------*/

int re_atoi(const char* src, const regmatch_t* re_subs)
{
	int res = 0;
	size_t len;
	char* buf;

	len = re_strlen(re_subs);

	if(!(buf = malloc(len + 1)))
		return(res);

	memcpy(buf, src + re_subs->rm_so, len);
	buf[len] = 0;

	res = atoi(buf);

	free(buf);

	return(res);
}

/*------------------------------------------------------------------------*/

char re_char(const char* src, const regmatch_t* re_subs)
{
	return(src[re_subs->rm_so]);
}

/*------------------------------------------------------------------------*/

int re_strcmp(const char* s, const char* mask)
{
	regex_t re;
	int res;

	regcomp(&re, mask, REG_EXTENDED);
	res = regexec(&re, s, 0, NULL, 0);
	regfree(&re);

	return(res);
}

/*------------------------------------------------------------------------*/

int re_parse(const char* s, const char* mask, size_t* nmatch, regmatch_t** pmatch)
{
	regex_t re;
	int res;

	*nmatch = 0;
	*pmatch = NULL;

	if((res = regcomp(&re, mask, REG_EXTENDED)))
		return(res);

	*nmatch = re.re_nsub + 1;

	if((*pmatch = malloc(sizeof(regmatch_t) * (*nmatch))) == NULL)
	{
		res = -1;
		goto err;
	}

	if((res = regexec(&re, s, *nmatch, *pmatch, 0)))
		free(*pmatch);

err:
	regfree(&re);

	return(res);
}
