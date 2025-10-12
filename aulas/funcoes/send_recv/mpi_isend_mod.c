#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[]) { /* mpi_isend.c  */
int i, meu_ranque, num_procs;
int pot2, destino, meu_valor;
int reducao, recebido, etiq=1, cont=1;
MPI_Status estado;
MPI_Request pedido_envia;
MPI_Request pedido_recebe;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &meu_ranque);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    pot2 = num_procs;
    while (((pot2 % 2) == 0) && pot2 > 1)
        pot2 /= 2;
    if (pot2 != 1) {
        if (meu_ranque == 0)
           printf("Por favor execute com número de processos potencia de 2 \n");
        MPI_Finalize();
        exit(0);
        }

    meu_valor = meu_ranque*num_procs;
    reducao = meu_valor;

    for (i = 1; i <= (num_procs/2); i += i) {
        if ((meu_ranque/i)%2 == 0) 
            destino = meu_ranque + i;
        else 
            destino = meu_ranque-i;
            
    	// (2) - Original invertido
        MPI_Isend(&reducao, cont, MPI_INT, destino, etiq, MPI_COMM_WORLD, &pedido_envia);
        MPI_Irecv(&recebido, cont, MPI_INT, destino, etiq, MPI_COMM_WORLD, &pedido_recebe);
        //MPI_Isend(&reducao, cont, MPI_INT, destino, etiq, MPI_COMM_WORLD, &pedido_envia);

	// (1) - Isend-Irecv por Send-Recv
	// (3) - Irecv-Isend por Recv-Send DEADLOCK
        //MPI_Send(&reducao, cont, MPI_INT, destino, etiq, MPI_COMM_WORLD);
        //MPI_Recv(&recebido, cont, MPI_INT, destino, etiq, MPI_COMM_WORLD, &estado);
        //MPI_Send(&reducao, cont, MPI_INT, destino, etiq, MPI_COMM_WORLD); 

	// (4) - Original sem wait */
        MPI_Wait(&pedido_envia, &estado);
        MPI_Wait(&pedido_recebe, &estado);
 

        if (recebido > reducao)
            reducao = recebido;
    }
    printf("Meu valor = %d, redução = %d \n", meu_valor, reducao);
    MPI_Finalize();
    return(0);
}
