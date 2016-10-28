//
// Created by piotrek on 27.10.16.
//

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>

#define max_clients_amount 3

int current_clients_amount=0;

int sockets[max_clients_amount];



void spread_message(char* message, int sender_id){
    int i;

    printf("%s",message);

    for(i=0;i<current_clients_amount;i++){


        if(i!=sender_id){
            char buffer[100];
            int n= sprintf(buffer, "%s\n",message);
            write(i, buffer, n);
        }



    }
}

struct Thread_data_t
{
    int socket;
    int id;
};

void* client_loop(void *t_data) {



    int rcvd;
    char buffer[1204];

    struct Thread_data_t data=*(struct Thread_data_t *)t_data;

    int sck=data.socket;
    int id=data.id;

    while (rcvd = recv(sck, buffer, 1024, 0)) {
        send(sck, buffer, rcvd, 0);

        //printf("%s",buffer);
        spread_message(buffer,id);

    }

    close(sck);

    pthread_exit(NULL);

}
int QUEUE_SIZE=5;
int SERVER_PORT= 1235;


int main(){

    int is_running=1;

    pthread_t id;
    id=0;

    struct sockaddr_in address,client_address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(SERVER_PORT);


    int my_socket = socket(AF_INET,SOCK_STREAM,0);
    bind(my_socket, (struct sockaddr*)&address, sizeof(struct sockaddr));

    int foo=1;

    setsockopt(my_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&foo, sizeof(foo));



    listen(my_socket, QUEUE_SIZE);

    while(is_running) {

        int nTmp = sizeof(struct sockaddr);
        int client_socket = accept(my_socket, (struct sockaddr*)&client_address, &nTmp);

        printf("POLACZENIE\n");



        char buffer[100];




        int n= sprintf(buffer, "%s %d\n","Uzytkownik:",id);


        write(client_socket, buffer, n);

        struct Thread_data_t data;
        data.socket=client_socket;
        data.id=id;


        sockets[id]=client_socket;

        pthread_create(&id, NULL, client_loop, (void *)&data);



        id++;
        current_clients_amount++;

    }



}


