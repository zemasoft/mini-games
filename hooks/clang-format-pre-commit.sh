#!/bin/bash

set -e

including_paths=(
  ^SlidingPuzzle/src/
  ^Snake/src/
  ^Videostop/src/
)

excluding_paths=(
)

extensions="c|h"

if git rev-parse --verify HEAD >/dev/null 2>&1
then
  against=HEAD
else
  against=$(git hash-object -t tree /dev/null)
fi

for name in $(git diff --staged --name-only --diff-filter=AM ${against})
do

  include=false
  for including_path in "${including_paths[@]}"
  do
    if [[ ${name} =~ ${including_path} ]]
    then
      include=true
      break
    fi
  done

  if [[ ${include} == false ]]
  then
    continue
  fi

  exclude=false
  for excluding_path in "${excluding_paths[@]}"
  do
    if [[ ${name} =~ ${excluding_path} ]]
    then
      exclude=true
      break
    fi
  done

  if [[ ${exclude} == true ]]
  then
    continue
  fi

  if [[ ! ${name} =~ \.(${extensions})$ ]]
  then
    continue
  fi

  clang-format -i --style=file ${name}
  git add ${name}

done
