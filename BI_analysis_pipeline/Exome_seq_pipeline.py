#!/usr/bin/python

import sys,os,glob

bwa_path	=	'/BioEdu/WES/Tools/bwa-0.7.12/bwa'
samtools_path	=	'/BioEdu/WES/Tools/samtools-1.3.1/samtools'
bedtools_path	=	'/BioEdu/WES/Tools/bedtools-2.21.0/bin/bedtools'
picardtools_path=	'/BioEdu/WES/Tools/picard-tools-2.2.2/picard.jar'
gatk_path	=	'/BioEdu/WES/Tools/GenomeAnalysisTK-3.7/GenomeAnalysisTK.jar'
sickle_path	=	'/BioEdu/WES/Tools/sickle-1.33/sickle'
bcftools_path	=	'/BioEdu/WES/Tools/bcftools-1.3.1/bcftools'
snpeff_path	=	'/BioEdu/WES/Tools/snpEff_4.2'

def fastq_link():
	if os.path.isfile('ExomeSeq-raw_1.fastq.gz'):
		os.system('rm ExomeSeq-raw_1.fastq.gz')
	if os.path.isfile('ExomeSeq-raw_2.fastq.gz'):
		os.system('rm ExomeSeq-raw_2.fastq.gz')
	os.system('''

ln -s /BioEdu/WES/Fastq/ExomeSeq-raw_1.fastq.gz .
ln -s /BioEdu/WES/Fastq/ExomeSeq-raw_2.fastq.gz .

''')

def fastq_trimming():
	print '## fastq trimming...'
	os.system('''

/BioEdu/WES/Tools/sickle-1.33/sickle pe -t sanger -f ExomeSeq-raw_1.fastq.gz -r ExomeSeq-raw_2.fastq.gz -o ExomeSeq_1.fastq.gz -p ExomeSeq_2.fastq.gz -s tmp.fastq.gz -g
rm tmp.fastq.gz

''')

def reference_download():
	if not os.path.isdir('ref'):
		os.mkdir('ref')
	if not os.path.isfile('ref/chr8.fa'):
		print '## reference downloading...'
		os.system('''

wget http://hgdownload.cse.ucsc.edu/goldenPath/hg19/chromosomes/chr8.fa.gz -O ref/chr8.fa.gz
gunzip ref/chr8.fa.gz

''')

def alignment():
	if not os.path.isfile('ref/chr8.fa.bwt'):
		print '## generating new reference index files...'
		os.system('%s index ref/chr8.fa'%bwa_path)
	print '## generating alignment file...'
	os.system('''

%s mem -t 8 -R '@RG\\tPL:Illumina\\tID:BioEdu\\tSM:ExomeSeq\\tLB:TruSeq' ref/chr8.fa ExomeSeq_1.fastq.gz ExomeSeq_2.fastq.gz | %s view -b -q 20 > ExomeSeq.chr8.bam
%s sort -O BAM ExomeSeq.chr8.bam > ExomeSeq.sorted.chr8.bam

'''%(bwa_path,samtools_path,samtools_path))
def remove_duplicates():
	print '## removing PCR duplicated reads...'

	os.system('''

java -jar %s MarkDuplicates I=ExomeSeq.sorted.chr8.bam O=ExomeSeq.rmdup.chr8.bam M=ExomeSeq.rmdup.chr8.metrics REMOVE_DUPLICATES=true
%s index ExomeSeq.rmdup.chr8.bam

'''%(picardtools_path,samtools_path))


def remove_offtarget():
	print '## removing off-target reads...'
	os.system('''

%s intersect -wa -u -a ExomeSeq.rmdup.chr8.bam -b /BioEdu/WES/DB/Target/Target_region.bed > ExomeSeq.rmdup.target.bam
%s index ExomeSeq.rmdup.target.bam

'''%(bedtools_path,samtools_path))

def depth_of_coverage():
	print '## calculating depth of coverage...'
	os.system('java -jar %s -T DepthOfCoverage -R ref/chr8.fa -I ExomeSeq.rmdup.target.bam -o ExomeSeq.rmdup.target.depcov -ct 1 -ct 5 -ct 10 -ct 20 -ct 30 -omitBaseOutput -L /BioEdu/WES/DB/Target/Target_region.bed'%gatk_path)

def indel_realignment():
	if not os.path.isfile('ref/chr8.fa.fai'):
		print '## generating fasta index file...'
		os.system('%s faidx ref/chr8.fa'%samtools_path)
	if not os.path.isfile('ref/chr8.dict'):
		print '## generating fasta sequence dictionary...'
		os.system('java -jar %s CreateSequenceDictionary R=ref/chr8.fa O=ref/chr8.dict'%picardtools_path)
	print '## indel realignment...'
	os.system('java -jar %s -T IndelRealigner -I ExomeSeq.rmdup.target.bam -R ref/chr8.fa -targetIntervals /BioEdu/WES/DB/Target/Target_region.bed -o ExomeSeq.target.realigned.bam -known /BioEdu/WES/DB/Annotation/Mills_and_1000G_gold_standard.indels.hg19.chr8.vcf.gz'%gatk_path)

