#include <stdio.h>
#include <stdbool.h>
#include <time.h>

// Definindo a estrutura de uma página
typedef struct {
    int numero_pagina;
    int bits_referencia;  // Alteração: Utilizaremos int para envelhecimento
} Pagina;

// Função para imprimir o estado atual dos quadros
void imprimir_quadros(Pagina quadros[], int max_quadros, int ciclo) {
    printf("Ciclo %d - Quadros: ", ciclo);
    for (int i = 0; i < max_quadros; i++) {
        if (quadros[i].numero_pagina != -1) {
            printf("[%d-%d] ", quadros[i].numero_pagina, quadros[i].bits_referencia);
        } else {
            printf("[ ] ");
        }
    }
    printf("\n");
}

// Função que implementa o algoritmo do Relógio com envelhecimento
int algoritmo_relogio_envelhecimento(int referenciadas[], int tamanho_referenciadas, int max_quadros) {
    Pagina quadros[max_quadros];  // Quadros de página
    int falta_pagina = 0;         // Contador de faltas de página
    int ponteiro = 0;             // Ponteiro para percorrer os quadros

    for (int i = 0; i < max_quadros; i++) {
        quadros[i].numero_pagina = -1;  // Inicializa as páginas nos quadros como inválidas
        quadros[i].bits_referencia = 0;  // Inicializa os bits de referência como zero
    }

    for (int j = 0, ciclo = 1; j < tamanho_referenciadas; j++, ciclo++) {
        int pagina_numero = referenciadas[j];
        bool pagina_na_memoria = false;

        // Verifica se a página já está nos quadros
        for (int i = 0; i < max_quadros; i++) {
            if (quadros[i].numero_pagina == pagina_numero) {
                pagina_na_memoria = true;
                quadros[i].bits_referencia |= 1;  // Atualiza o bit de referência (menos significativo)
                break;
            }
        }

        // Se a página não estiver na memória, ocorre uma falta de página
        if (!pagina_na_memoria) {
            falta_pagina++;

            // Procura uma página não referenciada
            while (quadros[ponteiro].bits_referencia & 1) {
                // Reduz gradualmente o bit de referência
                quadros[ponteiro].bits_referencia >>= 1;
                ponteiro = (ponteiro + 1) % max_quadros;  // Move para a próxima página
            }

            // Substitui a página
            quadros[ponteiro].numero_pagina = pagina_numero;
            quadros[ponteiro].bits_referencia |= 1;  // Atualiza o bit de referência
            ponteiro = (ponteiro + 1) % max_quadros;  // Move para a próxima página
        }

        // Imprime o estado atual dos quadros
        imprimir_quadros(quadros, max_quadros, ciclo);
    }

    return falta_pagina;
}

// Função principal
int main() {
    clock_t inicio, fim;
    double tempoDecorrido;
    int referenciadas[] = {1, 2, 3, 4, 5, 1, 2, 6, 4, 5, 3, 5, 6};
    int tamanho_referenciadas = sizeof(referenciadas) / sizeof(referenciadas[0]);
    int max_quadros = 3;

    inicio = clock();
    int faltas = algoritmo_relogio_envelhecimento(referenciadas, tamanho_referenciadas, max_quadros);
    fim = clock();
    tempoDecorrido = ((double)(fim - inicio) / CLOCKS_PER_SEC);
    
    printf("Número total de faltas de página: %d\n", faltas);
    printf("Tempo decorrido: %f segundos\n", tempoDecorrido);

    return 0;
}
