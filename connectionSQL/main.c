#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <locale.h>
#include "mysql\include\mysql.h"

struct user_now{
    int ra;
    char nome[250];
    char email[250];
};

struct user_now user;

int verificacao_acesso(MYSQL *conexao, char senha);

void erro(MYSQL *conexao){
    fprintf(stderr,  "\n%s\n", mysql_error(conexao));
    mysql_close(conexao);
    exit(1);
}

MYSQL *__stdcall obterconexao(){
    char *servidor = "127.0.0.1";
    char *usuario = "root";
    char *senha = "";
    char *nomebanco = "bancoC";
    int porta = 3306;

    MYSQL *conexao = mysql_init(NULL);

    if(!mysql_real_connect(conexao, servidor, usuario, senha, nomebanco, porta, NULL, 0)){
        erro(conexao);
        return NULL;
    }
    else{
        return conexao;
    }
}

int login(MYSQL *conexao){ // login no app
    MYSQL_RES *resultado;
    MYSQL_ROW row;
    char nome_admin[] = "admin";
    char senha_admin[] = "admin";
    int encontrado = 0, RA, type_login;
    char query[750], senha[100];
    printf("1 - aluno \n");
    printf("2 - proferssor \n");
    printf("logar como: ");
    scanf("%d", &type_login);
    if(type_login == 3){
        printf("digite a senha: ");
        scanf("%s", &senha);
        if (strcmp(senha, senha_admin) == 0)
        {
            return 2;
        }
        
        
    }
    switch (type_login)
    {
    case 1:
        printf("digite seu RA: ");
        scanf("%i", &RA);
        //printf("presionne ENTER caso for sua primeira fez acessando \ndigite sua senha: ");
        printf("digite sua senha: ");
        scanf(" %s", &senha);
        //fgets(senha, 100, stdin);
        //senha[strcspn(senha, "\n")] = "\0";
        sprintf(query, "SELECT idaluno, nome, senha, email FROM aluno WHERE IDALUNO = %i and senha = '%s'", RA, senha);
        break;
    case 2:
        printf("digite seu id: ");
        scanf("%i", &RA);
        printf("digite sua senha: ");
        scanf("%s", &senha);
        sprintf(query, "SELECT nome, senha FROM professor WHERE IDALUNO = %i and senha = '%s'", RA, senha);
        break;
    default:
        break;
    }
    fflush(stdin);
    if(mysql_query(conexao, query)){
        erro(conexao);
    }
    resultado = mysql_use_result(conexao);
    while((row = mysql_fetch_row(resultado)) != NULL)
    {
        strcat(user.nome, row[1]);
        user.ra = row[0];
        strcat(user.email, row[3]);
        mysql_free_result(resultado);
        return type_login;
    } 
    mysql_free_result(resultado);
    return 0;
    
}


void inserir_aluno(MYSQL *conexao){ // adicionar alunos na tabela
    char nome[250], email[250],senha[100], resp, query[750];
    int telefone, cont_loop, i=0;
    fflush(stdin);
    printf("quantos alunos gostaria de inserir?: ");
    getchar();
    sprintf("%i", &cont_loop);
    do{
        getchar();
        printf("insira o nome do aluno: ");
        scanf(" %s", &nome);
        getchar();
        printf("insira o email do aluno: ");
        scanf(" %s", &email);
        getchar();
        printf("insira o telefone do aluno");
        scanf(" %i", &telefone);
        getchar();
        sprintf(query, "INSERT INTO aluno(nome, email, telefone) VALUES ('%s', %s, %i)", nome, email, telefone);
        if(mysql_query(conexao, query)){
            erro(conexao);
        }
        else{
            printf("aluno cadastrado");
        }
    }while(i < cont_loop);
    
}

