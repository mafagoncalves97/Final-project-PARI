/**
 * @file server.c
 * @author Mafalda Gonçalves (mafalda.goncalves@ua.pt)
 * @brief Servidor TCP que lida com vários clientes. Permite a aquisição e o envio dos dados da imagem capturada. Baseado no código fornecido pelo docente nas aulas.
 * @version 0.1
 * @date 2020-01-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#define _MAIN_C_

#include "myserver.h"

int main(int argc, char *argv[])
{
	int NotTime[5];
	NotTime[0]=1;NotTime[1]=0;NotTime[2]=0;NotTime[3]=0;NotTime[4]=0;

	int sockfd, clientsockfd, portno, pid;
	int ret,res;
	char clntName[INET_ADDRSTRLEN];	                  
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int c=0;
	signal(SIGCHLD, SIG_IGN);

	if(argc < 2)
	{
		fprintf(stderr, "Syntax: %s <port_num>\n", argv[0]);
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}

	//Cria um socket para a comunicação
	sockfd = socket(AF_INET, SOCK_STREAM, 0);	
	if(sockfd < 0) myerror("ERROR opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));	
	portno                    = atoi(argv[1]);	
	serv_addr.sin_family      = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port        = htons(portno);
	ret = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));	
	if(ret < 0) myerror("ERROR on binding");

	listen(sockfd, 20);           
	clilen = sizeof(cli_addr);

	int flags = fcntl(sockfd, F_GETFL, 0); fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);  
	infinite_loopG=1;
	
	while(infinite_loopG)
	{	

		clientsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if(clientsockfd < 0) 
		{
			if (errno == EAGAIN) continue;  
			myerror("ERROR on accept");
		}
		c=0;
		if(inet_ntop (AF_INET, &cli_addr.sin_addr.s_addr, clntName, sizeof(clntName)) != NULL)
			printf("Client %s connected/%d\n", clntName, ntohs(cli_addr.sin_port));
		else
			printf("Unable to get client address\n");


		pid = fork();	                         
		if(pid < 0) myerror("ERROR on fork");	

		if(pid == 0)	                         
		{

			close(sockfd);
			
			//Aquando o establecimento da ligação, toma sempre a iniciativa de enviar a imagem.
			if (c==0){
				send_image(clientsockfd,clntName,NotTime);
				c++;
			}
			//Fica sempre à espera que o cliente lhe peça uma imagem. 
			do{	
				res=receive_message(clientsockfd,clntName); 
			}while(1);
			
			exit(0);	                
		}
		else	      
			close(clientsockfd);
	}

	close(sockfd);
	return 0;	
}









