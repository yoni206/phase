#!/bin/bash
COUNT_PATH=$1
RESULTS_PATH=$2

cat $COUNT_PATH | cut -d, -f1 | sort > tmp1.csv
cat $RESULTS_PATH | cut -d, -f1 | sort > tmp2.csv
if cmp tmp1.csv tmp2.csv; then
    echo ""
else
    echo "filenemaes differ. aborting."
    exit 1
fi

paste -d',' $COUNT_PATH $RESULTS_PATH > tmp3.csv
cat tmp3.csv | cut -d, -f1,2,4- > combined.csv

rm tmp1.csv
rm tmp2.csv
rm tmp3.csv
