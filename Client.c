/* 
* The MIT License (MIT)
* Copyright (c) 2014 Adib HANNACHI
* adib.hannachi.tn@gmail.com 
*           ------  Client  ------
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#define MAX_PORT 5
#define PORT_ARRAY_SIZE (MAX_PORT+1)
#define MAX_MSG 80
#define MSG_ARRAY_SIZE (MAX_MSG+1)
#define str(x) # x
#define xstr(x) str(x)
int main()
{	
	int socketDescriptor, status;
	unsigned int msgLength;
	struct addrinfo hints, *servinfo;
	struct timeval timeVal;
	fd_set readSet;
	char msg[MSG_ARRAY_SIZE],serverPort[PORT_ARRAY_SIZE];
	
	puts("The server name :");
	memset(msg, 0, sizeof msg); // Mise à zéro du tampon
	scanf("%"xstr(MAX_MSG)"s", msg); // nom du serveur

	puts("The port number ");
	memset(serverPort, 0, sizeof serverPort); // Mise à zéro du tampon
	scanf("%"xstr(MAX_PORT)"s", serverPort); // numero de port serveur
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	if ((status = getaddrinfo(msg, serverPort, &hints, &servinfo)) != 0) 
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}
	if ((socketDescriptor = socket(servinfo->ai_family, servinfo->ai_socktype,servinfo->ai_protocol)) == -1) 
	{
		perror("socket:");
		exit(EXIT_FAILURE);
	}

	puts("\n Welcome to our sports hall.");
	puts("Your ID Please ");
	memset(msg,0, sizeof msg);
	scanf(" %"xstr(MAX_MSG)"[^\n]%*c", msg);
		if ((msgLength = strlen(msg)) > 0) 
		{
			// Envoi de l'identifiant au serveur
			if (sendto(socketDescriptor, msg, msgLength, 0,servinfo->ai_addr, servinfo->ai_addrlen) == -1) 
			{
				perror("sendto:");
				close(socketDescriptor);
				exit(EXIT_FAILURE);
			}
			// Attente de la réponse pendant une seconde.
			FD_ZERO(&readSet);
			FD_SET(socketDescriptor, &readSet);
			timeVal.tv_sec = 1;
			timeVal.tv_usec = 0;
		}

	puts("Number of athletes that you want to register :");
	memset(msg, 0, sizeof msg); // Mise à zéro du tampon
	scanf(" %"xstr(MAX_MSG)"[^\n]%*c", msg);
	// Arrêt lorsque l'utilisateur saisit une ligne ne contenant que la lettre q.
	while (strcmp(msg, "q")) 
	{
		if ((msgLength = strlen(msg)) > 0) 
		{
			// Envoi de le nombre insérer au serveur.
			if (sendto(socketDescriptor, msg, msgLength, 0,servinfo->ai_addr, servinfo->ai_addrlen) == -1) 
			{
				perror("sendto:");
				close(socketDescriptor);
				exit(EXIT_FAILURE);
			}
			// Attente de la réponse pendant une seconde.
			FD_ZERO(&readSet);
			FD_SET(socketDescriptor, &readSet);
			timeVal.tv_sec = 1;
			timeVal.tv_usec = 0;
			if (select(socketDescriptor+1, &readSet, NULL, NULL, &timeVal)) 
			{
				// Lecture de la ligne modifiée par le serveur.
				memset(msg, 0, sizeof msg); // Mise à zéro du tampon
				if (recv(socketDescriptor, msg, sizeof msg, 0) == -1) 
				{
					perror("recv:");
					close(socketDescriptor);
					exit(EXIT_FAILURE);
				}
				printf("Réponse: %s\n", msg);
			}
			else 
			{
				puts("**The server did not reply in the second.");
			}
		}
		puts("\nTo exit, please insert 'q' only");
		memset(msg, 0, sizeof msg); // Mise à zéro du tampon
		scanf(" %"xstr(MAX_MSG)"[^\n]%*c", msg);
	}
	close(socketDescriptor);
	return 0;
}
