#ifndef __RE_H
#define __RE_H

#include <regex.h>

size_t re_strlen(const regmatch_t* re_subs);

int re_atoi(const char* src, const regmatch_t* re_subs);

char re_char(const char* src, const regmatch_t* re_subs);

int re_strcmp(const char* s, const char* mask);

int re_parse(const char* s, const char* mask, size_t* nmatch, regmatch_t** pmatch);

#endif /* __RE_H */
