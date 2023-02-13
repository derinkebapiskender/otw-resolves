#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include<math.h>

#define DEST_IP "13.50.135.239" // ping vortex.labs.overthewire.org
#define DEST_PORT 5842

/* usage of functions
  int socket(int domain, int type, int protocol);
  int send(int sockfd, const void *msg, int len, int flags);
  int recv(int sockfd, void *buf, int len, unsigned int flags);
*/

int main(){

    char recvBuff[1024] = { 0 };
    int sockfd,n=4, valread;
    struct sockaddr_in dest_addr;

    memset(recvBuff, '0',sizeof(recvBuff));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&dest_addr, '0', sizeof(dest_addr));

    dest_addr.sin_family = AF_INET;          // konak bayt sıralaması
    dest_addr.sin_port = htons(DEST_PORT);   // short, ağ bayt sıralaması
    dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);

    int conn = connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if(conn < 0){
      printf("\n baglanti hatasi\n");
      return 1;
    }

    unsigned int incoming_data_buffer[4];
    ssize_t bytes_recieved;

    for(int i = 0; i < n; i++){
        bytes_recieved = recv(sockfd, incoming_data_buffer + i, sizeof (unsigned int), 0);// Get the next integer and put it into the buffer
        if (bytes_recieved < 0) printf("error while recv: bytes_recieved()");
    }

    printf("\n Recieved integers\n");
    unsigned int sum = 0;
    for (int i = 0; i < n; i++){
        printf(" %u\n",incoming_data_buffer[i]);
        sum += incoming_data_buffer[i];
    }
    printf(" Sum: %u\n\n",sum);

    unsigned int* msgsum = &sum; //The sum is the message to send back.
    int lenmsg = sizeof(msgsum);
    ssize_t send_back;

    send_back = send(sockfd, msgsum, lenmsg, 0);
    if(send_back <0) printf("hata: send_back()");

    char newBuff[1024];
    ssize_t new_receive;

    new_receive = recv(sockfd, newBuff, 1024, 0);
    if(send_back <0) printf("hata: new_receive()");
    printf("\n bytes_recieved: %s",newBuff);

    close(sockfd);
    printf("\n baglanti kapatildi\n finito\n");
    /*int i = 5;
    int *s=&i;
    printf("%p",&s);*/
    return 0;
}
