//-----Interface de la mere---- fichier Mere.h

#ifndef MERE_H
#define MERE_H

#include <sys/msg.h>
#include <sys/time.h>
#include <Outils.h>


//les messages echanges par les boites
struct msgvoit{
	long type;//on balancera typeBarriere + type "vide"
	TypeUsager typeUsager;//AUCUN -> VIDE !!!!
	time_t heure;//obtenu avec time(NULL)
	unsigned int numvoit;
	int place;//a 0 pour l'entree utile uniquement pour la sortie (dans ce cas de 1 à 8)
};
//structure pour faciliter l'utilisation de la memoire partagee
struct memPartagee{
	int nbPlacesLibres;
	struct msgvoit enAttente[3];
	struct msgvoit places[8];
	
};

int main();
//Mode d'emploi: 
//processus principal

#endif
