#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ITENS 10

// 1. Criação de structs: Item
struct Item {
    char nome[30];
    char tipo[20];
    int quantidade;
};

// 2. Declaração do Vetor (Mochila com vetor)
struct Item mochilaVetor[MAX_ITENS];
int totalItensVetor = 0;

// Contadores para as operações (requisito 6)
long long comparacoesBuscaSequencialVetor = 0;
long long comparacoesBuscaBinariaVetor = 0;
long long comparacoesOrdenacaoVetor = 0;

// Flag para saber se o vetor está ordenado por nome (requisito 5)
bool vetorOrdenadoPorNome = false;

// --- Protótipos das funções do Vetor ---
void inserirItemVetor();
void removerItemVetor();
void listarItensVetor();
void exibirResultadoBusca(int indice, char* nomeBusca, long long numComparacoes, const char* tipoBusca); // Função auxiliar para exibir o resultado
int buscarSequencialVetor(char nomeBusca[30]); // Retorna o índice
void ordenarVetorPorNome();
int buscarBinariaVetor(char nomeBusca[30]); // Retorna o índice

// --- Função para pausar a tela ---
void pausarTela() {
    printf("\nPressione Enter para continuar...\n");
    getchar(); // Consome o '\n' pendente, se houver
    getchar(); // Espera por um novo Enter
}

// --- Menu Principal e Lógica ---
int main() {
    int opcao;
    do {
        printf("===================================================\n");
        printf("\n--- MOCHILA DE SOBREVIVENCIA (VETOR) ---\n");
        printf("===================================================\n");
        printf("1. Adicionar Item (Loot)\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens na mochila\n");
        printf("4. Buscar item (Sequencial)\n");
        printf("5. Ordenar Itens por Nome\n");
        printf("6. Buscar Item (Binaria - requer ordenacao)\n");
        printf("0. Sair\n");
        printf("===================================================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                inserirItemVetor();
                pausarTela();
                break;
            case 2:
                removerItemVetor();
                pausarTela();
                break;
            case 3:
                listarItensVetor();
                pausarTela();
                break;
            case 4: // Busca Sequencial
            {
                if (totalItensVetor == 0) {
                    printf("\nMochila vazia!\n");
                    pausarTela();
                    break;
                }
                char nomeBusca[30];
                printf("\n--- Buscar Item na Mochila (Sequencial) ---\n");
                printf("Digite o nome do item que deseja buscar: ");
                scanf(" %[^\n]", nomeBusca);

                comparacoesBuscaSequencialVetor = 0;
                int indiceEncontrado = buscarSequencialVetor(nomeBusca);
                
                exibirResultadoBusca(indiceEncontrado, nomeBusca, comparacoesBuscaSequencialVetor, "Sequencial");
                pausarTela();
                break;
            }
            case 5: // Ordenar Vetor
            {
                ordenarVetorPorNome();
                printf("Comparacoes realizadas (Ordenacao): %lld\n", comparacoesOrdenacaoVetor);
                pausarTela();
                break;
            }
            case 6: // Busca Binária
            {
                if (totalItensVetor == 0) {
                    printf("\nMochila vazia!\n");
                    pausarTela();
                    break;
                }
                if (!vetorOrdenadoPorNome) {
                    printf("\nERRO: O vetor nao esta ordenado por nome. Por favor, ordene primeiro (opcao 5)!\n");
                    pausarTela();
                    break;
                }
                char nomeBusca[30];
                printf("\n--- Buscar Item na Mochila (Binaria) ---\n");
                printf("Digite o nome do item que deseja buscar: ");
                scanf(" %[^\n]", nomeBusca);

                comparacoesBuscaBinariaVetor = 0;
                int indiceEncontrado = buscarBinariaVetor(nomeBusca);
                
                exibirResultadoBusca(indiceEncontrado, nomeBusca, comparacoesBuscaBinariaVetor, "Binaria");
                pausarTela();
                break;
            }
            case 0:
                printf("\nSaindo do sistema...\n");
                break;
            default:
                printf("Opcao invalida!\n");
                pausarTela();
        }
    } while (opcao != 0);
    return 0;
}

// --- Funções para a Mochila com Vetor ---

