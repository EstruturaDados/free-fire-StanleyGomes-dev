
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_COMPONENTES 20
#define NOME_SIZE 30
#define TIPO_SIZE 20

typedef struct {
    char nome[NOME_SIZE];
    char tipo[TIPO_SIZE];
    int prioridade; // 1..10
} Componente;

/* Vetor de componentes */
Componente componentes[MAX_COMPONENTES];
int totalComponentes = 0;

/* Flags e contadores globais (para exibição) */
bool ordenadoPorNome = false;
long long comparacoesBubbleNome = 0;
long long comparacoesInsertionTipo = 0;
long long comparacoesSelectionPrioridade = 0;

/* ---- Funções utilitárias de entrada ---- */
void limpa_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') s[len - 1] = '\0';
}

/* Lê linha com fgets e trata newline */
void read_line(char *buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    limpa_newline(buffer);
}

/* Lê inteiro seguro (usando fgets) */
int read_int() {
    char buf[64];
    read_line(buf, sizeof(buf));
    return atoi(buf);
}

/* ---- Protótipos exigidos ---- */
void bubbleSortNome(Componente arr[], int n, long long *comparacoes);
void insertionSortTipo(Componente arr[], int n, long long *comparacoes);
void selectionSortPrioridade(Componente arr[], int n, long long *comparacoes);

int buscaBinariaPorNome(Componente arr[], int n, const char chave[], long long *comparacoes);

void mostrarComponentes(Componente arr[], int n);
double medirTempo(void (*algoritmo)(Componente[], int, long long *), Componente arr[], int n, long long *comparacoes);

/* ---- Implementações ---- */

