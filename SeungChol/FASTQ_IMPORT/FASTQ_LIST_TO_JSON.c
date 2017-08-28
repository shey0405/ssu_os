#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{	
	char buffer[500] = {0};
	char id_file_f[500] = {0};
	char noextend[500] = {0};
	int i, n;
	
	strcpy(buffer, argv[2]);
	for(i = 0; i < strlen(buffer); i++){
		if(buffer[i] == '.')
			break;
		noextend[i] = argv[2][i];
	}
	buffer[i++] = '_';
	buffer[i++] = 'L';
	buffer[i++] = 'I';
	buffer[i++] = 'S';
	buffer[i++] = 'T';
	buffer[i++] = '.';
	buffer[i++] = 'j';
	buffer[i++] = 's';
	buffer[i++] = 'o';
	buffer[i++] = 'n';
	buffer[i++] = '\0';

	FILE *src = fopen(argv[2], "rt");
	FILE *des = fopen(buffer, "wt");

	fputs("{\"USER_ID\":\"", des);
	fputs(argv[1], des);
	fputs("\",\"FILE_NAME\":\"", des);
	fputs(argv[2], des);
	fputs("\",\"FASTQ_COL\":\"", des);
	
	strcpy(id_file_f, argv[1]);
	id_file_f[strlen(id_file_f)] = '_';
	n = strlen(id_file_f);

	for(i = 0; i < strlen(noextend); i++){
		id_file_f[n++] = noextend[i];
	}
	
	id_file_f[n++] = '_';
	id_file_f[n++] = 'F';
	id_file_f[n] = '\0';

	fputs(id_file_f, des);
	fputs("\",\"REFER_COL\":null,", des);
	fputs("\"IDX\":\"", des);
	
	fgets(buffer, sizeof(buffer), src);
	fgets(buffer, sizeof(buffer), src);
	fgets(buffer, sizeof(buffer), src);
	fgets(buffer, sizeof(buffer), src);

	buffer[strlen(buffer) - 1] = 34;
	fputs(buffer, des);
	fputs("}", des);
	
	fclose(src);
	fclose(des);

	return 0;
}
		
