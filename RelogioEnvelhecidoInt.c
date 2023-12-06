#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct {
    int pagina_numero;  // Alteração para aceitar inteiros
    int bits_referencia;
    int contador_envelhecimento;
} Pagina;

void imprimir_quadros(Pagina quadros[], int max_quadros, int ciclo) {
    printf("Ciclo %d - Quadros: ", ciclo);
    for (int i = 0; i < max_quadros; i++) {
        if (quadros[i].pagina_numero != -1) {
            printf("[%d-%d-%d] ", quadros[i].pagina_numero, quadros[i].bits_referencia, quadros[i].contador_envelhecimento);
        } else {
            printf("[ ] ");
        }
    }
    printf("\n");
}

int algoritmo_relogio_envelhecido(int referenciadas[], int tamanho_referenciadas, int max_quadros) {
    Pagina quadros[max_quadros];
    int falta_pagina = 0;
    int ponteiro = 0;

    for (int i = 0; i < max_quadros; i++) {
        quadros[i].pagina_numero = -1;  // Número inválido para indicar página vazia
        quadros[i].bits_referencia = 0;
        quadros[i].contador_envelhecimento = 0;
    }

    for (int j = 0, ciclo = 1; j < tamanho_referenciadas; j++, ciclo++) {
        int pagina_numero = referenciadas[j];
        bool pagina_na_memoria = false;

        for (int i = 0; i < max_quadros; i++) {
            if (quadros[i].pagina_numero == pagina_numero) {
                pagina_na_memoria = true;
                quadros[i].bits_referencia |= 1;
                quadros[i].contador_envelhecimento = 0;
                break;
            }
        }

        if (!pagina_na_memoria) {
            falta_pagina++;

            while (quadros[ponteiro].bits_referencia & 1) {
                quadros[ponteiro].bits_referencia >>= 1;
                ponteiro = (ponteiro + 1) % max_quadros;
            }

            quadros[ponteiro].pagina_numero = pagina_numero;
            quadros[ponteiro].bits_referencia |= 1;
            quadros[ponteiro].contador_envelhecimento = 0;
            ponteiro = (ponteiro + 1) % max_quadros;
        }

        for (int i = 0; i < max_quadros; i++) {
            if (quadros[i].pagina_numero != -1 && (quadros[i].bits_referencia & 1) == 0) {
                quadros[i].contador_envelhecimento++;
            }
        }

        imprimir_quadros(quadros, max_quadros, ciclo);
    }

    return falta_pagina;
}


// Função principal
int main() {
    clock_t inicio, fim;
    double tempoDecorrido;
    int referenciadas[] = {1, 2, 3, 4, 5, 1, 2, 6, 4, 5, 3, 5, 6};
    //    int referenciadas[] = {11, 22, 33, 44, 55, 11, 22, 33, 44, 55, 11, 22, 33, 44, 55};
    int tamanho_referenciadas = sizeof(referenciadas) / sizeof(referenciadas[0]);
    int max_quadros = 3;

    inicio = clock();
    int faltas = algoritmo_relogio_envelhecido(referenciadas, tamanho_referenciadas, max_quadros);
    fim = clock();
    tempoDecorrido = ((double)(fim - inicio) / CLOCKS_PER_SEC);
    
    printf("Número total de faltas de página: %d\n", faltas);
    printf("Tempo decorrido: %f segundos\n", tempoDecorrido);

    return 0;
}
