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
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
//------------------------------------------------------ Include personnel
#include "GestionClavier.h"
#include "Menu.h"

//------------------------------------------------------------- Constantes

//---------------------------------------------------- Variables de classe

//----------------------------------------------------------- Types privés


//----------------------------------------------------------------- PUBLIC
//-------------------------------------------------------- Fonctions amies

//----------------------------------------------------- Méthodes publiques

void GestionClavier()
// Algorithme :
//
{
	
	for(;;){
		Menu();
	
	}
	
	
} //----- Fin de Méthode

void Commande(char code, unsigned int valeur)
// Algorithme :
//
{
	switch(code)
	{
		case 'E':
			exit(0);
			break;
	}
	
} //----- Fin de Méthode





//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées

//------------------------------------------------------- Méthodes privées

