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
    for(i=0; i<lg ; i++)
    message[i] =motif;
    }
void afficher_message (char *message, int lg){
    int i;
    printf("message construit");
    for (i=0; i<lg ; i++) printf("%c", message[i]);printf("\n");}
  
  
 void source_udp(int num_port, char* host, int lg_msg, int nb_msg);
 void puits_udp(int num_port, int lg_msg, int nb_msg);   
    /* M
       m    A    
       m    a     I   
       m    a     i    N   
       m    a     i    n   */
int main (int argc, char **argv)
{
	int c;
	extern char *optarg;
	extern int optind;
	int nb_message = -1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
	int source = -1 ; /* 0=puits, 1=source */
	int protocole=0; /* 0 = TCP 1 = UDP*/
	int port=htons(atoi(argv[argc-1])); 
	char host [100];    
	int lg_msg= 30;
	//	char *message;
	char message[lg_msg];
    
    	
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
		    protocole =1 ;/* tester si le protocle est UDP*/
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
	    strcpy(host,argv[argc-2]); 
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
    //structures utilisées

    
    

void source_udp(int num_port, char* host, int lg_msg, int nb_msg) {
    // Creation du socket Source emetteur
    
    int sockS_UDP;
    	printf("--- %d \n",lg_msg);
    if((sockS_UDP=socket(AF_INET,SOCK_DGRAM,0))==-1){
        printf("echeck de creation du socket\n");
		exit(1);
	}
	
	//Creation @distante
    struct hostent *hp;
    struct sockaddr_in adr_distant;
    memset((char*)& adr_distant,0,sizeof(adr_distant));
    adr_distant.sin_family=AF_INET;
    adr_distant.sin_port=num_port;
    if ((hp = gethostbyname(host))==NULL){
    printf("erreur gethostbyname\n");
    exit(1);
    }
    memcpy((char*)&(adr_distant.sin_addr.s_addr),
    hp->h_addr,
    hp->h_length);
	// envoi des données
    //lg_msg= 30;
	char message[lg_msg];
	struct sockaddr *padr_dest;
	//message = malloc(lg_msg*sizeof(char));
	construire_message(message,'a', lg_msg);
	printf("--- \n");
	afficher_message (message,lg_msg);
	if (sendto(sockS_UDP,message,lg_msg,0,(struct sockaddr*)&adr_distant,sizeof( struct sockaddr))==-1)
	{
	    perror("sendto");
	    exit(-1);
	}
	//destruction du socket
	close(sockS_UDP);
}


void udp_puits(int num_port, int nb_msg, char* host, int lg_msg,char* message){
  int sockP_UDP;
  struct sockaddr_in adr_local;//Adresse du socket local
  struct sockaddr_in adr_distant; //Adresse du socket distant
  int lg_adr_local = sizeof(adr_local);
  int lg_adr_distant = sizeof(adr_distant);
  int taille_max_msg = 30;
  /* Création d'un socket local*/
  if ((sockP_UDP = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
	       	printf("Echec de la creation du socket puits UDP");
	       	exit(1);
	}
	memset((char*)&adr_local, 0, sizeof(adr_local));
	adr_local.sin_family = AF_INET; //domaine internet
	adr_local.sin_port = htons(num_port); //num de port
	adr_local.sin_addr.s_addr = INADDR_ANY; //toutes les adresses IP de la machine
	//Affectation de l'adresse au socket local à la représentation interne
	if (bind(sockP_UDP, (struct sockaddr*) &adr_local, lg_adr_local) == -1)
	{
		printf("Echec du bind.\n");
		exit(1);
	}
	if (nb_msg == -1){
			nb_msg = 10;
            printf("PUITS : lg_mess_lu=%d, port=%d, nb_receptions=infini, TP=udp\n",lg_msg,num_port);
        } else printf("PUITS : lg_mess_lu=%d, port=%d, nb_receptions=%d, TP=udp\n",lg_msg,num_port,nb_msg);

	while(1){
		/*Reception des messages et affichage*/
		for (int i=1; i<=nb_msg; i++)
		{
			if (recvfrom(sockP_UDP, message, taille_max_msg, 0, (struct sockaddr*) &adr_distant, (socklen_t *) &lg_adr_distant) == -1)
			{
			  	printf("Erreur lors de la reception du message (recvfrom).\n");
				exit(1);
			}
			else
			{
				/*Affichage du message recu*/
				afficher_message(message, lg_msg);
			}
		}
	}
/*Fermeture du socket et libération de la mémoire*/
	if (close(sockP_UDP) == -1)	
	{
		printf("Echec de destruction du socket");
		exit(1);
	}
}
