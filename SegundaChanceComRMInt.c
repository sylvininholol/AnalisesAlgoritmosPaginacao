#include <stdio.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    int pagina_numero;  // Alterado para aceitar inteiros
    int bits_referencia;
    int bit_modificado;
} Pagina;

void imprimir_quadros(Pagina quadros[], int max_quadros, int ciclo) {
    printf("Ciclo %d - Quadros: ", ciclo);
    for (int i = 0; i < max_quadros; i++) {
        if (quadros[i].pagina_numero != -1) {
            printf("[%d-R%d-M%d] ", quadros[i].pagina_numero, quadros[i].bits_referencia, quadros[i].bit_modificado);
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
    int inicio_ponteiro = *ponteiro;
    while (1) {
        if (quadros[*ponteiro].bits_referencia == 0 && quadros[*ponteiro].bit_modificado == 0) {
            return *ponteiro;
        } else if (quadros[*ponteiro].bits_referencia == 0) {
            // Não é necessário atualizar o bit de referência aqui
        } else {
            quadros[*ponteiro].bits_referencia = 0;
        }

        *ponteiro = (*ponteiro + 1) % max_quadros;

        if (*ponteiro == inicio_ponteiro) {
            break;
        }
    }

    return inicio_ponteiro;
}

int algoritmo_segunda_chance(int referenciadas[], int tamanho_referenciadas, int max_quadros) {
    Pagina quadros[max_quadros];
    int falta_pagina = 0;
    int ponteiro = 0;

    for (int i = 0; i < max_quadros; i++) {
        quadros[i].pagina_numero = -1;  // Inicializado com -1 para indicar que está vazio
        quadros[i].bits_referencia = 0;
        quadros[i].bit_modificado = 0;
    }

    for (int j = 0, ciclo = 1; j < tamanho_referenciadas; j++, ciclo++) {
        int pagina_numero = referenciadas[j];
        bool pagina_na_memoria = false;

        for (int i = 0; i < max_quadros; i++) {
            if (quadros[i].pagina_numero == pagina_numero) {
                pagina_na_memoria = true;
                atualizar_referencia(&quadros[i]);
                break;
            }
        }

        if (!pagina_na_memoria) {
            falta_pagina++;

            int vitima = encontrar_vitima(quadros, max_quadros, &ponteiro);
            quadros[vitima].pagina_numero = pagina_numero;
            atualizar_referencia(&quadros[vitima]);

            // Simulação da marcação de páginas modificadas
            if (quadros[vitima].bit_modificado == 0) {
                printf("Página %d foi modificada.\n", quadros[vitima].pagina_numero);
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
    int referenciadas[] = {1, 2, 3, 4, 5, 1, 2, 6, 4, 5, 3, 5, 6};
    int tamanho_referenciadas = sizeof(referenciadas) / sizeof(referenciadas[0]);
    int max_quadros = 3;

    inicio = clock();
    int faltas = algoritmo_segunda_chance(referenciadas, tamanho_referenciadas, max_quadros);
    fim = clock();
    
    tempoDecorrido = ((double)(fim - inicio)/ CLOCKS_PER_SEC);

    printf("Número total de faltas de página: %d\n", faltas);
    printf("Tempo decorrido: %f segundos\n", tempoDecorrido);

    return 0;
}
