// ShowTokens

#include <stdio.h>
#include <string.h>

#include "puissance4.h"

#ifndef CLRS

#define CLRS
#define ROUGE	"\x1b[38;2;255;0;0;1m"
#define JAUNE "\x1b[38;2;255;237;32;1m"
#define RESET "\x1b[0m"
#define CLIGN "\x1b[5m"

#endif

// variables externes

unsigned char tabPuissance4[MAXLINS][MAXCOLS];

void ShowTokens(void)
{
	int local_cptX;
	int local_cptY;
	
	char strCouleur[40]="\0";
	
	for(local_cptY=0;local_cptY<MAXLINS;local_cptY++)
	{
		for(local_cptX=0;local_cptX<MAXCOLS;local_cptX++)
		{
			if(tabPuissance4[local_cptY][local_cptX]=='R') strcpy(strCouleur,ROUGE);
			else strcpy(strCouleur,JAUNE);
			printf("[%s%c%s]",strCouleur,tabPuissance4[local_cptY][local_cptX],RESET);
		}
		printf("\n");
	}
}
