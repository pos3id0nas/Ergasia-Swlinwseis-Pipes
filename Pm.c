#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "ergasia2.h"

int main(const int argc, char * argv[]){
  int i, *fdin, *fdout;                                                         // DHLWSEIS - ARXIKOPOIHSEIS
  unsigned int *K, *T, *sizes;
  struct msg m;

  if(argc < 3){                                                                 // ELEGXOS ARGS
    fprintf(stderr, "LATHOS XRHSH: ./ergasia2pipes <size> [ARGS.?.] <num_tests?>\n");
    return EXIT_FAILURE;
  }

  const int num_procs = argc - 2;                                               // PAIDIA
  const int num_tests = atoi(argv[argc-1]);                                     // PLITHOS EPANALYPSEWN(TESTS)
                                                                                // DESMEYSEIS
  sizes     = (unsigned int*) calloc(num_procs, sizeof(int));                   // PINAKAS MEGETHOI TWN KYKLWN KAI COUNTERS
  K = (unsigned int*) calloc(num_procs, sizeof(int));                           // SHMEIA ENTOS TOY KYKLOU
  T = (unsigned int*) calloc(num_procs, sizeof(int));                           // SYNOLIKA SHMEIA POU EXOYN TESTARISTEI GIA TO SYGKEKRIMENO

                                                                                // DESMEYSEIS TWN PIPES IDS - 2 PIPES GIA KATHE DIERGASIA(2*)
	fdin  = (int*) calloc(num_procs, 2*sizeof(int));                              // GIA EISODO SWLHNA
	fdout = (int*) calloc(num_procs, 2*sizeof(int));                              // GIA EXODO SWLHNA

  for(i=0; i < num_procs; i++){
    sizes[i] = atoi(argv[1 + i]);
  }
  srand(getpid());                                                              // ARXIKOPOIHSH SYNARTHSHS RAND() PARAGWGH ARITHMWN
  for(i=0; i < num_procs; i++){                                                 // FOR LOOP DIADIKASIA GIA KATHE PAIDI
		if(	(pipe(&fdin[i*2]) == -1) || (pipe(&fdout[i*2]) == -1)){                 // DHMIOYRGIA TWN SWLHNWSEWN(PIPES) APOTYXIA=-1
			perror("pipe");
		}
    const pid_t pid = fork();                                                   // OK,PARAGWGH ANTIGRAFWN
    if(pid == -1){                                                              // APOTYXIA = -1
      perror("fork");
      return EXIT_FAILURE;
    }else if(pid == 0){                                                         // EPITYXIA = 0 (H GENNA PETYXE)

			close(fdin[(i*2)  + 1]);                                                  // KLEISIMO GRAPSIMATOS EISODOY
	    close(fdout[(i*2) + 0]);                                                  // KLEISIMO DIAVASMATOS EXODOY

			                                                                          // STELNOYME EISODO KAI EXODO STOYW SWLHNES ANTISTROFA
	    if( (dup2(fdin[(i*2) + 0],  fileno(stdin)) == -1)   ||	                  // EISAGWGH DEDOMENWN SE SWLHNA (DIAVASMA PAIDI)
	        (dup2(fdout[(i*2) + 1], fileno(stdout)) == -1) ){		                  // GRAFOYME APOTELESMATA PAIDIOY STHN EXODO TOY SWLHNA 
	      perror("dup2");
	      exit(EXIT_FAILURE);
	    }

      execl("./Pc", "./Pc", NULL);                                              // EXEC(l) PAME APO THN ALLH
      perror("execl");
      exit(EXIT_FAILURE);
    }else{
			close(fdin[(i*2) + 0]);                                                   // KLEISIMO - TELOS DIAVASMATOS DEDOMENWN EISODOY
	    close(fdout[(i*2) + 1]);                                                  // KLEISIMO - TELOS GRAPSIMATOS DEDOMENWN EXODOU
		}
  }

  for(i=0; i < num_tests; i++){
    const int idx = rand() % num_procs;                                         // PARAGWGH TYXAIWN SHMEIWN
    m.size   = sizes[idx];                                                      // SETARW THN DIAMOIRAZOMENH MNHMH
    m.result = -1;                                                              // XWRIS APOTELESMA

		if(	(write(fdin[(idx*2) + 1], &m, sizeof(struct msg)) < 0) ||               // STELNOYME PROS KAI DEXOMASTE APO THN DIERGASIA
				(read(fdout[(idx*2) + 0], &m, sizeof(struct msg)) < 0) ){
      break;
    }

    if(m.result == 1){                                                          // UPDATE TOYS METRHTES SYMFWNA ME TA APOTELESMATA
      K[idx]++;
    }
    T[idx]++;
  }
  printf("Tests: %d\n", i);                                                     // PRWTH EKTYPWSH
  m.size = 0;                                                                   // APOSTOLH MHNYMATOS EXODOY
  for(i=0; i < num_procs; i++){
		write(fdin[(i*2) + 1], &m, sizeof(struct msg));                             // APOSTOLH "0" GIA EXODO DIERGASIWN

		close(fdin[(i*2) + 1]);                                                     // KLEISIMO - TELOS DIAVASMATOS INPUTS
		close(fdout[(i*2) + 9]);                                                    // KLEISIMO - TELOS GRAPSIMATOS OUTPUTS
  }

  for(i=0; i < num_procs; i++){                                                 // ANAMONH OLOKLHRWSHS PAIDIOY
    wait(NULL);
  }
                                                                                // EKTYPWSEIS
  for(i=0; i < num_procs; i++){
    printf("T(%d)=%d ", sizes[i], T[i]);
  }
  printf("\n");

  for(i=0; i < num_procs; i++){
    printf("K(%d)=%d ", sizes[i], K[i]);
  }
  printf("\n");

  for(i=0; i < num_procs; i++){
    const float pi = 4* (float) K[i] / T[i];
    printf("P(%i) = %.2f\n", sizes[i], pi);
  }

  free(sizes);                                                                 // APODESMEYSEIS 
  free(K);
  free(T);
	free(fdin);
	free(fdout);

  return EXIT_SUCCESS;
}
