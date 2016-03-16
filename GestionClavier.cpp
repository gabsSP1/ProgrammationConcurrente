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

//------------------------------------------------------ Include personnel
#include "GestionClavier.h"
#include "Menu.h"

//------------------------------------------------------------- Constantes

//---------------------------------------------------- Variables de classe

//----------------------------------------------------------- Types privés


//----------------------------------------------------------------- PUBLIC
//-------------------------------------------------------- Fonctions amies

//----------------------------------------------------- Méthodes publiques
pid_t processusMenu;

void GestionClavier::GestionClavier()
// Algorithme :
//
{
	
	
	if( ( proceMenu=fork() ) == 0 )
	{
		Menu();
	}
	
	else 
	{
		for(;;)
		{
			pause();
		}
	}
	
	
} //----- Fin de Méthode

void GestionClavier::Commande(char code, unsigned int valeur)
// Algorithme :
//
{
	switch(code)
	{
		case(e):
			terminerTache();
		break;
	}
	
} //----- Fin de Méthode


void terminerTache()
{
	kill(processusMenu, SIGUSR2);
	exit(0);
}



//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées

//------------------------------------------------------- Méthodes privées

