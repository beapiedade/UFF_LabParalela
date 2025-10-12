#include <stdio.h>
#include <mpi.h>

#define TAM 1000
#define OPS 3 

int main(int argc, char** argv) {
    long double soma, subtracao, multiplicacao;
    int vet[TAM];
    long double resultados[OPS]; // [soma, subtracao, multiplicacao]

    int rank, num_procs;
    double tempo_inicial, tempo_final, tempo_total;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Verifica se o número de processos é o suficiente
    if (num_procs != OPS + 1) {
        if (rank == 0) {
            printf("ERRO: número de processos %d diferente de %d\n", num_procs, OPS + 1);
        }
        MPI_Finalize();
        return 1;
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    tempo_inicial = MPI_Wtime();

    // Processo 0: Inicialização e cálculo da soma
    if (rank == 0) { 
        resultados[0] = 0;
        resultados[1] = 0;
        resultados[2] = 1;
        for (int x = 0; x < TAM; x++) {
            vet[x] = x + 1;
        }

        for (int x = 0; x < TAM; x++) {
            resultados[0] += vet[x];
        }

    // Recebe os dados do estágio anterior
    } else {
        MPI_Recv(vet, TAM, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(resultados, OPS, MPI_LONG_DOUBLE, rank-1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Processo 1: Cálculo da subtração
    if (rank == 1) {
        for (int x = 0; x < TAM; x++) {
            resultados[1] -= vet[x];
        }

    // Processo 2: Cálculo da multiplicação
    } else if (rank == 2) {
        for (int x = 0; x < TAM; x++) {
            resultados[2] *= vet[x];
        }
        
    // Processo 3: Recebimento do vetor e resultados parciais
    } else {
        tempo_final = MPI_Wtime();
        tempo_total = tempo_final - tempo_inicial;
        printf("Soma = %.0Lf, ", resultados[0]);
        printf("Subtracao = %.0Lf, ", resultados[1]);
        printf("Multiplicacao = %Lf\n", resultados[2]);
        printf("TEMPO TOTAL: %.5fs\n", tempo_total);
    }
    
    // Se não for o último processo, envia o vetor original e os resultados parciais para o próximo processo
    if (rank < num_procs - 1) {
        MPI_Send(vet, TAM, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        MPI_Send(resultados, OPS, MPI_LONG_DOUBLE, rank + 1, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}