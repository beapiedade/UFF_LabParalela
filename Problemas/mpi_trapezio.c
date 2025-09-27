#include <stdio.h>
#include <math.h>
#include "mpi.h"

double funcao(double x) {
        double retorno = x*x;
        return retorno;
}

int main(int argc, char *argv[]) {
	int meu_ranque, num_procs;
	int origem, destino = 0;
	int etiq = 3; 
	double tempo_inicial, tempo_final; 
	
	double a = 0.0,  b = 1.0;
	long int n = 100000000;
	double h = (b - a) / n;
	double x;
	double integral = 0.0, total;
	double divisao, lim_inf, lim_sup;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	 
	if (meu_ranque == 0) {
		tempo_inicial = MPI_Wtime(); 
	}
	
	divisao = (n * h) / num_procs;
	lim_inf = a + meu_ranque * divisao;
	lim_sup = a + (meu_ranque + 1) * divisao;
	
	for (x = lim_inf; x < lim_sup; x += h) {
		integral += funcao(x);
	}
	
	integral = integral * h;

	if (meu_ranque == 0) { 
		total = integral; 
		for (origem = 1; origem < num_procs; origem++) { 
		     MPI_Recv(&integral, 1, MPI_DOUBLE, origem, etiq, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		     total += integral; 
		}
	} 

	else {
		MPI_Send(&integral, 1, MPI_DOUBLE, destino, etiq, MPI_COMM_WORLD);
	}

	if (meu_ranque == 0) {
		tempo_final = MPI_Wtime();
		printf("Com n = %ld trapezoides, a estimativa da integral de %lf até %lf = %lf \n", n, a, b, total);
		printf("Foram gastos %3.1f segundos\n\n", tempo_final-tempo_inicial);
	}
	MPI_Finalize();
	return(0);
} 
