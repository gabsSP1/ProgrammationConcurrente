/*************************************************************************
                           Sortie  -  description
                             -------------------
    début                : XXX
    copyright            : (C) XXX par XXX
    e-mail               : XXX
*************************************************************************/

//---------- Réalisation de la tâche <Sortie> (fichier Sortie.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <map>
#include <cstring>//pour tester
//------------------------------------------------------ Include personnel
#include "Sortie.h"
#include "Mere.h"
#include <Outils.h>
///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques

//------------------------------------------------------ Fonctions privées
//static type nom ( liste de paramètres )
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
//{
//} //----- fin de nom

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

static memPartagee* memSh; 
static map<pid_t, msgvoit> voituriers;
static struct sembuf reserver{0, -1, 0};
static struct sembuf liberer{0, 1, 0};//operations sur mutex
static int mutexMem;
static int semAtt [3];
//handler de mort
static void handlerSig(int noSignal)
{
	if( noSignal == SIGUSR2 )
	{
		map<pid_t, msgvoit>::iterator it = voituriers.begin(); 
		for( it = voituriers.begin(); it != voituriers.end(); it++ )
		{
			if( it->first != -1 ){
				kill( it->first, SIGUSR2 );
			}
		}
		shmdt( memSh );
		exit(0);
	}
	
	else if( noSignal == SIGCHLD )
	{
		int status;
		pid_t pid = waitpid ( -1, &status, 0 );
		msgvoit msg = voituriers[pid];
		voituriers.erase ( pid );
		AfficherSortie( msg.typeUsager, msg.numvoit, msg.heure, time(NULL));
		while ( semop( mutexMem, &reserver, 1 ) == -1);
		int minInd = -1;
		int min =10;
		time_t minT;
		for (int i=0; i<3; i++)
		{
			time_t tempT = memSh->enAttente[i].heure;
			int temp = memSh->enAttente[i].typeUsager;
			
			if ( temp !=0 && ( temp < min || ( temp == min && tempT < minT ) ) )
			{
				minInd = i;
				min = temp;
				minT = tempT;
			}
		}
		if ( minInd != -1 )
		{
			semop(semAtt [minInd], &liberer, 1);
			Effacer ( (TypeZone) (10 + minInd) );
			memSh->enAttente[minInd].typeUsager = AUCUN;
		}
		else
		{
			memSh->nbPlacesLibres += 1;
		}
		semop( mutexMem, &liberer, 1 );
	}
	
}


void Sortie ( int balS, int att1, int att2, int att3, int mem, int mutMem )
// Algorithme :
//
{	
	//handler pour une fin propre
	struct sigaction finSortie;
	finSortie.sa_handler = handlerSig;
	sigemptyset( &finSortie.sa_mask );
	finSortie.sa_flags = 0;
	sigaction( SIGUSR2, &finSortie, NULL );
	sigaction( SIGCHLD, &finSortie, NULL );
	//attachement de la memoire partagee
	memSh = (memPartagee*) shmat( mem, NULL, 0 );
	//structure de recuperation de message
	struct msgvoit msgRec;
	//semaphores
	mutexMem = mutMem;
	semAtt[0] = att1;
	semAtt[1] = att2;
	semAtt[2] = att3;
	
	
	for(;;)
	{
		//Verification de l'etat des voituriers
		int i = 0;
		//Etape 1 attente de messages
		while (msgrcv( balS, &msgRec, sizeof ( struct msgvoit ), 0,  0) == -1);
		//Utilisation de la memoire
		while ( semop( mutexMem, &reserver, 1 ) == -1);
		//on verifie qu'on fait bien sortir une voiture existante
		if(memSh->places[msgRec.place - 1].typeUsager != AUCUN)
		{
			//Etape 2 on complete le message avec le contenu de la place
			msgRec.typeUsager = memSh->places[msgRec.place - 1].typeUsager;
			msgRec.heure = memSh->places[msgRec.place - 1].heure;
			msgRec.numvoit = memSh->places[msgRec.place - 1].numvoit;
			//Etape 3 on fait sortir la voiture
			memSh->places[msgRec.place - 1].typeUsager = AUCUN;//on libere la place
			Effacer((TypeZone) msgRec.place );
			voituriers[ SortirVoiture( msgRec.place ) ] = msgRec;	
		}
		semop( mutexMem, &liberer, 1 );
		
	}
	pause();
} //----- fin de Nom

