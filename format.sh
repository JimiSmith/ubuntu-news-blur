#!/bin/sh 

if [ ! -n "$1" ]; then
echo "Syntax is: recurse.sh dirname filesuffix"
echo "Syntax is: recurse.sh filename"
echo "Example: recurse.sh temp cpp"
exit 1
fi

if [ -d "$1" ]; then
#echo "Dir ${1} exists"
if [ -n "$2" ]; then
filesuffix=$2
else
filesuffix="*"
fi

#echo "Filtering files using suffix ${filesuffix}"

file_list=`find ${1} -name "*.${filesuffix}" -type f`
for file2indent in $file_list
do 
echo "Indenting file $file2indent"
#!/bin/bash
astyle "$file2indent" --options="./format.astylerc"

done
else
if [ -f "$1" ]; then
echo "Indenting one file $1"
#!/bin/bash
astyle "$1" --options="./format.astylerc"

else
echo "ERROR: As parameter given directory or file does not exist!"
echo "Syntax is: format.sh dirname filesuffix"
echo "Syntax is: format.sh filename"
echo "Example: format.sh temp cpp"
exit 1
fi
fi
