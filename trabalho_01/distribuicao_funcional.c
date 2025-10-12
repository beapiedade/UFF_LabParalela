#include <stdio.h>
#include <mpi.h>

#define TAM 1000

int main(int argc, char** argv) {
    int x, soma, subtracao;
    long double multiplicacao;
    int vet[TAM];

    int rank, num_procs;
    double tempo_inicial, tempo_final, tempo_total;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Verifica se o número de processos é 4
    if (num_procs != 4) {
        if (rank == 0) {
            printf("ERRO: número de processos %d diferente de 4\n", num_procs);
        }
        MPI_Finalize();
        return 1;
    }

    MPI_Barrier(MPI_COMM_WORLD); 
    tempo_inicial = MPI_Wtime();

    // Inicializa o vetor em todos os processos exceto no mestre
    if (rank > 0) {
        for (x = 0; x < TAM; x++) {
            vet[x] = x + 1;
        }
    }

    // Funcionalidade do Mestre
    if (rank == 0) {
        // MPI_Recv(buffer, contagem, tipo, origem, tag, comunicador, status)
        MPI_Recv(&soma, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&subtracao, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&multiplicacao, 1, MPI_LONG_DOUBLE, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        tempo_final = MPI_Wtime();
        tempo_total = tempo_final - tempo_inicial;
        printf("Soma = %d, ", soma);
        printf("Subtracao = %d, ", subtracao);
        printf("Multiplicacao = %Lf\n", multiplicacao);
        printf("TEMPO TOTAL: %.6fs\n", tempo_total);
    
    // Funcionalidade do processo somador
    } else if (rank == 1) { 
        soma = 0;
        for (x = 0; x < TAM; x++) {
            soma += vet[x];
        }

        // MPI_Send(buffer, contagem, tipo, destino, tag, comunicador)
        MPI_Send(&soma, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    // Funcionalidade do processo subtrator
    } else if (rank == 2) { 
        subtracao = 0;
        for (x = 0; x < TAM; x++) {
            subtracao -= vet[x];
        }
        MPI_Send(&subtracao, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    // Funcionalidade do processo multiplicador
    } else if (rank == 3) { 
        multiplicacao = 1;
        for (x = 0; x < TAM; x++) {
            multiplicacao *= vet[x];
        }
        MPI_Send(&multiplicacao, 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}