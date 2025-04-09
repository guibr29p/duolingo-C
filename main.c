#include <stdlib.h> 
#include <stdio.h>

int strcmp(char *str1);
int login();

int main(){
    int escolha, usuario_logado;
    login();
    /*printf("1 - Licoes");
    printf("2 - Exercicios");
    printf("3 - Avaliacao");
    printf("4 - Ranker");
    printf("digite o comando");
    scanf("%i", &escolha);
    switch(escolha){
        case 1:
            printf("licoes");
        break;
        case 2:
            printf("exerciciocs");
        break;
        default:
            printf("comando invalido");
    }*/
    return 0;
}


int login(/*char *user,int senha*/){
    char user[3][50] = {
        "joao",
        "guilherme",
        "wilian",
    };

    int senha[3] = {
        1234567
    };

    for(int i = 0; i < 3; i++){
        strcmp(user[i]);
    }
    return 0;
}

int strcmpc(char *str1 /*, char *str2*/){
    
    return 0;
}