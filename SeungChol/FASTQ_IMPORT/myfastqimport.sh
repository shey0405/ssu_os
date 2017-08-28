#!/bin/bash
./FASTQ_LIST_TO_JSON $1 $2
LIST_NAME=${2%.*}_LIST.json
arangoimp --file $LIST_NAME --collection FASTQ_LIST
php QUERY.php $1 $2

COL_NAME=$1_${2%.*}_F
F_OR_P=$(php FASTQ_IMPORT_REFER.php $1 $2)
if [ -z $F_OR_P ]
then
./FULL_FASTQ_TO_JSON $2
FILE_NAME=${2%.*}_FULL.json
arangoimp --file $FILE_NAME --collection $COL_NAME --create-collection true --progress true
else
./PART_FASTQ_TO_JSON $2
FILE_NAME=${2%.*}_PART.json
arangoimp --file $FILE_NAME --collection $COL_NAME --create-collection true --progress true
fi




