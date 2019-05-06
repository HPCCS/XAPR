#!/bin/bash

for filename in ../DELIMITER_TRACE_FILES1/*;
do
  fname=$(basename "$filename")
  export TRACE_FILE_NAME=$fname
  echo "is processing ... "$TRACE_FILE_NAME
  ./gpu_rel_feature
done 
