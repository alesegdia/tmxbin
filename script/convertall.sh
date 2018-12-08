#!/usr/bin/env bash


if [ ! $# -eq 1 ]; then
	echo "Need to provide file."
	exit 1
fi

if [[ ! -d $1 ]]; then
	echo "$1 is not a directory"
	exit 1
fi

basedir=$( dirname ${BASH_SOURCE[0]} )/../bin/

tsx2tsbbin=$basedir/tsx2tsb
tmx2tmbbin=$basedir/tmx2tmb

if [[ ! -f $tsx2tsbbin ]] || [[ ! -f $tmx2tmbbin ]]; then
	echo "You need to build project"
	exit 1
fi

echo "TILESETS"
for f in assets/*.tsx; do
	printf "\nConverting $f..."
	eval "$tsx2tsbbin $f"
done

for f in assets/*.tmx; do
	printf "\nConverting $f..."
	eval "$tmx2tmbbin $f"
done
