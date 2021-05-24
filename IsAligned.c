#include <stdbool.h>
#include <stdio.h>

#include "puissance4.h"

unsigned char tabPuissance4[MAXLINS][MAXCOLS];

//**********************************************************************************
// algorithme permettant de déterminer si les jetons sont alignés...
//**********************************************************************************
bool IsAligned(short lastdropx,short lastdropy,unsigned char cTarget)
{
	short cptColonne=lastdropx;
	short cptLigne=lastdropy;
	short count;
	
	short sRechercheDernierJeton;

	if(lastdropy<0 && lastdropx<0) return false;
	
	// Le plus simple d'abord en horizontal ou vertical
	
	//**********************************************************************************
	// VERTICALEMENT toujours vers le bas
	//**********************************************************************************
		
	count=0;
	while(cptLigne<MAXLINS) 
	{
		if(tabPuissance4[cptLigne][cptColonne]==cTarget) 
		{
			count++;
		}
		else break;
		cptLigne++;
	}
	if(count>=4) return true;
		
	//**********************************************************************************
	// HORIZONTALEMENT vers la DROITE --> 
	//**********************************************************************************
	
	count=0;
	cptLigne=lastdropy;
	while(cptColonne<MAXCOLS) 
	{
		if(tabPuissance4[cptLigne][cptColonne]==cTarget)
		{
			count++;
		}
		else break;
		cptColonne++;
	}
	if(count>=4) return true;
		
	//**********************************************************************************
	// HORIZONTALEMENT vers la GAUCHE <--
	//**********************************************************************************
	
	count=0; // on a pas trouvé donc il faut recommencer le compte
	cptColonne--; // ...à la sortie de la boucle nous avons déterminé la position en x du dernier jeton le plus à droite...
	while(cptColonne>=0)
	{
		if(tabPuissance4[cptLigne][cptColonne]==cTarget)
		{
			count++;
		}
		else break;
		cptColonne--;
	}
	if(count>=4) return true;
	
	//**********************************************************************************
	// PLUS COMPLEXE: en diagonal
	//**********************************************************************************
	
	// Détermination du sens de la diagonale
	// revenir sur le jeton que l'on vient d'insérer
	
	cptColonne=lastdropx;
	cptLigne=lastdropy;
	count=0;
	
	if(cptLigne==MAXLINS-1) // le jeton est à la base réelle du plateau de jeu
	{
		// deux possibilités en matière de diagonales: en haut à droite ou gauche
		
		if(tabPuissance4[cptLigne-1][cptColonne-1]==cTarget && cptColonne>0) // haut à gauche et éviter "les effets de bord" 
		{
			printf("(%c) diagonale détectée ! HG\n",cTarget);
			for(;tabPuissance4[cptLigne][cptColonne]==cTarget;cptLigne--,cptColonne--)
			{
				count++;
				printf("[HG] (%d,%d) %c (%d)\n",cptColonne,cptLigne,tabPuissance4[cptLigne][cptColonne],count);
			}
			if(count>=4) return true;
			
			// pour pouvoir éventuellement détecter une autre diagonale dont le jeton courant fait partie
			
			cptLigne=lastdropy;
			cptColonne=lastdropx;
		}
		
		// Il se peut que le jeton que l'on vient d'insérer soit compris dans deux diagonales l'une à droite l'autre à gauche...
		// Il faut donc (ici plus haut) remettre cptLigne et cptColonne à leurs valeurs initiales respectives
		// et remettre le compteur de jetons à 0... 
		
		count=0;
		if(tabPuissance4[cptLigne-1][cptColonne+1]==cTarget && cptColonne<MAXCOLS-1) // haut à droite et éviter "les effets de bord"
		{
			printf("(%c) diagonale détectée ! HD\n",cTarget);
			for(;tabPuissance4[cptLigne][cptColonne]==cTarget;cptLigne--,cptColonne++)
			{
				count++;
				printf("[HD] (%d,%d) %c (%d)\n",cptColonne,cptLigne,tabPuissance4[cptLigne][cptColonne],count);
			}
			if(count>=4) return true;
			cptLigne=lastdropy;
			cptColonne=lastdropx;
		}
	}
	
	// Autre cas de figure: le jeton inséré ne se trouve pas sur la dernière ligne...
	
	count=0;						
	if(tabPuissance4[cptLigne+1][cptColonne+1]==cTarget && cptLigne<MAXLINS-1 && cptColonne<MAXCOLS-1) // test vers le bas à droite
	{
		printf("(1) diagonale détectée ! HG/BD\n");
		
		// Nous allons "redescendre" dans la diagonale pour rechercher le jeton le plus en bas...
		
		for(	sRechercheDernierJeton=0;
					tabPuissance4[cptLigne+sRechercheDernierJeton][cptColonne+sRechercheDernierJeton]==cTarget;
					sRechercheDernierJeton++) // nous chercherons toujours le jeton le plus en bas (convention)
		{
			if(cptLigne+sRechercheDernierJeton>MAXLINS-1 || cptColonne+sRechercheDernierJeton>MAXCOLS-1) 
			{
				break;
			}
		}
		printf("1 (%c) dernier jeton de la diagonale le plus en bas: (%d,%d)\n",cTarget,cptColonne+sRechercheDernierJeton-1,cptLigne+sRechercheDernierJeton-1);
		
		for(
				cptLigne=cptLigne+sRechercheDernierJeton-1,cptColonne=cptColonne+sRechercheDernierJeton-1; // -1 puis -1
				tabPuissance4[cptLigne][cptColonne]==cTarget;
				cptLigne--,cptColonne--)
		{
			if(cptColonne>=0) count++;
			printf("[HG/BD] (%d,%d) %c (%d)\n",cptColonne,cptLigne,tabPuissance4[cptLigne][cptColonne],count);
		}
		
		if(count>=4) return true;
		
		// pour pouvoir éventuellement détecter une autre diagonale...
		
		cptLigne=lastdropy;
		cptColonne=lastdropx;
	}
		
	count=0;
	if(tabPuissance4[cptLigne+1][cptColonne-1]==cTarget && cptLigne<MAXLINS-1 && cptColonne>0) // test vers le bas à gauche
	{
		printf("(2) diagonale détectée ! HD/BG\n");
		for(	sRechercheDernierJeton=0;
					tabPuissance4[cptLigne+sRechercheDernierJeton][cptColonne-sRechercheDernierJeton]==cTarget;
					sRechercheDernierJeton++) // nous chercherons toujours le jeton le plus en bas (convention)
		{
			if(cptLigne+sRechercheDernierJeton>MAXLINS-1 || cptColonne-sRechercheDernierJeton<0) 
			{
				break;
			}
		}
		printf("2 (%c) dernier jeton de la diagonale le plus en bas: (%d,%d)\n",cTarget,cptColonne-sRechercheDernierJeton+1,cptLigne+sRechercheDernierJeton-1);
		
		for(
				cptLigne=cptLigne+sRechercheDernierJeton-1,cptColonne=cptColonne-sRechercheDernierJeton+1; // -1 puis +1
				tabPuissance4[cptLigne][cptColonne]==cTarget;
				cptLigne--,cptColonne++)
		{
			if(cptColonne<=MAXCOLS-1)	count++;
			printf("[HD/BG] (%d,%d) %c (%d)\n",cptColonne,cptLigne,tabPuissance4[cptLigne][cptColonne],count);
		}
		
		if(count>=4) return true;
		cptLigne=lastdropy;
		cptColonne=lastdropx;
	}
	
	// oubli des autres directions sorry :{
	// il est rare de pouvoir faire une diagonale au-delà de la ligne 7 et qu'en plus le dernier jeton tombe pile poil dans un cas que je n'ai pas traité...
	// lors de mes tests: 2 et 3 étaient souvent exécutés ce qui est complètement débile, deux traitements pour la même diagonale --> pas digne de Saint Martin !!
	
	count=0;
	if(tabPuissance4[cptLigne-1][cptColonne+1]==cTarget && cptLigne>0 && cptColonne<MAXCOLS-1) // test vers le haut droite
	{
		printf("(3) diagonale détectée ! HD/BG\n");
		for(	sRechercheDernierJeton=0;
					tabPuissance4[cptLigne+sRechercheDernierJeton][cptColonne-sRechercheDernierJeton]==cTarget;
					sRechercheDernierJeton++) // nous chercherons toujours le jeton le plus en bas (convention)
		{
			if(cptLigne+sRechercheDernierJeton>MAXLINS-1 || cptColonne-sRechercheDernierJeton<0) 
			{
				break;
			}
		}
		printf("3 (%c) dernier jeton de la diagonale le plus en bas: (%d,%d)\n",cTarget,cptColonne-sRechercheDernierJeton+1,cptLigne+sRechercheDernierJeton-1);
		
		for(
				cptLigne=cptLigne+sRechercheDernierJeton-1,cptColonne=cptColonne-sRechercheDernierJeton+1; // -1 puis +1
				tabPuissance4[cptLigne][cptColonne]==cTarget;
				cptLigne--,cptColonne++)
		{
			if(cptColonne<=MAXCOLS-1)	count++;
			printf("[HD/BG] (%d,%d) %c (%d)\n",cptColonne,cptLigne,tabPuissance4[cptLigne][cptColonne],count);
		}
		
		if(count>=4) return true;
		cptLigne=lastdropy;
		cptColonne=lastdropx;
	}	
	
	// Je ne suis pas sûr mais il me semble que cette partie de code est redondante et donc inutile... (à tester --> fait !! ça m'énnerve les doublons en matière de traitements...).
	
	count=0;
	if(tabPuissance4[cptLigne-1][cptColonne-1]==cTarget && cptLigne>0 && cptColonne>0) // test vers le haut gauche
	{
		printf("(4) diagonale détectée ! HG/BD\n");
		for(	sRechercheDernierJeton=0;
					tabPuissance4[cptLigne+sRechercheDernierJeton][cptColonne+sRechercheDernierJeton]==cTarget;
					sRechercheDernierJeton++) // nous chercherons toujours le jeton le plus en bas (convention)
		{
			if(cptLigne+sRechercheDernierJeton>7 || cptColonne+sRechercheDernierJeton>MAXCOLS-1) 
			{
				break;
			}
		}
		printf("4 (%c) dernier jeton de la diagonale le plus en bas: (%d,%d)\n",cTarget,cptColonne+sRechercheDernierJeton-1,cptLigne+sRechercheDernierJeton-1);
		
		for(
				cptLigne=cptLigne+sRechercheDernierJeton-1,cptColonne=cptColonne-sRechercheDernierJeton+1; // -1 puis +1
				tabPuissance4[cptLigne][cptColonne]==cTarget;
				cptLigne--,cptColonne--)
		{
			if(cptColonne<=MAXCOLS-1)	count++;
			printf("[HG/BD] (%d,%d) %c (%d)\n",cptColonne,cptLigne,tabPuissance4[cptLigne][cptColonne],count);
		}
		
		if(count>=4) return true;
	}
	
	return false;
}
