#!/bin/sh

! command -v perl > /dev/null && echo perl not found && exit 1
! command -v gnuplot > /dev/null && echo gnuplot not found && exit 1

PLOTFILE=$(mktemp)
mkdir -p plots

find data -type f -not -name "*out*" | while read CURRENT
do
	echo Plotting "$CURRENT"
	OUT=plots/$(basename "$CURRENT").png
	cat "$CURRENT" | perl -pe 's/,(?=,)/,"#"/g' > "$PLOTFILE"
	gnuplot -e "datafile=\"$PLOTFILE\"" -e "algofile=\"$CURRENT.out.csv\"" -e "outputfile=\"$OUT\"" -p image.gnuplot
done

echo Files are in ./plots
rm "$PLOTFILE"
