#!/bin/bash
F_OR_P=$(php FASTQ_EXPORT_REFER.php $1 $2)
FASTQ_COL=$1_${2%.*}_F
if [ -z $F_OR_P ]
then
php FULL_FASTQ_EXPORT.php $FASTQ_COL > BEFORE_SORT_FASTQ.txt
cat BEFORE_SORT_FASTQ.txt | sort -k1,1 -n | cut -f 2- | tr "\t" "\n" > $2
else
php FASTQ_ID_EXPORT.php $FASTQ_COL > BEFORE_SORT_ID.txt
php SAM_SEQ_QUAL_EXPORT.php $F_OR_P > BEFORE_SORT_SEQ_QUAL.txt
cat BEFORE_SORT_ID.txt | sort -k1,1 -n | cut -f 2- > AFTER_SORT_ID.txt
cat BEFORE_SORT_SEQ_QUAL.txt | sort -k1,1 -n | cut -f 2- > AFTER_SORT_SEQ_QUAL.txt
./FASTQ_EXPORT $2
fi