int remover_aluno(MYSQL *conexao){ // remove aluno
    MYSQL_RES *res;
    MYSQL_ROW *row;
    int ra, count_remove;
    char query[250], resp, *r;
    r = &query;
    printf("quantos alunos gostaria de remover?: ");
    scanf("%d", &count_remove);
    for(int i = 0; i < count_remove; i++){
        printf("digite o RA do aluno: ");
        scanf("%i", &ra);
        sprintf(r, "SELECT ra, nome FROM aluno WHERE IDALUNO = %i", ra);
        if(mysql_query(conexao, query)){
            erro(conexao);
        }
        res = mysql_store_result(conexao);
        while((row = mysql_fetch_row(res)) != NULL){
            printf("RA:   %s \n", row[0]);
            printf("nome: %s \n", row[1]);
        }
        mysql_free_result(res);
        sprintf(r, "DELETE FROM aluno WHERE IDALUNO = %i", ra);
        printf("gostaria de apagar o aluno (s/n): ");
        scanf(" %c", &resp);
        if(resp == 'n'){
            printf("aluno nao removido \n");
            return 1;
        }
        else if(mysql_query(conexao, query)){
            printf("erro aluno nao foi removido \n");
        }
        else{
            printf("aluno romovido \n");
        }
    }
    return 0;
}
int update(MYSQL *conexao){
    MYSQL_RES *res;
    MYSQL_ROW *row;
    int escolha, ra, new_int;
    char cont, new_char[250], query[750], conf_alt;
    const char *menu[] = {
        "nome", 
        "email",
        "telf",
        "turma",
    };
    do{
        printf("1 - %s \n", menu[0]);
        printf("2 - %s \n", menu[1]);
        printf("3 - telefone \n");
        printf("4 - %s \n", menu[3]);
        printf("Digite o comando: ");
        scanf("%d", &escolha);
        if(escolha > 3){
            system("clear");
            printf("comando invalido tente novamente");
        }
    }while(escolha > 3);
    switch(escolha){
        case 1:
            printf("digite o novo nome: ");
            getchar();
            fgets(new_char, 250, stdin);
            new_char[strcspn(new_char, "\n")] = '\0';
            break;
        case 2:
            printf("digite o novo email: ");
            getchar();
            scanf("%s", &new_char);
            break;
        case 3:
            printf("digite o novo telf do aluno: ");
            getchar();
            scanf("%d", &new_int);
            break;
        case 4:
            printf("digite a nova turma do aluno: ");
            getchar();
            scanf("%d", &new_int);
            break;
        default:
            system("clear");
            printf("comando invalidon tente novamente");
    }
    printf("Digite o Ra do aluno: ");
    getchar();
    scanf(" %d", &ra);
    sprintf(query, "select idaluno, nome from aluno where IDALUNO = %i", ra);
    if(mysql_query(conexao, query)){
        printf("aluno não encontrado");
        return 0;
    }
    res = mysql_use_result(conexao);
    printf("Deseja altera o aluno? (s\\n)\n");
    while((row = mysql_fetch_row(res)) != NULL){
        printf("Ra: %s, \nNome: %s \n", row[0], row[1]);
    }
    printf("comando: ");
    getchar();
    scanf("%c", &conf_alt);
    mysql_free_result(res);
    escolha -= 1;
    if((escolha == 0 || escolha == 1) && (conf_alt = 's')){
        sprintf(query, "Update aluno set %s = '%s' where IDALUNO = %d", menu[(escolha)], new_char, ra);
    }
    else if((escolha == 2 || escolha == 3) && (conf_alt = 's')){
        sprintf(query, "Update aluno set %s = %i where IDALUNO = %d", menu[(escolha)], new_int, ra);
    }
    if(mysql_query(conexao, query)){
        erro(conexao);
    }
    else{
        printf("dados alterados");
        return 1;
    }
}

int seach_aluno(MYSQL *conexao){ // procura na tabela aluno

    MYSQL_RES *res;
    MYSQL_ROW *row;
    int resp, ra, turma;
    char query[750], nome[100];
    printf("1 - Turma \n");
    printf("2 - RA \n");
    printf("3 - Nome  \n");
    printf("digite seu comando: ");
    scanf("%i", &resp);
    switch (resp){
    case 1:
        //printf("Digite a turma que deseja procurar: ");
        //scanf("%i", &turma);
        //sprintf(query, "SELECT idaluno, nome, email, idturma from aluno WHERE IDALUNO = %i", turma);
        printf("manutenção");
        break;
    case 2:
        printf("Digite o RA do aluno: ");
        scanf("%i", &ra);
        sprintf(query, "SELECT idaluno, nome, email, idturma from aluno WHERE IDALUNO = %i", ra);
        break;
    case 3:
        printf("Digite o nome do aluno: ");
        scanf("%s", &nome);
        sprintf(query, "SELECT idaluno, nome, email, telf, idturma from aluno WHERE UPPER(nome) LIKE UPPER('%%%s%%')", nome);
        break;
    default:
        printf("Comando invalido");
        break;
    }
    if(mysql_query(conexao, query)){
        erro(conexao);
    }
    res = mysql_store_result(conexao);
    printf("Ra        |Nome                         |telefone                       |email                     |idturma\n");
    while((row = mysql_fetch_row(res)) != NULL){
        printf("%s   |%s     |%i    |%s  |%i \n", row[0], row[1], row[2], row[3], row[4]);  
    }
    mysql_free_result(res);
}

