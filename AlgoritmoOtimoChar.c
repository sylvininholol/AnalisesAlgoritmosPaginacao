#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACIDADE_MEMORIA 3

int pagefault;
// Função para inicializar a memória
void inicializarMemoria(char *memoria[]) {
    for (int i = 0; i < CAPACIDADE_MEMORIA; i++) {
        memoria[i] = NULL;
    }
}

// Função para imprimir o conteúdo da memória
void imprimirMemoria(char *memoria[]) {
    printf("Memoria Atual: [");
    for (int i = 0; i < CAPACIDADE_MEMORIA; i++) {
        if (memoria[i] != NULL) {
            printf("%s", memoria[i]);
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
int paginaNaMemoria(char *memoria[], char *pagina) {
    for (int i = 0; i < CAPACIDADE_MEMORIA; i++) {
        if (memoria[i] != NULL && strcmp(memoria[i], pagina) == 0) {
            return 1; // Página encontrada na memória
        }
    }
    return 0; // Página não encontrada na memória
}

// Função para substituir a página mais antiga na memória
void substituirPagina(char *memoria[], char *pagina, int num, int numpag, char *paginas[]) {
    //printf("entrou\n");
    pagefault++;
    //free(memoria[0]); // Liberar a memória alocada para a página mais antiga
    int c1 = 0, c2 = 0, c3 = 0, i, change = 0;
    for(i = num; i < numpag; i++)
    {
        if(strcmp(memoria[0], paginas[i]) == 0)
        {
            c1++;
        }
        if(strcmp(memoria[1], paginas[i]) == 0)
        {
            c2++;
        }
        if(strcmp(memoria[2], paginas[i]) == 0)
        {
            c3++;
        }
    }

    //printf("c1: %d c2: %d c3: %d\n", c1, c2, c3);

    if(c1 < c2 && c1<c3)
    {
        free(memoria[0]);
        change = 0;
    } 
    else if(c2 < c1 && c2<c3)
    {
        free(memoria[1]);
        change = 1;
    } 
    else if(c3 < c2 && c3<c1)
    {
        free(memoria[2]);
        change = 2;
    } else{
        free(memoria[0]);
    }

    for (int i = change; i < CAPACIDADE_MEMORIA - 1; i++) {
        memoria[i] = memoria[i + 1]; // Deslocar as páginas restantes para a esquerda
    }
    memoria[CAPACIDADE_MEMORIA - 1] = strdup(pagina); // Adicionar a nova página à memória
}

void addpagina(char *memoria[], char *pagina)
{
     for (int i = 0; i < CAPACIDADE_MEMORIA - 1; i++) {
        memoria[i] = memoria[i + 1]; // Deslocar as páginas restantes para a esquerda
    }
    memoria[CAPACIDADE_MEMORIA - 1] = strdup(pagina); // Adicionar a nova página à memória
}

int main() {
    char *paginas[] = {"jp", "sylvino", "mota", "breno", "jorge", "sylvino", "sylvino", "jp", "sylvino", "jorge", "mota", "sylvino", "mota", "sylvino"};
    int numPaginas = sizeof(paginas) / sizeof(paginas[0]);

    char *memoria[CAPACIDADE_MEMORIA];
    inicializarMemoria(memoria);

    for (int i = 0; i < numPaginas; i++) {
        printf("Acessando Pagina: %s\n", paginas[i]);

        if (!paginaNaMemoria(memoria, paginas[i])) {
            if(i<3)
            {
                addpagina(memoria, paginas[i]);
            }
            else{
            substituirPagina(memoria, paginas[i], i, numPaginas, paginas);
            }
        }

        imprimirMemoria(memoria);
        printf("\n");
    }

    // Liberar a memória alocada para as páginas na memória
    for (int i = 0; i < CAPACIDADE_MEMORIA; i++) {
        free(memoria[i]);
    }

    printf("page fault: %d", pagefault);
    return 0;
}