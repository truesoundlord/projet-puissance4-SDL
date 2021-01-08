#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include <uep_wide.h>

#ifndef NETOFF                  
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#endif

#ifndef CLRS

#define CLRS
#define ROUGE	"\x1b[38;2;255;0;0;1m"
#define JAUNE "\x1b[38;2;255;237;32;1m"
#define RESET "\x1b[0m"
#define CLIGN "\x1b[5m"

#endif

#define EXIT_ABRT -2
#define SRVPORT		4040

typedef struct s_Datas
{
	int		colonne;
	char	name[25];
}t_Datas;

// Prototype des fonctions

void ShowTokens(void);
bool IsAligned(short,short,unsigned char);

int AnalyzeCols(void);
int AnalyzeLines(void);
int AnalyzeDiagonals(void);

bool CanIPlay(int,Pile*);

bool IsAddressIP(char*);

void  ArretPropre(void); 
void  Interruptions(int);

// Définition des variables globales

bool 	bContreOrdi=false;
bool	bTourOrdi=false;

// pour permettre la sélection du mode réseau ou pas

bool  bNetworkMode=false;                         // février 2016
bool  bNetWorkStatus=true;                        // NEW

short cptX=0;
short cptY=0;

extern unsigned char tabPuissance4[8][8];
extern Stack	ColonnesInterdites;										// Pour empêcher l'ordinateur de choisir une colonne qui permettrait au joueur de gagner ou de contrer...
extern bool	colonnepleine[8];

unsigned char currentPlayer='R';						// Le joueur 'R' sera toujours humain...

unsigned short hazard=0;

unsigned short jetonsRouges=32;
unsigned short jetonsJaunes=32;

char strJoueurRouge[25];
char strJoueurJaune[25];
char strServerIP[255];
char strTemp[16];


t_Datas	Donnees;

// La connexion TCP

TCPsocket connexion;

// Gestion des signaux système

struct sigaction Signal;
struct sigaction EmpecherInt;

// Fonction principale

