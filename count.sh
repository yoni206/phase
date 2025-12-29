#!/bin/bash

# Helper Function
count_spaces_in_file() {
    local file="$1"

    # read and ignore first line
    IFS= read -r _ <"$file"

    {
        # skip first line inside the redirection
        IFS= read -r _
        while IFS= read -r line; do
            only_spaces="${line//[^ ]/}"
            echo "${#only_spaces}"
        done
    } <"$file"
}

# FYI: out put is: <path>,<#clauses>,<#literals>

p=$(realpath $1)
echo $p $(./count $p)
# count_spaces_in_file $p | sort | uniq -c | sed -E 's/ +/ /g' | sed -E 's/^ +//; s/ +$//' | tr '\n' ',' | tr ' ' ':' | sed 's/,$/;/'
