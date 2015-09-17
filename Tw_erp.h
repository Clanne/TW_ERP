#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>

typedef struct res{int code; int gestion; bool doable_dev; bool doable_gestion;}Res;


class Commande {
	public :
		Commande(int num, std::string name, int dev, int gestion, int rendu);

	  int id;
	  std::string nom;
	  int joursRendu;
	  int joursDev;
	  int joursGestion;
};

class Employe {
	public :
	  Employe(std::string post, int c, int g);

	  std::string job;
	  float code;
	  float gestion;
};


struct MonFoncteur
{
	bool operator() (Commande s1, Commande s2)
	{
		return s1.joursRendu < s2.joursRendu;
	}
};


class Entreprise {
	public :
	  Entreprise();

	  std::vector<Commande> l_commandes;
	  std::vector<Employe> l_employes;
	  void evaluer();
	  void trier_commandes();
	  float tabTravail [360][2];
	  Res faisable (Commande c, int debut, int debut_gestion);
		int recruter(int j, float dev, float gest);
};
