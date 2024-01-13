#!/bin/bash

set -e

make

if [ -f logapp ]; then
	echo "Build successful"

	rm -rf /output/*
	mkdir -p /output/tmp/bin

	cp logapp /output/tmp/bin/logapp
	echo "logapp" > /output/tmp/init
	cd /output/tmp
	zip -r /output/logapp.zip .
	rm -rf /output/tmp
else
	echo "Build failed"
	exit 1
fi