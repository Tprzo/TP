/* librairie standard ... */
#include <stdlib.h>
/* pour getopt */
#include <unistd.h>
/* déclaration des types de base */
#include <sys/types.h>
/* constantes relatives aux domaines, types et protocoles */
#include <sys/socket.h>
/* constantes et structures propres au domaine UNIX */
#include <sys/un.h>
/* constantes et structures propres au domaine INTERNET */
#include <netinet/in.h>
/* structures retournées par les fonctions de gestion de la base de
données du réseau */
#include <netdb.h>
/* pour les entrées/sorties */
#include <stdio.h>
/* pour la gestion des erreurs */
#include <errno.h>
// pour faire appel au fonction du tsock
#include "tsock.h"


int main(int argc, char **argv)
{
	int c;
	extern char *optarg;
	extern int optind;
	int nb_message =-1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
	int source = -1;	 /* 0=puits, 1=source */
	int protocole = 0;	 /* 0 = TCP | 1 = UDP*/
	int port = -1;
	char *host;
	int lg_msg = 30;
	port = htons(port);
	struct hostent *hp;

	while ((c = getopt(argc, argv, "pn:sul:")) != -1)
	{
		switch (c)
		{
		case 'p':
			if (source == 1)
			{
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1);
			}
			source = 0;
			break;
		case 's':
			if (source == 0)
			{
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1);
			}
			host = argv[argc - 2];
			source = 1;
			break;
		case 'n':
			nb_message = atoi(optarg);

			break;

		
		case 'l':
			lg_msg = atoi(optarg);
			printf("lg_mesg = %d\n", lg_msg);
			break;
		case 'u':
			protocole = 1;
			break;
		default:
			printf("usage: cmd [-p|-s][-n ##]\n");
			exit(1);
			break;
		}
	}

	if ((port = atoi(argv[argc - 1])) != -1)
	{
		port = htons(port);
	}

	host = argv[argc - 2];

	if (source == -1)
	{
		printf("usage: cmd [-p|-s][-n ##]\n");
		exit(1);
	}

	
		if (source == 1)
			printf("SOURCE : ");
		else
			printf("PUITS : ");

		if (protocole == 1)
			printf("Protocole de transport : UDP | ");

		else
			printf("Protocole de transport : TCP | ");

		if (nb_message == -1) 
		{
			if (source == 1)
			{
				nb_message = 10;
				printf("Nombre d'envois : %d |", nb_message);
			}
			else{
			

				printf("Nombre de receptions : infini |");
		}}

		else
		{
			if (source == 1)
				printf("Nombre d'envois = %d |", nb_message);
			else
				printf("nb de tampons à recevoir = %d |", nb_message);
		}
		printf("Host : %s\n", host);
	
		if (source == 1 && protocole == 1)
			source_udp(port, nb_message, lg_msg, host);
		if (source == 0 & protocole == 1)
			puits_udp(port, nb_message, lg_msg);
		if (source == 0 & protocole == 0)
			puits_tcp(port, nb_message, lg_msg);
		if (source == 1 & protocole == 0)
			source_tcp(port, nb_message, lg_msg, host);
	
	
}
