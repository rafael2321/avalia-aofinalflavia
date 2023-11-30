#include <iostream>
#include <string.h>
#include <Windows.h>
#include <locale>

struct Cliente {
    char nome[20];
    char cpf[20];
    char email[20];
    char status;
};

int tamanho(FILE *);
void cadastrar(FILE *);
void consultar(FILE *);
void geraarqtxt(FILE *);
void excluir(FILE *);

int main() {
	setlocale(LC_ALL, "Portuguese");
    int op;
    FILE *arq;

    if ((arq = fopen("clientes.dat", "rb+")) == NULL) {
        if ((arq = fopen("clientes.dat", "wb+")) == NULL) {
            printf("Falha ao abrir o arquivo!\n");
            system("pause");
        }
    }

    do {
        system("CLS");
        printf("\n_______ CLIENTES _______ \n");
        printf("1.Cadastrar\n");
        printf("2.Consultar por código\n");
        printf("3.Gerar arquivo\n");
        printf("4.Excluir registro\n");
        printf("5.Sair\n");
        printf("CODIGO:%d=\n", tamanho(arq));
        printf("Opção:");
        scanf("%d", &op);

        switch (op) {
            case 1:
                cadastrar(arq);
                break;
            case 2:
                consultar(arq);
                break;
            case 3:
                geraarqtxt(arq);
                break;
            case 4:
                excluir(arq);
                break;
            case 5:
                fclose(arq);
        }
    } while (op != 5);

    return 0;
}

void cadastrar(FILE *arq) {
    Cliente novoCliente;
    char confirma;
    novoCliente.status = ' ';

    fflush(stdin);
    printf("Cadastrando novo cliente:\n");
    printf("\nNúmero do registro:%d\n", tamanho(arq) + 1);
    printf("Nome..........:");
    gets(novoCliente.nome);
    printf("CPF...........:");
    gets(novoCliente.cpf);
    printf("E-mail........:");
    gets(novoCliente.email);

    printf("\nConfirma <s/n>:");
    fflush(stdin);
    scanf("%c", &confirma);

    if (toupper(confirma) == 'S') {
        printf("\ngravando...\n\n");
        fseek(arq, 0, SEEK_END);
        fwrite(&novoCliente, sizeof(Cliente), 1, arq);
    }
    system("pause");
}

void consultar(FILE *arq) {
    Cliente cliente;
    int nr;

    printf("\nConsulta pelo código\n");
    printf("\nInforme o Código...:");
    scanf("%d", &nr);

    if ((nr <= tamanho(arq)) && (nr > 0)) {
        fseek(arq, (nr - 1) * sizeof(Cliente), SEEK_SET);
        fread(&cliente, sizeof(Cliente), 1, arq);

        if (cliente.status == ' ') {
            printf("\nNome......:%s", cliente.nome);
            printf("\nCPF.......:%s", cliente.cpf);
            printf("\nE-mail....:%s\n\n", cliente.email);
        } else {
            printf("\nRegistro excluido! \n");
        }
    } else {
        printf("\nNumero de registro invalido!\n");
    }
    system("pause");
}

void geraarqtxt(FILE *arq) {
    char nomearq[20];
    printf("Nome do arquivo texto:");
    scanf("%s", nomearq);
    strcat(nomearq, ".txt");
    FILE *arqtxt = fopen(nomearq, "w");

    if (!arqtxt) {
        printf("Nao foi possivel criar esse arquivo!\n");
        system("pause");
    }

    fprintf(arqtxt, "Nome                CPF         E-mail                   Status\n");
    fprintf(arqtxt, "================================================================\n");

    int nr;
    Cliente cliente;

    for (nr = 0; nr < tamanho(arq); nr++) {
        fseek(arq, nr * sizeof(Cliente), SEEK_SET);
        fread(&cliente, sizeof(Cliente), 1, arq);
        fprintf(arqtxt, "%-20s%-12s%-25s- %c\n", cliente.nome, cliente.cpf, cliente.email, cliente.status);
    }

    fprintf(arqtxt, "================================================================\n");
    fclose(arqtxt);
}

void excluir(FILE *arq) {
    Cliente cliente;
    char confirma;
    int nr;

    printf("\nInforme o codigo do registro para excluir\n");
    scanf("%d", &nr);

    if ((nr <= tamanho(arq)) && (nr > 0)) {
        fseek(arq, (nr - 1) * sizeof(Cliente), SEEK_SET);
        fread(&cliente, sizeof(Cliente), 1, arq);

        if (cliente.status == ' ') {
            printf("\nNome......:%s", cliente.nome);
            printf("\nCPF.......:%s", cliente.cpf);
            printf("\nE-mail....:%s\n", cliente.email);
            printf("\nConfirma a exclusao: <s/n>\n");
            getchar();
            scanf("%c", &confirma);

            if (toupper(confirma) == 'S') {
                printf("\nexcluindo...\n\n");
                fseek(arq, (nr - 1) * sizeof(Cliente), SEEK_SET);
                cliente.status = '*';
                fwrite(&cliente, sizeof(Cliente), 1, arq);
            }
        } else {
            printf("Registro inexistente! \n");
        }
    } else {
        printf("\nNumero de registro invalido!\n");
    }
    system("pause");
}

int tamanho(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    return ftell(arq) / sizeof(Cliente);
}
