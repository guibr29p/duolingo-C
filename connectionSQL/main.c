#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include "mysql\include\mysql.h"

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
    int encontrado = 0, RA, type_login;
    char query[750], senha[100];
    printf("1 - aluno \n");
    printf("2 - proferssor \n");
    printf("logar como: ");
    scanf("%d", &type_login);
    switch (type_login)
    {
    case 1:
        printf("digite seu RA: ");
        scanf("%i", &RA);
        printf("digite sua senha: ");
        scanf("%s", &senha);
        sprintf(query, "SELECT nome, senha FROM aluno WHERE ra = %i and senha = '%s'", RA, senha);
        break;
    case 2:
        printf("digite seu id: ");
        scanf("%i", &RA);
        printf("digite sua senha: ");
        scanf("%s", &senha);
        sprintf(query, "SELECT nome, senha FROM professor WHERE ra = %i and senha = '%s'", RA, senha);
        break;
    default:
        break;
    }
    fflush(stdin);
    if(mysql_query(conexao, query)){
        erro(conexao);
    }
    resultado = mysql_store_result(conexao);
    while((row = mysql_fetch_row(resultado)) != NULL)
    {
       printf("\nbem vindo %s \n", row[0]);
       verificacao_acesso(row[0], row[1]);
       return encontrado = 1;
    } 
    mysql_free_result(resultado);
}

void inserir_aluno(MYSQL *conexao){ // adicionar alunos na tabela
    char nome[250], senha[100], resp, query[750];
    printf("insira o nome do aluno: ");
    scanf("%s", &nome);
    sprintf(query, "INSERT INTO aluno(nome) VALUES ('%s')", nome);
    if(mysql_query(conexao, query)){
        erro(conexao);
    }
    else{
        printf("aluno cadastrado");
    }
}

int remover_aluno(MYSQL *conexao){ // remove aluno
    MYSQL_RES *res;
    MYSQL_ROW *row;
    int ra;
    char query[250], resp, *r;
    r = &query;
    printf("digite o RA do aluno: ");
    scanf("%i", &ra);
    sprintf(r, "SELECT ra, nome FROM aluno WHERE ra = %i", ra);
    if(mysql_query(conexao, query)){
        erro(conexao);
    }
    res = mysql_store_result(conexao);
    while((row = mysql_fetch_row(res)) != NULL){
        printf("RA:   %s \n", row[0]);
        printf("nome: %s \n", row[1]);
    }
    mysql_free_result(res);
    sprintf(r, "DELETE FROM aluno WHERE ra = %i", ra);
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
    return 0;
}
int update(conexao){
    int escolha, ra;
    char cont, new_name[250], query[750], menu[4][10] = {
        "nome", 
        "email",
        "telefone",
        "turma",
    };
    
    printf("1 - %s \n", menu[0]);
    printf("2 - %s \n", menu[1]);
    printf("2 - %s \n", menu[2]);
    printf("3 - %s \n", menu[3]);
    printf("Digite o comando: ");
    scanf("%d", &escolha);
    printf("Digite o Ra do aluno");
    scanf("%i", ra);
    printf("digite o novo nome: ");
    fgets(new_name, 250, stdin);
    sprintf(query, "Update aluno set %s = %s where ra = ", menu[escolha], new_name, ra);
    fflush(stdin);
}
int seach_aluno(MYSQL *conexao){ // procura na tabela aluno
    MYSQL_RES *res;
    MYSQL_ROW *row;
    int resp, ra, turma;
    char query[750], nome[100];
    printf("1 - Turma \n");
    printf("2 - Aluno \n");
    printf("3 - Nome  \n");
    printf("digite seu comando: ");
    scanf("%i", &resp);
    switch (resp){
    case 1:
        printf("Digite a turma que deseja procurar: ");
        scanf("%i", &turma);
        sprintf(query, "SELECT ra, nome, email, idturma from aluno WHERE idturma = %i", turma);
        break;
    case 2:
        printf("Digite o RA do aluno: ");
        scanf("%i", &ra);
        sprintf(query, "SELECT ra, nome, email, idturma from aluno WHERE ra = %i", ra);
        break;
    case 3:
        printf("Digite o nome do aluno: ");
        scanf("%s", &nome);
        sprintf(query, "SELECT ra, nome, email, idturma from aluno WHERE nome LIKE '%%%s%%'", nome);
        break;
    default:
        printf("Comando invalido");
        break;
    }
    if(mysql_query(conexao, query)){
        erro(conexao);
    }
    res = mysql_use_result(conexao);
    if((row = mysql_fetch_row(res)) != NULL){
        printf("Ra        |Nome             |email          |idturma\n");
    }
    while((row = mysql_fetch_row(res)) != NULL){
        printf("%s   |%s     |%s    |%s \n", row[0], row[1], row[2], row[3]);  
    }
    mysql_free_result(res);
}

void verificacao_acesso(char *email, char *senha){ // verificar se é o primeiro acesso
    if(email == NULL || senha == ""){
        
    }
}

int main(){
    MYSQL *conexao = obterconexao();
    int navegacao, type_login;
    if(login(conexao) == 1){
        printf("1 - insirir aluno  \n");
        printf("2 - remover aluno  \n");
        printf("3 - procular aluno \n");
        printf("digite seu comando: ");
        scanf("%i", &navegacao);
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
        default:
            break;
        }
    } 
    system("pause");
    return 0;
}