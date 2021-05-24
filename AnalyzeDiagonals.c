// AnalyzeDiagonals
// Fonction qui permet de détecter l'alignement de trois 'R' ou 'J'.

#include <stdio.h>
#include <string.h>

#ifndef __UEPWIDE__
#include <uep_wide.h>
#endif

#include "puissance4.h"

unsigned char tabPuissance4[MAXLINS][MAXCOLS];
bool	colonnepleine[MAXLINS];
Stack	ColonnesInterdites;										// Pour empêcher l'ordinateur de choisir une colonne qui permettrait au joueur de gagner ou de contrer...

#define DEBUG

#define SD_MAXCOLS	((MAXCOLS/2)+1)
#define SD_MINCOLS	((MAXCOLS/2)-1)
#define SD_MAXBASE	3
#define MAXDIAGS		4
#define SD_LENGTH		(MAXDIAGS*2)

int AnalyzeDiagonals(void)
{
	// Partie la plus compliquée: elle consiste à transformer une diagonale en ligne en conservant les valeurs se trouvant dans cette diagonale.

	int		cptChercherCaseVide;

	bool	bDanger=false;						// il y a danger à true
	bool	bOpportunite=false;				// il y a possibilité de gagner (à true)
	bool  bDirection=false;					// true: gauche false: droite
	
	short	cptLigne=MAXLINS;					// on commencera toujours de la base de la première zone (voir plus bas) 
	short	cptColonne=0;							// on commencera toujours par la gauche pour aller vers la droite
	short	sNumDiagonale=0;						
	short	sBase=0;												// on commence à la base du tableau puis on "remonte" d'une ligne
	
	char	strBalayageDiagonales[SD_LENGTH][5];		// ???? pourquoi 10 diagonales ???? 

	
	// Voir document .pdf pour comprendre l'approche (je ne sais pas si il y a mieux... cherchez, trouvez et démontrez !!).
	while(sBase<SD_MAXBASE)
	{
		// Premier balayage: vers la droite
		
		for(sNumDiagonale=0;sNumDiagonale<MAXDIAGS;sNumDiagonale++)																														// permet de déplacer la base de la diagonale vers la droite... 
		{
			for(cptColonne=0,cptLigne=(MAXLINS-1)-sBase;cptColonne<SD_MAXCOLS;cptColonne++,cptLigne--)													// permet de sélectionner la case de la diagonale
			{
				strBalayageDiagonales[sNumDiagonale][cptColonne]=tabPuissance4[cptLigne][cptColonne+sNumDiagonale];
			}
			strBalayageDiagonales[sNumDiagonale][4]='\0';	// sinon effet de bord garanti...
		}

		// Second balayage: vers la gauche
		
		for(sNumDiagonale=0;sNumDiagonale<MAXDIAGS;sNumDiagonale++)																														 
		{
			for(cptColonne=MAXCOLS-1,cptLigne=(MAXLINS-1)-sBase;cptColonne>SD_MINCOLS;cptColonne--,cptLigne--)																				// (>3)										
			{
				strBalayageDiagonales[(SD_LENGTH-1)-sNumDiagonale][MAXCOLS-cptColonne-1]=tabPuissance4[cptLigne][cptColonne-sNumDiagonale];
			}
			strBalayageDiagonales[(SD_LENGTH-1)-sNumDiagonale][4]='\0';	// sinon effet de bord garanti...
		}
				
		for(sNumDiagonale=0,cptLigne=(MAXLINS-1)-sBase;sNumDiagonale<SD_LENGTH;sNumDiagonale++)
		{
#ifdef DEBUG
			short sDebug;
			if(sNumDiagonale>=4) sDebug=1;
			else sDebug=0;
			printf("[AD][%d][%d][%s][%-4s]\n",sBase,sNumDiagonale,sDebug?"droite":"gauche",strBalayageDiagonales[sNumDiagonale]);
#endif
			
			// L'attaque est la meilleure défense (nous mettons en priorité le coup gagnant...)
			
			if(strstr(strBalayageDiagonales[sNumDiagonale],"JJJ ")!=NULL)
			{
				bOpportunite=true;
				if(sNumDiagonale>=4) // partie gauche (la diagonale voit sa base à droite: les indices des diagonales seront logiquement égaux ou plus grand que 4)
				{
					bDirection=true;
				}
				else // partie droite (la diagonale voit sa base à gauche: les indices des diagonales seront plus petit que 4)
				{
					bDirection=false;
				}
			}
			if(strstr(strBalayageDiagonales[sNumDiagonale],"J JJ")!=NULL)
			{
				bOpportunite=true;
				if(sNumDiagonale>=4) 
				{
					bDirection=true;
				}
				else 
				{
					bDirection=false;
				}
			}
			if(strstr(strBalayageDiagonales[sNumDiagonale],"JJ J")!=NULL)
			{
				bOpportunite=true;
				if(sNumDiagonale>=4) 
				{
					bDirection=true;
				}
				else 
				{
					bDirection=false;
				}
			}
			if(strstr(strBalayageDiagonales[sNumDiagonale]," JJJ")!=NULL)
			{
				bOpportunite=true;
				if(sNumDiagonale>=4) 
				{
					bDirection=true;
				}
				else 
				{
					bDirection=false;
				}
			}
			
			// Défense 
			
			if(strstr(strBalayageDiagonales[sNumDiagonale],"RRR ")!=NULL)			
			{
				bDanger=true;
				if(sNumDiagonale>=4) 
				{
					bDirection=true;
				}
				else 
				{
					bDirection=false;
				}
				
			}
			if(strstr(strBalayageDiagonales[sNumDiagonale],"R RR")!=NULL)			
			{
				bDanger=true;
				if(sNumDiagonale>=4) 
				{
					bDirection=true;
				}
				else 
				{
					bDirection=false;
				}
			}
			if(strstr(strBalayageDiagonales[sNumDiagonale],"RR R")!=NULL)
			{
				bDanger=true;
				if(sNumDiagonale>=4) 
				{
					bDirection=true;
				}
				else 
				{
					bDirection=false;
				}
			}
			if(strstr(strBalayageDiagonales[sNumDiagonale]," RRR")!=NULL)
			{
				bDanger=true;
				if(sNumDiagonale>=4) 
				{
					bDirection=true;
				}
				else 
				{
					bDirection=false;
				}
			}
			if(bDanger || bOpportunite)
			{
				// Qu'est-ce que ça implique que bDirection soit à "true" (vrai) ?
				// Cela implique que la diagonale pour laquelle il y a soit un danger ou une opportunité est dirigée vers le HAUT à GAUCHE (x-- et y--).
				
				// cptColonne est censé être la colonne qui contient le jetton le plus en bas dans la diagonale
				
				if(bDirection) // true (scan vers la gauche)
				{
					cptColonne=sNumDiagonale-1; // ??? je sais pas pourquoi ???
#ifdef DEBUG
					printf("[g] cptcolonne: %d (%d)\n",cptColonne,sNumDiagonale);			// DEBUG correct
#endif
					
					// Chercher la première case qui ne contient rien (ASCII 32) dans les limites du tableau et dans la diagonale
				
					cptChercherCaseVide=0;
					while(cptLigne-cptChercherCaseVide>=0 && cptColonne-cptChercherCaseVide>=0)
					{
						if(tabPuissance4[cptLigne-cptChercherCaseVide][cptColonne-cptChercherCaseVide]==32) break;
						cptChercherCaseVide++;
					}
								
					// Vérifier qu'il y a bien un jeton en-dessous dans la colonne détectée
					
					// S'inspirer de la manière de chercher la première case vide dans la colonne (fonction main()) pour vérifier sinon la colonne risque d'être bloquée
					// alors que le danger ou l'opportunité n'est pas du tout immédiat !!
										
					int lignecasevide;
					int futurecolonne=cptColonne-cptChercherCaseVide+1;							// ?? en général cptcolonne contient l'indice de la colonne correct ??
					
					for(lignecasevide=0;lignecasevide<MAXLINS;lignecasevide++)
					{
						if(tabPuissance4[lignecasevide][futurecolonne-1]!=' ')
							break;
					}
					lignecasevide--;
					
#ifdef DEBUG
					printf("[g] ligne contenant la case vide: %d (cible: %d,%d) case du dessous de la case cible --> %d\n",lignecasevide,futurecolonne-1,cptLigne-cptChercherCaseVide,cptLigne-cptChercherCaseVide+1);
#endif
					
					// nous obtenons dans lignecasevide le numéro de la première ligne vide...
					
					if(tabPuissance4[cptLigne-cptChercherCaseVide+1][futurecolonne-1]!=32)
					{
						if(!colonnepleine[futurecolonne-1])
							return futurecolonne;
					}
					else 
					{
						if(bDanger)
						{
#ifdef DEBUG
							printf("(g) Il y a du danger mais pas immédiatement...\n");
#endif
							if(lignecasevide==cptLigne-cptChercherCaseVide+1)
								Push((void*)&futurecolonne,&ColonnesInterdites,sizeof(int));
							bDanger=false;
						}
						else
						{
#ifdef DEBUG
							printf("(g) Je peux gagner mais pas maintenant...\n");
#endif
							if(lignecasevide==cptLigne-cptChercherCaseVide+1)
								Push((void*)&futurecolonne,&ColonnesInterdites,sizeof(int));
							bOpportunite=false;
						}
					}
				}
				else	// false (scan vers la droite)
				{
					cptColonne=sNumDiagonale;
					
					// Chercher la première case qui ne contient rien (ASCII 32) dans les limites du tableau (horrible boucle inadaptée, berk !! caca !!)
					
					cptChercherCaseVide=0;
					while(cptLigne-cptChercherCaseVide>=0 && cptColonne+cptChercherCaseVide<=MAXCOLS-1)
					{
						if(tabPuissance4[cptLigne-cptChercherCaseVide][cptColonne+cptChercherCaseVide]==32) break;
						cptChercherCaseVide++;
					}
										
					// Vérifier qu'il y a bien un jeton en-dessous

					int lignecasevide;
					int futurecolonne=cptColonne+cptChercherCaseVide+1;
					
					
					for(lignecasevide=0;lignecasevide<MAXLINS;lignecasevide++)
					{
						if(tabPuissance4[lignecasevide][futurecolonne-1]!=' ')
							break;
					}
					lignecasevide--;
					
#ifdef DEBUG
					printf("[d] ligne contenant la case vide: %d (cible: %d,%d) case du dessous de la case cible --> %d\n",lignecasevide,futurecolonne-1,cptLigne-cptChercherCaseVide,cptLigne-cptChercherCaseVide+1);
#endif
					
					if(tabPuissance4[cptLigne-cptChercherCaseVide+1][futurecolonne-1]!=32)
					{
						if(!colonnepleine[futurecolonne-1])
							return futurecolonne;
					}
					else 
					{
						if(bDanger)
						{
#ifdef DEBUG
							printf("(d) Il y a du danger mais pas immédiatement...\n");
#endif
							if(lignecasevide==cptLigne-cptChercherCaseVide+1)
								Push((void*)&futurecolonne,&ColonnesInterdites,sizeof(int));
							bDanger=false;
						}
						else
						{
#ifdef DEBUG
							printf("(d) Je peux gagner mais pas maintenant...\n");
#endif
							if(lignecasevide==cptLigne-cptChercherCaseVide+1)
								Push((void*)&futurecolonne,&ColonnesInterdites,sizeof(int));
							bOpportunite=false;
						}
					}
				}
			}
		}
		sBase++;
	}
	return -1;
}