int verificacao_acesso(MYSQL *conexao, char senha){ // verificar se é o primeiro acesso
    if(senha == NULL){
        fflush(stdin);
        char senha[100];
        printf("seja bem vindo %s", user.nome);
        printf("vejo que seu primeiro acesso");
        printf("Poderia insira sua senha: ");
        scanf("%s ", &senha);
        return 0;
    }
}
void rank(MYSQL *conexao){
    MYSQL_RES res;
    MYSQL_ROW row;
    
}

void exercicios() {
    setlocale(LC_ALL, "pt_BR");
    int escolha_modulo, resposta, acertos = 0;
    char enter;
    printf("\nEscolha o módulo de exercícios:\n");
    printf("1 - Operadores Lógicos\n");
    printf("2 - Operadores Matemáticos\n");
    printf("3 - Alocação de Variável e Ponteiro\n");
    printf("4 - if-else\n");
    printf("5 - Loops\n");
    printf("Digite o número da opção desejada: ");
    scanf("%d", &escolha_modulo);
    while((enter = getchar()) != '\n' && enter != EOF); //limpa buffer

    switch(escolha_modulo) {
    case 1: //operadores lógicos
        printf("\nExercício 1:\nO resultado de (1 && 0) é:\n1) Verdadeiro \n2) Falso\n3) 2\nResposta: ");
        scanf("%d", &resposta); if(resposta==1) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 2:\nO resultado de (!1 || 1) é:\n1) 0\n2) 1\n3) -1\nResposta: ");
        scanf("%d", &resposta); if(resposta==2) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 3:\nO resultado de (0 || 0) é:\n1) 0\n2) 1\n3) -1\nResposta: ");
        scanf("%d", &resposta); if(resposta==1) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 4:\nO resultado de !(1 && 1) é:\n1) 0\n2) 1\n3) -1\nResposta: ");
        scanf("%d", &resposta); if(resposta==1) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 5:\nO resultado de (1 || (0 && 1)) é:\n1) 0\n2) 1\n3) -1\nResposta: ");
        scanf("%d", &resposta); if(resposta==2) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);
        break;

    case 2: //operadores matemáticos
        printf("\nExercício 1:\nO valor de 2 + 3 * 4 é:\n1) 20\n2) 14\n3) 12\nResposta: ");
        scanf("%d", &resposta); if(resposta==2) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 2:\nO valor de 10 / 2 + 7 é:\n1) 12\n2) 5\n3) 17\nResposta: ");
        scanf("%d", &resposta); if(resposta==1) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 3:\nO valor de 10 %% 3 é:\n1) 1\n2) 2\n3) 3\nResposta: ");
        scanf("%d", &resposta); if(resposta==2) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 4:\nO valor de 5 - 8 * 2 é:\n1) -11\n2) -16\n3) 6\nResposta: ");
        scanf("%d", &resposta); if(resposta==1) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 5:\nO valor de 2 * 2 * 2 é:\n1) 6\n2) 8\n3) 4\nResposta: ");
        scanf("%d", &resposta); if(resposta==2) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);
        break;

    case 3: //alocação de variável e ponteiro
        printf("\nExercício 1:\nQual comando aloca memória dinamicamente em C?\n1) malloc\n2) alloc\n3) calloc\nResposta: ");
        scanf("%d", &resposta); if(resposta==1) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 2:\nint *p; int x=10; p = &x; O valor de *p é:\n1) 10\n2) 0\n3) p\nResposta: ");
        scanf("%d", &resposta); if(resposta==1) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 3:\nDepois de int *p = NULL; p == NULL é:\n1) verdadeiro\n2) falso\n3) erro\nResposta: ");
        scanf("%d", &resposta); if(resposta==1) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 4:\nPara liberar memória alocada dinamicamente, usa-se:\n1) free\n2) delete\n3) release\nResposta: ");
        scanf("%d", &resposta); if(resposta==1) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 5:\nO operador & em C retorna:\n1) valor da variável\n2) endereço da variável\n3) nada\nResposta: ");
        scanf("%d", &resposta); if(resposta==2) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);
        break;

    case 4: //if-else
        printf("\nExercício 1:\nO que será impresso?\nint x=5;\nif(x>2) printf(\"A\"); else printf(\"B\");\n1) A\n2) B\n3) nada\nResposta: ");
        scanf("%d", &resposta); if(resposta==1) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 2:\nif(0) printf(\"X\"); else printf(\"Y\");\n1) X\n2) Y\n3) XY\nResposta: ");
        scanf("%d", &resposta); if(resposta==2) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 3:\nint a=3; if(a==3) printf(\"Certo\"); else printf(\"Errado\");\n1) Certo\n2) Errado\n3) nada\nResposta: ");
        scanf("%d", &resposta); if(resposta==1) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 4:\nif(2<1) printf(\"Sim\"); else printf(\"Nao\");\n1) Sim\n2) Nao\n3) nada\nResposta: ");
        scanf("%d", &resposta); if(resposta==2) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 5:\nint b=10; if(b!=10) printf(\"A\"); else printf(\"B\");\n1) A\n2) B\n3) nada\nResposta: ");
        scanf("%d", &resposta); if(resposta==2) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);
        break;

    case 5: //loops
        printf("\nExercício 1:\nQuantas vezes o código executa o printf?\nfor(int i=0; i<3; i++) printf(\"Oi\");\n1) 1\n2) 2\n3) 3\nResposta: ");
        scanf("%d", &resposta); if(resposta==3) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 2:\nint i=0; while(i<2){i++;} Quantas vezes roda?\n1) 1\n2) 2\n3) 3\nResposta: ");
        scanf("%d", &resposta); if(resposta==2) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 3:\nO que faz o comando break dentro de um loop?\n1) Continua o loop\n2) Sai do loop\n3) Repete\nResposta: ");
        scanf("%d", &resposta); if(resposta==2) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 4:\nQual o valor final de x?\nint x=0; for(int i=0;i<4;i++) x=x+2;\n1) 4\n2) 6\n3) 8\nResposta: ");
        scanf("%d", &resposta); if(resposta==3) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);

        printf("\nExercício 5:\nQual comando faz o loop pular para a próxima iteração?\n1) end\n2) jump\n3) continue\nResposta: ");
        scanf("%d", &resposta); if(resposta==3) acertos++;
        while((enter = getchar()) != '\n' && enter != EOF);
        break;

    default:
        printf("\nOpção inválida!\n");
        return;
    }
    printf("\nVocê acertou %d de 5 questões!\n", acertos);
}

