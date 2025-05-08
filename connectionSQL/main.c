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
    int encontrado = 0, RA;
    char query[750], senha[100];
    printf("digite seu RA: ");
    scanf("%i", &RA);
    printf("digite sua senha: ");
    scanf("%s", &senha);
    sprintf(query, "SELECT * FROM aluno WHERE ra = %i and senha = '%s'", RA, senha);
    if(mysql_query(conexao, query)){
        erro(conexao);
    }
    resultado = mysql_store_result(conexao);
    while((row = mysql_fetch_row(resultado)) != NULL)
    {
       printf("\nbem vindo %s \n", row[1]);
       verificacao_acesso(row[2], row[4]);
       return encontrado = 1;
    } 
    if (!encontrado){
        printf("RA ou senha invalido.\n");
        return encontrado = 0;
    } 
    mysql_free_result(resultado);
}

void inserir(MYSQL *conexao){ // adicionar alunos na tabela
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

int remover(MYSQL *conexao){ // remove aluno
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

void seach_aluno(MYSQL *conexao){ // procura na tabela aluno
    MYSQL_RES *res;
    MYSQL_ROW *row;
    int resp, ra, turma;
    char query[750];
    printf("1 - turma \n");
    printf("2 - aluno \n");
    printf("digite seu comando: ");
    scanf("%i", &resp);
    switch (resp){
    case 1:
        printf("digite a turma que deseja procurar: ");
        scanf("%i", &turma);
        sprintf(query, "SELECT ra, nome, email, idturma from aluno WHERE idturma = %i", turma);
        break;
    case 2:
        printf("digite o RA do aluno: ");
        scanf("%i", &ra);
        sprintf(query, "SELECT ra, nome, email, idturma from aluno WHERE ra = %i", ra);
        break;
    default:
        printf("comando invalido");
        break;
    }
    if(mysql_query(conexao, query)){
        erro(conexao);
    }
    res = mysql_use_result(conexao);
    while((row = mysql_fetch_row(res)) != NULL){
        printf("Ra:      %s \n", row[0]);
        printf("nome:    %s \n", row[1]);
        printf("email:   %s \n", row[2]);
        printf("idturma: %s \n", row[3]);
    }
    mysql_free_result(res);
}

void verificacao_acesso(char *email, char *senha){ // verificar se é o primeiro acesso
    if(email == NULL || senha == ""){
        
    }
}

int main(){
    MYSQL *conexao = obterconexao();
    int navegacao;
    
    if(login(conexao) == 1){
        printf("1 - insirir aluno \n");
        printf("2 - remover aluno \n");
        printf("3 - procular aluno \n");
        printf("digite seu comando: ");
        scanf("%i", &navegacao);
        switch (navegacao)
        {
        case 1:
            inserir(conexao);
            break;
        case 2:
            remover(conexao);
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