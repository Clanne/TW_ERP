#include "Tw_erp.h"
#include <iostream>
Commande::Commande(int num, std::string name, int dev, int gestion, int rendu)
{
  id = num;
  nom = name;
  joursDev = dev;
  joursRendu = rendu;
  joursGestion = gestion;
}

Employe::Employe(std::string post, int c, int g)
{
  job = post;
  code = c/100;
  gestion = g/100;
}


Entreprise::Entreprise()
{
  std::string line, tmp;
  char separator =' ';
  int pos;
  int id, dev, gestion, rendu, code;
  std::string nom, job;
  std::ifstream projets("projets.txt", std::ios::in);
  if (projets)
  {
    while (getline(projets,line))
    {
      pos = 0;
      id =(int)(line.at(pos));
      pos = line.find(separator);
      line.erase(0, pos + 1);

      pos = line.find(separator);
      nom = line.substr(0, pos);
      line.erase(0, pos + 1);

      pos = line.find(separator);
      dev =atoi(line.substr(0, pos).c_str());
      line.erase(0, pos + 1);

      pos = line.find(separator);
      gestion =atoi(line.substr(0, pos).c_str());
      line.erase(0, pos + 1);

      pos = line.find(separator);
      rendu =atoi(line.substr(0, pos).c_str());

      l_commandes.push_back(Commande(id,nom,dev,gestion,rendu));
    }
  }
  std::ifstream employes("employes.txt", std::ios::in);
  {
    while (getline(employes,line))
    {
      pos = line.find(separator);
      job = line.substr(0, pos);
      line.erase(0, pos + 1);


      pos = line.find(separator);
      code =atoi(line.substr(0, pos).c_str());
      line.erase(0, pos + 1);

      pos = line.find(separator);
      gestion =atoi(line.substr(0, pos).c_str());

      l_employes.push_back(Employe(job,code,gestion));
    }
  }
}



void Entreprise::evaluer()
{
  int temps_dev = 0, temps_gestion = 0;
  int OK = 0;
  for (size_t i = 0; i < l_commandes.size(); i++)
  {
    Res tmp = faisable(l_commandes.at(i), temps_dev, temps_gestion);
    if ( !(tmp.doable_dev && tmp.doable_gestion) )
    {
      std::cout << "ACHTUNG ACHTUNG ES IST VERBOTEN" << std::endl;
      OK = 1;
    }
    temps_dev += tmp.code;
    temps_gestion += tmp.gestion;
  }
  if (!OK)
    std::cout << "Avec tout ce pognon, tu nous invites au resto ?" << std::endl;
  else
    std::cout << "ICH MÖCHTE SPIELEN !!!! SPIELEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEN" << std::endl;
}


Res Entreprise::faisable (Commande c, int debut_code, int debut_gestion)
{
  int jours_code =0, jours_gestion;
  int temps_dev_dispo = c.joursRendu - debut_code;
  int temps_gestion_dispo = c.joursRendu - debut_gestion;
  for (int i = 0; i < l_employes.size(); i++)
  {
    jours_code    += l_employes.at(i).code * temps_dev_dispo;
    jours_gestion += l_employes.at(i).gestion * temps_gestion_dispo;
  }
 
  return (Res){jours_code, jours_gestion, !(jours_code > temps_dev_dispo),!(jours_gestion > temps_gestion_dispo)};
}


int Entreprise::recruter()
{
}

int main(int argc, char* argv[])
{
	Entreprise e = Entreprise();
	e.evaluer();
	return 0;
}
