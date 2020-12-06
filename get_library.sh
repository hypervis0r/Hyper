#!/bin/bash

PATH_TO_LIBRARY="../HyperProtocol/"
LIBRARY_COPY=`cp ${PATH_TO_LIBRARY}libhyper.a lib/libhyper.a`
INCLUDE_COPY=`cp -r ${PATH_TO_LIBRARY}include lib/`

$LIBRARY_COPY
echo "libhyper.a copied"
$INCLUDE_COPY
echo "Includes copied"
