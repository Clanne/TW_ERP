#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

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

class Entreprise {
	public :
	  Entreprise();
	  
	  std::vector<Commande> l_commandes;
	  std::vector<Employe> l_employes;
	  void evaluer();
	  res faisable (Commande c, int debut);
	  int recruter();
};

