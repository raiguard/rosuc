#!/usr/bin/env sh

for name in $(find src -type f -path *.cpp); do
  if [ -z "$1" ] || [ "$name" != "src/Main.cpp" ]; then
    echo "$name"
  fi
done

if [ -n "$1" ]; then
  for name in $(find tests -type f -path *.cpp); do
    echo "$name"
  done
fi
