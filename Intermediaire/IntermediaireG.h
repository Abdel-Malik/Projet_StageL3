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
    static constexpr double M_PI = 3.1415926;
    static const int GEAR_MIN = 0;
    static const int GEAR_MAX = 6;
    static const int ALPHA = 1.2;

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
    double coeffCouple[4] = {0.0000002,-0.0015774,2.8671445,191.44755};
    double coeffConso[3] = {0.0000304,-0.0812751,243.36373};
    double coefficientEquation[5] = {0.000000002351,10422.766,-0.0000069,-0.0094361,207.25751};
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
        return puissanceMoteur/1000;
    };
    double getConsommation(){
        return consommation;
    };
    double getConsoRalenti(){
        return 3.48;
    };

    //kW
    double getPuissanceMoteur(double r){
         return ((M_PI/30)*r*getCoupleMoteur(r))/1000;
    };
    double getCoupleMoteur(double r){
        return chargeMoteur*calculPolynome(r,coeffCouple);
    };

    double getConsommation(double r){
        double y = getCoupleMoteur(r);
        return coefficientEquation[0]*(r*r*r)+coefficientEquation[1]*(1/(r+1))+coefficientEquation[2]*r*y+coefficientEquation[3]*y+coefficientEquation[4];
    };
    double consoAvecCoeff(double r){
        double y = getPuissanceMoteur(r);
        double yTh = y/chargeMoteur;
        double conso = coefficientEquation[0]*(r*r)+coefficientEquation[1]*y+coefficientEquation[2];
        double consoTh = coefficientEquation[0]*(r*r)+coefficientEquation[1]*yTh+coefficientEquation[2];
        return ((conso)*(1+ALPHA*(1-((coeffCouple[0]*r*r+coeffCouple[1]*r+coeffCouple[2])/consoTh))));
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

    double calculPolynome(double val, double coeffs[]){
        double res = 0;
        double x = 1;
        for(int i = sizeof(coeffs)-1; i>=0;i--){
            res += (x*coeffs[i]);
            x = x*val;
        }
        return res;
    }
};
#endif
