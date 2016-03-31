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
};
//structure pour faciliter l'utilisation de la memoire partagee
struct memPartagee{
	int nbPlacesLibres;
	struct msgvoit enAttente[3];
	struct msgvoit Places[8];
	
};

int main();
//Mode d'emploi: 
//processus principal

#endif
