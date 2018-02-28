#!/bin/bash
# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version
#
# Driver script for mem_du.py
#
# Gets output from nm, and analyses libraries
# Passes these to mem_du.py
#
# Args
# 1: The name of the elf executable to be analysed
#
#set -x
usage="$0: usage: $0 [-v] <executable> <output>"

if [ 2 -ne $# -a 3 -ne $# ]; then
  echo $usage 1>&2
  exit 1
fi
if [ "-v" = "$1" ]; then
  verbose=-v
  shift
else
  verbose=
fi
elf=$1
out=$2
if [ ! -f ${elf} ]; then
  echo "$0: ${elf} not found, terminating" 1>&2
  exit 1
fi
dir=`dirname ${elf}`
tmp=${dir}/tmp$$
nm_file=${tmp}.1
loc_file=${tmp}.2
a_file=${tmp}.3
lib_file=${tmp}.4.a

# Get the nm info
# Temporarily use objdump as nm for kcc is unreliable
# The symbols we don't want are
# L_usw[0-9].*
# Lc_*
# L_[0-9].*
# \.*
# loop[0-9].*
# The first grep gets us symbols only in sections starting .
# We also don't want the debug section
objdump -t ${elf} | grep ' \.' | grep -v '\.debug' | grep -v " L_usw" | grep -v " Lc_" | grep -v " L_[0-9]" | grep -v " \.[^ ]*$" | grep -v " loop[0-9]" | sed -e "s/	00000000 / /;s/ ......../ /;s/\.initc/D/;s/\.zeroinit/B/;s/\.text.* /T /;s/\.const/R/;s/\.mmu/B/" > ${nm_file}
ret=$?
if [ 0 -ne $ret ]; then
  echo "$0: nm failed, terminating" 1>&2
  exit 1
fi

# Make a lib
# This is solely because interrogating every .o separately is too slow
ar crs ${lib_file} `find ${dir} -name \*.o`
nm --print-file-name ${tmp}.4.a | grep -v ' \.' | grep -v " U " | grep -v " N " | grep -v "t L0" | grep -v " Lc_" | grep -v " L_usw" | grep -v "L_[0-9]" | grep -v " loop[0-9]" > ${a_file}
for x in `find ${dir} -name \*.o`; do y=`basename $x`; z=`dirname $x | sed -e "s?${dir}??;s?^/??;s/^$/./"`; grep ":$y:" ${a_file} | sed -e "s?^${lib_file}?$z?"; done | sed -e "s/:00* . /:/" > ${loc_file}
ret=$?
if [ ! ${ret} ]; then
  echo "$0: nm failed, terminating" 1>&2
  exit 1
fi

wd=`dirname $0`
${wd}/mem_du.py $verbose -n ${nm_file} -l ${loc_file} -o ${out} ${elf}
ret=$?
rm ${tmp}.{1,2,3,4.a}
exit $ret
