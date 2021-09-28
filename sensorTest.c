//
// Created by Jose Carlos Acosta on 26/09/21.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define MAX 20
#define TCP_PORT 8000

void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(unsigned long long int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

int getRandomNum(int max)
{
    srand(time(0));
    return rand() % max;
}

int main(int argc, const char * argv[]) {

    struct sockaddr_in direccion;
    char buffer[5];

    int cliente;

    int escritos;

    /* Leer la IP del servidor */
    if (argc != 2) {
        printf("Error. Use: %s A.B.C.D \n", argv[0]);
        exit(-1);
    }

    /* Crear el socket */
    cliente = socket(PF_INET, SOCK_STREAM, 0);

    /* Establecer los campos de la struct */
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;
    inet_aton(argv[1], &direccion.sin_addr);
    printf("dirección: %s \n", argv[1]);
    /* Establecer la conexión con el servidor */
    int estado = connect(cliente, (struct sockaddr *) &direccion, sizeof(direccion));
    printf("estado %d \n", estado);
    if(estado == -1) {
        printf("Error en la conexión\n");
    }

    printf("Conexión establecida al servidor central\n");
    while (1) {
        short reading = (short) (rand() % 256);
        printf("Enviando %d al servidor central\n", reading);
        // Escribir datos en el socket
        write(cliente, &buffer, sizeof(buffer));
        sleep(1);
    }

    close(cliente);


    return 0;
}

