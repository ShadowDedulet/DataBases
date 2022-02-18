#!/bin/sh
# greeting script


if [ $# -gt 0 ]; then
        echo "hello, $1";
        exit 0;
fi

echo "hello";
echo "name not provided" 1>&2;
exit 1;
