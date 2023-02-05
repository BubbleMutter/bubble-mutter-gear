#!/bin/bash

array=(violin cello piano)
echo "array size is ${#array[*]}"      # size of array
echo "array[1] item is ${array[1]}"    # access element
echo "array[1] size is ${#array[1]}"   # strlen of element
for e in ${array[@]}; do echo $e; done # foreach
array+=("head" "body" "tail")          # expand array