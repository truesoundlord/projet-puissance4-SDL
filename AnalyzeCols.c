// Cette fonction consiste à transformer les colonnes en lignes
// de manière a détecter facilement les ensembles de trois (3) 'R' ou 'J'

#include <string.h>
#include <stdbool.h>

#include "puissance4.h"

// variables externes

unsigned char tabPuissance4[MAXLINS][MAXCOLS];
bool	colonnepleine[MAXLINS];

//#define DEBUG2021 

int AnalyzeCols(void)
{
	int		cptLigne;
	int		cptColonne;
	char 	strColonnes[MAXCOLS][MAXLINS+1]={{0}};
	char	*offset;
	
	for(cptColonne=0;cptColonne<MAXCOLS;cptColonne++)	
	{
		for(cptLigne=0;cptLigne<MAXLINS;cptLigne++)
		{
			strColonnes[cptColonne][cptLigne]=tabPuissance4[cptLigne][cptColonne];	// on copie l'élément de la colonne et on le place sur une ligne (rotation de 90° vers la gauche)
		}
		strColonnes[cptColonne][cptLigne]='\0';
#ifdef DEBUG2021
		printf("[%s]\n",strColonnes[cptColonne]);
#endif
	}
	for(cptColonne=0;cptColonne<MAXCOLS;cptColonne++)
	{
		if((offset=strstr(strColonnes[cptColonne]," JJJ"))!=NULL)
		{
#ifdef DEBUG2021
			printf("[%s] 'J' cptcolonne: %d\n",__func__,cptColonne);
#endif
			if(!colonnepleine[cptColonne])
				return cptColonne+1;
		}
		if((offset=strstr(strColonnes[cptColonne]," RRR"))!=NULL)
		{
#ifdef DEBUG2021
			printf("[%s] 'R' cptcolonne: %d\n",__func__,cptColonne);
#endif
			if(!colonnepleine[cptColonne])
				return cptColonne+1;
		}
	}
	return -1;
}
