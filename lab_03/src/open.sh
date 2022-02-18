#!/bin/bash
# open script


if [ $# -gt 0 ]; then
        app_path=$1;
        $app_path 1>stdout.log 2>stderr.log;
        echo return $?>return.log;
        cat stdout.log stderr.log return.log;
        exit 0;
fi
echo "path not provided">&2;
exit 1;
