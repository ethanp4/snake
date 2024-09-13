#!/bin/bash
header="./include/credentials.h"

echo "#ifndef CREDENTIALS_H" > $header
echo "#define CREDENTIALS_H" >> $header
echo "" >> $header

if [ -f .env ]; then
  . .env
  echo "static const bool authIncluded = true;" >> $header
  echo "static const char* userpwd = \""$user":"$pass\"";" >> $header
else 
  echo "static const bool authIncluded = false;" >> $header
  echo "static const char* userpwd = nullptr;" >> $header
fi

echo "" >> $header
echo "#endif" >> $header