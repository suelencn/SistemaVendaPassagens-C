//Alunos: Lorran Felipe Miranda e Suelen Caroline do Nascimento

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#define LINHA 10
#define COLUNA 4
#define TAMANHO 50

//estrutura do onibus
struct onibus{
    int poltrona;
    char cliente [30];
    char status [10];
};

//estrutura do cliente
struct cliente{
    char nome[30];
    char cpf[12];
    int usado;
};

//cabeçalho funções
void textcolor (int color);
void mudacor(struct onibus status);
void preenche_onibus(struct onibus matrizOnibus [LINHA][COLUNA]);
void preenche_clientes(struct cliente lista_de_cliente[], int tam);
void salvar_cliente(struct cliente lista_de_cliente [], int tam);
void salvar_onibus(struct onibus matrizOnibus [LINHA][COLUNA]);
struct cliente leCliente();
void incluiCliente(struct cliente lista_de_cliente[], int tam);
void imprimir_lista_clientes(struct cliente lista_de_cliente[], int tam);
char* verificaCadastro (struct cliente lista_de_cliente[], int tam);
int verificaCPF(char st[], int tam);
void excluir_clinte(struct cliente lista_de_cliente[], int tam);
void pesquisa_cliente(struct cliente lista_de_cliente[], int tam);
void imprimir_onibus (struct onibus matrizOnibus [LINHA][COLUNA]);
void reservar_onibus (struct onibus matrizOnibus [LINHA][COLUNA], char resultado[]);
void vender_onibus (struct onibus matrizOnibus [LINHA][COLUNA], char resultado[]);
void alterar_reserva (struct onibus matrizOnibus [LINHA][COLUNA]);
void alterar_ocupado (struct onibus matrizOnibus [LINHA][COLUNA]);


//função principal
int main(){
    setlocale(LC_ALL, "Portuguese");

    struct onibus matrizOnibus[LINHA][COLUNA];
    struct cliente lista_de_cliente[TAMANHO];

    FILE *entradaOnibus;
    FILE *entradaClientes;

    entradaClientes = fopen("entradaClientes.bin", "rb");
    entradaOnibus = fopen("entradaOnibus.bin", "rb");
    if(entradaClientes == NULL && entradaOnibus == NULL){
        preenche_clientes(lista_de_cliente, TAMANHO);
        preenche_onibus(matrizOnibus);
        printf("Lista preenchida com sucesso!\n");
    } else {
        fread(lista_de_cliente, sizeof(struct cliente), TAMANHO, entradaClientes);
        fread(matrizOnibus, sizeof(struct onibus), LINHA*COLUNA, entradaOnibus);
        printf("Arquivo carregado com sucesso!\n");
        fclose(entradaClientes);
        fclose(entradaOnibus);
    }


    while (1){

        int op;
        printf("------------------------------\n");
        printf("      Escolha uma opção       \n");
        printf("------------------------------\n");
        printf("1 - Exibir poltronas do onibus\n"
               "2 - Fazer reserva\n"
               "3 - Comprar passagem\n"
               "4 - Novo cadastro de cliente\n"
               "5 - Exibir lista de clientes\n"
               "6 - Excluir cliente\n"
               "7 - Pesquisar cliente\n"
               "8 - Alterar acento reserva\n"
               "9 - Alterar acento ocupado\n"
               "0 - Sair\n\n\n");

        scanf("%d", &op);
        switch(op){
        case 1:
            imprimir_onibus(matrizOnibus);
            break;
        case 2:{
            char *resultado;
            resultado = verificaCadastro(lista_de_cliente, TAMANHO);
            printf("%s\n", resultado);
            if (resultado == NULL){
                return 0;
            } else
            reservar_onibus(matrizOnibus, resultado);
            }
            break;
        case 3:{
            char *resultado;
            resultado = verificaCadastro(lista_de_cliente, TAMANHO);
            printf("%s\n", resultado);
            if (resultado == NULL){
                return 0;
            } else
            vender_onibus(matrizOnibus, resultado);
            }
            break;
        case 4:
            incluiCliente(lista_de_cliente, TAMANHO);
            break;
        case 5:
            imprimir_lista_clientes(lista_de_cliente, TAMANHO);
            break;
        case 6:
            excluir_clinte(lista_de_cliente, TAMANHO);
            break;
        case 7:
            pesquisa_cliente(lista_de_cliente, TAMANHO);
            break;
        case 8:
            alterar_reserva(matrizOnibus);
            break;
        case 9:
            alterar_ocupado(matrizOnibus);
            break;
        case 0:
            salvar_cliente(lista_de_cliente, TAMANHO);
            salvar_onibus(matrizOnibus);
            return 0;
        }
    }
}

