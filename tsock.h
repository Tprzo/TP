#ifndef _tsock_
#define _tsock_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>


/* ---------------------------Declaration des fonctions---------------------------*/

void construire_message(char *message, char motif, int lg);
void afficher_message(char *message, int lg);
void source_udp(int num_port, char *host, int lg_msg, int nb_msg);
void udp_puits(int num_port, int lg_msg, int nb_msg);
void source_tcp(int num_port, char *host, int lg_msg, int nb_msg);
void puits_tcp(int num_port, int lg_msg, int nb_msg);


/* ---------------------------Definition des fonctions---------------------------*/
 
// CONSTRUIRE MESSAGE :  Construction du message 

void construire_message(char *message, char motif, int lg)
{
	int i;
	for (i = 0; i < lg; i++)
		message[i] = motif;
}

// AFFFICHER MESSAGE :  AFFICHAGE du message 

void afficher_message(char *message, int lg)
{
	int i;
	printf("message construit");
	for (i = 0; i < lg; i++)
		printf("%c", message[i]);
	printf("\n");
}
// SOURCE TCP :  Fonction de la source en utilisant TCP 

void source_tcp(int num_port, char *host, int lg_msg, int nb_msg)
{
	// Creation du socket Source emetteur
    printf("SOURCE : lg_mess_emis=%d, port=%d, nb_envois=%d, TP=tcp, dest=%s\n", lg_msg, num_port, nb_msg, host);
	int sockS_TCP;
	char motif = 'a';
	printf("--- %d \n", lg_msg);
	if ((sockS_TCP = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("echeck de creation du socket\n");
		exit(1);
	}

	//Creation @distante
	struct hostent *hp;
	struct sockaddr_in adr_distant;
	int lg_adr_distant = sizeof(adr_distant);
	memset((char *)&adr_distant, 0, sizeof(adr_distant));
	adr_distant.sin_family = AF_INET;
	adr_distant.sin_port = num_port;
	if ((hp = gethostbyname(host)) == NULL)
	{
		printf("erreur gethostbyname\n");
		exit(1);
	}
	memcpy((char *)&(adr_distant.sin_addr.s_addr),
		   hp->h_addr,
		   hp->h_length);
	/*Demande de connexion*/
	if (connect(sockS_TCP, (struct sockaddr *)&adr_distant, lg_adr_distant) == -1)
	{
		perror("Erreur lors de la demande de la connexion (connect).\n");
		exit(1);
	}

	
	// envoi des données
	
char *message = malloc(sizeof(char) * lg_msg);
	for (int i = 1; i <= nb_msg; i++)
	{
		/*Creation du message*/
		construire_message(message, motif++, lg_msg);
		afficher_message(message, lg_msg);

		/*Envoi d'un message*/
		if ((write(sockS_TCP, message, lg_msg)) == -1)
		{
			printf("Erreur lors de l'envoi du message (write).\n");
			exit(1);
		}
	}
	/*Phase de fermeture d'une connexion*/
	if (shutdown(sockS_TCP, 2) == -1)
	{
		printf("Erreur lors de la fermeture de la connexion (shutdown).\n");
		exit(1);
	}

	/*Destruction du socket*/
	if (close(sockS_TCP) == -1)
	{
		printf("Echec de destruction du socket");
		exit(1);
	}
	else
	{
		printf("SOURCE : fin\n");
	}
}

// Puits TCP :  Fonction du puits en utilisant TDP 

void puits_tcp(int num_port, int lg_msg, int nb_msg)
{
			char *message = malloc(sizeof(char) * lg_msg);
			int sock; //Représentation interne du socket local
			int sock_bis; //Représention interne socket local dédié échange 
			struct sockaddr_in adr_distant; //Adresse du socket distant
			struct sockaddr_in adr_local; //Adresse du socket local
			int lg_adr_distant = sizeof(adr_distant);
			int lg_adr_local = sizeof(adr_local);
			int lg_msg_recu = -1; 
			int taille_max_msg = 30;

			/*Phase d'etablissement de connexion*/

			/*Création d'un socket local*/
				if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
				{
						printf("Echec de la creation du socket puits TCP\n");
						exit(1);
				}

			/*Construction de l'adresse de socket local*/
			//Affectation domaine et numero de port
			memset((char*)&adr_local, 0, sizeof(adr_local));
			adr_local.sin_family = AF_INET; //domaine internet
			//modif : j'ai enlevé le htons car c'est déjà fait ds le main
			adr_local.sin_port = num_port; //num de port
			adr_local.sin_addr.s_addr = INADDR_ANY; //toutes les adresses IP de la machine

			//Affectation de l'adresse au socket local à la représentation interne
			if (bind(sock, (struct sockaddr*) &adr_local, lg_adr_local) == -1)
			{
				printf("Echec du bind.\n");
				exit(1);
			}

			/*Dimensionnement de la file d'attente des demandes de connexion entrantes*/
			if (listen(sock, 5) == -1)
			{
				printf("Erreur de listen.\n");
				exit(1);
			}

			if (nb_msg == -1){
					printf("PUITS : lg_mess_lu_max= %d, port=%d, nb_receptions=infini, TP=tcp\n", lg_msg ,num_port);
				} else printf("PUITS : lg_mess_lu=  %d, port=%d, nb_receptions=%d, TP=tcp\n", lg_msg ,num_port,nb_msg);

			while(1)
			{
					/*Acceptation d'une demande de connexion*/
				if ((sock_bis = accept(sock, (struct sockaddr*) &adr_distant, (socklen_t *) &lg_adr_distant)) == -1)
				{
					printf("Echec du accept.\n");
					exit(1);
				}


				int i = 1;
				while ((lg_msg_recu = read(sock_bis, message, taille_max_msg)) > 0) { 
					afficher_message (message, lg_msg);
					i++;
				}


				/*Phase de fermeture d'une connexion*/
				if (shutdown(sock_bis, 2) == -1)
				{
					printf("Erreur lors de la fermeture de la connexion (shutdown).\n");
					exit(1);
				}

				/*Destruction du socket*/
				if (close(sock_bis) == -1)	
				{
					printf("Echec de destruction du socket");
					exit(1);
				}
			}
		

}

// Source UDP :  Fonction de la source en utilisant UDP 
void source_udp(int num_port, char *host, int lg_msg, int nb_msg)
{
	// Creation du socket Source emetteur

	int sockS_UDP;
	printf("--- %d \n", lg_msg);
	if ((sockS_UDP = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		printf("echeck de creation du socket\n");
		exit(1);
	}

	//Creation @distante
	struct hostent *hp;
	struct sockaddr_in adr_distant;
	memset((char *)&adr_distant, 0, sizeof(adr_distant));
	adr_distant.sin_family = AF_INET;
	adr_distant.sin_port = num_port;
	if ((hp = gethostbyname(host)) == NULL)
	{
		printf("erreur gethostbyname\n");
		exit(1);
	}
	memcpy((char *)&(adr_distant.sin_addr.s_addr),
		   hp->h_addr,
		   hp->h_length);
	// envoi des données
	//lg_msg= 30;
	char message[lg_msg];
	//struct sockaddr *padr_dest;
	//message = malloc(lg_msg*sizeof(char));
	construire_message(message, 'a', lg_msg);
	printf("--- \n");
	afficher_message(message, lg_msg);
	if (sendto(sockS_UDP, message, lg_msg, 0, (struct sockaddr *)&adr_distant, sizeof(struct sockaddr)) == -1)
	{
		perror("sendto");
		exit(-1);
	}
	//destruction du socket
	close(sockS_UDP);
}
// Puits UDP :  Fonction du  puits en utilisant UDP 
void udp_puits(int num_port, int nb_msg, int lg_msg)
{

	if (nb_msg == -1)
	{
		printf("PUITS : lg_mess_lu=%d, port=%d, nb_receptions=infini, TP=udp\n", lg_msg, num_port);
	}
	else
	{
		printf("PUITS : lg_mess_lu=%d, port=%d, nb_receptions=%d, TP=udp\n", lg_msg, num_port, nb_msg);
	}

	char *message = malloc(sizeof(char) * lg_msg);
	int sockP_UDP;
	struct sockaddr_in adr_local;	//Adresse du socket local
	struct sockaddr_in adr_distant; //Adresse du socket distant
	int lg_adr_local = sizeof(adr_local);
	int lg_adr_distant = sizeof(adr_distant);

	/* Création d'un socket local*/
	if ((sockP_UDP = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		printf("Echec de la creation du socket puits UDP");
		exit(1);
	}
	memset((char *)&adr_local, 0, sizeof(adr_local));
	adr_local.sin_family = AF_INET;			//domaine internet
	adr_local.sin_port = num_port;			//num de port
	adr_local.sin_addr.s_addr = INADDR_ANY; //toutes les adresses IP de la machine
	//Affectation de l'adresse au socket local à la représentation interne
	if (bind(sockP_UDP, (struct sockaddr *)&adr_local, lg_adr_local) == -1)
	{
		printf("Echec du bind.\n");
		exit(1);
	}
	int octets_recus;

	while (1)
	{
		/*Reception des messages et affichage*/
		octets_recus = recvfrom(sockP_UDP, message, lg_msg, 0, (struct sockaddr *)&adr_distant, (socklen_t *)&lg_adr_distant);
		if (octets_recus == -1)
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
	/*Fermeture du socket et libération de la mémoire*/
	if (close(sockP_UDP) == -1)
	{
		printf("Echec de destruction du socket");
		exit(1);
	}
}

#endif