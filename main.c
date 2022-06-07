#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define PATH "data/"

#define FILECOUNT 4
const char *FILES[FILECOUNT] = {
 "gw-bangle2.1-sitting.csv",
 "gw-bangle2.1-sitting-raised.csv",
 "gw-bangle2.1-typing.csv",
 "gw-bangle2.1-wander.csv"
};

int HOWMUCH[FILECOUNT] = {1,1,1,1}; // how much do we care about these?


#define DEBUG 0
#define HEARTRATE_DEVICE_VC31 1
const uint16_t hrmPollInterval = 40; // default Bangle.js 2 rate

#include "../Espruino/libs/misc/heartrate.c"

typedef struct {
  short x,y,z;
} Vector3;

// =====================================
const char *linescanstr = 0;
void linestart(const char *src) {
  linescanstr = src;
}
const char *linetoken() {
  static char token[256];
  int n = 0;
  while (*linescanstr && *linescanstr!=',' && *linescanstr!='\n') {
    token[n++] = *(linescanstr++);
  }
  if (*linescanstr==',' || *linescanstr=='\n')
    linescanstr++;
  token[n] = 0;
  return token;
}
char linetokenempty() {
  return (*linescanstr==',') || (*linescanstr=='\n') || !*linescanstr;
}
// =====================================

void testHRM(char *filename, char *outfile) {
  printf("Test %s\n-----------------------------\n", filename);
  // init
  hrm_init();
  // go
  char * line = NULL;
  size_t len = 0;
  int read;
  FILE *fp = fopen(filename, "r");
  FILE *fop = 0;
  if (outfile) {
    fop = fopen(outfile, "w");
    fprintf(fop, "n,x,y,z,hrmRaw,hrmFilt,hrmAvr,beat,wasLow\n");
  }
  Vector3 accel;
  bool first = true;
  int n = 0;
  while ((read = getline(&line, &len, fp)) != -1) {
    //printf("> %s", line);
    // Time,Acc_x,Acc_y,Acc_z,HRM_b,HRM_c,HRM_r,HRM_f,PPG_r,PPG_o,BTHRM
    linestart(line);
    currentSystemTime = (long)strtod(linetoken(), NULL); // actually a double...
    if (!linetokenempty()) {
        accel.x = (short)(int)(8192*strtod(linetoken(), NULL));
        accel.y = (short)(int)(8192*strtod(linetoken(), NULL));
        accel.z = (short)(int)(8192*strtod(linetoken(), NULL));
    } else {
        linetoken();linetoken();linetoken();
    }
    linetoken(); // HRM_b - calculated BPM with current firmware (plus BTHRM output? :/)
    linetoken(); // HRM_c - calculated confidence with current firmware (plus BTHRM output? :/)
    if (!linetokenempty()) {
        int hrmRaw = (int)strtol(linetoken(), NULL, 10); // HRM_r raw
        //printf("- %d\n", hrmRaw);
        if (!first) {
          bool beat = hrm_new(hrmRaw);
          if (beat) printf("HRM %d bpm, confidence %d%%\n", hrmInfo.bpm10/10, hrmInfo.confidence);
          fprintf(fop, "%d,%d,%d,%d,%d,%d,%d,%d,%d\n", n++,accel.x,accel.y,accel.z,hrmRaw,hrmInfo.filtered,hrmInfo.avg,beat*10000,hrmInfo.wasLow*100 );
        }
        first = false;
    } else {
        linetoken(); // HRM_r
    }
    linetoken(); // HRM_f
    linetoken(); // PPG_r
    linetoken(); // PPG_o
    if (!linetokenempty()) {
        int btHRM = (int)strtol(linetoken(), NULL, 10); // BTHRM
        printf("BT HRM %d\n", btHRM);
    }
  }
  if (fop) fclose(fop);
  fclose(fp);
  if (line)
  free(line);
  printf("\n");
}

static int testAll(bool outputFiles) {
  int fileCnt = 0;
  int differences = 0;
  // show the config and output format
  if (outputFiles) {
    printf("File, Expected, Simulated, Diff, %%, (Original)\n");
  }
  while (fileCnt < FILECOUNT) {
    char buf[256], obuf[256];
    strcpy(buf, PATH);
    strcat(buf, FILES[fileCnt]);
    strcpy(obuf, buf);
    strcat(obuf, ".out.csv");
    //if (outputFiles) printf("VVV %s\n", FILES[fileCnt]);
    testHRM(buf, outputFiles ? obuf : NULL);

    // work out accuracy %
   /* float pc;
    if (EXPECTED_STEPS[fileCnt] != 0)
      pc = (100*(float)stepCounter / (float)EXPECTED_STEPS[fileCnt]);
    else
      pc = 0.00;
   
    if (outputFiles) printf("%s, %d, %d, %d, %2.2f %%, (%d)\n", FILES[fileCnt], EXPECTED_STEPS[fileCnt],
			    stepCounter,  stepCounter - EXPECTED_STEPS[fileCnt], pc, origStepCounter);
    int d = stepCounter - EXPECTED_STEPS[fileCnt];
    differences += d*d*HOWMUCH[fileCnt];*/
    fileCnt++;
  }

  return differences;
}


int main(int argc, char *argv[]) {
  printf("github.com/gfwilliams/EspruinoHRMTestHarness\n");
  printf("---------------------------------------------\n");

  bool bruteForce = false;
  //printf("argc %d\n",argc);
  if (argc>1) {
    if (strcmp(argv[1],"--bruteforce") == 0) { // match
      bruteForce = true;
    } else {
      printf("Unknown argument!\n\n");
      printf("USAGE:\n");
      printf(" ./main\n");
      printf("   Run single test on all available step data\n");
//      printf(" ./main --bruteforce\n");
//      printf("   Brute-force all available arguments on all available step data\n");
      return 1;
    }
  }

  int d = testAll(true);
  printf("TOTAL DIFFERENCE %d\n", int_sqrt32(d));
  // =======================
  // comment this out to brute-force over the data to find the best coefficients
/*  if (!bruteForce) return 0;
  // =======================
  int bestDiff = 0xFFFFFFF;
  int best_stepCounterThreshold = 0;

  for (stepCounterThreshold = STEPCOUNTERTHRESHOLD_MIN; stepCounterThreshold<=STEPCOUNTERTHRESHOLD_MAX; stepCounterThreshold+=STEPCOUNTERTHRESHOLD_STEP) {
    printf("testing %d \n", stepCounterThreshold);
    int d = testAll(false);
    if (d<bestDiff) {
      printf("           BEST %d\n", d);
      bestDiff = d;
      best_stepCounterThreshold = stepCounterThreshold;
    }
  }

  printf("best difference %d\n", int_sqrt32(d));
  printf("stepCounterThreshold %d\n", best_stepCounterThreshold);*/
  return 0;
}
