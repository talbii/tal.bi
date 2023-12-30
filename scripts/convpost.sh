#/bin/zsh
#
# convpost: convert a Markdown file into a post 
#
# If $SITE_ROOT is defined, that root is used, and otherwise cwd is used. Given
# root path R, the file R/src/routes/posts/<file>/+page.md is created. Its
# contents are copied from the original input file.
#
# Given multiple files, this process is done for each file.
#

if [[ $# -lt 1 ]]; then
    echo "I expect at least one argument!"
    exit 1
fi

if [[ -n $SITE_ROOT ]]; then
    root="$SITE_ROOT"
else
    root=$( pwd )
fi

dir="$root/src/routes/posts/"

function copy_file {
    local file="$1"
    local noext="${file%.*}"
    mkdir -p "$dir/$noext"
    cat $file > "$dir/$noext/+page.md"
}

for file in "$@"
do 
    copy_file $file
done


