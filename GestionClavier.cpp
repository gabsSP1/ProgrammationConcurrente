/*************************************************************************
                           GestionClavier  -  description
                             -------------------
    début                : ${date}
    copyright            : (C) ${year} par ${user}
*************************************************************************/

//---------- Réalisation de GestionClavier (fichier GestionClavier.cpp) --

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
using namespace std;
#include <iostream>
//------------------------------------------------------ Include personnel
#include "GestionClavier.h"
#include "Menu.h"
#include "Mere.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//---------------------------------------------------- Variables statiques
static int nbVoiture;
static int bE1, bE2, bE3, bS;

//------------------------------------------------------ Fonctions privées
static void sendMessageE (TypeUsager typeUsager, long type, int boite)
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
{
	msgvoit msg;
	msg.typeUsager = typeUsager;
	msg.type = type;
	msg.numvoit = nbVoiture%999+1;
	nbVoiture++;
	msgsnd (boite, (void *) &msg, sizeof(msgvoit), 0);
}//----- fin de sendMessage

static void sendMessageS (int place, int boite)
// Mode d'emploi :
//
// Contrat :
//
// Algorithme :
//
{
	msgvoit msg;
	msg.place = place;
	msgsnd (boite, (void *) &msg, sizeof(msgvoit), 0);
}//----- fin de sendMessageS

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void GestionClavier(int balE1, int balE2, int balE3, int balS)
// Algorithme :
//
{
	bE1=balE1;
	bE2=balE2;
	bE3=balE3;
	bS=balS;
	nbVoiture=0;
	for(;;)
	{
		Menu();
	}
	
	
} //----- Fin de GestionClavier

void Commande(char code, unsigned int valeur)
// Algorithme :
//
{
	switch(code)
	{
		case 'E':
			exit(0);
			break;
			
		case 'P':
			switch(valeur)
			{
				case 1:
					sendMessageE(PROF, PROF_BLAISE_PASCAL, bE1);
					break;
					
				case 2:
					sendMessageE(PROF, ENTREE_GASTON_BERGER, bE3);
					break;
			}
			break;
			
		case 'A':
			switch(valeur)
			{
				case 1:
					sendMessageE(AUTRE, AUTRE_BLAISE_PASCAL, bE2);
					break;
					
				case 2:
					sendMessageE(AUTRE, ENTREE_GASTON_BERGER, bE3);
					break;
			}
			break;
			
		case 'S':
			sendMessageS(valeur, bS);
			break;
		
			
			
	}
	
} //----- Fin de GestionClavier





