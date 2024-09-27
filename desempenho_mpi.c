#include <mpi.h>
#include <stdio.h>

#define NUM_FILOSOFOS 5
#define ROUNDS 100000000 / NUM_FILOSOFOS 

int main() {
    int rank, tamanho;
    int garfos[NUM_FILOSOFOS] = {0};  
    int contadorComidas = 0;          
    int contadorPensamentos = 0;      
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

    double inicio = MPI_Wtime();  

    for (int i = 0; i < ROUNDS; i++) {
        
        contadorPensamentos++;

       
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
            }
        }

        
        contadorComidas++;

        
        garfos[garfoEsquerdo] = 0;
        garfos[garfoDireito] = 0;
    }

    double fim = MPI_Wtime();  

    
    int totalComidas[NUM_FILOSOFOS];
    int totalPensamentos[NUM_FILOSOFOS];

    MPI_Gather(&contadorComidas, 1, MPI_INT, totalComidas, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(&contadorPensamentos, 1, MPI_INT, totalPensamentos, 1, MPI_INT, 0, MPI_COMM_WORLD);

    
    if (rank == 0) {
        printf("\nResumo final:\n");
        for (int i = 0; i < NUM_FILOSOFOS; i++) {
            printf("Filósofo %d: Comeu %d vezes e Pensou %d vezes.\n", i + 1, totalComidas[i], totalPensamentos[i]);
        }
        printf("Tempo de execução: %f segundos\n", fim - inicio);
    }

    MPI_Finalize();  
    return 0;
}
