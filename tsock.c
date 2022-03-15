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
// pour faire appel au fonction de bal 
#include "bal.h"




int main(int argc, char **argv)
{
	int c;
	extern char *optarg;
	extern int optind;
	int nb_message = -1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
	int source = -1;	 /* 0=puits, 1=source */
	int protocole = 0;	 /* 0 = TCP 1 = UDP*/
	int port = htons(atoi(argv[argc-1]));
	char* host; 
	int lg_msg = 30;
	int bal=-1;
	int nb_lettres=3;
	int nBAL=-1;
	

	
	struct message
	{
		char *message;
		struct message *message_suivante;
	};
	struct bal
	{
		int num_bal;
		struct bal *bal_suivante;
		struct message *lettre;
	};
	struct liste
	{
		struct bal *first;
		struct bal *last;
		struct bal *courant;
	};

	while ((c = getopt(argc, argv, "pn:sul:be:r")) != -1)
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
			source = 1;
			break;
		case 'b' :
            bal=1;
            break;
		case 'e' :
			bal=1;
            nBAL = atoi(optarg);
            break;
		case 'n':
			nb_message = atoi(optarg);
			break;
		case 'r' :
            bal=1;
            nBAL = atoi(optarg);
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
			break;
		}
	}

	if (source == -1)
	{
		printf("usage: cmd [-p|-s][-n ##]\n");
		exit(1);
	}

	if (source == 1)
	{
	    //modif : initialisation de la variable host
		host = argv[argc-2];
		if (nb_message == -1)
			nb_message = 10;
	}
	/* creation du socket*/
	if (source == 1)
	{
		printf("on est dans la source\n");
		if (protocole == 0)
			source_tcp (port,nb_message,lg_msg,host);
		else
			source_udp(port,nb_message,lg_msg,host);
	}
	

	if (source == 0) {
		printf("on est dans le puits\n");
	    if (protocole == 0)
	    {
	     
		    puits_tcp(port,nb_message,lg_msg);
	    }
	    else
	    {
		    puits_udp(port, nb_message, lg_msg);
	    }
	}
}

