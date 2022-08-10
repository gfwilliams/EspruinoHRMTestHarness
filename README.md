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


## CSV data format

The CSV input data contains the following columns:

* Time - Current time (milliseconds since 1970)
* Acc_x,Acc_y,Acc_z - X,Y,Z acceleration in Gs
* HRM_b - BPM figure reported by internal HRM algorithm in Bangle.js (we are running our own algorithm in this repo)
* HRM_c - BPM confidence figure (0..100%) reported by internal HRM algorithm in Bangle.js (we are running our own algorithm in this repo)
* HRM_r - `e.raw` from the `Bangle.on("HRM-raw"` event. This is the value that gets passed to the HRM algorithm.
* HRM_f - `e.filt` from the `Bangle.on("HRM-raw"` event. This is the filtered value that comes from the Bangle's HRM algorithm and which is used for peak detection (we are running our own algorithm in this repo)
* PPG_r - `e.vcPPG` from the `Bangle.on("HRM-raw"` event. This is the PPG value direct from the sensor
* PPG_o - `e.vcPPGoffs` from the `Bangle.on("HRM-raw"` event. This is the PPG offset used to map `e.vcPPG` to `e.raw` so there are no glitches when the exposure values in the sensor change.
* BTHRM - BPM figure from external Bluetooth HRM device (this is our reference BPM)

## Graphs

With `gnuplot` installed you can run `gnuplot -p gnuplot.plot` to plot graphs of acceleration vs step count/theshold data for debugging.
The `plotfiles.sh` script will plot all available data as images into the `plots` folder.

To compile, run and display the most recent graphs, you can do `pkill gnuplot;make clean;make && ./main && gnuplot -p gnuplot.plot`
