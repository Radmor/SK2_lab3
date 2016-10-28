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


struct Thread_data_t
{
    int socket;
};

void* listen_loop(void *t_data) {

    int rcvd;
    char buffer[1204];

    struct Thread_data_t data=*(struct Thread_data_t *)t_data;

    int sck=data.socket;
    //int id=data.id;

    while (rcvd = recv(sck, buffer, 1024, 0)) {
        //send(sck, buffer, rcvd, 0);

        printf("%s",buffer);
        //spread_message(buffer,id);

    }

    close(sck);

    pthread_exit(NULL);

}

int main()
{
    int s_desc = socket(PF_INET, SOCK_STREAM, 0);
    if(s_desc == -1)
        perror("Deskryptor");

    //struct hostent *host;
    //host = gethostbyname("google.com");

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(1235);
    //printf("%s\n", host->h_name);
    //printf("%s\n", host->h_addr_list[0]);
    //printf("%s\n", host->h_addr);
    //address.sin_addr.s_addr = inet_addr(host->h_addr_list[0]);
    //inet_aton("150.254.32.130", &address.sin_addr);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(s_desc, (struct sockaddr*)&address, sizeof(struct sockaddr)) == -1)
        perror("Polaczenie");

    char buf[256];
    int bytes = read(s_desc, buf, 256);
    write(1, buf, bytes);

    char* output;

    char buffer[100];


    struct Thread_data_t data;
    data.socket=s_desc;

    pthread_t id=0;

    pthread_create(&id, NULL, listen_loop, (void *)&data);

    while(1){
        scanf("%s",buf);
        int n= sprintf(buffer, "%s\n",buf);
        write(s_desc, buffer, n);
    }

    close(s_desc);

    return 0;
}
