// ConsoleApplication1.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "pch.h"
#include <cstdio>
#include <cstdlib>

struct Vec3
{
	float x;
	float y;
	float z;
}; 

int appel(int & s)
{
	s++;
	return s;
}

Vec3 incrX(Vec3 _in) 
{
	_in.x++;
	return _in;
}

int main()
{
	//Vec3 toto;
	int x = 66;
	x = appel(x);
	printf("xval : %d\n", x);
	Vec3 toto = { 1,2,3 };
	incrX(toto);
	printf("xval : %f\n", toto.x);
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Conseils pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.


/*int foo = 0;
	const char * label = "sapin";
	const char label2[5] = { 's', 'a', 'p', 'i', 'n'};
	float y = 0.5f;
	double ff = 0.5f;

	printf("Hello world this is a text : %d label : %s %f\n", foo, label, 66.66f);
	printf("Hello world this is a text : %d fration : %s %f\n", { (float) }1.0 66.66f);
	printf("Hello world this is a text : %d fration : %s %f\n", { (double) } 66.66f);
	printf("hello world : l2 : %s\n", label2);*/