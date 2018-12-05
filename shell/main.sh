#!/bin/sh

while IFS='' read -r year cause1 cause2 deaths age_adj; do
    echo "$year $cause1 $cause2 $deaths $age_adj"
done < "$1"

echo $0
echo $1