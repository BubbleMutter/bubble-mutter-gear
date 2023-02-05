#!/bin/bash

# for i++ is only avaiable for bash
for (( i=1; i<=10; i++ )); do
    echo $i
done

for i in {1..10}; do echo $i; done

for i in {10..1}; do echo $i; done