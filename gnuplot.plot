set datafile separator ","
set lmargin 5
set rmargin 5

# You can comment some of these out with hash

#set term wxt 0
set title "heart rate plot"
show title
plot "data/gw-bangle2.1-sitting-raised.csv.out.csv" using 5 title 'raw' with lines, \
     "" using 6 title 'filt' with lines, \
     "" using 7 title 'avg' with lines, \
     "" using 8 title 'BEAT' with lines, \
     "" using 9 title 'wasLow' with lines

#set term wxt 1
#....
