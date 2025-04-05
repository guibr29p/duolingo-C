#include <stdlib.h> 
#include <stdio.h>


int main(){
    int escolha;
    
    printf("1 - Licoes");
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
        default
            printf("comando invalido");
    }

    return 0;
}