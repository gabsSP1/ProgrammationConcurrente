#Makefile de l'application TP-MULTITACHE

TP_MULTITACHE : Mere.o GestionClavier.o
	g++ -L/share/public/tp/tp-multitache/ -o tp-multitache.exe Mere.o GestionClavier.o-std=c++11 -ltp -lncurses -ltcl

Mere.o : Mere.cpp Mere.h
	g++ -std=c++11 -I/share/public/tp/tp-multitache/ -c Mere.cpp 

GestionClavier.o : GestionClavier.cpp GestionClavier.h
	g++ -std=c++11 -I/share/public/tp/tp-multitache/ -c GestionClavier.cpp 


