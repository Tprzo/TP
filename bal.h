#ifndef _tsock_
#define _tsock_
#include <stdio.h>
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
int maxsize=9999;

typedef struct BAL
{
    int num;
    int nb;
    struct LETTRE *l_first;
    struct LETTRE *l_last;
    struct LETTRE *l_current;
    struct BAL *suiv;
} BAL;

typedef struct LETTRE
{
    int num;
    int lg;
    char *message;
    struct LETTRE *suiv;
} LETTRE;

typedef struct LISTE_BAL
{
    struct BAL *first;
    struct BAL *last;
    struct BAL *current;
    int nb;
} LISTE_BAL;
// Declaration des fonctions

void construire_message(char *message, char motif, int lg, int i);
void afficher_message(char *message, int lg);
LISTE_BAL *init_BAL();
void add_BAL(int num, LISTE_BAL *liste);
BAL *find_BAL(LISTE_BAL *liste, int num);
int find_BALR(LISTE_BAL *liste, int num);
void add_LETTRE(int n, int lg, BAL *bal, char *mess);
void printBAL(BAL *bal, int lg);
void empty(BAL *bal);
void EBAL(int port, char *dest, int nb_message, int lg_msg, int nBAL);
void SBAL(int port, char *host);
void RBAL(int port, char *dest, int nBAL);
/*---------------------------------------------------------
  ---------------------- Gestion BAL ----------------------
  ---------------------------------------------------------*/
LISTE_BAL *init_BAL()
{
    LISTE_BAL *liste = (LISTE_BAL *)malloc(sizeof(struct LISTE_BAL));
    liste->first = NULL;
    liste->last = NULL;
    liste->current = NULL;
    liste->nb = 0;
    return liste;
}

void printLISTE(struct LISTE_BAL *liste)
{
    printf("    %d BAL dans notre liste       \n\n", liste->nb);
    liste->current = liste->first;
    while (liste->current != NULL)
    {
        printf("                        BAL n°%d : %d Lettres          \n", liste->current->num, liste->current->nb);
        liste->current = liste->current->suiv;
    }
    printf("              __________________________________________\n\n");
}
/*---------------------------------------------------------
  ----------- Afficher le contenu d'une BAL ---------------
  ---------------------------------------------------------*/
void printBAL(BAL *bal, int lg)
{
    printf("Contenu de la BAL n°%d qui contient %d lettres \n", bal->num, bal->nb);
    bal->l_current = bal->l_first;
    printf("\n");
    int n = 1;
    while (bal->l_current != NULL)
    {
        printf("BAL n°%d | %d Lettres, lettre n°%d : [", bal->num, bal->nb, n);
        afficher_message(bal->l_current->message, lg);
        bal->l_current = bal->l_current->suiv;
        n++;
    }
    printf("\n\n");
}
/*---------------------------------------------------------
  -------------------- Ajouter une BAL --------------------
  ---------------------------------------------------------*/
void add_BAL(int n, LISTE_BAL *liste)
{
    BAL *nouv = malloc(sizeof(struct BAL));
    nouv->num = n;
    nouv->nb = 0;
    nouv->l_first = NULL;
    nouv->l_last = NULL;
    nouv->l_current = NULL;
    nouv->suiv = NULL;

    if (liste->first == NULL)
    {
        liste->first = nouv;
        liste->last = nouv;
    }
    else
    {
        liste->last->suiv = nouv;
        liste->last = nouv;
    }
    liste->nb++;
}
/*---------------------------------------------------------
  ---------------------- Numero  BAL ----------------------
  ---------------------------------------------------------*/

BAL *find_BAL(LISTE_BAL *liste, int num)
{
    BAL *bal = malloc(sizeof(struct BAL));
    liste->current = liste->first;
    if (liste->first == NULL)
    {
        add_BAL(num, liste);
        bal = liste->first;
    }
    else
    {
        liste->current = liste->first;

        if (liste->first == liste->last)
        {
            if (liste->first->num == num)
                bal = liste->current;
            else
            {
                add_BAL(num, liste);
                bal = liste->last;
            }
        }
        else if (liste->first->num == num)
            bal = liste->first;
        else
        {
            int var = 0;
            while (var == 0)
            {
                if (liste->current->suiv == NULL)
                    var = -1;
                else
                {
                    liste->current = liste->current->suiv;
                    if (liste->current->num == num)
                        var = 1;
                    if (liste->current == NULL)
                        var = -1;
                }
            }
            if (var == 1)
                bal = liste->current;
            else
            {
                add_BAL(num, liste);
                bal = liste->last;
            }
        }
    }
    return bal;
}

