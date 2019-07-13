#!/bin/sh
if [ $# -eq 4 ]; then
ref_engine=$1
comp_engine=$2
position_info=$3
depth=$4

echo "Depth =" $depth

if [ depth == 0 ]; then
echo "Depth is zero, I'm out"
exit 0
fi

# | cut -c -5

$ref_engine << EOF > tempFile1
$position_info
go perft $depth
quit
EOF

$comp_engine << EOF >> tempFile1
$position_info
go perft $depth
quit
EOF

cat tempFile1 | sort | uniq -u | grep -v "Stock" > tempFile2

if [ -s tempFile2 ]; then
cat tempFile2

#newDepth=`expr $4 - 1`
#$0 $1 $2 "$3" $newDepth
else
echo "Engines give identical results"
fi

else
	echo "Usage: ref_engine comp_engine position_info depth"
fi

rm tempFile{1,2} 2> /dev/null
