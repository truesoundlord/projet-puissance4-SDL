// Cette fonction consiste à transformer les colonnes en lignes
// de manière a détecter facilement les ensembles de trois (3) 'R' ou 'J'

#include <string.h>
#include <stdbool.h>

// variables externes

unsigned char tabPuissance4[8][8];
bool	colonnepleine[8];

int AnalyzeCols(void)
{
	int		cptLigne;
	int		cptColonne;
	char 	strColonnes[8][9];
	char	*offset;
	
	for(cptColonne=0;cptColonne<8;cptColonne++)
	{
		for(cptLigne=0;cptLigne<8;cptLigne++)
		{
			strColonnes[cptColonne][cptLigne]=tabPuissance4[cptLigne][cptColonne];	// on copie l'élément de la colonne et on le place sur une ligne (rotation de 90° vers la gauche)
		}
		strColonnes[cptColonne][cptLigne]='\0';
	}
	for(cptColonne=0;cptColonne<8;cptColonne++)
	{
		if((offset=strstr(strColonnes[cptColonne]," JJJ"))!=NULL)
		{
			if(!colonnepleine[cptColonne])
				return cptColonne+1;
		}
		if((offset=strstr(strColonnes[cptColonne]," RRR"))!=NULL)
		{
			if(!colonnepleine[cptColonne])
				return cptColonne+1;
		}
	}
	return -1;
}