def basequality_recalibration():
	print '## base recalibration ...'
	os.system('''

java -Xmx4g -jar %s -T BaseRecalibrator -R ref/chr8.fa -I ExomeSeq.target.realigned.bam -knownSites /BioEdu/WES/DB/Annotation/dbSnp150_chr8.vcf.gz -o ExomeSeq.target.recal.table
java -Xmx4g -jar %s -T PrintReads -R ref/chr8.fa -I ExomeSeq.target.realigned.bam -BQSR ExomeSeq.target.recal.table -o ExomeSeq.target.recal.bam
java -Xmx4g -jar %s -T AnalyzeCovariates -R ref/chr8.fa -BQSR ExomeSeq.target.recal.table -plots BQSR.pdf

'''%(gatk_path,gatk_path,gatk_path))


def variant_calling():
	print '## variant calling : samtools...'
	os.system('''
%s mpileup -q 20 -gf ref/chr8.fa -o ExomeSeq.target.samt.raw.bcf ExomeSeq.target.recal.bam
%s call -m -v -O v ExomeSeq.target.samt.raw.bcf -o ExomeSeq.target.samt.raw.vcf
'''%(samtools_path,bcftools_path))
	print '## variant calling : GATK...'
	os.system('java -Xmx4g -jar %s -T HaplotypeCaller -dt NONE -R ref/chr8.fa -I ExomeSeq.target.recal.bam --genotyping_mode DISCOVERY --output_mode EMIT_VARIANTS_ONLY -stand_call_conf 20 -o ExomeSeq.target.gatk.raw.vcf'%gatk_path)


def combine_vcf():
	if not glob.glob('WES*.raw.vcf.gz')==[]:
		print '## remove old vcf.gz'
		os.system('rm WES*.raw.vcf.gz')
	os.system('cp /BioEdu/WES/WES_vcf/WES*.vcf .')
	print '## generate new vcf.gz'
	os.system('''
bgzip WES1.gatk.raw.vcf
tabix -p vcf WES1.gatk.raw.vcf.gz

bgzip WES1.samt.raw.vcf
tabix -p vcf WES1.samt.raw.vcf.gz

bgzip WES2.gatk.raw.vcf
tabix -p vcf WES2.gatk.raw.vcf.gz

bgzip WES2.samt.raw.vcf
tabix -p vcf WES2.samt.raw.vcf.gz

java -jar %s -T CombineVariants -R /BioEdu/WES/DB/hg19_full_ref/hg19.fasta -dt NONE -genotypeMergeOptions UNIQUIFY --variant WES1.gatk.raw.vcf.gz --variant WES1.samt.raw.vcf.gz --variant WES2.gatk.raw.vcf.gz --variant WES2.samt.raw.vcf.gz > WES.combined.vcf

java -jar %s/SnpSift.jar filter "(DP>=20 & QUAL>=30)" WES.combined.vcf > WES.combined.filt.vcf

'''%(gatk_path,snpeff_path))

def annotation():
	print '## snpeff annotation...'
	os.system('java -jar %s/snpEff.jar -v hg19 WES.combined.filt.vcf > WES.snpeff.vcf'%snpeff_path)
	print '## clinvar annotation...'
	os.system('java -jar %s/SnpSift.jar annotate /BioEdu/WES/DB/Annotation/clinvar_20170705.vcf.gz WES.snpeff.vcf > WES.snpeff.clinvar.vcf'%snpeff_path)


def usage():
	print '''
usage: python %s [Exe_step]

==Exe step list====
Alignment
BAM_manipulation
Make_vcf
Annotation
===================
'''%sys.argv[0]

if __name__=='__main__':
	try:
		sExe_step	=	sys.argv[1]
	except IndexError:
		usage();sys.exit()
	if not sExe_step in ['Alignment','BAM_manipulation','Make_vcf','Annotation']:
		print 'Argument name %s was not recognized'
		usage();sys.exit()
	if sExe_step	==	'Alignment':
		fastq_link()
		fastq_trimming()
		reference_download()
		alignment()
	elif sExe_step	==	'BAM_manipulation':
		remove_duplicates()
		remove_offtarget()
		depth_of_coverage()
		indel_realignment()
		basequality_recalibration()
	elif sExe_step	==	'Make_vcf':
		variant_calling()
		combine_vcf()
	elif sExe_step	==	'Annotation':
		annotation()

