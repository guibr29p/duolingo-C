#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

int login(char *user,int senha)

int main(){
    int escolha, user_senha;
    char user_name[50];
    
    printf("1 - Licoes \n");
    printf("2 - Exercicios \n");
    printf("3 - Avaliacao \n");
    printf("4 - Rank \n");
    printf("digite o comando: ");
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
    }
    return 0;
}


int login(char *user,int senha){
    char user_list[3][50] = {
        "joao",
        "guilherme",
        "wilian",
    };

    int senha_list[3] = {
        1234567
    };

    for(int i = 0; i < 3; i++){
        strcmp(user_list[i], user);
    }
    return 0;
}

