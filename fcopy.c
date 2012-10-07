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

#include <stdio.h>

/*------------------------------------------------------------------------*/

int fcopy(const char* src, const char* dst)
{
	char buf[0x1000];
	FILE *fr, *fw;
	int res = -1;
	size_t rd;

	if(!(fr = fopen(src, "rb")))
		return(res);

	if(!(fw = fopen(dst, "wb")))
		goto err_w;

	while(!feof(fr))
	{
		if(!(rd = fread(buf, 1, sizeof(buf), fr)))
		{
			if(ferror(fr))
				goto err;

			continue;
		}

		if(!fwrite(buf, 1, rd, fw))
			goto err;
	}

	res = 0;

err:
	fclose(fw);

err_w:
	fclose(fr);

	return(res);
}
