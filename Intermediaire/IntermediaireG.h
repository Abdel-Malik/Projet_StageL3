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
    double coeffP[3] = {-0.0001502,0.5648365,-263.93706};
    double coeffCons[3] = {0.0000352,-0.0985985,257.98788};
    double A[3] = {0.000009,-0.174561,211.91476};
    double rapportTransmission[7] = {4.24,3.36,1.91,1.42,1,0.72,0.62}; // gear : R - 1 - 2 - 3 - 4 - 5 - 6

    //Données Véhicule
    private:
    bool ralentiMot;
    int currentGear;
    int nbRoues;
    std::vector<double> rayonRoues;

    //Interaction Véhicule
    double vitesseVehicule;
    double rotationMoteur;
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
    };

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
    double getRapportBoiteDeVitesse(int rapport){
        return rapportTransmission[rapport];
    }
    double getVitesse(){
        return vitesseVehicule;
    };
    double getRotationMoteur(){
        return rotationMoteur;
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
    double getPuissanceMoteur(double r){
         return chargeMoteur*(coeffP[0]*r*r+coeffP[1]*r+coeffP[2]);
    };
    double getConsommation(double r){
        double y = getPuissanceMoteur(r);
        return A[0]*(r*r)+A[1]*y+A[2];
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

    double n(double x, double maxi){
        return x/maxi;
    }


    /*setter*/
    void setFreinageRoue(int i, double chrg){
        freinRoues[i] = chrg;
    };

    void majMDV(bool a, int g, double v, double r, double p, double cM, double cF, double conso){
        ralentiMot = a;
        currentGear = g;
        vitesseVehicule = v;
        rotationMoteur =r;
        puissanceMoteur = p;
        chargeMoteur = cM;
        chargeFrein = cF;
        consommation = conso;
    }
};
#endif
