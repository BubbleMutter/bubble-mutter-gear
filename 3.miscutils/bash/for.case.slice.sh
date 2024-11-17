#!/bin/bash
# for i++ is only avaiable for bash
for (( i=1; i<=10; i++ )); do echo $i; done
for i in {1..10}; do echo $i; done
for i in {10..1}; do echo $i; done
for i in $(seq 1 10); do echo $i; done

# case
case $param in
    bubble)
        echo bubble
    ;;
    mutter)
        echo mutter
    ;;
    *)
        echo default
    ;;
esac

# slice
slice=(violin cello piano)
echo "array size is ${#slice[*]}"      # size of array
echo "array[1] item is ${slice[1]}"    # access element
echo "array[1] size is ${#slice[1]}"   # strlen of element
for e in ${slice[@]}; do echo $e; done # foreach
slice+=("head" "body" "tail")          # expand array
for e in ${slice[@]}; do echo $e; done # foreach