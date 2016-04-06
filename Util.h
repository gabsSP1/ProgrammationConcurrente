//-----Utilitaires---- fichier Util.h

#ifndef UTIL_H
#define UTIL_H


#include <sys/msg.h>
#include <sys/time.h>
#include <Outils.h>


//les messages echanges par les boites
struct msgvoit{
	long type;//prend en entree typeBarriere
	TypeUsager typeUsager;//AUCUN implique une place vide
	time_t heure;//obtenu avec time(NULL)
	unsigned int numvoit;
	int place;//a 0 pour l'entree, utilise uniquement par la sortie (dans ce cas de 1 à 8)
};
//structure pour faciliter l'utilisation de la memoire partagee
struct memPartagee{
	int nbPlacesLibres;
	struct msgvoit enAttente[3];
	struct msgvoit places[8];
	
};


#endif
