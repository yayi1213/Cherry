#include <stdio.h>
#include <string.h>
#include <ctype.h>
static int
get_line_num(FILE *fp)
{
	int line_num;
	long pos;
	long end_pos;
	
	ASSERT(1, fp != NULL);
	
	end_pos = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	pos = ftell(fp);
	line_num = 1;
	while (pos < end_pos) {
		int c = fgetc(fp);
		if (c == EOF || c == '\0')
			break;
		if (c == '\n')
			line_num++;
		pos++;
	}
	
	return line_num;
}