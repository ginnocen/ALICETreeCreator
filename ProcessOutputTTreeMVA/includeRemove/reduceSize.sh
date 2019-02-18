#!/bin/bash

linenew=$1
lineold=$2

hadd "${linenew}" "${lineold}"
rm "${lineold}"
mv "${linenew}" "${lineold}"
