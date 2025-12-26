#!/bin/bash

script_dir="$(dirname "$(realpath "$0")")"
bms_dir=$script_dir/bms

echo "path, number of clauses with that many literals, number of literals"

for f in `ls $bms_dir | grep cnf` ; do
    p=`realpath $bms_dir/$f`
    echo -n $p,
    $script_dir/./count.sh $p | sort | uniq -c | sed -E 's/ +/ /g' | sed -E 's/^ +//; s/ +$//'  | tr '\n' ',' |tr ' ' ':' | sed 's/,$//' 
done

echo
