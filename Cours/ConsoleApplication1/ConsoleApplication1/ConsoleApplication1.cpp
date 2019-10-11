// ConsoleApplication1.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "pch.h"
#include <cstdio>
#include <cstdlib>
#include <chrono>

struct Vec3
{
	int x;
	int y;
	int z;
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

Vec3 stackOverflow(Vec3 _in)
{
	Vec3 temp = _in;
	temp.y++;
	return stackOverflow(temp);
}

int Strlen(const char * src)
{
	int counter = 0;
	for (int yes = 0; yes < 1024; ++yes)
	{
		char current = src[yes];
		if (current == 0)
		{
			return yes;
		}
	}
	
	return counter;

	/*int counter = 0;
	while (true)
	{
	char current = *src;
	if (current ==0)
	break;
	src++;
	counter++;
	}
	return counter;*/
}

int Strcpy(char * dest, const char * src)
{
	int ourge = 0;
	for (ourge = 0; ourge < 1024; ourge++)
	{
		if (ourge != 0)
		{
			*dest = *src;
			*dest++;
			*src++;
		}
	}
	return ourge;
}

int main()
{
	int _i = 0;
	

	const char * source = "mon lapin est dodu";
	char * dest = (char*)calloc(1024, sizeof(char));
	Strcpy(dest, source);

	printf("dest len:%d\nval:%s", Strlen(source), dest);
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

	////Vec3 toto;
		//int x = 66;
		//x = appel(x);
		//printf("xval : %d\n", x);
		//Vec3 toto = { 1,2,3 };
		//incrX(toto);
		//printf("xval : %f\n", toto.x); 

		/*Vec3 bob = { 1,2,3 };
		bob = stackOverflow(bob);
		printf("val x : %f\n, bob.x");*/

//Vec3 vecTab[3];
//vecTab[0] = { 66,66,66 };
//vecTab[1] = { 4,5,6 };
//vecTab[2] = { 7,8,9 };
//
//printf("v0x %f\n", vecTab[0].x);
//vecTab[0].x++;
//printf("v0x %f\n", vecTab[0]);
//
//Vec3* iter = &vecTab[0];
//int i = 0;
//for (i = 0; i < 3; ++i)
//{
//	printf("val vec x: %d\n", iter->x);
//	iter++;
//}
//
//Vec3 * t0 = 0;
//Vec3 * t1 = nullptr;
//Vec3 * t2 = &vecTab[1];
//
//(*t2).y = 777;
//t2->z = 888;
//
//Vec3 * t3 = t2 + 1;
//t2++;

/*const char * label = "sapin";
const char label2[6] = { 's', 'a', 'p', 'i', 'n',0 };
const char * ptr = &label2[0];
ptr++;
printf("%c\n", *ptr);
auto start = std::chrono::system_clock::now();
int * bigBlock = (int*) (malloc(1024 * 1024 * 1024 * 1024));

for (int k = 0; k < 64 * 1024 * 1024; ++k)
{
	bigBlock[k] = 0xdeadbeef;
}

printf("beef ? : %x\n", bigBlock[1024 * 1024]);
auto end = std::chrono::system_clock::now();
auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
printf("time ? : %d\n", millis);*/


//Vec3 tableau[26];
//Vec3 *tableau0 = (Vec3*)malloc(sizeof(Vec3) * 16);
//memset(tableau, 0, sizeof(Vec3) * 16);

//Vec3 *tableau1 = (Vec3*)calloc(sizeof(Vec3) , 16);

//Vec3 *tableau2 = new Vec3[16];
//Vec3 * unSeulVec = new Vec3(); //allocation dynamique
//Vec3 unSeulVecAussiMaisSurLeTas = Vec3(); //allocation statique