// ==================== Avaliação ======================
void avaliacao() {
    int resposta, acertos = 0;
    char enter;
    setlocale(LC_ALL, "pt_BR");
    printf("\nAvaliação de Operadores Lógicos\n");
    printf("Pergunta 1: (4 < 3) || (2 == 2) resulta em:\n1) 0\n2) 1\n3) 2\nResposta: ");
    scanf("%d", &resposta); if(resposta==2) acertos++;
    while((enter = getchar()) != '\n' && enter != EOF);

    printf("\nAvaliação de Operadores Matemáticos\n");
    printf("Pergunta 2: O valor de (8 / 2) + 1 é:\n1) 3\n2) 4\n3) 5\nResposta: ");
    scanf("%d", &resposta); if(resposta==3) acertos++;
    while((enter = getchar()) != '\n' && enter != EOF);

    printf("\nAvaliação de Alocação de Variável\n");
    printf("Pergunta 3: Qual declaração está correta para um inteiro chamado 'x'?\n1) int x;\n2) inteiro x;\n3) var x;\nResposta: ");
    scanf("%d", &resposta); if(resposta==1) acertos++;
    while((enter = getchar()) != '\n' && enter != EOF);

    printf("\nAvaliação de if-else\n");
    printf("Pergunta 4: O que acontece se a condição do if for falsa?\n1) O bloco if executa\n2) O else executa\n3) Nada executa\nResposta: ");
    scanf("%d", &resposta); if(resposta==2) acertos++;
    while((enter = getchar()) != '\n' && enter != EOF);

    printf("\nAvaliação de Loops\n");
    printf("Pergunta 5: Quantas vezes o while abaixo executa?\nint i=0; while(i<2){i++;}\n1) 0\n2) 1\n3) 2\nResposta: ");
    scanf("%d", &resposta); if(resposta==3) acertos++;
    while((enter = getchar()) != '\n' && enter != EOF);

    printf("\nAvaliação de Ponteiro\n");
    printf("Pergunta 6: O que faz o operador & em C?\n1) Soma dois valores\n2) Retorna o endereço de memória da variável\n3) Multiplica por 2\nResposta: ");
    scanf("%d", &resposta); if(resposta==2) acertos++;
    while((enter = getchar()) != '\n' && enter != EOF);

    printf("\nAvaliação de String\n");
    printf("Pergunta 7: Como copiar uma string para outra em C?\n1) copy(dest, src);\n2) strcpy(dest, src);\n3) dest = src;\nResposta: ");
    scanf("%d", &resposta); if(resposta==2) acertos++;
    while((enter = getchar()) != '\n' && enter != EOF);

    printf("\nAvaliação de Função\n");
    printf("Pergunta 8: Como declarar uma função que retorna int e recebe float?\n1) int nome(float x);\n2) float nome(int x);\n3) void nome(float x);\nResposta: ");
    scanf("%d", &resposta); if(resposta==1) acertos++;
    while((enter = getchar()) != '\n' && enter != EOF);

    printf("\nAvaliação de Struct\n");
    printf("Pergunta 9: Como declarar uma struct chamada Carro com campo int ano?\n1) struct Carro { int ano; };\n2) Carro { int ano; };\n3) struct Carro int ano;\nResposta: ");
    scanf("%d", &resposta); if(resposta==1) acertos++;
    while((enter = getchar()) != '\n' && enter != EOF);

    printf("\nVocê acertou %d de 9 questões!\n", acertos);
}



