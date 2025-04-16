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
    }
    else{
        return conexao;
    }
}

int login(MYSQL *conexao, char *registrer, char *senha){
    MYSQL_RES *resultado;
    MYSQL_ROW row;
    int encontrado = 0;
    char *query;
    sprintf(query, "SELECT * FROM usuarios WHERE (nome = '%s' or email = '%s') and senha = '%s'", registrer, registrer, senha);
    if(mysql_query(conexao, query)){
        erro(conexao);
    }
    resultado = mysql_store_result(conexao);
    while ((row = mysql_fetch_row(resultado)) != NULL)
    {
       printf("\nbem vindo %s \n", row[1]);
       return encontrado = 1;
    } 
    if (!encontrado) {
        printf("Usuário ou senha inválidos.\n");
    } 
    mysql_free_result(resultado);
}
int main(){
    MYSQL *conexao = obterconexao();
    char nome[100], senha[100];
    printf("digite seu nome ou emai: ");
    scanf("%s", &nome);
    printf("digite sua senha: ");
    scanf("%s", &senha);
    
    if(login(conexao, nome, senha) == 1){
        printf("menu ");
    }
    return 0;
}



void inserir(MYSQL *conexao){
    char query[100];
    
}