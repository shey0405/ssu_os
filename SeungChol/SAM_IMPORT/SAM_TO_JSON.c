#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	FILE * src = fopen(argv[1], "rt");
	FILE * header = fopen("SRR490124_header.json", "wt");
	FILE * sam1 = fopen("SRR490124_sam1.json", "wt");
	FILE * sam2 = fopen("SRR490124_sam2.json", "wt");
	FILE * supplement = fopen("SRR490124_sup.json", "wt");

	char buffer[1000] = {0};
	char sortnumber[20] = {0};
	
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
	
	int n = 0;
	int i = 0;
	int bit;
	int ch;

	while(1){
		fgets(buffer, sizeof(buffer), src);
		if(buffer[0] != '@')
			break;

		for(i = 0; i < strlen(buffer) - 1; i++)
			if(buffer[i] == '\t')
				buffer[i] = '|';

		fputs("{\"_key\":\"", header);
		sprintf(sortnumber, "%d", ++n);
		fputs(sortnumber, header);
		fputs("\",\"H\":\"", header);
		buffer[strlen(buffer) - 1] = 34;
		fputs(buffer, header);
		fputc('}', header);
		fputc('\n', header);
	}
	fclose(header);
	//n = 0;
	fseek(src, -strlen(buffer), SEEK_CUR);
	
	while(1){
		fscanf(src, "%s %s %s %s %s %s %s %s %s %s %s", QNAME, BITFLAG, RNAME, POS, MAPQ, CIGAR, RNEXT, PNEXT, TLEN, SEQ, QUAL);
		if(feof(src) != 0){
			fclose(src);
			fclose(sam1);
			fclose(sam2);
			fclose(supplement);
			return 0;
		}
		fgetc(src);
		fgets(OPTION, sizeof(OPTION), src);
		for(i = 0; i < strlen(OPTION) - 1; i++){
 	               if(OPTION[i] == '\t')
        	                OPTION[i] = '|';
       		 }
       		 OPTION[strlen(OPTION) - 1] = '\0';

		bit = atoi(BITFLAG);
		
		if(bit >= 2048){
			fputs("{\"_key\":\"", supplement);
		        sprintf(sortnumber, "%d", ++n);
       		        fputs(sortnumber, supplement);
       		        fputs("\",", supplement);

       		        fprintf(supplement,"\"QNAME\":\"%s\",\"BITFLAG\":%s,\"RNAME\":\"%s\",\"POS\":%s,\"MAPQ\":%s,\"CIGAR\":\"%s\",\"RNEXT\":\"%s\",\"PNEXT\":%s,\"TLEN\":%s,\"SEQ\":\"%s\",\"QUAL\":\"%s\",\"OPTION\":\"%s\"}", QNAME, BITFLAG, RNAME, POS, MAPQ, CIGAR, RNEXT, PNEXT, TLEN, SEQ, QUAL, OPTION);
       		        fputc('\n', supplement);
		}
		//need update
		else{
			if(bit >= 128){
				fputs("{\"_key\":\"", sam2);
                       		sprintf(sortnumber, "%d", ++n);
                        	fputs(sortnumber, sam2);
                        	fputs("\",", sam2);

                        	fprintf(sam2,"\"QNAME\":\"%s\",\"BITFLAG\":%s,\"RNAME\":\"%s\",\"POS\":%s,\"MAPQ\":%s,\"CIGAR\":\"%s\",\"RNEXT\":\"%s\",\"PNEXT\":%s,\"TLEN\":%s,\"SEQ\":\"%s\",\"QUAL\":\"%s\",\"OPTION\":\"%s\"}", QNAME, BITFLAG, RNAME, POS, MAPQ, CIGAR, RNEXT, PNEXT, TLEN, SEQ, QUAL, OPTION);
                        	fputc('\n', sam2);
			}
			else{
				fputs("{\"_key\":\"", sam1);
                                sprintf(sortnumber, "%d", ++n);
                                fputs(sortnumber, sam1);
                                fputs("\",", sam1);

                                fprintf(sam1,"\"QNAME\":\"%s\",\"BITFLAG\":%s,\"RNAME\":\"%s\",\"POS\":%s,\"MAPQ\":%s,\"CIGAR\":\"%s\",\"RNEXT\":\"%s\",\"PNEXT\":%s,\"TLEN\":%s,\"SEQ\":\"%s\",\"QUAL\":\"%s\",\"OPTION\":\"%s\"}", QNAME, BITFLAG, RNAME, POS, MAPQ, CIGAR, RNEXT, PNEXT, TLEN, SEQ, QUAL, OPTION);
                                fputc('\n', sam1);
			}
		}
	}
}

		











		