int main(int argc, char *argv[])
{
  if(argc>3 || argc<2 || argv[1]==NULL)
  {
      printf( "USAGE:\t./puissance4 nom_joueurRouge nom_joueurJaune\n"
              "or\n"
							"\t\t./puissance4 nom_joueur comp (vs computer mode -- easy)\n"
							"or\n"
              "\t\t./puissance4 nom_joueur ServerIP (client mode)\n"
              "or\n"
              "\t\t./puissance4 nom_joueur (server mode)\n");
      exit(1);
  }
  
  if(argv[2]==NULL) // cas du serveur réseau
  {
    // récupérer l'adresse IP de la machine
    IPaddress interfaces[2];
    int rc=SDLNet_GetLocalAddresses(interfaces,2);
    strncpy(strServerIP,SDLNet_ResolveIP(&interfaces[rc-1]),255);
		//printf("[DEBUG] %s\n",strServerIP);
    bNetworkMode=true;
  }
  else 
  {
    strcpy(strTemp,argv[2]);
    if(!IsAddressIP(argv[2]))
    {
      // argv[2][0] n'est pas une adresse IP
      // mode local joueur contre joueur
      strncpy(strJoueurJaune,argv[2],25);
      if(strcmp(argv[2],"comp")==0) bContreOrdi=true;
      bNetworkMode=false;
    }
    else
    {
      strncpy(strServerIP,strTemp,16);
      bNetworkMode=true;
    }
  }
  
  // Prise en charge du réseau

  if(SDLNet_Init()==-1)
  {    
      printf("SDLNet_Init: %s", SDLNet_GetError());
      exit(2);
  }

  // Risque de dépassement de buffer, faudra utiliser strncpy au lieu de strcpy...

  strncpy(strJoueurRouge,argv[1],25); 

  bool Client=true;
  bool ClientConnecte=false;

  IPaddress ip;
  TCPsocket tcpsock;
  TCPsocket new_tcpsock;

  fflush(stdout);
  
  srand(time(NULL));
  hazard=rand()%99+1;

  Signal.sa_flags=0;
  Signal.sa_handler=Interruptions;
  
  sigaction(SIGINT,&Signal,NULL); // enregistrement du signal à détourner
    
  if(bNetworkMode)
  {
    if(SDLNet_ResolveHost(&ip,strServerIP,SRVPORT)==-1)
    {
        printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
        exit(2);
    }

    tcpsock=SDLNet_TCP_Open(&ip);
    if(!tcpsock) 
    {
      //printf("[Client mode] SDLNet_TCP_Open: %s\n", SDLNet_GetError());
      Client=false;																					// Il n'y a personne au bout du fil...
    }
    else
      printf("Connecté au serveur (%s)\n",SDLNet_ResolveIP(SDLNet_TCP_GetPeerAddress(tcpsock)));  

    //******************************************************************************
    // PARTIE SERVER
    //******************************************************************************
    if(!Client)
    {
      // create a listening TCP socket on port SRVPORT (server)
      if(SDLNet_ResolveHost(&ip,NULL,SRVPORT)==-1) 
      {
        printf("SDLNet_ResolveHost: %s", SDLNet_GetError());
        exit(2);
      }
        // le serveur met un "verrou" sur le port TCP SRVPORT
      tcpsock=SDLNet_TCP_Open(&ip);
      if(!tcpsock) 
      {
        printf("[Server mode] SDLNet_TCP_Open: %s", SDLNet_GetError());
      }
      printf("[Server mode] waiting for connection...\n");
      while(!ClientConnecte)
      {
        // fait un "polling" (vérifie/teste) sur le port SRVPORT pour une requête de connexion
        new_tcpsock=SDLNet_TCP_Accept(tcpsock);
        if(!new_tcpsock) 
        {
            sleep(1);
        }
        else 
          ClientConnecte=true;
      }

      // recup nom client

      int codeerreur=SDLNet_TCP_Recv(new_tcpsock,strJoueurJaune,25);
      if(codeerreur==-1)
          printf("[Server] SDLNet_TCP_Recv: (%s)", SDLNet_GetError());

      // envoi du nom du serveur

      codeerreur=SDLNet_TCP_Send(new_tcpsock,argv[1],25);
      if(codeerreur==-1)
            printf("[Server] SDLNet_TCP_Send: (%s)", SDLNet_GetError());
      // Déterminer qui commence...
      // par convention le protocole prévoit que colonne = -1 si on détermine qui commence

      Donnees.colonne=-1;
      memset(Donnees.name,0,25);

      if(hazard<51) 
      {
        //if(bContreOrdi) bTourOrdi=true;
        currentPlayer='J';
        printf("%s commence !! (%c)\n",strJoueurJaune,currentPlayer);
        strncpy(Donnees.name,strJoueurJaune,25);
      }
      else 
      {
        printf("%s commence !! (%c)\n",strJoueurRouge,currentPlayer);
        strncpy(Donnees.name,strJoueurRouge,25);
      }
      // Le serveur envoie les informations au client
      codeerreur=SDLNet_TCP_Send(new_tcpsock,(void*)&Donnees,sizeof(t_Datas));
    } // fin partie serveur
    //******************************************************************************
    // PARTIE CLIENT
    //******************************************************************************
    else
    {
      // envoi du nom du client
	
      int codeerreur=SDLNet_TCP_Send(tcpsock,argv[1],25);  
      if(codeerreur ==-1)
      {
          printf("[Client] SDLNet_TCP_Send: (%s)", SDLNet_GetError());
      }
	
      // récup nom du serveur

      codeerreur=SDLNet_TCP_Recv(tcpsock,strJoueurRouge,25);
      if(codeerreur ==-1)
      {
          printf("[Client] SDLNet_TCP_Recv: (%s)", SDLNet_GetError());
      }
      strncpy(strJoueurJaune,argv[1],25);

      // attente pour savoir si le client commence ou pas...

      codeerreur=SDLNet_TCP_Recv(tcpsock,(void*)&Donnees,sizeof(t_Datas));
      printf("Données reçues du serveur: %s\n",Donnees.name);

      if(strcmp(argv[1],Donnees.name)==0) 
      {
        // le serveur sera TOUJOURS 'R'
        currentPlayer='J';
      }
      else currentPlayer='R';
    } // fin partie client
  } // fin partie network
  else
  {
    // Déterminer qui commence
    if(hazard<51) 
    {
      if(bContreOrdi) bTourOrdi=true;
      currentPlayer='J';
      printf("%s commence !! (%c)\n",strJoueurJaune,currentPlayer);
    }
    else 
    {
      printf("%s commence !! (%c)\n",strJoueurRouge,currentPlayer);
    }
  } // fin partie sans réseau
    
	// "Vider" le tableau en mémoire
	
	for(cptY=0;cptY<8;cptY++)
		for(cptX=0;cptX<8;cptX++)
			tabPuissance4[cptY][cptX]=' ';
	
	for(cptX=0;cptX<8;cptX++) colonnepleine[cptX]=false;
	
	unsigned short cptTours;
	
	// Boucle principale (éviter les effets de bord)... 
		
	for(cptTours=1;cptTours<64;cptTours++)
	{
		int Colonne;
    
		if(currentPlayer=='J')
			printf("...au tour de %s de jouer...(%s%c%s)\n",strJoueurJaune,JAUNE,currentPlayer,RESET); // strJoueurJaune sera toujours "l'autre"
		else
			printf("...au tour de %s de jouer...(%s%c%s)\n",strJoueurRouge,ROUGE,currentPlayer,RESET); // strJoueurRouge sera toujours "moi" 
		ShowTokens();
		
		//**********************************************************************************************************************************************************
		// Si nous ne jouons pas contre l'ordinateur...
		//**********************************************************************************************************************************************************
				
		if(!bTourOrdi)
		{
			// Je pense, mais je n'ai pas vraiment réfléchi au problème que la pile des "colonnes interdites" doit être vidée à chaque fois que le joueur
			// humain joue. Cela peut, en effet, modifier considérablement les perspectives... dans le cas où toutes les colonnes sont injouables pour l'ordinateur
			// (cas rares).    
			
			EmptyStack(&ColonnesInterdites);
      
      // TODO: boucle do-while (voir cours) pour éviter tout sabotage en entrée...
			
      if(!bNetworkMode)                   
      {
        printf("Entrez le numéro de la colonne [1,8]: ");
        //scanf("%d",&Colonne);																// ELEVES: changer scanf() par getch()
				do
				{
					Colonne=getch()-48;
					if(Colonne+48==4) 
					{
						printf("\n");
						if(bNetworkMode) ArretPropre();
						exit(EXIT_ABRT);
					}
				}while(Colonne<1 || Colonne >8 || !CanIPlay(Colonne,&ColonnesInterdites));
				printf("\n");
      }
      else
      {
				//**********************************************************************************************************************************************************
				// PARTIE RESEAU JOUEUR CONTRE JOUEUR (ou "AI" si argv[1] est "comp")
				//**********************************************************************************************************************************************************
					
        if(Client) connexion=tcpsock; // Si je suis client j'écris et lis sur tcpsock
        else connexion=new_tcpsock;   // sinon j'écris et lis sur new_tcpsock
        
        // BUG --> comme on ne veut pas trop se compliquer la vie on envoie un message particulier sur le réseau AVANT de fermer la socket...
				// C'est à cela que sert le "ACK" il sert à s'assurer que "l'autre bout de la socket" a bien reçu le message d'arrêt...
				
				// Tentative d'évitemment d'exécution de fonctions dites "bloquantes" (le cas du Recv qui est interrompu par CTRL-C) qui ont tendance à foutre la 
				// merde si elles sont intérrompues (par CTRL-C)...
					
				// Phase plus méchante: ne pas permettre de faire un CTRL-C pendant cette période (à voir si c'est nécessaire) --> voir getch() dans cssm_wide.c
        
        if(!bNetWorkStatus) 
        {
          ArretPropre();																									// Si le flag "bNetworkStatus" passe à "faux" alors il faut procéder à l'arrêt propre.
          EmptyStack(&ColonnesInterdites);
          //printf("[DEBUG:%s] Ici !!!\n",Client?"Client":"Serveur");
        
          SDLNet_TCP_Close(connexion);
          exit(EXIT_ABRT);
        }
        
        int codeerreur;
        
        // comparer le nom du joueur courant avec la donnée reçue du réseau

        if(strcmp(argv[1],Donnees.name)!=0 && bNetWorkStatus) 
				{
					// on ne peut pas interrompre le Recv
					
					EmpecherInt.sa_handler=SIG_IGN;																											// le signal sera ignoré
					sigaction(SIGINT,&EmpecherInt,NULL);
										
					codeerreur=SDLNet_TCP_Recv(connexion,(void*)&Donnees,sizeof(t_Datas));
					
					EmpecherInt.sa_handler=Interruptions;																											// le comportement par défaut du signal sera rétabli
					EmpecherInt.sa_flags=0;
					sigaction(SIGINT,&EmpecherInt,NULL);
				}
        else
        {
					if(bNetWorkStatus)
					{
						// 2021
						
						if(strcmp(argv[1],"comp")==0)
						{
							sleep(1);
							Colonne=AnalyzeCols();															// l'ordinateur tente de savoir si il est en danger sur les colonnes
							if(Colonne==-1)
								Colonne=AnalyzeLines();														// si pas en danger il tente de savoir si il est en danger sur les lignes
							if(Colonne==-1)
							{
								Colonne=AnalyzeDiagonals();												// si pas en danger il tente de savoir si il est en danger sur les diagonales
								printf("[AD] Colonne à jouer: %d\n",Colonne);
							}
							if(Colonne==-1)
							{
								do
								{
									sleep(1);
									srand(time(NULL));
									Colonne=rand()%8+1;
								}while(!CanIPlay(Colonne,&ColonnesInterdites));		// tant que tu comptes jouer une colonne interdite tu cherches un autre numéro de colonne
							}
							printf("L'ordinateur a choisi la colonne %d\n",Colonne);
						}
						else
						{
							printf("Entrez le numéro de la colonne [1,8]: ");
							//scanf("%d",&Colonne);																// ELEVES: changer scanf() par getch()
							
							do
							{
								Colonne=getch()-48;
								if(Colonne+48==4) 
								{
									printf("\n");
									if(bNetworkMode) ArretPropre();
									exit(EXIT_ABRT);
								}
							}while(Colonne<1 || Colonne >8 || !CanIPlay(Colonne,&ColonnesInterdites));
						}
						Donnees.colonne=Colonne;
						strncpy(Donnees.name,argv[1],25);
						codeerreur=SDLNet_TCP_Send(connexion,(void*)&Donnees,sizeof(t_Datas));
					}
        }
				
				// détecter l'éventuel message d'arrêt...
        
        //printf("[DEBUG] [%-*s]\n",strlen(Donnees.name),Donnees.name);

        if(codeerreur==-1)
        {
          printf("[DEBUG] SDLNet_TCP_Recv: (%s)", SDLNet_GetError());
        }
      }      		
		}
		
		//**********************************************************************************************************************************************************
		// Nous jouons contre l'ordinateur...
		//**********************************************************************************************************************************************************
						
		else
		{
			sleep(1);
			Colonne=AnalyzeCols();															// l'ordinateur tente de savoir si il est en danger sur les colonnes
			if(Colonne==-1)
				Colonne=AnalyzeLines();														// si pas en danger il tente de savoir si il est en danger sur les lignes
			if(Colonne==-1)
			{
				Colonne=AnalyzeDiagonals();												// si pas en danger il tente de savoir si il est en danger sur les diagonales
				printf("[AD] Colonne à jouer: %d\n",Colonne);
			}
			if(Colonne==-1)
			{
				do
				{
					sleep(1);
					srand(time(NULL));
					Colonne=rand()%8+1;
				}while(!CanIPlay(Colonne,&ColonnesInterdites));		// tant que tu comptes jouer une colonne interdite tu cherches un autre numéro de colonne
				//printf("Puis-je jouer la colonne %d --> %s\n",Colonne,CanIPlay(Colonne,&ColonnesInterdites)?"oui":"non");
			}
			printf("L'ordinateur a choisi la colonne %d\n",Colonne);
		}
    
		//**********************************************************************************************************************************************************
		// Si nous sommes en réseau...
    //**********************************************************************************************************************************************************
		
    if(bNetworkMode) 
    {
      //printf("[%s] ICI !!\n",Client?"client":"serveur");
      //printf("[DEBUG] reçu du réseau: %d\n",Donnees.colonne);
			
			// Un des joueurs indique qu'il met fin à la partie
			
      Colonne=Donnees.colonne;
      if(Colonne==-1000) 
			{
				//ArretPropre();
				printf("[2021] partie réseau interrompue... (message: %s) [%08d]\n",Donnees.name,getpid());
				EmptyStack(&ColonnesInterdites);
				SDLNet_TCP_Close(connexion);
        exit(EXIT_ABRT);
			}
		}
		
		//**********************************************************************************************************************************************************
		// ...suite indépendante
		//**********************************************************************************************************************************************************
		
		Colonne--;
		// On ne devrait pas se trouver dans ce cas de figure, mais vaut mieux être prudents...
		if(Colonne<1) Colonne=0;
		if(Colonne>8) Colonne=7;
		
		// Je sais plus à quoi servent ces lignes de code (je n'avais rien commenté à l'époque --> gommette rouge !!)
		
		for(cptY=0;cptY<8;cptY++)													// je pense que c'est pour pouvoir afficher le jeton au-dessus du dernier jeton inséré dans la colonne choisie (en fait j'en suis sûr ^^)...			
		{
			if(tabPuissance4[cptY][Colonne]!=' ')
				break;
		}
		cptY--;																						// à la sortie de la boucle cptY est toujours à cptY+1 (il aurait fallu utiliser do-while...)
		if(cptY>=0 && cptY<=7)
			tabPuissance4[cptY][Colonne]=currentPlayer;			// on met le jeton du joueur courant en tête de la colonne (au dessus de tous les autres jetons dans la colonne)...		
		else
		{
			// Il y a eu un bug il faudra surveiller :{
			colonnepleine[Colonne]=true;
			continue;	// c'est normalement interdit en logique de programmation mais m... ! je fais ce que je veux vu que je sais ce que je fais !!											
		}
		// Vérifier, une fois le jeton ajouté dans la colonne, si ils sont alignés ou pas...

		if(IsAligned(Colonne,cptY,currentPlayer))
		{
			if(currentPlayer=='R')
				printf("%s%s\t%s a gagné !!  %s (%s%c%s)\n",CLIGN,ROUGE,strJoueurRouge,RESET,ROUGE,currentPlayer,RESET);
			else
				printf("%s%s\t%s a gagné !!  %s (%s%c%s)\n",CLIGN,JAUNE,strJoueurJaune,RESET,JAUNE,currentPlayer,RESET);	
			
			break;																						// interdit normalement, mais bon... alternative --> cptTours=64; mais toutes les instructions des lignes suivantes seront exécutées... pfff ! 
		}
		
		//printf("[DEBUG 2021] IsAligned AFTER\n");
		
		if(currentPlayer=='R') 
		{
			jetonsRouges--;
			currentPlayer='J';
			if(bContreOrdi)
				bTourOrdi=true;																	// l'ordinateur jouera toujours en jaune...
			
			// changer de joueur
      if(bNetworkMode)
        strncpy(Donnees.name,strJoueurJaune,25);
		}
		else 
		{
			jetonsJaunes--;
			currentPlayer='R';
			if(bContreOrdi)
				bTourOrdi=false; // c'est au tour du joueur (R) de jouer, et il ne s'agit pas de l'ordinateur (note: si on voulait faire une démo, c'est ici que tout se jouerait ^^)
			
			// changer de joueur
      if(bNetworkMode)
        strncpy(Donnees.name,strJoueurRouge,25);  
		}
		printf("[R:%02d\tJ:%02d]\n",jetonsRouges,jetonsJaunes);
		fflush(stdin);
	} // end for 
	printf("[DEBUG 2021] AFTER LOOP \n");
	printf("[DEBUG 2021] [%c] (tours:%02d)\n",currentPlayer,cptTours);
	
	// cas 2021: le joueur (comp) ne sait plus jouer car toutes les possibilités restantes
	// le mèneront à une défaite humiliante ^^
		
	ShowTokens();	
	exit(0);
}

void Interruptions(int signal)
{
  // Gestion des signaux système
  switch(signal)
  {
    case SIGINT: // CTRL-C
                  //printf("CTRL-C appuyé !!\n");
                  bNetWorkStatus=false;
									//printf("[DEBUG] %s\n",bNetWorkStatus?"vrai":"faux");
  }
}

void ArretPropre(void) 
{
  // protocole de fermeture des sockets le programme recevant le signal SIGINT doit s'assurer que de l'autre côté nous avons reçu le message
  // local_socket représente la socket sur laquelle écrire
  
  printf("[DEBUG]Arrêt propre !!\n");
  printf("[DEBUG]Envoi d'un message à l'autre bout... (my pid: %08d)\n",getpid());
  
	sprintf(Donnees.name,"STOP %08d",getpid());
  Donnees.colonne=-1000;
  
  // envoi du message d'arrêt...
  
  SDLNet_TCP_Send(connexion,(void*)&Donnees,sizeof(t_Datas));
  fflush(stdout);
}
