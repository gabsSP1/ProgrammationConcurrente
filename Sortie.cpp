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
	//operation sur semaphores
	struct sembuf reserver{0, -1, 0};
	struct sembuf liberer{0, 1, 0};//operations sur mutex
	
	for(;;)
	{
		//Verification de l'etat des voituriers
		int i = 0;
		//Etape 1 attente de messages
		msgrcv( balS, &msgRec, sizeof ( struct msgvoit ), 0,  0);
		char* testStr;
		sprintf(testStr, "Entree recue %d Usager %d nbPlacelibres %d", msgRec.place, memSh->places[msgRec.place - 1].typeUsager, memSh->nbPlacesLibres );
		Effacer( MESSAGE );
		Afficher( MESSAGE, testStr );
		
		
		//Utilisation de la memoire
		while ( semop( mutMem, &reserver, 1 ) == -1);
		//on verifie qu'on fait bien sortir une voiture existante
		if(memSh->places[msgRec.place - 1].typeUsager != AUCUN)
		{
			//Etape 2 on complete le message avec le contenu de la place
			msgRec.typeUsager = memSh->places[msgRec.place - 1].typeUsager;
			msgRec.heure = memSh->places[msgRec.place - 1].heure;
			msgRec.numvoit = memSh->places[msgRec.place - 1].numvoit;
			//Etape 3 on fait sortir la voiture
			memSh->places[msgRec.place - 1].typeUsager = AUCUN;//on libere la place
			switch( msgRec.place )
			{
				case 1:
					Effacer( ETAT_P1 );
					break;
				case 2:
					Effacer( ETAT_P2 );
					break;
				case 3:
					Effacer( ETAT_P3 );
					break;
				case 4:
					Effacer( ETAT_P4 );
					break;
				case 5:
					Effacer( ETAT_P5 );
					break;
				case 6:
					Effacer( ETAT_P6 );
					break;
				case 7:
					Effacer( ETAT_P7 );
					break;
				case 8:
					Effacer( ETAT_P8 );
					break;
				default://ce cas ne devrait jamais arriver
					break;
				
			}
			voituriers[ SortirVoiture( msgRec.place ) ] = msgRec;
			
			//Etape 4 on fait rentrer les voitures
			if( memSh->enAttente[0].typeUsager == AUCUN
					&& memSh->enAttente[1].typeUsager == AUCUN
					&& memSh->enAttente[2].typeUsager == AUCUN )
			{//Si il n'y a personne
				memSh->nbPlacesLibres += 1;
			}
			//Si il y a exactement une personne en attente
			else if( memSh->enAttente[1].typeUsager == AUCUN
					&& memSh->enAttente[2].typeUsager == AUCUN )
			{
				semop(att1, &liberer, 1);
			}
			else if( memSh->enAttente[0].typeUsager == AUCUN
					&& memSh->enAttente[2].typeUsager == AUCUN )
			{
				semop(att2, &liberer, 1);
			}
			else if( memSh->enAttente[0].typeUsager == AUCUN
					&& memSh->enAttente[1].typeUsager == AUCUN )
			{
				semop(att3, &liberer, 1);
			}
			//Si il y a exactement deux personnes en attente
			else if( memSh->enAttente[2].typeUsager == AUCUN )
			{
				if( memSh->enAttente[0].typeUsager == memSh->enAttente[1].typeUsager )
				{
					if( memSh->enAttente[0].heure < memSh->enAttente[1].heure )
					{
						semop(att1, &liberer, 1);
					}
					else
					{
						semop(att2, &liberer, 1);
					}
				}
				else
				{
					if( memSh->enAttente[0].typeUsager == PROF )
					{
						semop(att1, &liberer, 1);
					}
					else
					{
						semop(att2, &liberer, 1);
					}
				}
				
			}
			else if( memSh->enAttente[1].typeUsager == AUCUN )
			{
				if( memSh->enAttente[0].typeUsager == memSh->enAttente[2].typeUsager )
				{
					if( memSh->enAttente[0].heure < memSh->enAttente[2].heure )
					{
						semop(att1, &liberer, 1);
					}
					else
					{
						semop(att3, &liberer, 1);
					}
				}
				else
				{
					if( memSh->enAttente[0].typeUsager == PROF )
					{
						semop(att1, &liberer, 1);
					}
					else
					{
						semop(att3, &liberer, 1);
					}
				}
				
			}
			else if( memSh->enAttente[0].typeUsager == AUCUN )
			{
				if( memSh->enAttente[2].typeUsager == memSh->enAttente[1].typeUsager )
				{
					if( memSh->enAttente[1].heure < memSh->enAttente[2].heure )
					{
						semop(att2, &liberer, 1);
					}
					else
					{
						semop(att3, &liberer, 1);
					}
				}
				else
				{
					if( memSh->enAttente[1].typeUsager == PROF )
					{
						semop(att2, &liberer, 1);
					}
					else
					{
						semop(att3, &liberer, 1);
					}
				}
				
			}
			//Si toutes les barrières sont en attente
			else
			{
					//Si toutes les barrières sont en attentes il y a au moins un prof qui attend
					if( memSh->enAttente[0].typeUsager == PROF )
					{
						if( memSh->enAttente[1].typeUsager == PROF )
						{
							if( memSh->enAttente[0].heure < memSh->enAttente[1].heure )
							{
								semop(att1, &liberer, 1);
							}
							else
							{
								semop(att2, &liberer, 1);
							}
						}
						else if ( memSh->enAttente[2].typeUsager == PROF )
						{
							if( memSh->enAttente[0].heure < memSh->enAttente[2].heure )
							{
								semop(att1, &liberer, 1);
							}
							else
							{
								semop(att3, &liberer, 1);
							}
						}
						else
						{
							semop(att1, &liberer, 1);
						}
					}
					else if ( memSh->enAttente[1].typeUsager == PROF )
					{
						if ( memSh->enAttente[2].typeUsager == PROF )
						{
							if( memSh->enAttente[1].heure < memSh->enAttente[2].heure )
							{
								semop(att2, &liberer, 1);
							}
							else
							{
								semop(att3, &liberer, 1);
							}
						}
						else
						{
							semop(att2, &liberer, 1);
						}
					}
					else if ( memSh->enAttente[2].typeUsager == PROF )
					{
						semop(att3, &liberer, 1);
					}
					
			}
		}
		semop( mutMem, &liberer, 1 );
		
	}
	pause();
} //----- fin de Nom

