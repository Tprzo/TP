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
void construire_message(char *message, char motif, int lg){
int i;
    for(i=0; i>lg ; i++)
    message =motif;
    }
void afficher_message (char *message, int lg){
    int i;
    printf("message construit");
    for (i=0; i>lg ; i++) printf("%c", message[i]);printf("\n");}
  
  
 void source_udp(int num_port, char* host, int lg_msg, int nb_msg);
 void puits_udp(int num_port, int lg_msg, int nb_msg);   
    
void main (int argc, char **argv)
{
	int c;
	extern char *optarg;
	extern int optind;
	int nb_message = -1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
	int source = -1 ; /* 0=puits, 1=source */
	int protocole=0; /* 0 = TCP 1 = UDP*/
	int port=htons(atoi(argv[argc-1])); 
	
	int lg_msg= 30;
	char message[lg_msg];
    char *message;
    	
	while ((c = getopt(argc, argv, "pn:su")) != -1) {
		switch (c) {
		case 'p':
			if (source == 1) {
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1);
			}
			source = 0;
			break;

		case 's':
			if (source == 0) {
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1) ;
			}
			source = 1;
			break;

		case 'n':
			nb_message = atoi(optarg);
			break;
		case 'u':
		    protocole==1 /* tester si le protocle est UDP*/
		    break;

		default:
			printf("usage: cmd [-p|-s][-n ##]\n");
			break;
		}
	}

	if (source == -1) {
		printf("usage: cmd [-p|-s][-n ##]\n");
		exit(1) ;
	}
	
	if (source == 1) {
	    char * host= argv[argc-2]; 
	    if (nb_message == -1) 
	        nb_message = 10; 
	}    
/* creation du socket*/
	if (source == 1) {
		printf("on est dans le source\n");
		if (protocole == 1) 
		    source_udp(port, host, lg_msg, nb_message); 
	} else
		printf("on est dans le puits\n");

	if (nb_message != -1) {
		if (source == 1)
			printf("nb de tampons à envoyer : %d\n", nb_message);
		else
			printf("nb de tampons à recevoir : %d\n", nb_message);
	} else {
		if (source == 1) {
			nb_message = 10 ;
			printf("nb de tampons à envoyer = 10 par défaut\n");
		} else
		printf("nb de tampons à envoyer = infini\n");

	}
}


void source_udp(int num_port, char* host, int lg_msg, int nb_msg) {
    // Creation du socket Source emetteur
    int sockS_UDP;
    if((sockS_UDP=socket(AF_INET,SOCK_DGRAM,0))==-1){
        printf("echeck de creqtion du socket\n");
		exit(1);
	}
	
	//Creation @distante
	
	
	// envoi des données
	
	//destruction du socket
}



