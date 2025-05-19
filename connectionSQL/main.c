#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include "mysql\include\mysql.h"
struct user_now{
    int ra;
    char nome[250];
    char email[250];
};

struct user_now user;

int verificacao_acesso(MYSQL *conexao, char *senha);

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
        printf("presionne enter caso for sua primeira fez acessando \ndigite sua senha: ");
        fgets(senha, 100, stdin);
        senha[strcspn(senha, "\n")] = "\0";
        sprintf(query, "SELECT nome, senha, email FROM aluno WHERE ra = %i and senha = '%s'", RA, senha);
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
       verificacao_acesso(conexao, row[1]);
       sprintf(user.nome, row[0]);
       user.ra = row[1];
       sprintf(user.email, row[2])
       return type_login;
    } 
    mysql_free_result(resultado);
}

void inserir_aluno(MYSQL *conexao){ // adicionar alunos na tabela
    char nome[250], email[250],senha[100], resp, query[750];
    int telefone, cont_loop;
    fflush(stdin);
    printf("quantos alunos gostaria de inserir?: ");
    sprintf("%d", &cont_loop);
    for(int i = 1; i <= cont_loop; i++){
        printf("insira o nome do aluno: ");
        scanf("%s", &nome);
        printf("insira o email do aluno: ");
        scanf("%s", &email);
        printf("insira o telefone do aluno");
        scanf("%i", &telefone);
        sprintf(query, "INSERT INTO aluno(nome, email, telefone) VALUES ('%s', %s, %i)", nome, email, telefone);
        if(mysql_query(conexao, query)){
            erro(conexao);
        }
        else{
            printf("aluno cadastrado");
        }
    }
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
        "telefone",
        "turma",
    };
    do{
        printf("1 - %s \n", menu[0]);
        printf("2 - %s \n", menu[1]);
        printf("2 - %s \n", menu[2]);
        printf("3 - %s \n", menu[3]);
        printf("Digite o comando: ");
        scanf("%d", &escolha);
        if(escolha > 3){
            system("clear");
            printf("comando invalido tente novamente");
        }
    }while(escolha > 3);
    printf("Digite o Ra do aluno: ");
    sprintf(query, "select ra, nome from aluno where ra = %i", ra);
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
    scanf("%c", &conf_alt);
    mysql_free_result(res);
    switch(escolha){
    case 1:
        printf("digite o novo nome: ");
        fgets(new_char, 250, stdin);
        new_char[strcspn(new_char, "\n")] = '\0';
        break;
    case 2:
        printf("digite o novo email: ");
        scanf("%s", &new_char);
        break;
    case 3:
        printf("digite o novo telefone do aluno: ");
        scanf("%d", &new_int);
        break;
        scanf("%d", &ra);
    case 4:
        printf("digite a nova turma do aluno: ");
            scanf("%d", &new_int);
            break;
    default:
        system("clear");
        printf("comando invalidon tente novamente");
    }
    if((escolha == 0 || escolha == 1) && (conf_alt = 's')){
        sprintf(query, "Update aluno set %s = '%s' where ra = %d", menu[(escolha - 1)], new_char, ra);
    }
    else if((escolha == 2 || escolha == 3) && (conf_alt = 's')){
        sprintf(query, "Update aluno set %s = %s where ra = %d", menu[(escolha - 1)], new_char, ra);
    }
    if(mysql_query(conexao, query)){
        erro(conexao);
    }
    else{
        printf("dados alterados");
        return 1;
    }

}
void rank_aluno(MYSQL conexao){
    
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
        sprintf(query, "SELECT ra, nome, email, telefone, idturma from aluno WHERE nome LIKE '%%%s%%'", nome);
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
        printf("Ra        |Nome                         |telefone                       |email                     |idturma\n");
    }
    while((row = mysql_fetch_row(res)) != NULL){
        printf("%s   |%s     |%s    |%s  \n", row[0], row[1], row[2], row[3]);  
    }
    mysql_free_result(res);
}

int verificacao_acesso(MYSQL *conexao, char *senha){ // verificar se é o primeiro acesso
    if(senha == ""){
        char senha[100];
        printf("seja bem vindo %s", user.nome);
        printf("vejo que seu primeiro acesso");
        printf("Poderia insira sua senha:  ");
        scanf("%s", &senha);
        return 0;
    }
}

int main(){
    MYSQL *conexao = obterconexao();
    int navegacao, acesso_login;
    acesso_login = login(conexao);
    if(acesso_login == 2){
        do{
            printf("1 - insirir aluno  \n");
            printf("2 - remover aluno  \n");
            printf("3 - procular aluno \n");
            printf("4 - Alterar dados dos alunos \n");
            printf("5 - sair \n");
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
            case 4:
                update(conexao);
                break;
            default:
                break;
            }
        }while (navegacao != 5);
    }
    else if(acesso_login == 1){
        printf("\nbem vindo %s \n", user.nome);
        printf("1  = informaçao do usuario \n");
        printf("2 - rank\n");
        printf("3 - Exercicios\n");
        printf("5 - Avaliação \n");
        switch(navegacao)
        {
        case 1:
            
            break;
        case 2:
        
            break;
        case 3:
            
            break:
        default:
        
            break;
        }
    }
    mysql_close(conexao);
    mysql_commit(conexao);
    system("pause");
    return 0;
}