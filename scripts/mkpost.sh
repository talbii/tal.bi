#/bin/zsh
#
# mkpost: creates a new post, and opens it.  
#
# - If $SITE_ROOT is defined, that root is used, and otherwise cwd is used.
# Given root path R, the directory R/src/routes/posts/ is created.  
# - If $EDITOR is defined, the post is opened using that editor. If no editor is
# specified, the post is not opened, but its path is printed to stdout.
#

if [[ $# -ne 1 ]]; then
    echo 'Usage: mkpost [title]'
    echo ' creates a new +page.md file at /posts/[title]. if $EDITOR is defined, also open the file for editing.'
    exit 1
fi

if [[ -n $SITE_ROOT ]]; then
    root="$SITE_ROOT"
else
    root=$( pwd )
fi

dir="$root/src/routes/posts/$1"
mkdir -p $dir
filepath="$dir/+page.md"

cat > $filepath <<- EOF
---
title: "$1"
date: $(date +"%Y-%m-%dT%H:%M:%S%z")
tags: []
---
EOF

if [[ -n $EDITOR ]]; then
    $EDITOR $filepath 
else
    echo "$filepath"
fi
