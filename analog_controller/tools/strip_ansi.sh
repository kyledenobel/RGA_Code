#!/bin/bash

usage="$(basename "$0") [-h] [INPUT_FILE] -- program to strip ansi escape codes from a file

The output is to stdout.

where:
    -h  show this help text
    INPUT_FILE file to act upon (pass in - to use stdin)"

while getopts 'h' option; do
  case "$option" in
    h) echo "$usage"
       exit
       ;;
   \?) printf "illegal option: -%s\n" "$OPTARG" >&2
       echo "$usage" >&2
       exit 1
       ;;
  esac
done
shift $((OPTIND - 1))

if [ $# -ge 1 -a -f "$1" ] ; then
    exec < "$1";
fi

sed -e 's/\x1b\[[0-9;]*m//g' -

