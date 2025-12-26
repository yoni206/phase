#!/bin/bash
script_dir="$(dirname "$(realpath "$0")")"
bms_dir=$script_dir/bms
[[ -d $bms_dir ]] && { echo "dir $bms_dir exists. aborting."; exit 1; } || mkdir $bms_dir 
cp track_main_2025.uri $bms_dir
cd $bms_dir
wget --content-disposition -i track_main_2025.uri
ls | grep cnf | xargs xz -d
