#!/bin/sh

echo KNOTEDITOR_ROOT=$1
echo KNOTEDITOR_DATA=\$KNOTEDITOR_ROOT/data
echo
echo export KNOTEDITOR_ROOT KNOTEDITOR_DATA
echo
echo \$KNOTEDITOR_ROOT/bin/KnotEditor \$@
