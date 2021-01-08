// TODO permettre à l'ordinateur de contrer des situations de danger en diagonales... (pas envie de le faire pour le moment --> élèves)
// FAIT et ça marche !! A tester plus en profondeur...

// Vous aurez remarqué que nous utilisons des références à des variables globales 
// dans quasiment toutes les fonctions de ce programme... ce qui est interdit !!

// Normalement toutes ces fonctions devraient se trouver dans d'autres fichiers sources
// mais comme j'ai envie d'aller au plus simple (pour commencer) j'ai mis toutes les
// fonctions dans le code source principal...


// L'étape suivante consistera à séparer les fonctions dans un autre fichier source
// et là, le fait d'avoir utilisé des variables globales va nous poser des soucis
// nous devrons donc "passer" ces variables aux fonctions (ce que je ne fais pas
// pour l'instant -- cette fonction montre une manière de procéder).

// Au niveau du fonctionnement du jeu... à partir d'un certain moment, l'ordinateur ne sait plus choisir de colonne il va falloir gérer cela
// car le joueur est OBLIGE de jouer une colonne (cette technique est appelée "forcer la colonne")... je ne la gère pas encore :{

// Dans les parties de débug j'ai plusieurs fois eu un soucis où l'ordinateur "pétait un plomb" il ne pouvait jouer que la colonne 5
// et j'ai eu une boucle infinie... (bin oui si je suis assez naze pour oublier de modifier le nombre d'éléments dans EmptyStack())

#ifndef __UEPWIDE__
#include <uep_wide.h> 
#endif



bool CanIPlay(int targetcolumn,Pile *LaPile)
{
	// Le contenu de la pile est recréé à chaque analyse des lignes et des diagonales (à vérifier pour être sûr).
	while(LaPile->nbElem>0)
	{
		int *popped=Pop(LaPile,sizeof(int));
#ifdef DEBUG
		printf("popped: %d (Colonne choisie: %d)\n",*popped,targetcolumn);
#endif
		
		if(*popped==targetcolumn) 
		{
			Push((void*)&targetcolumn,LaPile,sizeof(int));				// oui bin il faut remettre l'élément dans la pile (après vérifications)
			return false;
		}
	}
	return true;
} 
