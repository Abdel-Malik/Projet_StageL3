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
    for(int i =0; i <291; i++){
        vehicule.avancer(i);
        vehicule.setGear(gb.optimiserRapport());
        if(vehicule.getVitesseKMh() > 60){
            vehicule.setpAcc(0);
            b = true;
        }
        if(vehicule.getVitesseKMh() < 33.9&&b)
            vehicule.setpAcc(1);
        scg.calculConsommationInstantanee();
    }
    scg.calculConsommationMoyenne();
    scg.affichageConsommationMoyenne();
    return 0;
}
