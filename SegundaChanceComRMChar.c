#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

typedef struct {
    char pagina_string[20];
    int bits_referencia;
    int bit_modificado;
} Pagina;

void imprimir_quadros(Pagina quadros[], int max_quadros, int ciclo) {
    printf("Ciclo %d - Quadros: ", ciclo);
    for (int i = 0; i < max_quadros; i++) {
        if (quadros[i].pagina_string[0] != '\0') {
            printf("[%s-R%d-M%d] ", quadros[i].pagina_string, quadros[i].bits_referencia, quadros[i].bit_modificado);
        } else {
            printf("[ ] ");
        }
    }
    printf("\n");
}

void atualizar_referencia(Pagina* quadro) {
    quadro->bits_referencia = 1;
}

int encontrar_vitima(Pagina quadros[], int max_quadros, int* ponteiro) {
    int inicio_ponteiro = *ponteiro;  // Adicionado para evitar loop infinito
    while (1) {
        if (quadros[*ponteiro].bits_referencia == 0 && quadros[*ponteiro].bit_modificado == 0) {
            return *ponteiro;
        } else if (quadros[*ponteiro].bits_referencia == 0) {
            // Não é necessário atualizar o bit de referência aqui
        } else {
            quadros[*ponteiro].bits_referencia = 0;
        }

        // Mudança: Verificar o bit_modificado antes de atualizar o ponteiro
        if (quadros[*ponteiro].bit_modificado == 0) {
            return *ponteiro;
        }

        *ponteiro = (*ponteiro + 1) % max_quadros;

        // Adicionado para evitar loop infinito
        if (*ponteiro == inicio_ponteiro) {
            break;
        }
    }

    // Retorna a posição original do ponteiro
    return inicio_ponteiro;
}


int algoritmo_segunda_chance(char referenciadas[][20], int tamanho_referenciadas, int max_quadros) {
    Pagina quadros[max_quadros];
    int falta_pagina = 0;
    int ponteiro = 0;

    for (int i = 0; i < max_quadros; i++) {
        quadros[i].pagina_string[0] = '\0';
        quadros[i].bits_referencia = 0;
        quadros[i].bit_modificado = 0;
    }

    for (int j = 0, ciclo = 1; j < tamanho_referenciadas; j++, ciclo++) {
        char* pagina_string = referenciadas[j];
        bool pagina_na_memoria = false;

        for (int i = 0; i < max_quadros; i++) {
            if (strcmp(quadros[i].pagina_string, pagina_string) == 0) {
                pagina_na_memoria = true;
                atualizar_referencia(&quadros[i]);
                break;
            }
        }

        if (!pagina_na_memoria) {
            falta_pagina++;

            int vitima = encontrar_vitima(quadros, max_quadros, &ponteiro);
            strcpy(quadros[vitima].pagina_string, pagina_string);
            atualizar_referencia(&quadros[vitima]);

            // Simulação da marcação de páginas modificadas
            if (quadros[vitima].bit_modificado == 0) {
                printf("Pagina %s foi modificada.\n", quadros[vitima].pagina_string);
                quadros[vitima].bit_modificado = 1;
            }

            imprimir_quadros(quadros, max_quadros, ciclo);
        }
    }

    return falta_pagina;
}

int main() {
    clock_t inicio, fim;
    double tempoDecorrido;
   // char referenciadas[][20] = {"chefe", "cozinheiro", "cliente", "consumidor", "produtor", "chefe", "cozinheiro", "cliente", "consumidor", "produtor", "chefe", "cozinheiro", "cliente", "consumidor", "produtor", "chefe", "cozinheiro", "cliente", "consumidor", "produtor"};
   // char referenciadas[][20] = {"jp", "sylvino", "mota", "breno", "jorge", "sylvino", "sylvino", "jp", "sylvino", "jorge", "mota", "sylvino", "mota", "sylvino"};
    char referenciadas[][20] = {"ladrao", "policial", "detetive", "carpinteiro", "bombeiro", "ladrao", "policial", "detetive", "carpinteiro", "bombeiro", "ladrao", "policial", "detetive", "carpinteiro", "bombeiro", "ladrao", "policial", "detetive", "carpinteiro", "bombeiro"};
    int tamanho_referenciadas = sizeof(referenciadas) / sizeof(referenciadas[0]);
    int max_quadros = 3;

    inicio = clock();
    int faltas = algoritmo_segunda_chance(referenciadas, tamanho_referenciadas, max_quadros);
    fim = clock();
    tempoDecorrido = ((double)(fim - inicio) / CLOCKS_PER_SEC);

    printf("Numero total de faltas de pagina: %d\n", faltas);
    printf("Tempo decorrido: %f segundos\n", tempoDecorrido);

    return 0;
}
