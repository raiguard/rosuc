#!/usr/bin/env sh

for name in $(find src -type f -path *.cpp); do
  echo "$name"
done
