#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>

#define TCP_PORT 8000

typedef struct {
    int ** days;
} sensor;

int main(int argc, const char * argv[]) {
    char buffer[5];


    struct sockaddr_in direccion;



    int servidor, cliente;
    ssize_t leidos, escritos;
    int continuar = 1;
    pid_t pid;

    int *shared;
    int shmid;


    shmid = shmget(IPC_PRIVATE, 5 * sizeof(sensor), IPC_CREAT | 0666);

    if (argc != 2) {
        printf("Use: %s IP_Servidor \n", argv[0]);
        exit(-1);
    }

    // Crear el socket
    servidor = socket(PF_INET, SOCK_STREAM, 0);

    // Enlace con el socket
    inet_aton(argv[1], &direccion.sin_addr);
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;

    bind(servidor, (struct sockaddr *) &direccion, sizeof(direccion));

    // Escuhar
    listen(servidor, 10);

    escritos = sizeof(direccion);

    while (continuar)
    {
        cliente = accept(servidor, (struct sockaddr *) &direccion, &escritos);

        printf("Aceptando conexiones en %s:%d \n",
               inet_ntoa(direccion.sin_addr),
               ntohs(direccion.sin_port));

        pid = fork();

        if (pid == 0) continuar = 0;

    }

    if (pid == 0) {
        close(servidor);
        int reading_count = 0;
        int days_count = 0;
        shared = shmat(shmid, (void *) 0, 0);
        s
        if(cliente >= 0) {
            short ** days = (short **) malloc(30 * sizeof(int ));

            short ** aux = days;
            short ** end = (days + 30);
            for (; aux < end; aux++ ){
                aux = (short *) malloc(10 * sizeof(int));
            }

            while (leidos = read(cliente, &buffer, sizeof(buffer))){
                if (reading_count == 5) {
                    days_count ++;
                    reading_count = 0;
                    printf("Me movi de dia %d\n", days_count);
                }
                printf("leidos %zd\n", leidos);
                printf("read: %s\n", buffer);
                reading_count ++;

                *(*(days + days_count) + reading_count) = (short) buffer;
            }


        }




        exit(0);
    } else {
        sensor * sensors = malloc(5 * sizeof(sensor));

    }


    return 0;
}
