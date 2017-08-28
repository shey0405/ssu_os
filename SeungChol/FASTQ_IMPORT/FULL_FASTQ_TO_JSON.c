#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char buf[500] = {0};
	char no_extend[500] = {0};
	int i = 0;

	strcpy(buf, argv[1]);
        for(i = 0; i < strlen(buf); i++){
                if(buf[i] == '.')
                        break;
                no_extend[i] = argv[1][i];
        }
        buf[i++] = '_';
        buf[i++] = 'F';
        buf[i++] = 'U';
        buf[i++] = 'L';
        buf[i++] = 'L';
        buf[i++] = '.';
        buf[i++] = 'j';
        buf[i++] = 's';
        buf[i++] = 'o';
        buf[i++] = 'n';
        buf[i++] = '\0';
	i = 0;
	
	FILE * src = fopen(argv[1], "rt");
	FILE * des = fopen(buf, "wt");
	char str1[109] = {0};
	char buffer[20] = {0};
	int ch = 0;
	int n = 0;
	
	while(1){
		fgetc(src);
		if(feof(src) != 0){
			fclose(src);
			fclose(des);
			return 0;
		}
		fputs("{\"_key\":\"", des);
		sprintf(buffer, "%d", ++n);
		fputs(buffer, des);
		fputs("\",", des);

		fputs("\"ID\":\"", des);
		while((ch = fgetc(src)) != ' ')
			str1[i++] = ch;
		str1[i] = '\0';
		fputs(str1, des);
		i = 0;

		fputs("\",\"IDX\":\"", des);
		while((ch = fgetc(src)) != '\n')
			str1[i++] = ch;
		str1[i] = '\0';
		fputs(str1, des);
		i = 0;

		fgets(str1, sizeof(str1), src);
		fputs("\",\"SEQ\":\"", des);
		str1[strlen(str1) - 1] = 34;
		fputs(str1, des);
		fputc(',', des);
		
		fgets(str1, sizeof(str1), src);
		
		fgets(str1, sizeof(str1), src);
		fputs("\"QUAL\":\"", des);
		
		str1[strlen(str1) - 1] = 34;
		fputs(str1, des);
		fputc('}', des);
		fputc('\n', des);
	}
	return 0;
}
