#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct st_lista {
    char nome[50];
    int chave;
    struct st_lista *prox;
    struct st_lista *ant;
}le;

void iniciarLista(le *lista) {
    lista->prox = NULL;
    lista->ant = NULL;
}
int v_vazia(le *lista) {
    if (lista->prox == NULL) {
        return 1;
    }
    return 0;
}
void inserirInicio(le *lista, char nome[]) {
    le *novo = (le *)malloc(sizeof(le));
    if(!novo) {
        printf("Erro ao alocar memoria\n");
        exit(1);
    }
    strcpy(novo->nome, nome);
    if(v_vazia(lista)) {
        lista->prox = novo;
        novo->prox = NULL;
        novo->ant = NULL;
    }else {
        lista->prox->ant = novo;
        novo->prox = lista->prox;
        lista->prox = novo;
        novo->ant = NULL;
    }
}
void inserirFinal(le *lista, char nome[]) {
    le *novo = (le *)malloc(sizeof(le));
    if(!novo) {
        printf("Erro ao alocar memoria\n");
        exit(1);
    }
    strcpy(novo->nome, nome);
    if(v_vazia(lista)) {
        lista->prox = novo;
        novo->ant = NULL;
        novo->prox = NULL;
    }else {
        le *aux = lista->prox;
        while(aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = novo;
        novo->ant = aux;
        novo->prox = NULL;
    }
}
void inserirListaDeNomes(le *lista, FILE *f) {
    char linha[50];
    while(fgets(linha, sizeof(linha), f)!= NULL) {
        inserirFinal(lista, linha);
    }
}
void imprimirLista(le *lista) {
    if(v_vazia(lista)) {
        printf("Lista vazia\n");
    } else {
        le *aux = lista->prox;
        while(aux!= NULL) {
            printf("%s\n", aux->nome);
            aux = aux->prox;
        }
    }
}
int main() {
    FILE *f = fopen("/usr/ws-projects/ws-clion/TrabalhoFinalEstruturaI/nomes.txt", "r");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
    le *lista = (le *)malloc(sizeof(le));
    if(!lista) {
        printf("Erro ao alocar memoria\n");
        exit(1);
    }
    iniciarLista(lista);
    inserirInicio(lista, "John");
    inserirFinal(lista, "Miguel");
    inserirListaDeNomes(lista, f);
    imprimirLista(lista);
    fclose(f);
    free(lista);
    return 0;
}
