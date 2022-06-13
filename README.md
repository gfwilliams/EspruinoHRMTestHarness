Bangle.js Heart rate monitor test harness
===========================================

This repo allows you to test out Espruino/Bangle.js's heart rate monitor
code on the desktop.

The actual HRM code is https://github.com/espruino/Espruino/blob/master/libs/misc/heartrate.c

## Getting data

* Get a bluetooth ECG heart rate monitor. Suggested: CooSpo HRM808S
* Install https://banglejs.com/apps/?id=bthrm 
* Settings -> Apps -> BT HRM
* Scan -> add
* From App Store, go to `HRM Accelerometer event recorder` : https://banglejs.com/apps/?id=hrmacceven­ts 
* Click 'Connect'
* Screen turns red
* Wait...
* Screen turns green and HRM data counter goes up
* Wait at least until 2000 samples recorded. More is ideal
* Click 'Save as CSV', save in `data` folder
* Edit `main.c` to add the file to the list 


## Running

Ensure the `Espruino` repo (https://github.com/espruino/Espruino) is installed at the same level as this repo,
as this code expects to use the code from Espruino itself.

Type `make && ./main` to run and execute the file (On Linux, Mac or WSL).

Right now, no comparison is done between Bluetooth HRM and calculated readings to get an accuracy value.


## Graphs

With `gnuplot` installed you can run `gnuplot -p gnuplot.plot` to plot graphs of acceleration vs step count/theshold data for debugging.
The `plotfiles.sh` script will plot all available data as images into the `plots` folder.

To compile, run and display the most recent graphs, you can do `pkill gnuplot;make clean;make && ./main && gnuplot -p gnuplot.plot`
