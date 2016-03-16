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
#include "GestionClavier.h"
#include <Outils.h>
//------------------------------------------------------------- Constantes

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
	
	//INIT
	InitialiserApplication(XTERM);
	//MOTEUR
	pid_t pidClavier;
	
	if( (pidClavier = fork() ) == 0)
	//Clavier
	{
		GestionClavier();
	}
	else
	{
		Moteur(pidClavier);
	}
	
	//DESTR
	TerminerApplication(false);
	return 0;
}


//------------------------------------------------------------------ PRIVE

static void Moteur(pid_t pidClavier){
	waitpid(pidClavier,NULL,0);
	
}
