#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char output_file[200] = {0};
	char id_file_h[200] = {0};
	char id_file_1[200] = {0};
	char id_file_2[200] = {0};
	char id_file_s[200] = {0};
	char no_extend[200] = {0};
	char buffer[1000] = {0};

	char QNAME[50] = {0};
        char BITFLAG[10] = {0};
        char RNAME[20] = {0};
        char POS[20] = {0};
        char MAPQ[10] = {0};
        char CIGAR[20] = {0};
        char RNEXT[10] = {0};
        char PNEXT[20] = {0};
        char TLEN[20] = {0};
        char SEQ[150] = {0};
        char QUAL[150] = {0};
        char OPTION[400] = {0};

	int i, n, ch;

	strcpy(output_file, argv[2]);
	for(i = 0; i < strlen(output_file); i++){
		if(output_file[i] == '.')
			break;
		no_extend[i] = argv[2][i];
	}

	output_file[i++] = '_';
	output_file[i++] = 'L';
	output_file[i++] = 'I';
	output_file[i++] = 'S';
	output_file[i++] = 'T';
	output_file[i++] = '.';
	output_file[i++] = 'j';
	output_file[i++] = 's';
	output_file[i++] = 'o';
	output_file[i++] = 'n';
	output_file[i++] = '\0';

	FILE *src = fopen(argv[2], "rt");
	FILE *des = fopen(output_file, "wt");

        fputs("{\"USER_ID\":\"", des);
        fputs(argv[1], des);
        fputs("\",\"FILE_NAME\":\"", des);
        fputs(argv[2], des);

        fputs("\",\"HEADER_COL\":\"", des);
        strcpy(id_file_h, argv[1]);
        id_file_h[strlen(id_file_h)] = '_';

        n = strlen(id_file_h);

        for(i = 0; i < strlen(no_extend); i++){
                id_file_h[n++] = no_extend[i];
        }

        id_file_h[n++] = '_';
        id_file_h[n++] = 'H';
        id_file_h[n] = '\0';

        fputs(id_file_h, des);

        fputs("\",\"SAM1_COL\":\"", des);
        strcpy(id_file_1, argv[1]);
        id_file_1[strlen(id_file_1)] = '_';

        n = strlen(id_file_1);

        for(i = 0; i < strlen(no_extend); i++){
                id_file_1[n++] = no_extend[i];
        }

        id_file_1[n++] = '_';
        id_file_1[n++] = '1';
        id_file_1[n] = '\0';

        fputs(id_file_1, des);

        fputs("\",\"SAM2_COL\":\"", des);
        strcpy(id_file_2, argv[1]);
        id_file_2[strlen(id_file_2)] = '_';

        n = strlen(id_file_2);

        for(i = 0; i < strlen(no_extend); i++){
                id_file_2[n++] = no_extend[i];
        }

        id_file_2[n++] = '_';
        id_file_2[n++] = '2';
        id_file_2[n] = '\0';

        fputs(id_file_2, des);

        fputs("\",\"SUP_COL\":\"", des);
        strcpy(id_file_s, argv[1]);
        id_file_s[strlen(id_file_s)] = '_';

        n = strlen(id_file_s);

        for(i = 0; i < strlen(no_extend); i++){
                id_file_s[n++] = no_extend[i];
        }

        id_file_s[n++] = '_';
        id_file_s[n++] = 'S';
        id_file_s[n] = '\0';

        fputs(id_file_s, des);

	while(1){
                fgets(buffer, sizeof(buffer), src);
                if(buffer[0] != '@')
                        break;
        }
        fseek(src, -strlen(buffer), SEEK_CUR);

        fscanf(src, "%s %s %s %s %s %s %s %s %s %s %s", QNAME, BITFLAG, RNAME, POS, MAPQ, CIGAR, RNEXT, PNEXT, TLEN, SEQ, QUAL);
	fgetc(src);
        fgets(OPTION, sizeof(OPTION), src);
	
	fputs("\",\"IDX1\":\"", des);
	fputs(QUAL, des);
	
	fscanf(src, "%s %s %s %s %s %s %s %s %s %s %s", QNAME, BITFLAG, RNAME, POS, MAPQ, CIGAR, RNEXT, PNEXT, TLEN, SEQ, QUAL);
        fgetc(src);
        fgets(OPTION, sizeof(OPTION), src);

        fputs("\",\"IDX2\":\"", des);
        fputs(QUAL, des);
	
	fputs("\",\"FASTQ1_FILE_NAME\":null,", des);
	fputs("\"FASTQ1_COL\":null,", des);
	fputs("\"FASTQ2_FILE_NAME\":null,", des);
	fputs("\"FASTQ2_COL\":null}", des);

	fclose(src);
	fclose(des);

	return 0;
}

