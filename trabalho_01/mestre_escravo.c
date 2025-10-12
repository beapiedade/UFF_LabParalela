#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define TAM 1000

int main(int argc, char** argv) {
    int x, soma, subtracao;
    long double multiplicacao;
    int lim_inf, lim_sup, fatia;
    int soma_total, subtracao_total;
    long double multiplicacao_total;

    int rank, num_procs, origem, destino = 0;
    double tempo_inicial, tempo_final, tempo_total;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Verifica se o número de processos é pelo menos 2
    if (num_procs < 2) {
        if (rank == 0) {
            printf("ERRO: número de processos %d diferente de 2\n", num_procs);
        }
        MPI_Finalize();
        return 1;
    }

    // Processo Mestre
    if (rank == 0) {
        tempo_inicial = MPI_Wtime();
        soma_total = 0;
        subtracao_total = 0;
        multiplicacao_total = 1;

        for (int origem = 1; origem < num_procs; origem++) {
            // O mestre espera para receber os 3 resultados de cada escravo
            MPI_Recv(&soma, 1, MPI_INT, origem, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&subtracao, 1, MPI_INT, origem, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&multiplicacao, 1, MPI_LONG_DOUBLE, origem, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // Acumula os resultados
            soma_total += soma;
            subtracao_total += subtracao;
            multiplicacao_total *= multiplicacao;
        }

        tempo_final = MPI_Wtime();
        tempo_total = tempo_final - tempo_inicial;
        printf("Soma = %d, ", soma_total);
        printf("Subtracao = %d, ", subtracao_total);
        printf("Multiplicacao = %Lf\n", multiplicacao_total);
        printf("TEMPO DE EXECUCAO: %.6f segundos\n", tempo_total);
    
    // Processo Escravo
    }  else {
        //Estabelece os limites
        fatia = TAM / (num_procs - 1);
        lim_inf = (rank - 1) * fatia;
        lim_sup = rank * fatia;
        
        // O processo com maior limite superior ajusta para TAM
        if (TAM - lim_sup == TAM % (num_procs - 1)) {
            lim_sup = TAM;
        }

        // Calcula soma, subtração e multiplicação
        printf("Processo %d -> Limites [%d, %d)\n", rank, lim_inf, lim_sup);
        soma = 0;
        subtracao = 0;
        multiplicacao = 1;
        for (x = lim_inf; x < lim_sup; x++) {
            soma += (x + 1);
            subtracao -= (x + 1);
            multiplicacao *= (x + 1);
        }

        // Envia os 3 resultados para o mestre
        MPI_Send(&soma, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&subtracao, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&multiplicacao, 1, MPI_LONG_DOUBLE, 0, 2, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}