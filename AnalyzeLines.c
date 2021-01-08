// Cette fonction consiste à copier les lignes du plateau de jeu
// de manière à détecter facilement les configurations différentes
// de trois (3) 'R' ou 'J'

#include <stdio.h>
#include <string.h>

#ifndef __UEPWIDE__
#include <uep_wide.h>
#endif

// #define DEBUG

unsigned char tabPuissance4[8][8];
bool	colonnepleine[8];
Stack	ColonnesInterdites;										// Pour empêcher l'ordinateur de choisir une colonne qui permettrait au joueur de gagner ou de contrer...

int AnalyzeLines(void)
{
	int		cptLigne;
	int		cptColonne;
	char 	strLignes[8][9];
	char	*offset;						// adresse de la première occurence d'une configuration de 'R' ou 'J' (déplacement en mémoire -> adresse de destination)
														// pour obtenir la "case" dans le tableau où déposer le jeton il faut faire le calcul suivant
														// emplacement de la case vide (1 à 4)+(adresse de destination-adresse source)
	
	
	// Affichage des lignes et des colonnes sur lequel je base "l'intelligence artificielle" de l'ordinateur"
	// Si l'ordinateur trouve trois 'R' ou trois 'J' dans une position bien particulière il jouera de manière
	// soit à contrer soit à attaquer !!
				
	// copie du tableau ligne par ligne
	for(cptLigne=0;cptLigne<8;cptLigne++)
	{
		for(cptColonne=0;cptColonne<8;cptColonne++)	
		{
			strLignes[cptLigne][cptColonne]=tabPuissance4[cptLigne][cptColonne];
		}
		strLignes[cptLigne][cptColonne]='\0';
	}
	
	// Simulation de l'intelligence artificielle de l'ordinateur...
	
	// TODO: prévoir que si il n'y a rien sur la ligne d'en-dessous il ne faut surtout pas qu'il mette son jeton dans cette colonne !!
	// cas puissance4_debug001.png
	
	int futurecolonne;
	
	for(cptLigne=0;cptLigne<8;cptLigne++)
	{
#ifdef DEBUG
		printf("[AL] strLigne: [%-8s]\n",strLignes[cptLigne]);
#endif
		if((offset=strstr(strLignes[cptLigne]," JJJ"))!=NULL)
		{
			futurecolonne=1+(offset-strLignes[cptLigne]);
			
			if(cptLigne<7 && cptLigne>0)
			{	
				// Améliorer le procédé: il est possible de jouer cette colonne si il y a deux (ou plus) cases "libres" (ASCII 32)
				// Utiliser l'exemple dans la fonction main() pour déterminer la première case libre...
				// --> ELEVES (boucles, fonctions)
				if(tabPuissance4[cptLigne+1][futurecolonne-1]==' ')		// si il n'y a pas de jeton sur la ligne du dessous...
				{
					Push((void*)&futurecolonne,&ColonnesInterdites,sizeof(int));
					strLignes[cptLigne][futurecolonne-1]='*';
					cptLigne--;																					// je ne suis pas certain que "continue" fasse évoluer cptLigne
					continue;																						// nous retournons au for -- interdit normalement
				}
			}
			if(!colonnepleine[futurecolonne-1])
				return futurecolonne;
		}
		if((offset=strstr(strLignes[cptLigne]," RRR"))!=NULL)
		{
			futurecolonne=1+(offset-strLignes[cptLigne]);
			
			if(cptLigne<7 && cptLigne>0)
			{	
				if(tabPuissance4[cptLigne+1][futurecolonne-1]==' ')		// si il n'y a pas de jeton sur la ligne du dessous...
				{
					Push((void*)&futurecolonne,&ColonnesInterdites,sizeof(int));
					strLignes[cptLigne][futurecolonne-1]='*';
					cptLigne--;																					// c'est pour ça qu'il faut continuer et envisager tous les cas de figure...
					continue;
				}
			}
			if(!colonnepleine[futurecolonne-1])
				return futurecolonne;
		}
		if((offset=strstr(strLignes[cptLigne],"JJJ "))!=NULL)
		{
			futurecolonne=4+(offset-strLignes[cptLigne]);
			
			if(cptLigne<7 && cptLigne>0)
			{	
				if(tabPuissance4[cptLigne+1][futurecolonne-1]==' ')		// si il n'y a pas de jeton sur la ligne du dessous...
				{
					Push((void*)&futurecolonne,&ColonnesInterdites,sizeof(int));
					strLignes[cptLigne][futurecolonne-1]='*';
					cptLigne--;
					continue;
				}
			}
			if(!colonnepleine[futurecolonne-1])
				return futurecolonne;
		}
		if((offset=strstr(strLignes[cptLigne],"RRR "))!=NULL) 
		{
			futurecolonne=4+(offset-strLignes[cptLigne]);
			
			if(cptLigne<7 && cptLigne>0)
			{	
				if(tabPuissance4[cptLigne+1][futurecolonne-1]==' ')		// si il n'y a pas de jeton sur la ligne du dessous...
				{
					Push((void*)&futurecolonne,&ColonnesInterdites,sizeof(int));
					strLignes[cptLigne][futurecolonne-1]='*';
					cptLigne--;
					continue;
				}
			}
			if(!colonnepleine[futurecolonne-1])
				return futurecolonne;
		}
		if((offset=strstr(strLignes[cptLigne],"JJ J"))!=NULL)
		{
			futurecolonne=3+(offset-strLignes[cptLigne]);
			
			if(cptLigne<7 && cptLigne>0)
			{
				if(tabPuissance4[cptLigne+1][futurecolonne-1]==' ')		// si il n'y a pas de jeton sur la ligne du dessous...
				{
					Push((void*)&futurecolonne,&ColonnesInterdites,sizeof(int));
					strLignes[cptLigne][futurecolonne-1]='*';
					cptLigne--;
					continue;
				}
			}
			if(!colonnepleine[futurecolonne-1])
				return futurecolonne;
		}
		if((offset=strstr(strLignes[cptLigne],"RR R"))!=NULL) 
		{
			futurecolonne=3+(offset-strLignes[cptLigne]);
			
			if(cptLigne<7 && cptLigne>0)
			{
				if(tabPuissance4[cptLigne+1][futurecolonne-1]==' ')		// si il n'y a pas de jeton sur la ligne du dessous...
				{
					Push((void*)&futurecolonne,&ColonnesInterdites,sizeof(int));
					strLignes[cptLigne][futurecolonne-1]='*';
					cptLigne--;
					continue;
				}	
			}
			if(!colonnepleine[futurecolonne-1])
				return futurecolonne;
		}
		if((offset=strstr(strLignes[cptLigne],"J JJ"))!=NULL)
		{
			futurecolonne=2+(offset-strLignes[cptLigne]);
			
			if(cptLigne<7 && cptLigne>0)
			{
				if(tabPuissance4[cptLigne+1][futurecolonne-1]==' ')		// si il n'y a pas de jeton sur la ligne du dessous...
				{
					Push((void*)&futurecolonne,&ColonnesInterdites,sizeof(int));
					strLignes[cptLigne][futurecolonne-1]='*';
					cptLigne--;
					continue;
				}
			}
			if(!colonnepleine[futurecolonne-1])
				return futurecolonne;
		}
		if((offset=strstr(strLignes[cptLigne],"R RR"))!=NULL)  
		{
			futurecolonne=2+(offset-strLignes[cptLigne]);
			
			if(cptLigne<7 && cptLigne>0)
			{
				if(tabPuissance4[cptLigne+1][futurecolonne-1]==' ')		// si il n'y a pas de jeton sur la ligne du dessous...
				{
					Push((void*)&futurecolonne,&ColonnesInterdites,sizeof(int));
					strLignes[cptLigne][futurecolonne-1]='*';
					cptLigne--;
					continue;
					
					//return -1;																					// alors on ne joue pas... ici il n'y a pas d'autres traitements qui suivent donc c'est bon !!			
					// FAUX !! Il se peut (voir puissance4_debug14.png) qu'il y ait sur la même ligne R RR R RR
					// Donc: R RR et RR R provoquent un danger
					// si R RR n'est pas imminent il devient R*RR mais il reste RR R à traiter !! Ce que l'algorithme n'a jamais fait jusqu'à présent :{ 
					// heureusement que je teste ^^
				}
			}
			if(!colonnepleine[futurecolonne-1])
				return futurecolonne;
		}
	}
	return -1;
}
