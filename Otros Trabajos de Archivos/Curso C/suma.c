#include <stdio.h>

int main()
{
    int num1;
    int num2;
    int resultado;
    printf("Ingrese el valor del primer número \n");
    scanf("%d",&num1);   //%d eso significa que va a recibir un valor entero
    printf("Ingrese el valor del segundo número\n");
    scanf("%d",&num2);
    resultado=num1+num2;
    printf("El resultado de la suma es: %d\n",resultado);

return 0;
    
}