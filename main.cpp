#include <iostream>
#include "ModeleVehicule/MDV.h"
#include "BoiteDeVitesse/GearBoxAI.h"
#include "Consommation/StockageConsommation.h"
using namespace std;

int main()
{
    MDV vehicule = MDV();
    IntermediaireG* i = vehicule.getI();
    vehicule.fct();
    GearBoxAI gb = GearBoxAI(i);
    StockageConsommationGeneral scg = StockageConsommationGeneral(i);
    bool b = false;
    for(int i =0; i <43; i++){
        vehicule.avancer(i);
        vehicule.setGear(gb.optimiserRapport());
        if(vehicule.getVitesseKMh() > 11.5){
            vehicule.setpAcc(0.2);
            //b = true;
        }
        if(vehicule.getVitesseKMh() < 48.5&&b)
            vehicule.setpAcc(0.3);
        scg.calculConsommationInstantanee();
    }
    scg.calculConsommationMoyenne();
    scg.affichageConsommationMoyenne();
    return 0;
}
