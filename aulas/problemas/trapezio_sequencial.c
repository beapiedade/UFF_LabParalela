#include <stdio.h>

float funcao(float x) {
	float resultado = x*x;
	return resultado;
}

int main(int argc, char *argv[]) {
	float integral;
	float h;
	float a, b, n;
	float x;
	int i;

	/* ENTRADA */
	printf("Digite a, b, e n \n");
	scanf("%f %f %f", &a, &b, &n);


	/* VALORES INICIAIS */
	h = (b - a) / n;
	integral = (funcao(a) + funcao(b)) / 2;
	x = a;

	/* SOMATÓRIO */
	for (i = 1; i != n-1; i++) {
		x += h;
		integral += funcao(x);
	}

	integral *= h;
	printf("N = %.2f trapezóides, de %.2f até %.2f = %.2f \n", n, a, b, integral);
} 