//função p/ mudar a cor no console
void textcolor (int color) {
    static int __BACKGROUND;
    HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    GetConsoleScreenBufferInfo(h, &csbiInfo);
    SetConsoleTextAttribute (GetStdHandle (STD_OUTPUT_HANDLE), color + (__BACKGROUND << 4));
}

//função p/ mudar cor status onibus
void mudacor(struct onibus status){

    if (strcmp(status.status,"Livre") == 0){
        textcolor(47);
    } else if (strcmp(status.status,"Reservado")==0){
        textcolor(111);
    } else if (strcmp(status.status,"Ocupado")==0){
        textcolor(79);
    }
}

//preenche lista do onibus
void preenche_onibus(struct onibus matrizOnibus [LINHA][COLUNA]){
    int indice = 1;
    //preenche matriz onibus com status livre e preenche o número da poltrona
    for(int i=0; i<LINHA; i++){
        for(int j=0; j<COLUNA; j++){
            matrizOnibus[i][j].poltrona = indice;
            strcpy (matrizOnibus[i][j].status,"Livre");
            indice ++;
        }
    }
}

//preenche lista clientes
void preenche_clientes(struct cliente lista_de_cliente[], int tam){
    //preenche a lista de clientes com usado=0
    for (int i=0; i<TAMANHO; i++){
        lista_de_cliente[i].usado = 0;
    }
}

//salva dados dos clientes em arquivo
void salvar_cliente(struct cliente lista_de_cliente [], int tam){
    FILE *arq = fopen("entradaClientes.bin", "wb");
    fwrite(lista_de_cliente, sizeof(struct cliente), TAMANHO, arq);
    fclose(arq);
}

//salva dados do onibus em arquivo
void salvar_onibus(struct onibus matrizOnibus [LINHA][COLUNA]){
    FILE *arq = fopen("entradaOnibus.bin", "wb");
    fwrite(matrizOnibus, sizeof(struct onibus), LINHA*COLUNA , arq);
    fclose(arq);
}

//funçao para ler informações dos clientes
struct cliente leCliente(){
    struct cliente aux;  //criando variavel auxiliar para receber os novos dados

    printf("\nInforme o nome completo a ser cadastrado\n");
    scanf(" %[^\n]s", aux.nome);
    toupper(aux.nome);
    printf("\nInforme o cpf, somente com números e sem espaços\n");
    scanf("%s", aux.cpf);

    while ((verificaCPF(aux.cpf, 11)) == 0){
        printf("CPF invalido\n");
        printf("\nInforme o cpf, somente com números e sem espaços\n");
        scanf("%s", aux.cpf);
    }

    printf("Cliente cadastrado com sucesso!\n");
    return aux;
};

//funçao para incluir clientes no vetor lista_de_cliente
void incluiCliente(struct cliente lista_de_cliente[], int tam){
    for (int i=0; i<tam; i++ ){
        if (lista_de_cliente[i].usado == 0){ // se a posição estiver livre, a função recebe as informações de leCliente
            lista_de_cliente[i] = leCliente();
            lista_de_cliente[i].usado = 1;
            return;
        }
    }
}

