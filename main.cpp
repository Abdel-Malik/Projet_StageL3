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
    for(int i =0; i <100; i++){
        vehicule.avancer(i);
        vehicule.setGear(gb.optimiserRapport());
        if(vehicule.getVitesseKMh() > 51){
            vehicule.setpAcc(0);
            b = true;
        }
        if(vehicule.getVitesseKMh() < 49&&b){
            vehicule.setpAcc(0.4);
        }
        scg.calculConsommationInstantanee();
        scg.affichageConsommationInstantanee();
    }
    scg.calculConsommationMoyenne();
    scg.affichageConsommationMoyenne();
    return 0;
}
