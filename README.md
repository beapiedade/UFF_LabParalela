# Laboratório de Programação Paralela

## Comandos

- mpicc -o teste arquivo.c
- mpirun -np 8 --oversubscribe teste

## Trabalho 1

Faça as seguintes versões MPI: 
- a) Uma versão com apenas 4 processos executando. Nesta versão, cada processo faz uma função: 1 soma, 1 subtrai e 1 multiplica. O outro processo é responsável por avisar cada um dos outros 3 a sua função, e ao término imprimir os resultados.
- b) Uma versão com MPI com qualquer número de processos baseado em mestre/escravo, utilizando operações de comunicação coletiva. A divisão das tarefas deve ser a mais balanceada possível, e a forma de implementação é livre. 
- c) Uma versão com MPI baseada no modelo Pipeline. 

Para todos os casos, apresente o resultado final, o tempo de execução, o speed up e a eficiência.