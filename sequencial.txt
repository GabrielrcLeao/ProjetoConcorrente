#include <stdio.h>
#include <time.h>

#define NUM_FILOSOFOS 5
#define ROUNDS 200000000  // Cada filósofo fará 20 milhões de iterações (100 milhões no total)

int main() {
    int garfos[NUM_FILOSOFOS] = {0};  
    int contadorComidas[NUM_FILOSOFOS] = {0};
    int contadorPensamentos[NUM_FILOSOFOS] = {0};

    clock_t inicio = clock();  

    for (int i = 0; i < ROUNDS; i++) {
        for (int filosofo = 0; filosofo < NUM_FILOSOFOS; filosofo++) {
            int garfoEsquerdo = filosofo;
            int garfoDireito = (filosofo + 1) % NUM_FILOSOFOS; 

           
            contadorPensamentos[filosofo]++;

            
            int garfosPegos = 0;
            while (!garfosPegos) {
                if (garfos[garfoEsquerdo] == 0 && garfos[garfoDireito] == 0) {
                    garfos[garfoEsquerdo] = 1; 
                    garfos[garfoDireito] = 1;  
                    garfosPegos = 1;           
                }
            }

           
            contadorComidas[filosofo]++;

            
            garfos[garfoEsquerdo] = 0; 
            garfos[garfoDireito] = 0;  
        }
    }

    clock_t fim = clock();  
    double tempoExecucao = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\nResumo final:\n");
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        printf("Filósofo %d: Comeu %d vezes e Pensou %d vezes.\n", i + 1, contadorComidas[i], contadorPensamentos[i]);
    }

    printf("Tempo de execução: %f segundos\n", tempoExecucao);

    return 0;
}