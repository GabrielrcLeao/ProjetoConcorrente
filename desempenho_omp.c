#include <stdio.h>
#include <omp.h>

#define NUM_FILOSOFOS 5
#define ROUNDS 100000000 / NUM_FILOSOFOS // Cada filósofo fará 20 milhões de iterações (100 milhões no total)

int main() {
    int garfos[NUM_FILOSOFOS] = {0};  // 0 significa que o garfo está disponível
    int contadorComidas[NUM_FILOSOFOS] = {0};
    int contadorPensamentos[NUM_FILOSOFOS] = {0};

    double inicio = omp_get_wtime();  // Inicia a medição do tempo

    #pragma omp parallel num_threads(NUM_FILOSOFOS)
    {
        int filosofo = omp_get_thread_num();
        int garfoEsquerdo = filosofo;
        int garfoDireito = (filosofo + 1) % NUM_FILOSOFOS; //garante que o filosofo 4 pegue o garfo 0

        for (int i = 0; i < ROUNDS; i++) {
            // Simula o filosofo pensando
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
            }

            // Simula o filosofo comendo
            contadorComidas[filosofo]++;

            // Simula o filosofo soltando os garfos
            #pragma omp critical
            {
                garfos[garfoEsquerdo] = 0;
                garfos[garfoDireito] = 0;
            }
        }
    }

    double fim = omp_get_wtime();  // Fim da medição do tempo
    printf("\nResumo final:\n");
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        printf("Filósofo %d: Comeu %d vezes e Pensou %d vezes.\n", i + 1, contadorComidas[i], contadorPensamentos[i]);
    }

    printf("Tempo de execução: %f segundos\n", fim - inicio);

    return 0;
}
