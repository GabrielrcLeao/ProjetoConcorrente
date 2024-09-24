#include <omp.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5
#define ROUNDS 3

int main() {
  int garfos[NUM_FILOSOFOS] = {0}; // 0 significa que o garfo está disponível
  int contadorComidas[NUM_FILOSOFOS] = {0};
  int contadorPensamentos[NUM_FILOSOFOS] = {0};

#pragma omp parallel num_threads(NUM_FILOSOFOS)
  {
    int filosofo = omp_get_thread_num();
    int garfoEsquerdo = filosofo;
    int garfoDireito =
        (filosofo + 1) %
        NUM_FILOSOFOS; // garante que o filosofo 4 pegue o garfo 0

    for (int i = 0; i < ROUNDS; i++) {
      // Simula o filosofo pensando
      printf("Filósofo %d está pensando.\n", filosofo + 1);
      sleep(1);
      contadorPensamentos[filosofo]++;

      // Simula o filosofo pegando os garfos (se ambos estiverem disponíveis)
      int garfosPegos = 0;
      while (!garfosPegos) {
#pragma omp critical
        {
          if (garfos[garfoEsquerdo] == 0 && garfos[garfoDireito] == 0) {
            garfos[garfoEsquerdo] = 1;
            garfos[garfoDireito] = 1;
            garfosPegos = 1;
          }
        }
        if (!garfosPegos) {
          sleep(1);
        }
      }

      // Simula o filosofo comendo
      printf("Filósofo %d pegou os garfos %d e %d.\n", filosofo + 1,
             garfoEsquerdo + 1, garfoDireito + 1);
      printf("Filósofo %d está comendo.\n", filosofo + 1);
      sleep(2);
      contadorComidas[filosofo]++;

// Simula o filosofo soltando os garfos
#pragma omp critical
      {
        garfos[garfoEsquerdo] = 0;
        garfos[garfoDireito] = 0;
      }
      printf("Filósofo %d soltou os garfos %d e %d.\n", filosofo + 1,
             garfoEsquerdo + 1, garfoDireito + 1);
    }
  }

  printf("\nResumo final:\n");
  for (int i = 0; i < NUM_FILOSOFOS; i++) {
    printf("Filósofo %d: Comeu %d vezes e Pensou %d vezes.\n", i + 1,
           contadorComidas[i], contadorPensamentos[i]);
  }

  return 0;
}
