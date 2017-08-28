#!/bin/bash
./SAM_LIST_TO_JSON $1 $2
IDX1=$(php SAM_IMPORT_IDX1.php $1 $2)
IDX2=$(php SAM_IMPORT_IDX2.php $1 $2)

FASTQ_COL1=$(php SAM_IMPORT_FASTQ_COL.php $1 $IDX1)
FASTQ_COL2=$(php SAM_IMPORT_FASTQ_COL.php $1 $IDX2)

if [ -z $FASTQ_COL1 ]
then
else
FILE_NAME=$(php SAM_IMPORT_FASTQ_FILE_NAME.php $FASTQ_COL1)
php FASTQ_SEQ_QUAL_NULL.php $FASTQ_COL1
php FASTQ_SAM_LIST_UPDATE1.php $1 $2 $1_${2%.*}_1 $FASTQ_COL1 $FILE_NAME
fi

if [ -z $FASTQ_COL2 ]
then
else
FILE_NAME=$(php SAM_IMPORT_FASTQ_FILE_NAME.php $FASTQ_COL2)
php FASTQ_SEQ_QUAL_NULL.php $FASTQ_COL2
php FASTQ_SAM_LIST_UPDATE2.php $1 $2 $1_${2%.*}_2 $FASTQ_COL2 $FILE_NAME
fi

./SAM_TO_JSON $2
arangoimp --file ${2%.*}_LIST.json --collection SAM_LIST 
arangoimp --file ${2%.*}_HEADER.json --collection $1_${2%.*}_H --create-collection true --progress true
arangoimp --file ${2%.*}_SAM1.json --collection $1_${2%.*}_1 --create-collection true --progress true
arangoimp --file ${2%.*}_SAM2.json --collection $1_${2%.*}_2 --create-collection true --progress true
arangoimp --file ${2%.*}_SUP.json --collection $1_${2%.*}_S --create-collection true --progress true
