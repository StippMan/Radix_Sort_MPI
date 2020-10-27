#!/bin/bash

SIZES="500000000"

for SIZE in $SIZES; do
  echo "=== radix_sort ==="
  ./radix_sort opa.txt $SIZE
  sleep 1
  echo ""
  echo "=== p_radix_sort ==="
  mpiexec -n 4 p_radix_sort opa.txt $SIZE
  sleep 1
  echo ""
done

