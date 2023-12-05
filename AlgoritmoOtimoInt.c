#include <stdio.h>
#include <stdlib.h>

#define CAPACIDADE_MEMORIA 3

int pagefault;

// Função para inicializar a memória
void inicializarMemoria(int memoria[]) {
    for (int i = 0; i < CAPACIDADE_MEMORIA; i++) {
        memoria[i] = -1; // Inicializa com um valor que não será confundido com uma página válida
    }
}

// Função para imprimir o conteúdo da memória
void imprimirMemoria(int memoria[]) {
    printf("Memoria Atual: [");
    for (int i = 0; i < CAPACIDADE_MEMORIA; i++) {
        if (memoria[i] != -1) {
            printf("%d", memoria[i]);
        } else {
            printf("-");
        }

        if (i < CAPACIDADE_MEMORIA - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

// Função para verificar se a página já está na memória
int paginaNaMemoria(int memoria[], int pagina) {
    for (int i = 0; i < CAPACIDADE_MEMORIA; i++) {
        if (memoria[i] != -1 && memoria[i] == pagina) {
            return 1; // Página encontrada na memória
        }
    }
    return 0; // Página não encontrada na memória
}

// Função para substituir a página mais antiga na memória
void substituirPagina(int memoria[], int pagina, int num, int numpag, int paginas[]) {
    pagefault++;

    int c1 = 0, c2 = 0, c3 = 0, i, change = 0;
    for (i = num; i < numpag; i++) {
        if (memoria[0] == paginas[i]) {
            c1++;
        }
        if (memoria[1] == paginas[i]) {
            c2++;
        }
        if (memoria[2] == paginas[i]) {
            c3++;
        }
    }

    if (c1 < c2 && c1 < c3) {
        change = 0;
    } else if (c2 < c1 && c2 < c3) {
        change = 1;
    } else if (c3 < c2 && c3 < c1) {
        change = 2;
    }

    for (int i = change; i < CAPACIDADE_MEMORIA - 1; i++) {
        memoria[i] = memoria[i + 1]; // Desloca as páginas restantes para a esquerda
    }
    memoria[CAPACIDADE_MEMORIA - 1] = pagina; // Adiciona a nova página à memória
}

void addPagina(int memoria[], int pagina) {
    for (int i = 0; i < CAPACIDADE_MEMORIA - 1; i++) {
        memoria[i] = memoria[i + 1]; // Desloca as páginas restantes para a esquerda
    }
    memoria[CAPACIDADE_MEMORIA - 1] = pagina; // Adiciona a nova página à memória
}

int main() {
    int paginas[] = {1, 2, 3, 4, 5, 2, 2, 1, 2, 5, 3, 2, 3, 2};
    int numPaginas = sizeof(paginas) / sizeof(paginas[0]);

    int memoria[CAPACIDADE_MEMORIA];
    inicializarMemoria(memoria);

    for (int i = 0; i < numPaginas; i++) {
        printf("Acessando Pagina: %d\n", paginas[i]);

        if (!paginaNaMemoria(memoria, paginas[i])) {
            if (i < 3) {
                addPagina(memoria, paginas[i]);
            } else {
                substituirPagina(memoria, paginas[i], i, numPaginas, paginas);
            }
        }

        imprimirMemoria(memoria);
        printf("\n");
    }

    printf("Page fault: %d\n", pagefault);

    return 0;
}
