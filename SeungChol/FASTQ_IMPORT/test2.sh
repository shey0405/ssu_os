#!/bin/bash
F_OR_P=$(php FASTQ_IMPORT_REFER.php $1 $2)

if [ -z $F_OR_P ]
then
echo no
else
echo $F_OR_P
fi

