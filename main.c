#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para um contato
struct Contato
{
    char nome[50];
    char celular[15];
    struct Contato *esquerda;
    struct Contato *direita;
};

// Função para comparar nomes
int compararNomes(const char *nome1, const char *nome2)
{
    return strcmp(nome1, nome2);
}

// Função para inserir um contato na árvore
struct Contato *inserir(struct Contato *raiz, struct Contato *novo)
{
    // Se a árvore está vazia, retorna o novo nó como raiz
    if (raiz == NULL)
    {
        return novo;
    }

    // Compara o nome para determinar onde inserir
    int comparacao = compararNomes(novo->nome, raiz->nome);

    if (comparacao < 0)
    {
        // Novo contato deve ir para a esquerda
        raiz->esquerda = inserir(raiz->esquerda, novo);
    }
    else if (comparacao > 0)
    {
        // Novo contato deve ir para a direita
        raiz->direita = inserir(raiz->direita, novo);
    }
    else
    {
        // Nome já existe, não insere duplicados
        printf("Contato com este nome já existe.\n");
        free(novo);
    }

    return raiz;
}

// Função para encontrar o nó mínimo em uma subárvore
struct Contato *encontrarMinimo(struct Contato *no)
{
    struct Contato *atual = no;
    while (atual && atual->esquerda != NULL)
    {
        atual = atual->esquerda;
    }
    return atual;
}

// Função para buscar um contato
struct Contato *buscar(struct Contato *raiz, const char *nome)
{
    // Se a árvore está vazia ou o nome foi encontrado
    if (raiz == NULL || compararNomes(nome, raiz->nome) == 0)
    {
        return raiz;
    }

    // Se o nome é menor, busca na subárvore esquerda
    if (compararNomes(nome, raiz->nome) < 0)
    {
        return buscar(raiz->esquerda, nome);
    }

    // Se o nome é maior, busca na subárvore direita
    return buscar(raiz->direita, nome);
}

// Função para remover um contato
struct Contato *remover(struct Contato *raiz, const char *nome)
{
    // Se a árvore está vazia
    if (raiz == NULL)
    {
        return raiz;
    }

    // Encontra o nó a ser removido
    int comparacao = compararNomes(nome, raiz->nome);

    if (comparacao < 0)
    {
        raiz->esquerda = remover(raiz->esquerda, nome);
    }
    else if (comparacao > 0)
    {
        raiz->direita = remover(raiz->direita, nome);
    }
    else
    {
        // Nó com o nome encontrado

        // Nó com um ou nenhum filho
        if (raiz->esquerda == NULL)
        {
            struct Contato *temp = raiz->direita;
            free(raiz);
            return temp;
        }
        else if (raiz->direita == NULL)
        {
            struct Contato *temp = raiz->esquerda;
            free(raiz);
            return temp;
        }

        // Nó com dois filhos: pega o sucessor in-order (menor na subárvore direita)
        struct Contato *temp = encontrarMinimo(raiz->direita);

        // Copia os dados do sucessor
        strcpy(raiz->nome, temp->nome);
        strcpy(raiz->celular, temp->celular);

        // Remove o sucessor
        raiz->direita = remover(raiz->direita, temp->nome);
    }
    return raiz;
}

// Função para listar contatos em ordem (in-order traversal)
void listar(struct Contato *raiz)
{
    if (raiz != NULL)
    {
        // Percorre a subárvore esquerda
        listar(raiz->esquerda);

        // Imprime o contato atual
        printf("Nome: %s, Celular: %s\n", raiz->nome, raiz->celular);

        // Percorre a subárvore direita
        listar(raiz->direita);
    }
}

// Função do menu
void menu()
{
    printf("\nMenu:\n");
    printf("1. Adicionar contato\n");
    printf("2. Buscar contato\n");
    printf("3. Remover contato\n");
    printf("4. Listar contatos\n");
    printf("5. Sair\n");
    printf("Escolha uma opção: ");
}

void liberarArvore(struct Contato* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

int main()
{
    struct Contato *raiz = NULL;
    int opcao;

    do
    {
        menu();
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer do Enter

        switch (opcao)
        {
        case 1:
        {
            // Adicionar contato
            struct Contato *novo = (struct Contato *)malloc(sizeof(struct Contato));

            printf("Nome: ");
            fgets(novo->nome, sizeof(novo->nome), stdin);
            novo->nome[strcspn(novo->nome, "\n")] = 0; // Remove newline

            printf("Celular: ");
            fgets(novo->celular, sizeof(novo->celular), stdin);
            novo->celular[strcspn(novo->celular, "\n")] = 0; // Remove newline

            novo->esquerda = NULL;
            novo->direita = NULL;

            raiz = inserir(raiz, novo);
            break;
        }
        case 2:
        {
            // Buscar contato
            char nome[50];
            printf("Nome para buscar: ");
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = 0; // Remove newline

            struct Contato *encontrado = buscar(raiz, nome);
            if (encontrado)
            {
                printf("Contato encontrado: %s, Celular: %s\n",
                       encontrado->nome, encontrado->celular);
            }
            else
            {
                printf("Contato não encontrado.\n");
            }
            break;
        }
        case 3:
        {
            // Remover contato
            char nome[50];
            printf("Nome para remover: ");
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = 0; // Remove newline

            raiz = remover(raiz, nome);
            break;
        }
        case 4:
        {
            // Listar contatos
            printf("Contatos:\n");
            listar(raiz);
            break;
        }
        case 5:
            printf("Saindo...\n");
            liberarArvore(raiz);
            break;
        default:
            printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 5);

    return 0;
}