// função para imprimir lista de clientes cadastrados
void imprimir_lista_clientes(struct cliente lista_de_cliente[], int tam){
    for (int i=0; i<tam; i++){
        if (lista_de_cliente[i].usado == 1){
            printf(" %s - %s\n ", lista_de_cliente[i].nome, lista_de_cliente[i].cpf);
        }
    }
}

//função para verificar se o cliente está cadastrado
char* verificaCadastro (struct cliente lista_de_cliente[], int tam){
    char nomecliente [50];
    char nomecliente2 = 'X';

    printf("Informe o nome do cliente cadastrado: ");
    scanf(" %[^\n]s",nomecliente);

     //laço p/ verificar se o cliente informado esta cadastrado
    for(int i=0; i< tam; i++){
        if (strcmp(lista_de_cliente[i].nome,nomecliente) == 0){
            return lista_de_cliente[i].nome;
        }
    }
    printf("Cliente não está cadastrado");
    return NULL;
}

// função p/ verificar cpf
int verificaCPF(char st[], int tam) {
/*
retorna 1, caso o cpf seja valido
retorna 0, caso o cpf seja invalido
*/
    int saida = 1;

    if(strlen(st) != 11) {
        saida = 0;
    }

    for(int i=0;i<tam;i++) {
        if(st[i] < 48 || st[i] > 57) {
            saida = 0;
            break;
        }
    }
    return saida;
}

//funçao para excluir cliente a partir da funcao que procura o nome que deseja excluir
void excluir_clinte(struct cliente lista_de_cliente[], int tam){
    char aux[30];

    printf("Informe o nome do cadastro que deseja excluir: ");
    scanf("%s", aux);

    for (int i=0; i<tam; i++){
        if (strcmp(lista_de_cliente[i].nome,aux) == 0){
            lista_de_cliente[i].usado = 0;  //quando o nome for excluido da lista.usado vai receber 0 novamente, ou seja, possível de incluir outro nome na posição
            printf("Cliente excluido!");
            return 1;
        }
    }
}

// função pesquisa cliente
void pesquisa_cliente(struct cliente lista_de_cliente[], int tam){
    char pesquisa [50];

    int op;
    printf("Escolha uma opção:\n"
           "1 - CPF\n"
           "2 - Nome\n");
    scanf("%d", &op);

    switch(op){
    case 1:
        printf("Informe o CPF:\n");
        scanf("%s", pesquisa);
        break;
    case 2:
        printf("Informe o Nome:\n");
        scanf(" %[^\n]s", pesquisa);
        break;
    }

    toupper(pesquisa);
    for (int i=0; i<tam; i++){
        if (strcmp(lista_de_cliente[i].cpf,pesquisa)|| strcmp(lista_de_cliente[i].nome,pesquisa)){
            printf("%s - %s\n", lista_de_cliente[i].nome, lista_de_cliente[i].cpf);
            return 1;
            } else {
                printf("Cliente não encontrado!");
            }
    }
}

//função p/ imprimir status do onibus
void imprimir_onibus (struct onibus matrizOnibus [LINHA][COLUNA]){
    for(int i=0; i<LINHA; i++){
        for(int j=0; j<COLUNA; j++){
            if(j == 0 || j == 2 ){
                mudacor(matrizOnibus[i][j]);
                printf("[%2d]-%s \t", matrizOnibus[i][j].poltrona, matrizOnibus[i][j].status);
                textcolor(15);
            }
            if (j == 3){
                mudacor(matrizOnibus[i][j]);
                printf("[%2d]-%s \n", matrizOnibus[i][j].poltrona, matrizOnibus[i][j].status);
                textcolor(15);
            }
            if (j == 1){
                mudacor(matrizOnibus[i][j]);
                printf("[%2d]-%s \t\t", matrizOnibus[i][j].poltrona, matrizOnibus[i][j].status);
                textcolor(15);
            }
        }
    }
}

