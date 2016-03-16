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
#include <sys/wait.h>
#include <unistd.h>
//------------------------------------------------------ Include personnel
#include "Mere.h"
#include <Outils.h>
//------------------------------------------------------------- Constantes

//----------------------------------------------------------- Prototypes prives


//----------------------------------------------------------------- PUBLIC

// type ${file_base}::Méthode ( liste de paramètres )
// Algorithme :
//
//{
//} //----- Fin de Méthode


int main()
{
	
	//INIT
	InitialiserApplication(XTERM);
	//MOTEUR
	sleep(10);
	
	//DESTR
	TerminerApplication(false);
	return 0;
}


//------------------------------------------------------------------ PRIVE