/* Bubble sort por nome (ordenacao lexicografica) */
void bubbleSortNome(Componente arr[], int n, long long *comparacoes) {
    *comparacoes = 0;
    if (n <= 1) return;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            (*comparacoes)++; // comparacao entre strings
            if (strcmp(arr[j].nome, arr[j + 1].nome) > 0) {
                Componente tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

/* Insertion sort por tipo (string) */
void insertionSortTipo(Componente arr[], int n, long long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Componente key = arr[i];
        int j = i - 1;
        // primeiro comparacao feita ao entrar no while (se j >= 0)
        while (j >= 0) {
            (*comparacoes)++;
            if (strcmp(arr[j].tipo, key.tipo) > 0) {
                arr[j + 1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = key;
    }
}

/* Selection sort por prioridade (int) - menor prioridade primeiro (1 -> 10) */
void selectionSortPrioridade(Componente arr[], int n, long long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            (*comparacoes)++;
            if (arr[j].prioridade < arr[min_idx].prioridade) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            Componente tmp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = tmp;
        }
    }
}

/* Busca binaria por nome (requer vetor ordenado por nome) */
int buscaBinariaPorNome(Componente arr[], int n, const char chave[], long long *comparacoes) {
    int l = 0, r = n - 1;
    *comparacoes = 0;
    while (l <= r) {
        int m = l + (r - l) / 2;
        (*comparacoes)++;
        int cmp = strcmp(arr[m].nome, chave);
        if (cmp == 0) return m;
        if (cmp < 0) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

/* Mostrar vetor de componentes formatado */
void mostrarComponentes(Componente arr[], int n) {
    if (n == 0) {
        printf("\n(sem componentes cadastrados)\n");
        return;
    }
    printf("\n=== Componentes (total: %d) ===\n", n);
    for (int i = 0; i < n; i++) {
        printf("%2d) Nome: %-28s | Tipo: %-10s | Prioridade: %2d\n",
               i + 1, arr[i].nome, arr[i].tipo, arr[i].prioridade);
    }
    printf("==============================\n");
}

/* medirTempo: recebe um ponteiro para funcao de ordenacao e retorna segundos */
double medirTempo(void (*algoritmo)(Componente[], int, long long *), Componente arr[], int n, long long *comparacoes) {
    clock_t inicio = clock();
    algoritmo(arr, n, comparacoes);
    clock_t fim = clock();
    return (double)(fim - inicio) / CLOCKS_PER_SEC;
}

/* ---- Funcoes do menu (entrada/saida) ---- */

void adicionarComponente() {
    if (totalComponentes >= MAX_COMPONENTES) {
        printf("\nLimite atingido (%d componentes). Remova antes de adicionar.\n", MAX_COMPONENTES);
        return;
    }
    Componente c;
    printf("\nDigite o nome do componente (max %d caracteres): ", NOME_SIZE - 1);
    read_line(c.nome, NOME_SIZE);
    if (strlen(c.nome) == 0) {
        printf("Nome invalido. Operacao cancelada.\n");
        return;
    }

    printf("Digite o tipo do componente (ex: controle/suporte/propulsao) (max %d): ", TIPO_SIZE - 1);
    read_line(c.tipo, TIPO_SIZE);
    if (strlen(c.tipo) == 0) {
        strcpy(c.tipo, "desconhecido");
    }

    printf("Digite a prioridade (1-10): ");
    int p = read_int();
    if (p < 1) p = 1;
    if (p > 10) p = 10;
    c.prioridade = p;

    componentes[totalComponentes++] = c;
    ordenadoPorNome = false;
    printf("Componente cadastrado com sucesso!\n");
}

void removerComponente() {
    if (totalComponentes == 0) {
        printf("\nNenhum componente para remover.\n");
        return;
    }
    char nomeBusca[NOME_SIZE];
    printf("\nDigite o nome do componente a remover: ");
    read_line(nomeBusca, NOME_SIZE);
    int pos = -1;
    for (int i = 0; i < totalComponentes; i++) {
        if (strcmp(componentes[i].nome, nomeBusca) == 0) {
            pos = i;
            break;
        }
    }
    if (pos == -1) {
        printf("Componente '%s' nao encontrado.\n", nomeBusca);
        return;
    }
    for (int i = pos; i < totalComponentes - 1; i++) {
        componentes[i] = componentes[i + 1];
    }
    totalComponentes--;
    ordenadoPorNome = false;
    printf("Removido com sucesso.\n");
}

void opcaoOrdenarEPesquisar() {
    int escolha;
    printf("\nEscolha algoritmo de ordenacao:\n");
    printf("1 - Bubble sort (por nome)\n");
    printf("2 - Insertion sort (por tipo)\n");
    printf("3 - Selection sort (por prioridade)\n");
    printf("0 - Voltar\n");
    printf("Opcao: ");
    escolha = read_int();

    if (escolha == 0) return;

    // Vamos fazer cópia do vetor para ordenar? Não: ordenamos o vetor principal.
    double tempo = 0.0;
    long long comps = 0;

    switch (escolha) {
        case 1:
            tempo = medirTempo(bubbleSortNome, componentes, totalComponentes, &comps);
            comparacoesBubbleNome = comps;
            ordenadoPorNome = true;
            printf("\nBubble sort (nome) executado. Comparacoes: %lld | Tempo: %.6f s\n", comps, tempo);
            mostrarComponentes(componentes, totalComponentes);
            break;
        case 2:
            tempo = medirTempo(insertionSortTipo, componentes, totalComponentes, &comps);
            comparacoesInsertionTipo = comps;
            // ordenar por tipo quebra propriedade 'ordenadoPorNome'
            ordenadoPorNome = false;
            printf("\nInsertion sort (tipo) executado. Comparacoes: %lld | Tempo: %.6f s\n", comps, tempo);
            mostrarComponentes(componentes, totalComponentes);
            break;
        case 3:
            tempo = medirTempo(selectionSortPrioridade, componentes, totalComponentes, &comps);
            comparacoesSelectionPrioridade = comps;
            ordenadoPorNome = false;
            printf("\nSelection sort (prioridade) executado. Comparacoes: %lld | Tempo: %.6f s\n", comps, tempo);
            mostrarComponentes(componentes, totalComponentes);
            break;
        default:
            printf("Opcao invalida.\n");
    }
}

/* Buscar por nome (sequencial) - mostra comparacoes também */
void buscarSequencial() {
    if (totalComponentes == 0) {
        printf("\nNenhum componente cadastrado.\n");
        return;
    }
    char chave[NOME_SIZE];
    printf("\nDigite o nome do componente a buscar (sequencial): ");
    read_line(chave, NOME_SIZE);

    long long comps = 0;
    int idx = -1;
    for (int i = 0; i < totalComponentes; i++) {
        comps++;
        if (strcmp(componentes[i].nome, chave) == 0) {
            idx = i;
            break;
        }
    }

    if (idx != -1) {
        printf("\nComponente encontrado (sequencial) na posicao %d:\n", idx + 1);
        printf("Nome: %s | Tipo: %s | Prioridade: %d\n", componentes[idx].nome, componentes[idx].tipo, componentes[idx].prioridade);
    } else {
        printf("\nComponente '%s' nao encontrado (sequencial).\n", chave);
    }
    printf("Comparacoes (sequencial): %lld\n", comps);
}

/* Buscar por nome (binaria) */
void buscarBinaria() {
    if (totalComponentes == 0) {
        printf("\nNenhum componente cadastrado.\n");
        return;
    }
    if (!ordenadoPorNome) {
        printf("\nERRO: O vetor nao esta ordenado por nome. Use bubble sort (opcao de ordenacao) primeiro.\n");
        return;
    }
    char chave[NOME_SIZE];
    printf("\nDigite o nome do componente a buscar (binaria): ");
    read_line(chave, NOME_SIZE);

    long long comps = 0;
    int idx = buscaBinariaPorNome(componentes, totalComponentes, chave, &comps);

    if (idx != -1) {
        printf("\nComponente encontrado (binaria) na posicao %d:\n", idx + 1);
        printf("Nome: %s | Tipo: %s | Prioridade: %d\n", componentes[idx].nome, componentes[idx].tipo, componentes[idx].prioridade);
    } else {
        printf("\nComponente '%s' nao encontrado (binaria).\n", chave);
    }
    printf("Comparacoes (binaria): %lld\n", comps);
}

/* Montagem final (confirma a presenca do componente-chave e mostra ordenacao atual) */
void montagemFinal() {
    if (totalComponentes == 0) {
        printf("\nNenhum componente cadastrado.\n");
        return;
    }
    char chave[NOME_SIZE];
    printf("\nDigite o nome do componente-chave que destrava a torre: ");
    read_line(chave, NOME_SIZE);

    // Se vetor nao estiver ordenado por nome, avisa mas permite buscar sequencialmente
    if (!ordenadoPorNome) {
        printf("\nAviso: vetor nao esta ordenado por nome. Faremos busca sequencial para confirmar.\n");
        long long comps = 0;
        int idx = -1;
        for (int i = 0; i < totalComponentes; i++) {
            comps++;
            if (strcmp(componentes[i].nome, chave) == 0) { idx = i; break; }
        }
        if (idx != -1) {
            printf("\nComponente-chave encontrado (sequencial) -> %s (Tipo: %s | Prioridade: %d)\n",
                   componentes[idx].nome, componentes[idx].tipo, componentes[idx].prioridade);
        } else {
            printf("\nComponente-chave '%s' NAO encontrado. Nao e possivel ativar a torre.\n", chave);
        }
        printf("Comparacoes (sequencial): %lld\n", comps);
    } else {
        // busca binaria
        long long comps = 0;
        int idx = buscaBinariaPorNome(componentes, totalComponentes, chave, &comps);
        if (idx != -1) {
            printf("\nComponente-chave encontrado (binaria) -> %s (Tipo: %s | Prioridade: %d)\n",
                   componentes[idx].nome, componentes[idx].tipo, componentes[idx].prioridade);
            printf(">>> Torre ativada! Plano de fuga executado com sucesso. <<<\n");
        } else {
            printf("\nComponente-chave '%s' NAO encontrado na ordenacao por nome. Torre nao ativada.\n", chave);
        }
        printf("Comparacoes (binaria): %lld\n", comps);
    }

    printf("\nEstado atual dos componentes (exibindo para conferencia):\n");
    mostrarComponentes(componentes, totalComponentes);
}

/* ---- Menu principal ---- */
int main() {
    setbuf(stdout, NULL); // facilita a exibição em alguns consoles
    int opcao;
    printf("=== SISTEMA: Montagem da Torre de Fuga (Desafio Final) ===\n");

    do {
        printf("\n----- Menu -----\n");
        printf("1 - Adicionar componente (max %d)\n", MAX_COMPONENTES);
        printf("2 - Remover componente (por nome)\n");
        printf("3 - Listar componentes\n");
        printf("4 - Ordenar / medir algoritmos (bubble/insertion/selection)\n");
        printf("5 - Buscar componente (sequencial)\n");
        printf("6 - Buscar componente (binaria) [requer ordenacao por nome]\n");
        printf("7 - Montagem final / verificar componente-chave\n");
        printf("0 - Sair\n");
        printf("Escolha: ");

        opcao = read_int();

        switch (opcao) {
            case 1:
                adicionarComponente();
                break;
            case 2:
                removerComponente();
                break;
            case 3:
                mostrarComponentes(componentes, totalComponentes);
                break;
            case 4:
                opcaoOrdenarEPesquisar();
                break;
            case 5:
                buscarSequencial();
                break;
            case 6:
                buscarBinaria();
                break;
            case 7:
                montagemFinal();
                break;
            case 0:
                printf("\nEncerrando. Boa sorte na fuga!\n");
                break;
            default:
                printf("Opcao invalida.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}