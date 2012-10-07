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
