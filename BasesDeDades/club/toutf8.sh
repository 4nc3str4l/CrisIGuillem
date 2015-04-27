#!/bin/bash

while read file
do
  echo "Converting ${file}"
  mv ${file} ${file}.old
  iconv -f ISO-8859-1 -t utf-8 ${file}.old > ${file}
done < <( find . -iname '*.sql' )
