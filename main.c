#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 53

typedef struct st_lista {
    char nome[50];
    int chave;
    struct st_lista *prox;
    struct st_lista *ant;
} le;

le* hashTable[TABLE_SIZE];
int chave = 0;
int quantNode = 0;
int distribucao[TABLE_SIZE] = {0};

int hashFunction(char nome[]) {
    char firstChar = toupper(nome[0]);

    if (firstChar >= 'A' && firstChar <= 'Z') {
        return firstChar - 'A';
    } else {
        return 52;
    }
}

void dividir(le* origem, le** frente, le** tras) {
    le* rapido;
    le* lento;
    lento = origem;
    rapido = origem->prox;

    while (rapido != NULL) {
        rapido = rapido->prox;
        if (rapido != NULL) {
            lento = lento->prox;
            rapido = rapido->prox;
        }
    }

    *frente = origem;
    *tras = lento->prox;
    lento->prox = NULL;
}

le* mesclar(le* a, le* b) {
    le* resultado = NULL;

    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    if (a->chave <= b->chave) {
        resultado = a;
        resultado->prox = mesclar(a->prox, b);
        resultado->prox->ant = resultado;
        resultado->ant = NULL;
    } else {
        resultado = b;
        resultado->prox = mesclar(a, b->prox);
        resultado->prox->ant = resultado;
        resultado->ant = NULL;
    }
    return resultado;
}

void mergeSort(le** cabecaRef) {
    le* cabeca = *cabecaRef;
    le* a;
    le* b;

    if ((cabeca == NULL) || (cabeca->prox == NULL)) {
        return;
    }

    dividir(cabeca, &a, &b);

    mergeSort(&a);
    mergeSort(&b);

    *cabecaRef = mesclar(a, b);
}

void inserirOrdenado(le** cabecaRef, le* novo) {
    novo->prox = *cabecaRef;
    if (*cabecaRef != NULL) {
        (*cabecaRef)->ant = novo;
    }
    *cabecaRef = novo;
}

void inserirNome(char nome[]) {
    le *novo = (le *)malloc(sizeof(le));
    if(!novo) {
        printf("Erro ao alocar memoria\n");
        exit(1);
    }
    strcpy(novo->nome, nome);
    chave++;
    novo->chave = chave;
    novo->prox = NULL;
    novo->ant = NULL;

    int index = hashFunction(nome);

    distribucao[index]++;

    if (hashTable[index] == NULL) {
        hashTable[index] = novo;
    } else {
        inserirOrdenado(&hashTable[index], novo);
        mergeSort(&hashTable[index]);
    }
    quantNode++;
}

le* buscarNome(char nome[]) {
    int index = hashFunction(nome);
    le* aux = hashTable[index];

    while (aux != NULL) {
        if (strcmp(aux->nome, nome) == 0) {
            printf("Nome '%s' encontrado na posição %d\n", nome, index);
            return aux;
        }
        aux = aux->prox;
    }

    printf("Nome '%s' não encontrado\n", nome);
    return NULL;
}

void removerNome(char nome[]) {
    int index = hashFunction(nome);
    le* aux = hashTable[index];

    while (aux != NULL) {
        if (strcmp(aux->nome, nome) == 0) {
            if (aux->ant != NULL) {
                aux->ant->prox = aux->prox;
            } else {
                hashTable[index] = aux->prox;
            }
            if (aux->prox != NULL) {
                aux->prox->ant = aux->ant;
            }
            free(aux);
            distribucao[index]--;
            printf("Nome '%s' removido com sucesso\n", nome);
            return;
        }
        aux = aux->prox;
    }

    printf("Nome '%s' não encontrado para remoção\n", nome);
}

void imprimirTabela() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("Index %d: ", i);
        if (hashTable[i] == NULL) {
            printf("Lista vazia\n");
        } else {
            le *aux = hashTable[i];
            while(aux != NULL) {
                printf("[%s] [%d] -> ", aux->nome, aux->chave);
                aux = aux->prox;
            }
            printf("NULL\n");
        }
    }
}

char* getFiltroPorIndice(int indice) {
    static char filtro[20];
    if (indice < 26) {
        sprintf(filtro, "A-Z (%c)", 'A' + indice);
    } else if (indice == 52) {
        strcpy(filtro, "Outros caracteres");
    }
    return filtro;
}

void imprimirHistograma() {
    printf("\nHistograma de Distribuição:\n");

    for (int i = 0; i < TABLE_SIZE; i++) {
        int numNomes = distribucao[i];
        char* filtro = getFiltroPorIndice(i);
        printf("Index %d: %d nomes (%s)\n", i, numNomes, filtro);
    }
}

void lerArquivoEInserirNomes(char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return;
    }

    char nome[50];
    while (fgets(nome, sizeof(nome), arquivo) != NULL) {
        nome[strcspn(nome, "\n")] = '\0';
        inserirNome(nome);
    }

    fclose(arquivo);
}

int main() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }

    lerArquivoEInserirNomes("/usr/ws-projects/ws-clion/TrabalhoFinalEstruturaI/nomes.txt");

    imprimirTabela();

    imprimirHistograma();

    printf("\nInserindo nomes...\n");
    inserirNome("Lucas");
    inserirNome("Ana");

    printf("\nConsultando nomes...\n");
    buscarNome("LUCAS");
    buscarNome("PEDRO");

    printf("\nRemovendo nomes...\n");
    removerNome("LUCAS");
    removerNome("JOAO");
    printf(getFiltroPorIndice(25));

    return 0;
}
