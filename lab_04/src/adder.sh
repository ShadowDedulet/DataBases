#!/bin/sh
# greeting script

if [ $# -gt 0 ]; then
  SUM=0;
  for word in "$@"
  do
    SUM=$((SUM + ${#word}));
  done

  echo "sum of lengths = $SUM";
  exit 0;
fi

echo "arguments not given" 1>&2;
exit 1;