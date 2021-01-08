// AnalyzeDiagonals
// Fonction qui permet de détecter l'alignement de trois 'R' ou 'J'.

#include <stdio.h>
#include <string.h>

#ifndef __UEPWIDE__
#include <uep_wide.h>
#endif

unsigned char tabPuissance4[8][8];
bool	colonnepleine[8];
Stack	ColonnesInterdites;										// Pour empêcher l'ordinateur de choisir une colonne qui permettrait au joueur de gagner ou de contrer...

//#define DEBUG

int AnalyzeDiagonals(void)
{
	// Partie la plus compliquée: elle consiste à transformer une diagonale en ligne en conservant les valeurs se trouvant dans cette diagonale.
	short	cptLigne=7;								// on commencera toujours de la base de la première zone (voir plus bas) 
	short	cptColonne=0;							// on commencera toujours par la gauche pour aller vers la droite
	short	sNumDiagonale=0;						
	short	sBase=0;
	char	strBalayageDiagonales[10][5];
	//char	*offset;
	short	iColonneJetonLePlusBas;
	int		cptChercherCaseVide;
	
	bool	bDanger=false;						// il y a danger à true
	bool	bOpportunite=false;				// il y a possibilité de gagner (à true)
	bool  bDirection=false;					// true: gauche false: droite
	
	// Voir document .pdf pour comprendre l'approche (je ne sais pas si il y a mieux... cherchez, trouvez et démontrez !!).
	while(sBase<5)
	{
		// Premier balayage: vers la droite
		
		for(sNumDiagonale=0;sNumDiagonale<5;sNumDiagonale++)																														// permet de déplacer la base de la diagonale vers la droite... 
		{
			for(cptColonne=0,cptLigne=7-sBase;cptColonne<5;cptColonne++,cptLigne--)																				// permet de sélectionner la case de la diagonale (<4))
			{
				strBalayageDiagonales[sNumDiagonale][cptColonne]=tabPuissance4[cptLigne][cptColonne+sNumDiagonale];
			}
			strBalayageDiagonales[sNumDiagonale][4]='\0';	// sinon effet de bord garanti...
		}

		// Second balayage: vers la gauche
		
		for(sNumDiagonale=0;sNumDiagonale<5;sNumDiagonale++)																														 
		{
			for(cptColonne=7,cptLigne=7-sBase;cptColonne>2;cptColonne--,cptLigne--)																				// (>3)										
			{
				strBalayageDiagonales[9-sNumDiagonale][7-cptColonne]=tabPuissance4[cptLigne][cptColonne-sNumDiagonale];
			}
			strBalayageDiagonales[9-sNumDiagonale][4]='\0';	// sinon effet de bord garanti...
		}
				
		for(iColonneJetonLePlusBas=0,cptLigne=7-sBase;iColonneJetonLePlusBas<10;iColonneJetonLePlusBas++)
		{
#ifdef DEBUG
			printf("[AD][%d][%d][%-4s]\n",sBase,iColonneJetonLePlusBas,strBalayageDiagonales[iColonneJetonLePlusBas]);
#endif
			
			// L'attaque est la meilleure défense (nous mettons en priorité le coup gagnant...)
			
			if(strstr(strBalayageDiagonales[iColonneJetonLePlusBas],"JJJ ")!=NULL)
			{
				bOpportunite=true;
				if(iColonneJetonLePlusBas>=5) // partie gauche
				{
					bDirection=true;
				}
				else // partie droite
				{
					bDirection=false;
				}
			}
			if(strstr(strBalayageDiagonales[iColonneJetonLePlusBas],"J JJ")!=NULL)
			{
				bOpportunite=true;
				if(iColonneJetonLePlusBas>=5) // partie gauche
				{
					bDirection=true;
				}
				else // partie droite
				{
					bDirection=false;
				}
			}
			if(strstr(strBalayageDiagonales[iColonneJetonLePlusBas],"JJ J")!=NULL)
			{
				bOpportunite=true;
				if(iColonneJetonLePlusBas>=5) // partie gauche
				{
					bDirection=true;
				}
				else // partie droite
				{
					bDirection=false;
				}
			}
			if(strstr(strBalayageDiagonales[iColonneJetonLePlusBas]," JJJ")!=NULL)
			{
				bOpportunite=true;
				if(iColonneJetonLePlusBas>=5) // partie gauche
				{
					bDirection=true;
				}
				else // partie droite
				{
					bDirection=false;
				}
			}
			
			// Défense 
			
			if(strstr(strBalayageDiagonales[iColonneJetonLePlusBas],"RRR ")!=NULL)			
			{
				bDanger=true;
				if(iColonneJetonLePlusBas>=5) // partie gauche
				{
					bDirection=true;
				}
				else // partie droite
				{
					bDirection=false;
				}
				
			}
			if(strstr(strBalayageDiagonales[iColonneJetonLePlusBas],"R RR")!=NULL)			
			{
				bDanger=true;
				if(iColonneJetonLePlusBas>=5) // partie gauche
				{
					bDirection=true;
				}
				else // partie droite
				{
					bDirection=false;
				}
			}
			if(strstr(strBalayageDiagonales[iColonneJetonLePlusBas],"RR R")!=NULL)
			{
				bDanger=true;
				if(iColonneJetonLePlusBas>=5) // partie gauche
				{
					bDirection=true;
				}
				else // partie droite
				{
					bDirection=false;
				}
			}
			if(strstr(strBalayageDiagonales[iColonneJetonLePlusBas]," RRR")!=NULL)
			{
				bDanger=true;
				if(iColonneJetonLePlusBas>=5) // partie gauche
				{
					bDirection=true;
				}
				else // partie droite
				{
					bDirection=false;
				}
			}
			if(bDanger || bOpportunite)
			{
				// Qu'est-ce que ça implique que bDirection soit à "true" (vrai) ?
				// Cela implique que la diagonale pour laquelle il y a soit un danger ou une opportunité est dirigée vers le HAUT à GAUCHE (x-- et y--).
				
				if(bDirection) // true (scan vers la gauche)
				{
					cptColonne=iColonneJetonLePlusBas-2; // ??? je sais pas pourquoi ???
					
					// Chercher la première case qui ne contient rien (ASCII 32) dans les limites du tableau et dans la diagonale
					
					// exemple de "boucle for" qui fout la merde dans la logique, un while aurait été beaucoup plus convenable ET LISIBLE (logiquement parlant)
					
/*				for(cptChercherCaseVide=0;tabPuissance4[cptLigne-cptChercherCaseVide][cptColonne-cptChercherCaseVide]!=32 && 
																		cptLigne-cptChercherCaseVide>=0 &&
																		cptColonne-cptChercherCaseVide>=0;cptChercherCaseVide++)
					{
#ifdef DEBUG
 						printf("(g)[DEBUG] (%d,%d) (%c)\n",	cptColonne-cptChercherCaseVide,cptLigne-cptChercherCaseVide,
																								tabPuissance4[cptLigne-cptChercherCaseVide][cptColonne-cptChercherCaseVide]);
#endif
					}
#ifndef DEBUG
					;
#endif
*/
					
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
					int futurecolonne=cptColonne-cptChercherCaseVide+1;
					
					for(lignecasevide=0;lignecasevide<8;lignecasevide++)
					{
						if(tabPuissance4[lignecasevide][futurecolonne-1]!=' ')
							break;
					}
					lignecasevide--;
					//printf("[g] lignecasevide: %d (cible: %d,%d) case du dessous de la case cible %d\n",lignecasevide,futurecolonne-1,cptLigne-cptChercherCaseVide,cptLigne-cptChercherCaseVide+1);
					
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
					cptColonne=iColonneJetonLePlusBas;
					
					// Chercher la première case qui ne contient rien (ASCII 32) dans les limites du tableau (horrible boucle inadaptée, berk !! caca !!)
					
/*					for(cptChercherCaseVide=0;tabPuissance4[cptLigne-cptChercherCaseVide][cptColonne+cptChercherCaseVide]!=32 &&
																		cptLigne-cptChercherCaseVide>=0 &&
																		cptColonne+cptChercherCaseVide<=7;cptChercherCaseVide++)
					{
#ifdef DEBUG
						printf("(d)[DEBUG] (%d,%d) (%c)\n",	cptColonne+cptChercherCaseVide,cptLigne-cptChercherCaseVide,
																								tabPuissance4[cptLigne-cptChercherCaseVide][cptColonne+cptChercherCaseVide]);
#endif
					}
#ifndef DEBUG
					;
#endif
 
*/ 
					cptChercherCaseVide=0;
					while(cptLigne-cptChercherCaseVide>=0 && cptColonne+cptChercherCaseVide<=7)
					{
						if(tabPuissance4[cptLigne-cptChercherCaseVide][cptColonne+cptChercherCaseVide]==32) break;
						cptChercherCaseVide++;
					}
										
					// Vérifier qu'il y a bien un jeton en-dessous

					int lignecasevide;
					int futurecolonne=cptColonne+cptChercherCaseVide+1;
					
					
					for(lignecasevide=0;lignecasevide<8;lignecasevide++)
					{
						if(tabPuissance4[lignecasevide][futurecolonne-1]!=' ')
							break;
					}
					lignecasevide--;
					//printf("[d] lignecasevide: %d (cible: %d,%d) case du dessous de la case cible %d\n",lignecasevide,futurecolonne-1,cptLigne-cptChercherCaseVide,cptLigne-cptChercherCaseVide+1);
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
