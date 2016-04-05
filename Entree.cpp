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
#include <iostream>
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
static memPartagee* mem;
static memPartagee* memTest;
static sembuf proberen;
static sembuf verhogen;
//------------------------------------------------------ Fonctions privées
static void terminerTacheEntree()
{
	for(map<pid_t, msgvoit>::iterator it = mapVoiture.begin(); it != mapVoiture.end(); it++)
	{
		if( it->first != -1 )
		{
			kill (it->first , SIGUSR2);	
		}
	}
	for(map<pid_t, msgvoit>::iterator it = mapVoiture.begin(); it != mapVoiture.end(); it++)
	{
		waitpid (it->first , NULL, 0);	
	}
	shmdt( (void*) mem);
	exit(0);
}

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
		while ( semop ( sMutex, &proberen, 1 ) == -1);
		mem->places[status - 1] = message;
		semop ( sMutex, &verhogen , 1 );
		AfficherPlace ( status, message.typeUsager, message.numvoit, message.heure );
	
		
	}
	
	else
	{
		terminerTacheEntree();
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
		mem = ( memPartagee* ) (shmat ( memoire, NULL, 0));
		struct sigaction action;
		action.sa_handler = stockerPlaceVoiture;
		sigemptyset ( &action.sa_mask );
		action.sa_flags = 0;
		sigaction ( SIGUSR2, &action, NULL );
		sigaction ( SIGCHLD, &action, NULL );
		typeBarriere = typeBarrieres;
		bE = boite;
		int p;
		for(;;)
		{
			msgvoit messageRecu;
			while ( msgrcv ( bE, (void*) &messageRecu, sizeof ( msgvoit ), 0,  MSG_NOERROR) == -1);
			DessinerVoitureBarriere ( typeBarriere, messageRecu.typeUsager ); 
			while (semop ( sMutex, &proberen, 1 )==-1);
			if (mem->nbPlacesLibres == 0)
			{
				mem->enAttente[typeBarriere-1] = messageRecu;
				semop ( sAttente, &proberen, 1);
			}
			else
			{			
				mem->nbPlacesLibres--;
			}	
			while ( semop ( sMutex, &verhogen, 1 ) == -1 );
			//Afficher ( REQUETE_R3, semctl(sAttente, 0, GETVAL, p));
			while ( semop ( sAttente, &proberen, 1 ) ==-1);
			Afficher ( REQUETE_R2, semctl(sAttente, 0, GETVAL, p));
			pid_t pidVoiturier = GarerVoiture ( typeBarriere );
			Afficher ( REQUETE_R2, messageRecu.numvoit);
			mapVoiture[pidVoiturier] = messageRecu;
			while ( semop ( sAttente, &verhogen, 1 ) == -1);
			sleep(1);
		}
} //----- fin de Nom