/*--------------------------------------------------------
------------------- Ajouter num fin BAL ------------------
---------------------------------------------------------*/
void add_LETTRE(int n, int lg, BAL *bal, char *mess)
    {
        bal->nb = (bal->nb) + 1;
        LETTRE *nouv;
        nouv = (LETTRE *)malloc(sizeof(LETTRE));
        nouv->num = n + 1;
        nouv->lg = lg;
        nouv->suiv = NULL;

        if (bal->l_first == NULL)
        {
            bal->l_first = nouv;
            bal->l_last = nouv;
            bal->l_current = nouv;
        }

        else
        {
            bal->l_last->suiv = nouv;
            bal->l_last = bal->l_last->suiv;
        }

        nouv->message = malloc(lg * sizeof(char));
        for (int i = 0; i < lg; i++)
            nouv->message[i] = mess[i];
    }
/*---------------------------------------------------------
  --- Detruire liste en fin d'utilisation de BAL ----------
  ---------------------------------------------------------*/
void empty(BAL * bal)
    {
        bal->l_current = bal->l_first;
        while (bal->l_current != NULL)
        {
            bal->l_current = bal->l_current->suiv;
            free(bal->l_first);
            bal->l_first = bal->l_current;
            (bal->nb)--;
        }
    }
int find_BALR(LISTE_BAL*liste, int num)
{
    int ret;
    if (liste->first==NULL)
    {
        ret=-1;
    }
    else
    {
        liste->current=liste->first;
        if (liste->current==liste->last)
        {
            if (liste->current->num==num)
            {
                if (liste->current->l_first==NULL)
                    ret=-1;
                else
                    ret=liste->current->l_first->lg;
            }
            else
                ret=-1;
        }
        else if (liste->first->num==num)
        {
            if (liste->current->l_first==NULL)
                ret=-1;

            else
                ret=liste->current->l_first->lg;
        }
        else
        {
            int var=0;
            while(var==0)
            {
                if (liste->current->suiv==NULL)
                    var=-1;
                else
                {
                    liste->current=liste->current->suiv;
                    if (liste->current->num==num)
                        var=1;
                    if (liste->current==NULL)
                        var=-1;
                }

            }
            if (var==1)
            {
                if (liste->current->l_first==NULL)
                    ret=-1;
                else
                    ret=liste->current->l_first->lg;
            }
            else
                ret=-1;
        }

    }
    return ret;
}
void EBAL(int port, char *dest, int nb_message, int lg_msg, int nBAL)
    {
        //Déclarations
        int sock;
        struct sockaddr_in addr_distant;
        int lg_addr_distant = sizeof(addr_distant);
        struct hostent *hp;
        char motif;
        char *message = malloc(lg_msg * sizeof(char));
        int envoi = -1;
        int lg_pdu = 50;
        int lg_recv;
        char *pdu = malloc(lg_pdu * sizeof(char));
        // Etablissement de connexion
        printf("            SOURCE : Emission de lettres pour la BAL n°%d\n", nBAL);
        printf("____________________________________________________________________\n\n");

        sprintf(pdu, "0 %d %d %d", nBAL, nb_message, lg_msg);

        //Création socket
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        {
            printf("Erreur à l'ouverture du Socket Stream");
            exit(1);
        }
        // Construction de l'@ socket distant
        memset((char *)&addr_distant, 0, sizeof(addr_distant));
        addr_distant.sin_family = AF_INET; //Internet
        addr_distant.sin_port = port;      //Numéro de Port

        //Affectation IP
        if ((hp = gethostbyname(dest)) == NULL)
        {
            printf("Erreur de requête IP.\n");
            exit(1);
        }

        memcpy((char *)&(addr_distant.sin_addr.s_addr), hp->h_addr, hp->h_length);

        //Demande de connexion

        if (connect(sock, (struct sockaddr *)&addr_distant, sizeof(addr_distant)) == -1)
        {
            printf("Erreur lors de la connexion, en attente de la tentative suivante \n");
            exit(1);
        }
        // Envoi PDU
        if ((envoi = write(sock, pdu, lg_pdu)) == -1)
        {
            printf("Echec de l'envoi du PDU Emetteur (fonction write en défaut)\n");
            exit(1);
        }
        // Transfert de donnees
        for (int i = 1; i <= nb_message; i++)
        {
            printf("SOURCE : lettre n°%d (%d) [", i, lg_msg);

            //Création du message
            construire_message(message, motif, lg_msg, i);
            //printbuffer2(nBAL, message);
            afficher_message(message, lg_msg);

            //Envoi du message

            if ((envoi = write(sock, message, (lg_msg) /*,0,(struct sockaddr*)&addr_distant,lg_addr_distant)*/)) == -1)
            {
                printf("Echec de l'envoi du message (fonction write en défaut)\n");
                exit(1);
            }
        }
        // Fermeture de connexion
        if (shutdown(sock, 2) == -1)
        {
            printf("Erreur à la fermeture de la connexion TCP \n");
            exit(1);
        }

        //Fermeture Socket
        if (close(sock) == -1)
        {
            printf("Echec de la fermeture du socket distant");
            exit(1);
        }

        free(message);
        free(pdu);
        printf("Envoi effectué avec succès\n");
    }
