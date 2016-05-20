/* 
* The MIT License (MIT)
* Copyright (c) 2014 Adib HANNACHI
* adib.hannachi.tn@gmail.com 
*           ------  Server  ------
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
pthread_mutex_t mut;
int C0=20;
#define MAX_PORT 5
#define PORT_ARRAY_SIZE (MAX_PORT+1)
#define MAX_MSG 80
#define MSG_ARRAY_SIZE (MAX_MSG+1)
#define str(x) # x
#define xstr(x) str(x)
int main()
{	
	FILE* f;	
	int listenSocket, status, j;
	unsigned short int msgLength;
	struct addrinfo hints, *servinfo;
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLength = sizeof clientAddress;
	char msg[MSG_ARRAY_SIZE],msgg[MSG_ARRAY_SIZE],id[MSG_ARRAY_SIZE], listenPort[PORT_ARRAY_SIZE];
f=fopen("histo.txt", "w");
close(f);
                        
	memset(listenPort, 0, sizeof listenPort); // Mise à zéro du tampon
	pthread_mutex_init (&mut,NULL);
	puts("the port number used by Server (on listen ) : ");
	scanf("%"xstr(MAX_PORT)"s", listenPort);
	memset(&hints, 0, sizeof hints);
	
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_DGRAM; // UDP
	hints.ai_flags = AI_PASSIVE; // Toutes les adresses disponibles

	if ((status = getaddrinfo(NULL, listenPort, &hints, &servinfo)) != 0) 
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}
	if ((listenSocket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) 
	{
		perror("socket:");
		exit(EXIT_FAILURE);
	}
	if (bind(listenSocket, servinfo->ai_addr, servinfo->ai_addrlen) == -1) 
	{
		close(listenSocket);
		perror("bind:");
		exit(EXIT_FAILURE);
	}
	/*
	* Libération de la mémoire occupée par les enregistrements
	*/
	freeaddrinfo(servinfo);
	printf("Waiting query on port :%s\n", listenPort);
	/* Mise à zéro du tampon de façon à connaître le délimiteur de fin de chaîne.
	*/	

	while (1) 
	{
		// Mise à zéro du tampon de façon à connaître le délimiteur de fin de chaîne.
		memset(id, 0, sizeof id);
		if (recvfrom(listenSocket, id, sizeof id, 0, (struct sockaddr *) &clientAddress,&clientAddressLength) == -1) 
		{
			perror("recvfrom:");
			close(listenSocket);
			exit(EXIT_FAILURE);
		}
		msgLength = strlen(id);
		memset(msg, 0, sizeof msg);
		if (recvfrom(listenSocket, msg, sizeof msg, 0, (struct sockaddr *) &clientAddress,&clientAddressLength) == -1) 
		{
			perror("recvfrom:");
			close(listenSocket);
			exit(EXIT_FAILURE);
		}
		msgLength = strlen(msg);
		if (msgLength > 0) 
		{
			// Affichage de l'adresse IP du client.
			printf(">> From: %s", inet_ntoa(clientAddress.sin_addr));
			// Affichage du numéro de port du client.
			printf("\t Client ID :  %s\n", id);
			// Affichage de la ligne reçue
			printf(" Number of registrations requested: %s\n", msg);
			// Conversion de cette ligne en majuscules.
			pthread_mutex_lock(&mut);			
				f=fopen("history.txt", "a+");
                                j=atoi(msg);
				if (j<C0)
				{
					C0=C0-j;
					strcpy(msgg,"Inscris");
					fprintf(f,"%s","Client\t");
					fprintf(f,"%s \t",id);
					fprintf(f,"%d",j);
					fprintf(f,"%s","\t Inscris\n"); 						
				}
				else
				{
					strcpy(msgg,"NO Inscris");
					fprintf(f,"%s","Client\t");
			                fprintf(f,"%s \t",id);
					fprintf(f,"%d",j);
					fprintf(f,"%s","\t NO Inscris\n");
				}
				fclose(f);
			pthread_mutex_unlock(&mut);
			// Renvoi de la ligne convertie au client.
			 msgLength=strlen(msgg);
			if (sendto(listenSocket, msgg, msgLength, 0,(struct sockaddr *)&clientAddress,clientAddressLength) == -1) 
			{
				perror("sendto:");
				close(listenSocket);
				exit(EXIT_FAILURE);
			}
		}
	}
}

