#include <iostream>
#include <stdio.h>
#include "manometer.h"

#define NFREQ 15

void set_freqs(int * fr,AbstractMeter ** m)
{

  int afreqs[] = {200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000, 1100000, 1200000, 1300000, 1400000, 1500000, 1600000};
  float sfreqs[] = {200 , 300 , 400 , 500 , 600 , 700 , 800 , 900 , 1000 , 1100 , 1200 , 1300 , 1400 , 1500 , 1600};
  for(int mindex = 0; mindex<4 ; mindex++){
    for(int i=0;i<NFREQ;i++){
      if(fr[mindex] == afreqs[i]){
        m[mindex]->setValue(sfreqs[i]);
        break;
      }
    }
  }

}

