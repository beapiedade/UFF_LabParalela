#include <stdio.h>
#include <time.h>

#define TAM 1000

int main () {
    clock_t tempo_final, tempo_inicial = clock();
    double tempo_total;

    int x, soma = 0, subtracao = 0;
    long double multiplicacao = 1;
    int vet[TAM];

    for (x = 0; x < TAM; x++) {
        vet[x] = x + 1;
    }

    for (x = 0; x < TAM; x++) {
        printf("vet[%d] = %d\n", x, vet[x]);
    }

    for (x = 0; x < TAM; x++) {
        soma = soma + vet[x];
        subtracao = subtracao - vet[x];
        multiplicacao = multiplicacao * vet[x];
    }
    
    tempo_final = clock();
    tempo_total = (((double)tempo_final - (double)tempo_inicial) / CLOCKS_PER_SEC);
    printf("Soma = %d, ", soma);
    printf("Subtracao = %d, ", subtracao);
    printf("Multiplicacao = %Lf\n", multiplicacao);
	printf("TEMPO TOTAL: %.6fs\n", tempo_total);
}