/*************************************************************************
                           Entree  -  description
                             -------------------
    début                : 
    copyright            : (C) Entree par gspecq sbelletier
    e-mail               : 
*************************************************************************/

//---------- Réalisation de la tâche <Entree> (fichier Entree.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <map>
//------------------------------------------------------ Include personnel
#include "Mere.h"


///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static int bE;
static int memoireId;
static TypeBarriere typeBarriere;
static map<pid_t, msgvoit> mapVoiture;
static int sMutex, sAttente;
static memPartagee mem;
static sembuf proberen;
static sembuf verhogen;
//------------------------------------------------------ Fonctions privées
static void stockerPlaceVoiture ( int noSignal )
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
{
	
	if ( noSignal == SIGCHLD )
	{
		int status;
		pid_t pid = waitpid ( -1, &status, 0 );
		msgvoit message = mapVoiture[pid];
		mapVoiture.erase ( pid );
		status = WEXITSTATUS (status );
		semop ( sMutex, &proberen, 1 );
		mem.Places[status] = message;
		semop ( sMutex, &verhogen , 1 );
		AfficherPlace ( status, message.typeUsager, message.numvoit, message.heure );
	
		
	}
	
	else
	{
		exit(0);
	}	

} //----- fin de nom

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Entree ( int boite, int memoire, TypeBarriere typeBarrieres, int semMutex, int semAttente )
// Algorithme :
//
{
		sMutex = semMutex;
		sAttente = semAttente;
		proberen = {0, -1, 0};
		verhogen = {0, 1, 0};
		memoireId = memoire;
		mem = *(( memPartagee* ) shmat ( memoire, NULL, 0));
		struct sigaction action;
		action.sa_handler = stockerPlaceVoiture;
		sigemptyset ( &action.sa_mask );
		action.sa_flags = SA_RESTART;
		sigaction ( SIGUSR2, &action, NULL );
		sigaction ( SIGCHLD, &action, NULL );
		typeBarriere = typeBarrieres;
		bE = boite;
		int p;
		for(;;)
		{
			msgvoit messageRecu;
			while (msgrcv ( bE, (void*) &messageRecu, sizeof ( msgvoit ), 0,  MSG_NOERROR) == -1);
			DessinerVoitureBarriere ( typeBarriere, messageRecu.typeUsager ); 
			semop ( sMutex, &proberen, 1 );
			if (mem.nbPlacesLibres == 0)
			{
				mem.enAttente[typeBarriere-1] = messageRecu;
				semop ( sAttente, &proberen, 1);
				AfficherRequete ( typeBarriere, messageRecu.typeUsager, messageRecu.heure );
			}
			else
			{			
				mem.nbPlacesLibres--;
			}	
			semop ( sMutex, &verhogen, 1 );
			Afficher ( REQUETE_R3, semctl(sAttente, 0, GETVAL, p));
			while ( semop ( sAttente, &proberen, 1 ) ==-1);
			Afficher ( REQUETE_R2, semctl(sAttente, 0, GETVAL, p));
			pid_t pidVoiturier = GarerVoiture ( typeBarriere );
			mapVoiture[pidVoiturier] = messageRecu;
			semop ( sAttente, &verhogen, 1 );
		}
} //----- fin de Nom