int main(){
    MYSQL *conexao = obterconexao();
    setlocale(LC_ALL, "Portuguese_Brazil.UTF-8");
    int navegacao, acesso_login;
    acesso_login = login(conexao);
    if(acesso_login == 2){
        do{
            printf("\nbem vindo %s \n", user.nome);
            printf("1 - insirir aluno  \n");
            printf("2 - remover aluno  \n");
            printf("3 - procular aluno \n");
            printf("4 - Alterar dados dos alunos \n");
            printf("5 - sair \n");
            printf("digite seu comando: ");
            scanf("%i", &navegacao);
            while(getchar()!='\n');
            switch (navegacao)
            {
            case 1:
                inserir_aluno(conexao);
                break;
            case 2:
                remover_aluno(conexao);
                break;
            case 3:
                seach_aluno(conexao);
                break;
            case 4:
                update(conexao);
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                break;
            }
        }while (navegacao != 5);
    }
    else if(acesso_login == 1){
        do {
            printf("\nbem vindo %s \n", user.nome);
            printf("1  = informaçao do usuario \n");
            printf("2 - rank \n");
            printf("3 - Exercicios \n");
            printf("5 - Avaliação \n");
            printf("6 - sair \n");
            printf("digite seu comando: ");
            scanf("%i", &navegacao);
            while(getchar()!='\n');
            switch(navegacao)
            {
            case 1:
                printf("Nome: %s\n", user.nome);
                printf("RA: %d\n", user.ra);
                break;
            case 2:
                printf("Ranking ainda não implementado.\n");
                break;
            case 3:
                exercicios();
                break;
            case 5:
                avaliacao();
                break;
            case 6:
                printf("Saindo...\n");
                break;
            default:
                printf("Comando inválido.\n");
                break;
            }
        } while (navegacao != 6);
    }
    else{
        printf("aluno não encontrado");
    }
    mysql_close(conexao);
    mysql_commit(conexao);
    system("pause");
    return 0;
}