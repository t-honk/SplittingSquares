#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo "Usage: $0 <target_version>"
	exit 1
fi

proj_dir="C:/Users/stone/Documents/Code/C/Splitting Squares"
staging_dir="C:/Users/stone/Documents/Code/C/Splitting Squares - Staging"
ver_dir="$staging_dir/$1"
zip_name="SplittingSquares-$1_win.zip"
mkdir -p "$ver_dir"

make clean
make windows

cp "$proj_dir/bin/SplittingSquares.exe" "$ver_dir"
cp -r "$staging_dir/libsdl2/." "$ver_dir"
cp -r "$proj_dir/fonts" "$ver_dir"
7z a -tzip -mx9 "$staging_dir/$zip_name" "$ver_dir/."
