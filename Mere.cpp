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
#include "Util.h"
#include "GestionClavier.h"
#include "Sortie.h"
#include "Heure.h"
#include <Outils.h>
#include "Entree.h"
//------------------------------------------------------------- Constantes
#define DROITS 0660
//----------------------------------------------------------- Prototypes prives
static void Moteur( pid_t pidClavier );

//----------------------------------------------------------------- PUBLIC

int main()
{
	struct sigaction actionSigInt;
	actionSigInt.sa_handler = SIG_DFL;
	sigemptyset( &actionSigInt.sa_mask );
	actionSigInt.sa_flags = 0;
	
	//INITIALISATION
	InitialiserApplication(XTERM);
	//blocage de SIGINT
	//note : ATTENTION les enfants heritent des blocages de la mère
	sigset_t signauxABloquer;
	sigemptyset( &signauxABloquer );
	sigaddset( &signauxABloquer, SIGINT );
	sigprocmask( SIG_BLOCK, &signauxABloquer, NULL );
	//memoire partagée
	int mem;//memoire
	mem = shmget( IPC_PRIVATE, sizeof( struct memPartagee ), IPC_CREAT | DROITS );
	struct memPartagee* memInit; //utilise pour initialiser la memoire
	memInit = (memPartagee*) shmat( mem, NULL, 0 );
	memInit->nbPlacesLibres = 8;
	for( int i=0 ; i<3 ; i++ )
	{
		memInit->enAttente[i].typeUsager =  AUCUN;
	}
	for( int i=0 ; i<8 ; i++ )
	{
		memInit->places[i].typeUsager =  AUCUN;
	}
	shmdt( memInit );//initialisation fini -> on detache
	int mutMem;//mutex mempartagée
	mutMem = semget( IPC_PRIVATE, 1 ,IPC_CREAT | DROITS );
	semctl ( mutMem, 0, SETVAL, 1); 
	//structures entrée
	int balE1, balE2, balE3;//boites aux lettres
	balE1 = msgget( IPC_PRIVATE, IPC_CREAT | DROITS );
	balE2 = msgget( IPC_PRIVATE, IPC_CREAT | DROITS );
	balE3 = msgget( IPC_PRIVATE, IPC_CREAT | DROITS );
	int att1, att2, att3;//semaphores d'attente aux barrieres
	att1 = semget( IPC_PRIVATE, 1 ,IPC_CREAT | DROITS );
	att2 = semget( IPC_PRIVATE, 1 ,IPC_CREAT | DROITS );
	att3 = semget( IPC_PRIVATE, 1 ,IPC_CREAT | DROITS );
	semctl ( att1, 0, SETVAL, 1 );
	semctl ( att2, 0, SETVAL, 1 ); 
	semctl ( att3, 0, SETVAL, 1 );  
	//structures sortie
	int balS;//boite au lettre
	balS = msgget( IPC_PRIVATE, IPC_CREAT | DROITS );
	
	
	//MOTEUR
	pid_t pidClavier;
	pid_t pidGestionHeure;
	pid_t pidSortie;
	pid_t pidPBlaisePascal;
	pid_t pidABlaisePascal;
	pid_t pidGastonBerger;
	//Heure
	pidGestionHeure = ActiverHeure( );
	
	
	if( (pidPBlaisePascal = fork() ) == 0)
	//Entree 1
	{
		Entree(balE1, mem, PROF_BLAISE_PASCAL, mutMem, att1);
	}
	
	if( (pidABlaisePascal = fork() ) == 0)
	//Entree 2
	{
		Entree(balE2, mem, AUTRE_BLAISE_PASCAL, mutMem, att2);
	}
	
	if( (pidGastonBerger = fork() ) == 0)
	//Entree 3
	{
		Entree(balE3, mem, ENTREE_GASTON_BERGER, mutMem, att3);
	}
	
	if( (pidSortie = fork() ) == 0)
	//Sortie
	{
		Sortie( balS, att1, att2, att3, mem, mutMem );
	}
	if( (pidClavier = fork() ) == 0)
	//Clavier
	{
		GestionClavier(balE1, balE2, balE3, balS);
	}
	else
	{
		Moteur( pidClavier );
	}
	
	//DESTRUCTION
	//On tue la sortie
	kill( pidSortie, SIGUSR2 );
	//On tue la gestion de l'heure
	kill( pidGestionHeure , SIGUSR2 );
	//On tue les entrées
	kill( pidPBlaisePascal , SIGUSR2 );
	kill( pidABlaisePascal , SIGUSR2 );
	kill( pidGastonBerger , SIGUSR2 );
	//structures sortie
	msgctl( balS, IPC_RMID, 0 );
	//structures entrée
	semctl( att3, 0, IPC_RMID, 0 );//semaphores d'attente
	semctl( att2, 0, IPC_RMID, 0 ); 
	semctl( att1, 0, IPC_RMID, 0 ); 
	msgctl( balE3, IPC_RMID, 0 );//boites aux lettres
	msgctl( balE2, IPC_RMID, 0 );
	msgctl( balE1, IPC_RMID, 0 );
	//memoire partagee
	semctl( mutMem, 0, IPC_RMID, 0 );//mutex
	shmctl( mem, IPC_RMID, 0 );//memoire
	//Destruction 
	TerminerApplication(true);
	return 0;
}


//------------------------------------------------------------------ PRIVE

static void Moteur(pid_t pidClavier){
	waitpid(pidClavier,NULL,0);
	
}
