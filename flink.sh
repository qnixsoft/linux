#!/bin/sh -e
o_arch=riscv
d_arch=wasm

[[ ! -z "$1" ]]

destdir=`dirname $1`
basename=`basename $1`
fulldir=arch/$d_arch/$destdir
orig=arch/$o_arch/$destdir/$basename 

[[ -d $fulldir ]] || mkdir -p $fulldir
cd $fulldir

reldir=`echo $1 | tr "/" "\n" | sed "\$$d" | while read line ; do echo -n ../ ; done`
origrel=$reldir$o_arch/$1

if [[ ! -f $origrel ]] then
	echo -n "$orig not found. Delete dest? "
	read y
	[[ $y != "y" ]] || git rm $basename || rm $basename
	exit
fi

# echo cd $fulldir
if [[ -f $basename ]]; then
	if diff $origrel $basename >/dev/null; then
		git diff $origrel $basename
		echo -n "$origrel differs from $basename\nDelete and replace? "
		read y
		[[ "$y" == "y" ]]
	fi

	di="`git diff-index master -- $origrel`"

	if [[ ! -z "$di" ]]; then
		git diff master -- $origrel
		echo Only wasm architecture should have changes but
		echo $origrel does too. Do you wish to copy it here:
		pwd
		echo -n "restore the previous version? and git add $basename? "
		read y
		[[ "$y" == "y" ]]
		rm $basename
		cp $origrel .
		git checkout $origrel
		git add $basename
		exit 0
	else
		rm $basename
	fi
fi

# echo ln -fs $origrel
# ln -fs $origrel
echo cp $origrel .
cp $origrel .

# file $origrel
# file $basename
