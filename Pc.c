#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <math.h>
#include "ergasia2.h"

int main(void){
  struct msg m;

  srand(getpid());                                          // INTIALIZE SYNARTHSH RAND

  while(1){                                                 // ATERMON VROGXOS
                                                            // DIAVAZOYME APO STREAM(ROH) GIA EISODOS
		if(read(fileno(stdin), &m, sizeof(struct msg)) <= 0){   // EN ANAMONH AITHMATOS APO PM -- APOTYXIA = -1
      break;
    }
    const int size = m.size;
    if(size == 0){                                          // AN EXOYME ANTAPOKRISH ME TO ANALOGO MNM
      break;                                                // STOP
    }

    const float x = (float)rand()/((float) RAND_MAX / size);
    const float y = (float)rand()/((float) RAND_MAX / size);

    const float d = sqrt(x*x +y*y);

    m.result = (d < size) ? 1 : 0;                          // PERNAME APOTELESMATA
                                                            // XEKLEIDWMA MAMAS KAI ANAMONH GIA APOTELESMATA
		if(write(fileno(stdout), &m, sizeof(struct msg)) < 0){  // GRAFOYME STHN STREAM(ROH) EXODOS
      break;
    }
  }
  return EXIT_SUCCESS;
}
