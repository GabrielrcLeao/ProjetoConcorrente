#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5
#define ROUNDS 3

int main() {
    int rank, tamanho;
    int garfos[NUM_FILOSOFOS] = {0}; 
    int contadorComidas[NUM_FILOSOFOS] = {0};
    int contadorPensamentos[NUM_FILOSOFOS] = {0};
    int estadoGarfos[2]; 

    MPI_Init(NULL, NULL); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &tamanho);

    if (tamanho != NUM_FILOSOFOS) {
        printf("Este programa deve ser executado com %d processos.\n", NUM_FILOSOFOS);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int garfoEsquerdo = rank;
    int garfoDireito = (rank + 1) % NUM_FILOSOFOS;

    for (int i = 0; i < ROUNDS; i++) {
        
        printf("Filósofo %d está pensando.\n", rank + 1);
        sleep(1);
        contadorPensamentos[rank]++;

        
        int garfosPegos = 0;
        while (!garfosPegos) {
            estadoGarfos[0] = garfos[garfoEsquerdo];
            estadoGarfos[1] = garfos[garfoDireito];
            int status;

            
            MPI_Allreduce(estadoGarfos, &status, 2, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

            if (status == 0) { 
                
                garfos[garfoEsquerdo] = 1;
                garfos[garfoDireito] = 1;
                garfosPegos = 1;

               
                printf("Filósofo %d pegou os garfos %d e %d.\n", rank + 1, garfoEsquerdo + 1, garfoDireito + 1);
            } else {
                sleep(1); 
            }
        }

        
        printf("Filósofo %d está comendo.\n", rank + 1);
        sleep(2);
        contadorComidas[rank]++;

        
        garfos[garfoEsquerdo] = 0;
        garfos[garfoDireito] = 0;
        printf("Filósofo %d soltou os garfos %d e %d.\n", rank + 1, garfoEsquerdo + 1, garfoDireito + 1);
    }

    
    MPI_Barrier(MPI_COMM_WORLD);

    
    MPI_Gather(&contadorComidas[rank], 1, MPI_INT, contadorComidas, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(&contadorPensamentos[rank], 1, MPI_INT, contadorPensamentos, 1, MPI_INT, 0, MPI_COMM_WORLD);

    
    if (rank == 0) {
        printf("\nResumo final:\n");
        for (int i = 0; i < NUM_FILOSOFOS; i++) {
            printf("Filósofo %d: Comeu %d vezes e Pensou %d vezes.\n", i + 1, contadorComidas[i], contadorPensamentos[i]);
        }
    }

    MPI_Finalize();
    return 0;
}
