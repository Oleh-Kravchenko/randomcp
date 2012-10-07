#include <stdlib.h>
#include <string.h>

#include "re.h"

/*------------------------------------------------------------------------*/

size_t re_strlen(const regmatch_t* re_subs)
{
	if(!re_subs->rm_so)
		return(0);

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
	if(!re_subs->rm_so)
		return(0);

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
