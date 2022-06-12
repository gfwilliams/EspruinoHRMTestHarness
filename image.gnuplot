set datafile separator ','
set datafile missing '#'
set key top right outside autotitle columnhead
set size ratio 0.5 1,1
set term pngcairo size 1920,1920
set output outputfile


set multiplot layout 3,1

set title "Accelerometer and HRM"
set xtics nomirror rotate by -90

set ytics nomirror
set y2tics nomirror

set pointintervalbox 0.3
set style line 1 linewidth 1 pointtype 7 pointsize 0.1 pointinterval -1
set style line 2 linewidth 1 pointtype 7 pointsize 0.5 pointinterval -1

plot datafile using 1:2 with linespoints ls 1 linecolor "dark-green" axis x1y2, \
  "" using 1:3 with linespoints ls 1 linecolor "red" axis x1y2, \
  "" using 1:4 with linespoints ls 1 linecolor "blue" axis x1y2, \
  "" using 1:9 with linespoints ls 1 linecolor "gray", \
  "" using 1:10 with linespoints ls 1 linecolor "black", \
  "" using 1:7 with linespoints ls 1 linecolor "cyan" \

set title "BTHRM and HRM"

plot datafile using 1:5 with linespoints ls 2 linecolor "green", \
  "" using 1:6 with linespoints ls 1 linecolor "orange", \
  "" using 1:11 with linespoints ls 2 linecolor "purple", \
  "" using 1:8 with linespoints ls 1 linecolor "magenta" axis x1y2 

set title "Espruino HR algorithm"
plot algofile using 5 title 'raw' with linespoints ls 1 linecolor "purple", \
     "" using 6 title 'filt' with linespoints ls 1 linecolor "green", \
     "" using 7 title 'avg' with linespoints ls 1 linecolor "blue", \
     "" using 8 title 'BEAT' with linespoints ls 1 linecolor "brown", \
     "" using 9 title 'wasLow' with linespoints ls 1 linecolor "yellow" axis x1y2
