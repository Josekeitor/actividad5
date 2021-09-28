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
    short ** days;
} sensor;

int main(int argc, const char * argv[]) {
    short buffer;


    struct sockaddr_in direccion;



    int servidor, cliente;
    ssize_t leidos, escritos;
    int continuar = 1;
    pid_t pid;
    int sensor_count = 0;
    sensor *shared;
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
        sensor_count ++;
        close(servidor);
        int reading_count = 0;
        int days_count = 0;
        shared = shmat(shmid, (void *) 0, 0);
        printf("Dias %d, lecturas %d \n", days_count, reading_count);
        if(cliente >= 0) {
            short ** days = (short **) malloc(30 * sizeof(int ));

            short ** aux = days;
            short ** end = (days + 30);
            for (; aux < end; aux++ ){
                aux = (short *) malloc(10 * sizeof(int));
            }
            int read_bytes;
            while ((read_bytes = read(cliente, &buffer, sizeof(buffer))) > 0){
                if(read_bytes == sizeof(buffer)) {
                    if (reading_count == 5) {
                        days_count ++;
                        reading_count = 0;
                        printf("Me movi de dia %d\n", days_count);
                    }
                    printf("recibi: %d en el servidor\n", buffer);
                    printf("Read %d", read_bytes);
                    reading_count ++;

                    *(*(days + days_count) + reading_count) = buffer;

                    *(shared + sensor_count)->days = days;
                }

            }

            aux = days;
            end = (days + 30);
            for (; aux < end; aux++ ){
                short * aux_in = *aux;
                short * end_in = aux_in + 10;
                for(; aux_in < end_in; aux_in++){
                    free(aux_in);
                }
                free(aux);
            }
        }
        exit(0);
    } else {
        sensor * sensors = malloc(5 * sizeof(sensor));

    }


    return 0;
}
