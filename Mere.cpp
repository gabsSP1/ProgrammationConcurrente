/*************************************************************************
                           Mere.cpp  -  Processus principal
*************************************************************************/

//Realisation du Processus principal

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
using namespace std;
#include <iostream>
#include <cstdlib>
using namespace std;
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
//------------------------------------------------------ Include personnel
#include "Mere.h"
#include "GestionClavier.h"
#include "Sortie.h"
#include "Heure.h"
#include <Outils.h>
//------------------------------------------------------------- Constantes
#define DROITS 0660
//----------------------------------------------------------- Prototypes prives
static void Moteur(pid_t pidClavier);

//----------------------------------------------------------------- PUBLIC

// type ${file_base}::Méthode ( liste de paramètres )
// Algorithme :
//
//{
//} //----- Fin de Méthode


int main()
{
	struct sigaction actionSigInt;
	actionSigInt.sa_handler = SIG_DFL;
	sigemptyset( &actionSigInt.sa_mask );
	actionSigInt.sa_flags = 0;
	
	//INIT
	InitialiserApplication(XTERM);
	//blocage des signaux SIGINT, SIGUSR1 et SIGUSR2
	//note : ATTENTION les enfants heritent des blocages de la mère
	sigset_t signauxABloquer;
	sigemptyset( &signauxABloquer );
	sigaddset( &signauxABloquer, SIGINT );
	//sigaddset( &signauxABloquer, SIGUSR1 );
	//sigaddset( &signauxABloquer, SIGUSR2 );
	sigprocmask( SIG_BLOCK, &signauxABloquer, NULL);
	//structures entrée
	int balE1, balE2, balE3;//boites aux lettres
	balE1 = msgget( IPC_PRIVATE, IPC_CREAT | DROITS );
	balE2 = msgget( IPC_PRIVATE, IPC_CREAT | DROITS );
	balE3 = msgget( IPC_PRIVATE, IPC_CREAT | DROITS );
	int att1, att2, att3;//semaphores d'attente aux barrieres
	att1 = semget( IPC_PRIVATE, 1 ,IPC_CREAT | DROITS );
	att2 = semget( IPC_PRIVATE, 1 ,IPC_CREAT | DROITS );
	att3 = semget( IPC_PRIVATE, 1 ,IPC_CREAT | DROITS );
	//structures sortie
	int balS;//boite au lettre
	balS = msgget( IPC_PRIVATE, IPC_CREAT | DROITS );
	//memoire partagée
	int mem;//memoire
	mem = shmget( IPC_PRIVATE, sizeof(struct memPartagee), IPC_CREAT | DROITS );
	struct memPartagee* memInit; //utilise pour initialiser la memoire
	memInit = (memPartagee*) shmat( mem, NULL, 0 );
	memInit->nbPlacesLibres = 8;
	for( int i=0 ; i<3 ; i++ )
	{
		memInit->enAttente[i].typeUsager =  AUCUN;
	}
	for( int i=0 ; i<8 ; i++ )
	{
		memInit->Places[i].typeUsager =  AUCUN;
	}
	shmdt( memInit );//initialisation fini -> on detache
	int mutMem;//mutex mempartagée
	mutMem = semget( IPC_PRIVATE, 1 ,IPC_CREAT | DROITS );
	
	//MOTEUR
	pid_t pidClavier;
	pid_t pidGestionHeure;
	pid_t pidSortie;
	//Heure
	//On debloque pour pouvoir tuer l'heure -> evite qu'elle herite de l'ignorance de SIGUSR2 -> du kill
	sigprocmask( SIG_UNBLOCK, &signauxABloquer, NULL);
	pidGestionHeure = ActiverHeure( );
	sigprocmask( SIG_BLOCK, &signauxABloquer, NULL);
	
	
	if( (pidClavier = fork() ) == 0)
	//Clavier
	{
		GestionClavier();
	}
	else if( (pidSortie = fork() ) == 0)
	{
		Sortie();
	}
	else
	{
		Moteur(pidClavier);
	}
	
	//DESTR
	//On tue la sortie
	kill(pidSortie, SIGUSR2 );
	//On tue la gestion de l'heure
	kill( pidGestionHeure , SIGUSR2 );
	//memoire partagee
	semctl( mutMem, 0, IPC_RMID, 0 );//mutex
	shmctl( mem, IPC_RMID, 0 );//memoire
	//structures sortie
	msgctl( balS, IPC_RMID, 0 );
	//structures entrée
	semctl( att3, 0, IPC_RMID, 0 );//semaphores d'attente
	semctl( att2, 0, IPC_RMID, 0 ); 
	semctl( att1, 0, IPC_RMID, 0 ); 
	msgctl( balE3, IPC_RMID, 0 );//boites aux lettres
	msgctl( balE2, IPC_RMID, 0 );
	msgctl( balE1, IPC_RMID, 0 );
	//Destruction 
	TerminerApplication(true);
	return 0;
}


//------------------------------------------------------------------ PRIVE

static void Moteur(pid_t pidClavier){
	waitpid(pidClavier,NULL,0);
	
}
