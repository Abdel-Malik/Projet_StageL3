#include <iostream>
#include "ModeleVehicule/MDV.h"
#include "BoiteDeVitesse/GearBoxAI.h"
#include "abs/ABS.h"
#include "Consommation/StockageConsommation.h"
using namespace std;

int main()
{
    MDV vehicule = MDV();
    IntermediaireG* i = vehicule.getI();
    vehicule.fct();
    GearBoxAI gb = GearBoxAI(i);
    ABS abs = ABS(i);
    StockageConsommationGeneral scg = StockageConsommationGeneral(i);
    bool b = false;
    for(int i =0; i <200; i++){
        scg.calculConsommationInstantanee();
        scg.affichageConsommationInstantanee();
        vehicule.avancer(i);
        gb.optimiserRapport();
        vehicule.setGear(vehicule.getI()->getGear());
        if(vehicule.getVitesseKMh() > 51){
            //vehicule.setpAcc(0);
            b = true;
        }
        if(vehicule.getVitesseKMh() < 49&&b){
            vehicule.setpAcc(1);
        }
    }
    scg.calculConsommationMoyenne();
    scg.affichageConsommationMoyenne();
    return 0;
}
