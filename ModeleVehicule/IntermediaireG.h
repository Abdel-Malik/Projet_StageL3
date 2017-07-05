/*///////////////////////////////////////////////////////////////
// * Author : Abdel-Malik Bouhassoun
// * Date : 31 Mai 2017
// Ce header contient la classe intermédiaire Générale
//Elle regroupe les fonctionnalitées des classes intermediaire des 3 proets
//ABS-Conso-Gear_shift
///////////////////////////////////////////////////////////////*/
#ifndef _IntermediaireG_h_
#define _IntermediaireG_h_

#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>

#include "../BoiteDeVitesse/E_ModeConduite.h"

class IntermediaireG{

    /**attributs**/
    //Constantes_véhicule
    public:
    static const int GEAR_MIN = 0;
    static const int GEAR_MAX = 6;

    static constexpr double VITESSE_MIN = 0;
    static constexpr double VITESSE_MAX = 123.26;
    static constexpr double PUISSANCE_MIN = 0;
    static constexpr double PUISSANCE_MAX = 266;
    static constexpr double COUPLE_MIN = 0;
    static constexpr double COUPLE_MAX = 1700;
    static constexpr double RPM_MIN = 800;
    static constexpr double RPM_MAX = 2500;
    static constexpr double CHARGE_MIN = 0;
    static constexpr double CHARGE_MAX = 1;

    double maxX = 16641,maxY = 7728.4, maxM = 3930, maxZ=6.563*23.34;
    //Données Véhicule
    private:
    bool ralentiMot;
    int currentGear;
    int nbRoues;
    std::vector<double> rayonRoues;

    //Interaction Véhicule
    double vitesseVehicule;
    double puissanceMoteur;
    double consommation;
    double chargeMoteur;
    double chargeFrein;
    std::vector<double> vitesseRoues;
    std::vector<double> freinRoues;

    //autre
    int event = 0;

    /**Méthodes**/

    public:
    //Constructeur
    IntermediaireG(int nbRoues, double rayonRoue){
        ralentiMot = false;
        nbRoues = nbRoues;
        for(int i = 0; i < nbRoues ; i++){
            rayonRoues.push_back(rayonRoue);
            vitesseRoues.push_back(0);
            freinRoues.push_back(0);
        }

    /*getter*/
    //données véhicule
    bool ralentiMoteur(){
        return ralentiMot;
    };
    int getGear(){
        return currentGear;
    };
    double getNbRoues(){
        return nbRoues;
    };
        //en mètre
    double getRayonRoues(int i){
        return rayonRoues[i];
    };

    //interaction véhicule
    double getVitesse(){
        return vitesseVehicule;
    };
    void setVitesse(double a){
        vitesseVehicule = a;
    };
    double getPuissanceMoteur(){
        return puissanceMoteur;
    };
    double getConsommation(){
        return consommation;
    };
    double getChargeMoteur(){
        return chargeMoteur;
    };

    double getChargeFrein(){
        return chargeFrein;
    };

    double getVitesseAngulaire(int i){
        return vitesseRoues[i];
    };




    /*setter*/
    void setFreinageRoue(int i, double chrg){
        freinRoues[i] = chrg;
    };

    void majMDV(bool a, int g, double v, double p, double cM, double cF, double conso){
        ralentiMot = a;
        currentGear = g;
        vitesseVehicule = v;
        puissanceMoteur = p;
        chargeMoteur = cM;
        chargeFrein = cF;
        consommation = conso;
    }
};
#endif
