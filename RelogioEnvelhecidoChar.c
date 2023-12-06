#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct {
    char pagina_string[20];
    int bits_referencia;
    int contador_envelhecimento;  // Novo campo para o contador de envelhecimento
} Pagina;

void imprimir_quadros(Pagina quadros[], int max_quadros, int ciclo) {
    printf("Ciclo %d - Quadros: ", ciclo);
    for (int i = 0; i < max_quadros; i++) {
        if (quadros[i].pagina_string[0] != '\0') {
            printf("[%s-%d-%d] ", quadros[i].pagina_string, quadros[i].bits_referencia, quadros[i].contador_envelhecimento);
        } else {
            printf("[ ] ");
        }
    }
    printf("\n");
}

int algoritmo_relogio_envelhecido(char referenciadas[][20], int tamanho_referenciadas, int max_quadros) {
    Pagina quadros[max_quadros];
    int falta_pagina = 0;
    int ponteiro = 0;

    for (int i = 0; i < max_quadros; i++) {
        quadros[i].pagina_string[0] = '\0';
        quadros[i].bits_referencia = 0;
        quadros[i].contador_envelhecimento = 0;  // Inicializa o contador de envelhecimento
    }

    for (int j = 0, ciclo = 1; j < tamanho_referenciadas; j++, ciclo++) {
        char* pagina_string = referenciadas[j];
        bool pagina_na_memoria = false;

        for (int i = 0; i < max_quadros; i++) {
            if (strcmp(quadros[i].pagina_string, pagina_string) == 0) {
                pagina_na_memoria = true;
                quadros[i].bits_referencia |= 1;
                quadros[i].contador_envelhecimento = 0;  // Zera o contador ao referenciar
                break;
            }
        }

        if (!pagina_na_memoria) {
            falta_pagina++;

            while (quadros[ponteiro].bits_referencia & 1) {
                quadros[ponteiro].bits_referencia >>= 1;
                ponteiro = (ponteiro + 1) % max_quadros;
            }

            strcpy(quadros[ponteiro].pagina_string, pagina_string);
            quadros[ponteiro].bits_referencia |= 1;
            quadros[ponteiro].contador_envelhecimento = 0;  // Zera o contador ao adicionar nova página
            ponteiro = (ponteiro + 1) % max_quadros;
        }

        // Incrementa o contador de envelhecimento para páginas não referenciadas
        for (int i = 0; i < max_quadros; i++) {
            if (strcmp(quadros[i].pagina_string, "") != 0 && (quadros[i].bits_referencia & 1) == 0) {
                quadros[i].contador_envelhecimento++;
            }
        }

        imprimir_quadros(quadros, max_quadros, ciclo);
    }

    return falta_pagina;
}

int main() {
    clock_t inicio, fim;
    double tempoDecorrido;
   //char referenciadas[][20] = {"jp", "sylvino", "mota", "breno", "jorge", "sylvino", "sylvino", "jp", "sylvino", "jorge", "mota", "sylvino", "mota", "sylvino"};
    char referenciadas[][20] = {"ladrao", "policial", "detetive", "carpinteiro", "bombeiro", "ladrao", "policial", "detetive", "carpinteiro", "bombeiro", "ladrao", "policial", "detetive", "carpinteiro", "bombeiro", "ladrao", "policial", "detetive", "carpinteiro", "bombeiro"};
    int tamanho_referenciadas = sizeof(referenciadas) / sizeof(referenciadas[0]);
    int max_quadros = 3;

    inicio = clock();
    int faltas = algoritmo_relogio_envelhecido(referenciadas, tamanho_referenciadas, max_quadros);
    fim = clock();

    tempoDecorrido = ((double)(fim-inicio)/CLOCKS_PER_SEC);
    printf("Numero total de faltas de pagina: %d\n", faltas);
    printf("Tempo decorrido: %f segundos\n", tempoDecorrido);

    return 0;
}
