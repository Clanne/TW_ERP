#include "Tw_erp.h"
#include <iostream>
static float EFF = 0.8;
static int nbCo = 3; 

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
  code =(float) c/100.;
  gestion =(float) g/100.;
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
    while (getline(projets,line) && l_commandes.size() < nbCo)
    {
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
  
  
  float w_jour_code = 0, w_jour_gestion = 0;
  for (size_t i = 0; i < l_employes.size() ; i++)
  {
    w_jour_code += l_employes.at(i).code;
    w_jour_gestion += l_employes.at(i).gestion;
  }
  for (size_t i = 0 ; i < 1000 ; i++)
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
       //~ std::cout << l_commandes.at(i).id << "\tNom : " << l_commandes.at(i).nom << "\t NbDevreq : " << l_commandes.at(i).joursDev <<
	    //~ "\tNbGestreq : " << l_commandes.at(i).joursGestion << "\t NbJoursAvantRendu : " << l_commandes.at(i).joursRendu << "\t" << std::endl;
	    //~ std::cout << "COMMANDE INVALIDE\t nbJours fin developpement : " << (tmp.code + temps_dev) << "\t NbJours fin gestion : " << (tmp.gestion + temps_gestion) << std::endl;

      float ratio_dev = (float)( (tmp.code) - l_commandes.at(i).joursRendu ) / (float)( l_commandes.at(i).joursRendu - temps_dev ); /*jours_dev_entrop / nbjour_dev_dispo*/
      float ratio_gestion = (float)( (tmp.gestion) - l_commandes.at(i).joursRendu ) / (float)( l_commandes.at(i).joursRendu - temps_gestion ); /*jours_dev_entrop / nbjour_dev_dispo*/
      
     // std::cout << "RATIODEV : " << ratio_dev << std::endl;
     // std::cout << "RATIGEST : " << ratio_gestion	 << std::endl;

      recruter(temps_dev - 60,ratio_dev,ratio_gestion);
      OK += 1;
      i--;
    }
    else
    {
      temps_dev = tmp.code;
      temps_gestion = tmp.gestion;
    }
  }
  std::cout << "Quelle superbe entreprise, je voul'achête pour 1k de 1k d'€, mais vous deverez engager " << OK << " emloyes pour remplir vos objectifs"<< std::endl;
}


Res Entreprise::faisable (Commande c, int debut_code, int debut_gestion)
{
  float eff_code =0., eff_gestion=0.;
  int i,j,rendu = c.joursRendu;
  for (j = debut_gestion; eff_gestion < c.joursGestion/EFF ; j++)
  {
    eff_gestion += tabTravail[j][1];
  }
  for (i = debut_code; eff_code < c.joursDev/EFF; i++)
  {
    eff_code += tabTravail[i][0];
  //  std::cout << "TableauTravail : " << tabTravail[i][0] << std::endl;
  }

  bool faisable_code = (i <= rendu);
  bool faisable_gestion = (j <= rendu);
    
  return (Res){i, j, faisable_code,faisable_gestion};
}


void Entreprise::recruter(int j, float ratio_dev, float ratio_gestion)
{
  float dev, gest;
  if( ratio_dev >= 1 || ratio_gestion <= 0)
  {
	  dev = 1;
	  gest = 0;
  }
  else if ( ratio_gestion >= 1 || ratio_dev <= 0)
    {
	  dev = 0;
	  gest = 1;
    }
    else
    {   
	  dev = 0.5;
	  gest = 0.5;
    }	
  for (size_t i = j+60; i < j+80; i++)
  {
    tabTravail[i][0] += 0.5*dev;
    tabTravail[i][1] += 0.5*gest;
  }
  for (size_t i = j+80; i < 1000; i++)
  {
    tabTravail[i][0] += dev;
    tabTravail[i][1] += gest;
  }
  std::cout << "Vous devriez recruter quelqu'un qui fait " << dev << " de developpement et " << gest << " de gestion, la procedure doit commencer le jour :" << j << std::endl;
}

int main(int argc, char* argv[])
{
	std::string efficacite,airb;
	std::cout << "Quelle est l'efficacite de votre entreprise (entre 0 et 1) ?" << std::endl;
	std::cin >> efficacite;
	EFF = atof(efficacite.c_str());
	
	std::cout << "Votre entreprise travaille-elle sur le projet AirBOSS y/n ?" << std::endl;
	std::cin >> airb;
	if (!strcmp(airb.c_str(),"y"))
		nbCo = 4;	
	
	std::cout << nbCo << std::endl;
	Entreprise e = Entreprise();
	e.evaluer();
	return 0;
}
