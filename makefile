#Makefile de l'application TP-MULTITACHE

TP_MULTITACHE : Mere.o GestionClavier.o Sortie.o Entree.o
	g++ -L/shares/public/tp/tp-multitache/ -o tp-multitache/Parking Mere.o GestionClavier.o Sortie.o Entree.o -std=c++11 -ltp -lncurses -ltcl

Mere.o : Mere.cpp Mere.h
	g++ -std=c++11 -I/shares/public/tp/tp-multitache/ -c Mere.cpp 

GestionClavier.o : GestionClavier.cpp GestionClavier.h
	g++ -std=c++11 -I/shares/public/tp/tp-multitache/ -c GestionClavier.cpp

Sortie.o : Sortie.cpp Sortie.h
	g++ -std=c++11 -I/shares/public/tp/tp-multitache/ -c Sortie.cpp

Entree.o : Entree.cpp Entree.h
	g++ -std=c++11 -I/shares/public/tp/tp-multitache/ -c Entree.cpp

