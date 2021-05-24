/* 
 * File:   puissance4.h
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Créé le May 13, 2021, 11:49 AM
 *  
 */

#ifndef PUISSANCE4_H
#define PUISSANCE4_H

// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <uep_wide.h>

// **************************************************************************** 
// Définitions des constantes symboliques
// **************************************************************************** 

#ifndef CLRS

#define CLRS
#define ROUGE	"\x1b[38;2;255;0;0;1m"
#define JAUNE "\x1b[38;2;255;237;32;1m"
#define RESET "\x1b[0m"
#define CLIGN "\x1b[5m"

#endif

#define EXIT_ABRT -2
#define SRVPORT		4040

#define MAXCOLS 7
#define MAXLINS 6

// **************************************************************************** 
// Définition(s) des variables statiques/externes
// **************************************************************************** 

extern unsigned char tabPuissance4[MAXLINS][MAXCOLS];
extern Stack	ColonnesInterdites;										// Pour empêcher l'ordinateur de choisir une colonne qui permettrait au joueur de gagner ou de contrer...
extern bool	colonnepleine[MAXLINS];

// **************************************************************************** 
// Définition(s) des structures, types et énumérations
// **************************************************************************** 

typedef struct s_Datas
{
	int		colonne;
	char	name[25];
}t_Datas;

// **************************************************************************** 
// Définition(s) des fonctions
// **************************************************************************** 

void ShowTokens(void);
bool IsAligned(short,short,unsigned char);

int AnalyzeCols(void);
int AnalyzeLines(void);
int AnalyzeDiagonals(void);

bool CanIPlay(int,Pile*);

bool IsAddressIP(char*);

void  ArretPropre(void); 
void  Interruptions(int);

#endif /* PUISSANCE4_H */
