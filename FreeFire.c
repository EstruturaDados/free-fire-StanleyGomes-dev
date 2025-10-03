#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ITENS 10

struct item {
    char nome [30];
    char tipo [20];
    int quantidade;
};

struct item mochila[MAX_ITENS];
int totalItens = 0;

void inserirItem();
void removerItem();
void listarItens();
void buscarItem();

// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

int main() {

    int opcao;
    do {
        printf ("===================================================\n");
        printf ("\n--- MOCHILA DE SOBREVIVENCIA (CODIGO DA ILHA ---\n)");
        printf ("===================================================\n");
        printf ("1. Adicionar Item (Loot)\n");
        printf ("2. Remover Item\n");
        printf ("3. Listar Itens na mochila\n");
        printf ("4 - Buscar item\n");
        printf ("0 - Sair\n");
        printf ("===================================================\n");
        printf ("Escolha uma opção: \n");
        scanf ("%d", &opcao);
        
        switch (opcao) {
            case 1:
               inserirItem();
               break;
            case 2:
               removerItem();
               break;
            case 3:
               listarItens();
               break;
            case 4:
               buscarItem();
               break;
             case 0:
               printf("\nSaindo do sistema...\n");
               break;
            default:
               printf("opção invalida!\n");         
        } 
    }     while (opcao != 0);
          return 0 ;  

}
    // Menu principal com opções:
    // 1. Adicionar um item
    // 2. Remover um item
    // 3. Listar todos os itens
    // 4. Ordenar os itens por critério (nome, tipo, prioridade)
    // 5. Realizar busca binária por nome
    // 0. Sair

    // A estrutura switch trata cada opção chamando a função correspondente.
    // A ordenação e busca binária exigem que os dados estejam bem organizados.



void inserirItem() {
    if (totalItens >= MAX_ITENS) {
        printf("\nMochila cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    struct item novo;

    printf("\nDigite o nome do item: ");
    scanf(" %[^\n]", novo.nome);  

    printf("Digite o tipo do item (arma/munição/cura): ");
    scanf(" %[^\n]", novo.tipo);

    printf("Digite a quantidade: ");
    scanf("%d", &novo.quantidade);

    // Armazena no vetor
    mochila[totalItens] = novo;
    totalItens++;

    printf("\nItem cadastrado com sucesso!\n");
}

void removerItem() {
    if (totalItens == 0) {
        printf("\nMochila vazia! Nenhum item para remover.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do item a ser removido: ");
    scanf(" %[^\n]", nomeBusca);

    int encontrado = -1;
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado != -1) {
        // "Puxa" os itens seguintes para trás
        for (int j = encontrado; j < totalItens - 1; j++) {
            mochila[j] = mochila[j + 1];
        }
        totalItens--;
        printf("\nItem removido com sucesso!\n");
    } else {
        printf("\nItem não encontrado!\n");
    }
}

void listarItens() {
    if (totalItens == 0) {
        printf("\nMochila vazia!\n");
        return;
    }

    printf("\n=== Itens na mochila ===\n");
    for (int i = 0; i < totalItens; i++) {
        printf("%d) Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1,
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade);
    }
    printf("========================\n");
}

void buscarItem() {
    if (totalItens == 0) {
        printf("\nMochila vazia!\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do item a ser buscado: ");
    scanf(" %[^\n]", nomeBusca);

    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\nItem encontrado!\n");
            printf("Nome: %s | Tipo: %s | Quantidade: %d\n",
                   mochila[i].nome,
                   mochila[i].tipo,
                   mochila[i].quantidade);
            return;
        }
    }
}


// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).

// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
// Exibe a quantidade de comparações feitas (análise de desempenho).

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
