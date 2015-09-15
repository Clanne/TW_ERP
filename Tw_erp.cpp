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
  int id, dev, gestion, rendu;
  ifstream projets("projets.txt", ios::in);
  if (projets)
  {
    while (getline(projets,line))
    {
      pos = 0;
      id =(int)(line.at(pos));
      pos = line.find(separator)
      line.erase(0, pos + 1);

      pos = line.find(separator);
      nom = s.substr(0, pos);
      line.erase(0, pos + 1);

      pos = line.find(separator);
      dev =(int) s.substr(0, pos);
      line.erase(0, pos + 1);

      pos = line.find(separator);
      gestion =(int) s.substr(0, pos);
      line.erase(0, pos + 1);

      pos = line.find(separator);
      rendu =(int) s.substr(0, pos);

      l_commandes.push_back(Commande(id,nom,dev,gestion,rendu));
    }
  }
  l_commandes.
  ifstream employes("employes.txt", ios::in);
  {
    while (getline(employes,line))
    {
      pos = line.find(separator);
      job = s.substr(0, pos);
      line.erase(0, pos + 1);


      pos = line.find(separator);
      code =(int) s.substr(0, pos);
      line.erase(0, pos + 1);

      pos = line.find(separator);
      gestion =(int) s.substr(0, pos);

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
    res tmp = faisable(l_commandes.at(i), temps_dev, temps_gestion);
    if ( !(tmp.doable_dev && tmp.doable_gestion) )
    {
      std::cout << "ACHTUNG ACHTUNG ES IST VERBOTEN" << std::endl;
      OK = 1;
    }
    temps_dev += res.code;
    temps_gestion += res.gestion;
  }
  if (OK)
    std::cout << "Avec tout ce pognon, tu nous invites au resto ?" << std::endl;
  else
    std::cout << "ICH MÖCHTE SPIELEN !!!! SPIELEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEN" << std::endl;
}a


Res Entreprise::faisable (Commande c, int debut_code, int debut_gestion)
{
  int jours_code =0, i = 0, jours_gestion;
  int temps_dev_dispo = c.joursRendu - debut_code;
  int temps_gestion_dispo = c.joursRendu - debut_gestion;
  Employe e;
  while ( (e=l_employes.at(i)) != NULL)
  {
    jours_code    += e.code * temps_dev_dispo;
    jours_gestion += e.gestion * temps_gestion_dispo;
  }
  return {jours_code, jours_gestion, !(jours_code > temps_dev_dispo),!(jours_gestion > temps_gestion_dispo)};
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