void SBAL(int port, char *dest)
{
        //Déclarations
    int sock , sock2;
    struct sockaddr* addr_distant;
    struct sockaddr_in addr_local;
    int lg_addr_distant=sizeof(addr_distant);
    int lg_addr_local=sizeof(addr_local);
    struct hostent *hp;
    char motif;
    char *message;
    int lg_recv=-1;
    int lg_sent=-1;
    int lg_pdu=50;
    int type=-1;
    int nb;
    int lg;
    int n=1;
    int nBAL;
    BAL*bal=malloc(sizeof(struct BAL));
    char *pdu;
    LISTE_BAL* liste;

//Connexion


    //Création socket local
    if ((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        printf("Echec de la création d'un socket local\n");
        exit(1);
    }
    //Construction adresse socket local | Affectation port et domaine
    memset((char*)&addr_local, 0 , sizeof(addr_local));
    addr_local.sin_family=AF_INET;
    addr_local.sin_addr.s_addr=INADDR_ANY;
    addr_local.sin_port=port;

    //Bind
    if (bind(sock,(struct sockaddr *)&addr_local, lg_addr_local)==-1)
    {
        printf("Echec du bind.\n");
        exit(1);
    }
    //Check connexions entrantes
    if (listen(sock,100)<0)
    {
        printf("Trop de connexions en attentes, échec de la demande\n");
        exit(1);
    }
    liste=init_BAL();
    while (1)
    {
        if ((sock2=accept(sock,(struct sockaddr*)&addr_distant,&lg_addr_distant))==-1)
        {
            printf("Refus de connexion par le serveur\n");
            exit(1);
        }

        pdu=malloc(50*sizeof(char));
        if((lg_pdu=read(sock2,pdu, lg_pdu))<0)
        {
            printf("Echec de lecture du PDU entrant\n");
            exit(1);
        }
        sscanf(pdu, "%d %d %d %d", &type, &nBAL, &nb, &lg);
        //GESTION EMETTEUR
        if (atoi(pdu)==0)
        {
            printf("        ||||||| Réception des lettres pour la BAL n°%d |||||\n\n",nBAL);
            message=malloc(lg*sizeof(char));
            int n=0;
            sscanf(pdu, "%d %d %d %d", &type, &nBAL, &nb, &lg);
            bal=find_BAL(liste,nBAL);

            while (n!=nb)
            {
                message = malloc(lg* sizeof(char));
                if ((lg_recv = read(sock2, message, lg)) == -1)
                {
                    printf("Erreur de lecture\n");
                    exit(1);
                }
                if (lg_recv>0)
                {
                    add_LETTRE(n,lg, bal,message);
                }
                n++;
            }

            printBAL(bal,lg);
        }
        //GESTION RECEPTEUR
        else if (atoi(pdu)==1)
        {
            sscanf(pdu, "%d %d", &type, &nBAL);
            printf("        ||||||| Restitution des lettres de la BAL n°%d |||||||\n\n",nBAL);
            lg=find_BALR(liste,nBAL);
            if (lg==-1) // Gestion du cas ou la BAL est vide, on envoie un PDU qui sera analysé par le récepteur.
            {
                printf("        BAL inexistante, PDU=0 pour informer le récepteur\n\n");
                sprintf(pdu,"%d %d",lg,nb);
                //printf ("PDU à envoyer : %d\n",lg);
                int lg_sent=-1;
                nb=1;
                if ((lg_sent=write(sock2,pdu,lg_pdu))==-1) /*,0,(struct sockaddr*)&addr_distant,lg_addr_distant)*/
                {
                    printf("Echec de l'envoi du PDU  (fonction write en défaut)\n");
                    exit(1);
                }
            }
            else
            {
                bal=find_BAL(liste,nBAL);
                bal->l_current=bal->l_first;

                while(bal->l_current!=NULL)
                {
                    lg=bal->l_current->lg;
                    nb=bal->nb;
                    sprintf(pdu,"%d %d",lg,nb);

                    if ((lg_sent=write(sock2,pdu,lg_pdu))==-1) /*,0,(struct sockaddr*)&addr_distant,lg_addr_distant)*/
                    {
                        printf("Echec de l'envoi du PDU Emetteur (fonction write en défaut)\n");
                        exit(1);
                    }
                    message=malloc(lg*sizeof(char));
                    message=bal->l_current->message;

                    if ((lg_sent=write(sock2,message,lg))==-1)
                    {
                        printf("Erreur lors de l'envoi du message n°%d\n",n);
                        exit(1);
                    }
                    printf("BAL n°%d : Restitution de la lettre n°%d (%d) [",nBAL,n,lg);
                    afficher_message(message,lg);
                    bal->l_current=bal->l_current->suiv;
                    n++;
                }
                empty(bal);
                if ((shutdown(sock2 , 2))==-1)
                {
                    printf("Erreur à la fermeture de la connexion : shutdown\n");
                    exit(1);
                }
            }
        }
        else
        {
            printf("PDU non reconnu, on quitte par sécurité\n");
            exit(1);
        }
        printLISTE(liste);
        free(pdu);
        free(message);
} }  
void RBAL(int port, char* dest, int nBAL)
{
    //Déclarations
    int sock;
    struct sockaddr_in addr_distant;
    int lg_addr_distant = sizeof(addr_distant);
    struct hostent *hp;
    char *message; //Penser au free en fin de programme pour libérer l'espace mémoire
    int envoi = -1;
    int lg_pdu=50;
    int lg_recv=-1;
    int lg;
    int nb;
    char *pdu = malloc(lg_pdu*sizeof(char));

//---------------------------------------
//--------Etablissement connexion--------
//---------------------------------------

    sprintf(pdu,"1 %d",nBAL);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Erreur à l'ouverture du Socket Stream");
        exit(1);
    }

    //Construction adresse socket distant
    memset((char *) &addr_distant, 0, sizeof(addr_distant));
    addr_distant.sin_family = AF_INET;  //Internet
    addr_distant.sin_port = port;       //Numéro de Port

    //Affectation IP
    if ((hp = gethostbyname(dest)) == NULL) {
        printf("Erreur de requête IP.\n");
        exit(1);
    }

    memcpy((char *) &(addr_distant.sin_addr.s_addr), hp->h_addr, hp->h_length);

    //Demande de connexion

    if (connect(sock, (struct sockaddr *) &addr_distant, sizeof(addr_distant)) == -1) {
        printf("Erreur lors de la connexion, en attente de la tentative suivante \n");
        exit(1);
    }

//-----------------------------------------
//----------------Envoi PDU----------------
//-----------------------------------------

    if ((envoi = write(sock, pdu, lg_pdu)) == -1) /*,0,(struct sockaddr*)&addr_distant,lg_addr_distant)*/
    {
        printf("Echec de l'envoi du PDU Emetteur (fonction write en défaut)\n");
        exit(1);
    }
    char*lgmsg=malloc(maxsize* sizeof(char));
    nb=10;
    int n=1;
    lg_recv=1;
    printf("             PUITS : Réception du contenu de la BAL n°%d\n",nBAL);
    printf("____________________________________________________________________\n\n");

    while(n<=nb)
    {
        if ((lg_recv=read(sock,lgmsg,lg_pdu))==-1)
        {
            printf("Erreur à la réception du PDU de longueur de message\n");
            exit(1);
        }
        sscanf(lgmsg,"%d %d", &lg , &nb);
        if (lg==-1)
        {
            printf("       ATTENTION : Pas de courrier à récupérer dans la BAL n°%d\n\n",nBAL);
            exit(0);
        }

        message=malloc(lg*sizeof(char));
        if ((lg_recv=read(sock,message,lg))==-1)
        {
            printf("Erreur à la réception du message\n");
            exit(1);
        }
        printf("PUITS : Réception de la lettre n°%d : [",n);
        afficher_message(message,lg);
        n++;
    }

    printf("Fermeture de la Connexion\n");
    //Ciao le socket
    if(close(sock)==-1)
    {
        printf("Impossible de fermer le socket");
        exit(1);
    }
}

#endif