void inserirItemVetor() {
    if (totalItensVetor >= MAX_ITENS) {
        printf("\nMochila cheia! Nao e possivel adicionar mais itens.\n");
        return;
    }

    struct Item novo;
    printf("\nDigite o nome do item: ");
    scanf(" %[^\n]", novo.nome);

    printf("Digite o tipo do item (arma/municao/cura): ");
    scanf(" %[^\n]", novo.tipo);

    printf("Digite a quantidade: ");
    scanf("%d", &novo.quantidade);

    mochilaVetor[totalItensVetor] = novo;
    totalItensVetor++;
    vetorOrdenadoPorNome = false; // Se inseriu, a ordenacao pode ter sido quebrada
    printf("\nItem cadastrado com sucesso!\n");
}

void removerItemVetor() {
    if (totalItensVetor == 0) {
        printf("\nMochila vazia! Nenhum item para remover.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do item a ser removido: ");
    scanf(" %[^\n]", nomeBusca);

    int encontrado = -1;
    for (int i = 0; i < totalItensVetor; i++) {
        if (strcmp(mochilaVetor[i].nome, nomeBusca) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado != -1) {
        for (int j = encontrado; j < totalItensVetor - 1; j++) {
            mochilaVetor[j] = mochilaVetor[j + 1];
        }
        totalItensVetor--;
        vetorOrdenadoPorNome = false; // A remocao pode ter quebrado a ordenacao
        printf("\nItem removido com sucesso!\n");
    } else {
        printf("\nItem nao encontrado!\n");
    }
}

void listarItensVetor() {
    if (totalItensVetor == 0) {
        printf("\nMochila vazia!\n");
        return;
    }

    printf("\n=== Itens na mochila (Vetor) ===\n");
    for (int i = 0; i < totalItensVetor; i++) {
        printf("%d) Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1,
               mochilaVetor[i].nome,
               mochilaVetor[i].tipo,
               mochilaVetor[i].quantidade);
    }
    printf("==============================\n");
}

// Função auxiliar para exibir o resultado da busca no formato desejado
void exibirResultadoBusca(int indice, char* nomeBusca, long long numComparacoes, const char* tipoBusca) {
    printf("\n---------------------------\n");
    if (indice != -1) {
        printf("--- Item Encontrado! (%s) ---\n", tipoBusca);
        printf("Nome: %s\n", mochilaVetor[indice].nome);
        printf("Tipo: %s\n", mochilaVetor[indice].tipo);
        printf("Quantidade: %d\n", mochilaVetor[indice].quantidade);
    } else {
        printf("Item '%s' nao encontrado! (%s)\n", nomeBusca, tipoBusca);
    }
    printf("Comparacoes realizadas (%s): %lld\n", tipoBusca, numComparacoes);
    printf("---------------------------\n");
}


int buscarSequencialVetor(char nomeBusca[30]) {
    for (int i = 0; i < totalItensVetor; i++) {
        comparacoesBuscaSequencialVetor++; // Cada strcmp é uma comparação lógica
        if (strcmp(mochilaVetor[i].nome, nomeBusca) == 0) {
            return i; // Item encontrado
        }
    }
    return -1; // Item não encontrado
}


void ordenarVetorPorNome() {
    comparacoesOrdenacaoVetor = 0; // Reseta o contador de ordenação

    if (totalItensVetor <= 1) {
        vetorOrdenadoPorNome = true;
        printf("\nVetor com 0 ou 1 item ja esta ordenado.\n");
        return;
    }

    for (int i = 0; i < totalItensVetor - 1; i++) {
        for (int j = 0; j < totalItensVetor - 1 - i; j++) {
            comparacoesOrdenacaoVetor++;
            if (strcmp(mochilaVetor[j].nome, mochilaVetor[j + 1].nome) > 0) {
                struct Item temp = mochilaVetor[j];
                mochilaVetor[j] = mochilaVetor[j + 1];
                mochilaVetor[j + 1] = temp;
            }
        }
    }
    vetorOrdenadoPorNome = true;
    printf("\nVetor ordenado por nome com sucesso!\n");
}


int buscarBinariaVetor(char nomeBusca[30]) {
    int inicio = 0;
    int fim = totalItensVetor - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;

        comparacoesBuscaBinariaVetor++;
        int comparacao = strcmp(mochilaVetor[meio].nome, nomeBusca);

        if (comparacao == 0) {
            return meio; // Item encontrado
        } else if (comparacao < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return -1; // Item não encontrado
}