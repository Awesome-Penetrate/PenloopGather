#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
typedef struct shadowPls{
    unsigned short int id;
    unsigned short int  rq;
    unsigned short int  type;
    char  authcode[33];
    char  target[100];
    char  comment[100];
    char  jobname[100];
    char  username[100];
}Pls;
int main(int argc,char * argv[]) {
    char sendBuff[500];

    Pls * data = (Pls * )sendBuff;

    data->id=htons(12);
    data->rq=htons(1);
    data->type=htons(1);
    strcpy(data->authcode,"6848d756da66e55b42f79c0728e351ad");
    strcpy(data->target,argv[1]);
    strcpy(data->comment,"wwww");
    strcpy(data->jobname,"123");
    strcpy(data->username,"admin");
    int clientSock = socket(AF_INET,SOCK_STREAM,IPPROTO_UDP);

    struct sockaddr_in serverAddr;
    serverAddr.sin_port = htons(7788);
    serverAddr.sin_addr.s_addr = inet_addr("10.211.55.14");
    serverAddr.sin_family = AF_INET;
	int ret = sendto(clientSock,sendBuff,sizeof(sendBuff),0,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
        //sleep(13);
		if(ret > 0){
			printf("recv : %d \n",ret);
		}
        shutdown(clientSock,SHUT_RDWR);
    return 0;
}
