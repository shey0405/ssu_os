#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	FILE * src1 = fopen("AFTER_SORT_ID.txt", "rt");
	FILE * src2 = fopen("AFTER_SORT_SEQ_QUAL.txt", "rt");
	FILE * des = fopen(argv[1], "wt");
	
	char buffer[200] = {0};
	char BITFLAG[10] = {0};
	char SEQ[200] = {0};
	char QUAL[200] = {0};
	int bit;
	int i;

	while(1){
		fgets(buffer, sizeof(buffer), src1);
		if(feof(src1) != 0){
			fclose(src1);
			fclose(src2);
			fclose(des);
			return 0;
		}
		fputs(buffer, des);
		
		fscanf(src2, "%s %s %s", BITFLAG, SEQ, QUAL);
		bit = atoi(BITFLAG);
		
		for(i = 0; i < 4; i++)
			bit = bit / 2;
		bit = bit %  2;
	
		if(bit == 0){
			fputs(SEQ, des);
			fputc(10, des);
			fputs("+", des);
			fputc(10, des);
			fputs(QUAL, des);
			fputc(10, des);
		}
		else{
			for(i = strlen(SEQ) - 1; i >= 0; i--){
				if(SEQ[i] == 'A')
					fputc('T', des);
				else if(SEQ[i] == 'C')
					fputc('G', des);
				else if(SEQ[i] == 'G')
					fputc('C', des);
				else if(SEQ[i] == 'T')
					fputc('A', des);
				else if(SEQ[i] == 'N')
					fputc('N', des);
				else
					printf("errereerererer\n");
			}
			fputc(10, des);
			fputc('+', des);
			fputc(10, des);
			for(i = strlen(QUAL) - 1; i >= 0; i--)
				fputc(QUAL[i], des);
			fputc(10, des);
		}
	}
}
	