//função p/ fazer reserva
void reservar_onibus (struct onibus matrizOnibus [LINHA][COLUNA], char resultado[]){
   struct onibus aux;

   printf("Informe o número da poltrona: ");
   scanf("%d", &aux.poltrona);


   while (aux.poltrona < 0 || aux.poltrona >40){
        printf("\nNúmero de poltrona inválido");
        printf("\nInforme o número da poltrona: ");
        scanf("%d", &aux.poltrona);
   }

   for(int i=0; i<LINHA; i++){
        for(int j=0; j<COLUNA; j++){
            if (matrizOnibus[i][j].poltrona == aux.poltrona){
                if (strcmp(matrizOnibus[i][j].status,"Livre") == 0){
                    strcpy(matrizOnibus[i][j].cliente,resultado);
                    strcpy(matrizOnibus[i][j].status,"Reservado");
                    printf("Poltrona %2d - Reservada %s!\n", matrizOnibus[i][j].poltrona,matrizOnibus[i][j].cliente);
                    return;
                }else{
                    printf("Poltrona não está disponível!");
                }
            }

        }
    }


}

//função p/ vender poltronas
void vender_onibus (struct onibus matrizOnibus [LINHA][COLUNA], char resultado[]){
   struct onibus aux;

   printf("Informe o número da poltrona: ");
   scanf("%d", &aux.poltrona);

   while (aux.poltrona < 0 || aux.poltrona >40){
        printf("\nNúmero de poltrona inválido");
        printf("\nInforme o número da poltrona: ");
        scanf("%d", &aux.poltrona);
   }

   for(int i=0; i<LINHA; i++){
        for(int j=0; j<COLUNA; j++){
            if (matrizOnibus[i][j].poltrona == aux.poltrona){
                if (strcmp(matrizOnibus[i][j].status,"Livre") == 0){
                    strcpy(matrizOnibus[i][j].cliente,resultado);
                    strcpy(matrizOnibus[i][j].status,"Ocupado");
                    printf("Poltrona %2d - Vendida!\n", matrizOnibus[i][j].poltrona);
                    return;
                } else{
                    printf("Poltrona não está disponível!");
                }
            }
        }
    }
}

//função p/ alterar reserva
void alterar_reserva (struct onibus matrizOnibus [LINHA][COLUNA]){
    struct onibus aux;

   printf("Informe o número da poltrona: ");
   scanf("%d", &aux.poltrona);

   while (aux.poltrona < 0 || aux.poltrona >40){
        printf("\nNúmero de poltrona inválido");
        printf("\nInforme o número da poltrona: ");
        scanf("%d", &aux.poltrona);
   }

   for(int i=0; i<LINHA; i++){
        for(int j=0; j<COLUNA; j++){
            if (matrizOnibus[i][j].poltrona == aux.poltrona){
                if (strcmp(matrizOnibus[i][j].status,"Reservado") == 0){
                    strcpy(matrizOnibus[i][j].status,"Livre");
                    printf("Poltrona %2d - Livre!\n", matrizOnibus[i][j].poltrona);
                    return;
                } else{
                    printf("Poltrona não está reservada, verifique novamente a lista de poltronas.\n");
                }
            }
        }
    }
}

//função p/ deletar reserva
void alterar_ocupado (struct onibus matrizOnibus [LINHA][COLUNA]){
    struct onibus aux;

   printf("Informe o número da poltrona: ");
   scanf("%d", &aux.poltrona);

   while (aux.poltrona < 0 || aux.poltrona >40){
        printf("\nNúmero de poltrona inválido");
        printf("\nInforme o número da poltrona: ");
        scanf("%d", &aux.poltrona);
   }

   for(int i=0; i<LINHA; i++){
        for(int j=0; j<COLUNA; j++){
            if (matrizOnibus[i][j].poltrona == aux.poltrona){
                if (strcmp(matrizOnibus[i][j].status,"Ocupado") == 0){
                    strcpy(matrizOnibus[i][j].status,"Livre");
                    printf("Poltrona %2d - Livre!\n", matrizOnibus[i][j].poltrona);
                    return;
                } else{
                    printf("Poltrona não está ocupada, verifique novamente a lista de poltronas.\n");
                }
            }
        }
    }
}
