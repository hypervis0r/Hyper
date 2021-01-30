#!/bin/bash

PATH_TO_LIBRARY="../HyperProtocol/"
LIBRARY_COPY=`cp ${PATH_TO_LIBRARY}src/hyper.h lib/hyper.h`

$LIBRARY_COPY
echo "hyper.h copied"
