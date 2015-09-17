#include "Tw_erp.h"
#include <iostream>
#static const float EFF = 0.8

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
  code = c/100.;
  gestion = g/100.;
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
      pos = line.find(separator);
      id = atoi(line.substr(0,pos).c_str());
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
    trier_commandes();
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
  int w_jour_code, w_jour_gestion;
  for (size_t i = 0; i < employes.size() ; i++)
  {
    w_jour_code = l_employes.at(i).code;
    w_jour_gestion = l_employes.at(i).gestion;
  }
  for (size_t i = 0 ; i < 360 ; i++)
  {
      tabTravail[i][0] = w_jour_code;
      tabTravail[i][1] = w_jour_gestion;
  }
}

void Entreprise::trier_commandes(){
	std::sort (l_commandes.begin(), l_commandes.end(), MonFoncteur());
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
      std::cout << l_commandes.at(i).id << "\tNom : " << l_commandes.at(i).nom << "\t NbDevreq : " << l_commandes.at(i).joursDev <<
	    "\tNbGestreq : " << l_commandes.at(i).joursGestion << "\t NbJoursAvantRendu : " << l_commandes.at(i).joursRendu << "\t" << std::endl;
	    std::cout << "COMMANDE INVALIDE\t nbJours fin developpement : " << (tmp.code + temps_dev) << "\t NbJours fin gestion : " << (tmp.gestion + temps_gestion) << std::endl;

      float ratio_dev = (float)( (tmp.code + temps_dev) - l_commandes.at(i).joursRendu ) / (float)( l_commandes.at(i).joursRendu - temps_dev ); /*jours_dev_entrop / nbjour_dev_dispo*/
      float ratio_gestion = (float)( (tmp.gestion + temps_gestion) - l_commandes.at(i).joursRendu ) / (float)( l_commandes.at(i).joursRendu - temps_gestion ); /*jours_dev_entrop / nbjour_dev_dispo*/

      if ( ratio_dev > 1 || ratio_gestion == 0)
      {
        recruter(temps_dev - 60,1,0);
      }
      else if ( ratio_gestion > 1 || ratio_dev == 0)
        {
          recruter(temps_gestion - 60,0,1);
        }
        else
          recruter(min(temps_dev,temps_gestion),0.5,0.5);
      OK += 1;
      i--;
    }
    else
    {
      temps_dev += tmp.code;
      temps_gestion += tmp.gestion;
    }
  }
  std::cout << "Quelle superbe entreprise, je voul'achête pour 1k de 1k d'€, mais vous deverez engager " << OK << "emloyes pour remplir vos objectifs"<< std::endl;
}


Res Entreprise::faisable (Commande c, int debut_code, int debut_gestion)
{
  float eff_code =0., eff_gestion=0.;

  for (size_t j = debut_gestion; j < c.joursRendu; j++)
  {
    eff_gestion += tabTravail[j][1];
  }
  for (int i = debut_code; i < c.joursRendu; i++)
  {
    eff_code += tabTravail[j][0];
  }

	eff_code *= EFF;
	eff_gestion *= EFF;

  float tmp_dev_nec  =(float)c.joursDev / eff_code;
  float tmp_gest_nec =(float)c.joursGestion / eff_gestion;

  bool faisable_code = (tmp_dev_nec < temps_dev_dispo);
  bool faisable_gestion = (tmp_gest_nec < temps_gestion_dispo);


  return (Res){tmp_dev_nec, tmp_gest_nec, faisable_code,faisable_gestion};
}


int Entreprise::recruter(int j, float dev, float gest)
{
  for (size_t i = j+60; i < 20; i++)
  {
    tabTravail[i][0] += 0.5*dev;
    tabTravail[i][1] += 0.5*gestion;
  }
  for ( i = j+80; i < 360; i++)
  {
    tabTravail[i][0] += 1*dev;
    tabTravail[i][1] += 1*gestion;
  }
  std::cout << "Tu dois recruter qqn qui fait " << dev << " de dev et " << gest << " de gestion" << std::endl;
}

int main(int argc, char* argv[])
{
	Entreprise e = Entreprise();
	e.evaluer();
	return 0;
}
