# Algoritmos de substituição de páginas

## Objetivo

O objetivo deste projeto é a implementação de um programa para simular os principais algoritmos de substituição de páginas usados no gerenciamento da memória virtual. O programa simulará os seguintes algoritmos:

    - FIFO (First In, First Out)
    - OPT (Algoritmo ótimo)
    - LRU (Least Recently Used)

## Requisitos

O único requisito necessário para a execução desse projeto é ter o compilador g++ instalado no computador.

## Instruções de execução

As instruções a seguir são para a execução deste programa via terminal de um ambiente Linux.

Para compilar, use o seguinte comando:

    g++ simula_vm.cc -o out -std=c++17


Para executar o programa, é necessário passar a quantidade de quadros de memória e o nome do arquivo com a cadeia de referências, como mostrado no exemplo abaixo:

    ./out 4 arquivo.txt
