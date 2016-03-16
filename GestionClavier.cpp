/*************************************************************************
                           GestionClavier  -  description
                             -------------------
    d�but                : ${date}
    copyright            : (C) ${year} par ${user}
*************************************************************************/

//---------- R�alisation de GestionClavier (fichier GestionClavier.cpp) --

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include syst�me
using namespace std;
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

//------------------------------------------------------ Include personnel
#include "GestionClavier.h"
#include "Menu.h"

//------------------------------------------------------------- Constantes

//---------------------------------------------------- Variables de classe

//----------------------------------------------------------- Types priv�s


//----------------------------------------------------------------- PUBLIC
//-------------------------------------------------------- Fonctions amies

//----------------------------------------------------- M�thodes publiques
void GestionClavier::GestionClavier()
// Algorithme :
//
{
	pid_t proceMenu;
	
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
	
	
} //----- Fin de M�thode

void GestionClavier::Commande(char code, unsigned int valeur)
// Algorithme :
//
{
	switch(code)
	{
		case(e):
			exit(0);
		break;
	}
	
} //----- Fin de M�thode




//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- M�thodes prot�g�es

//------------------------------------------------------- M�thodes priv�es
