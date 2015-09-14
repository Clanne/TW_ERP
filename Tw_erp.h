#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

typedef struct res{int code; int gestion; bool doable_dev; bool doable_gestion};

public class entreprise {

  vector<Commande> l_commandes;
  vector<Employe> l_employes;
  public void evaluer();
  public res faisable (Commande c, int debut);
  public int recruter();

}

public class Commande {
  public int id;
  std::string nom;
  public int joursRendu;
  public int joursDev;
  public int joursGestion;
}

public class Employe {
  std::string job;
  float code;
  float gestion;
}
