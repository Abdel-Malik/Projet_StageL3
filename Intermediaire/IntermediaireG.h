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

#include "../Gear_shift/E_ModeConduite.h"

class IntermediaireG{

    /**attributs**/
    //Constantes_véhicule
    public:
    static const int GEAR_MIN = 1;
    static const int GEAR_MAX = 12;

    static constexpr double VITESSE_MIN = 0;
    static constexpr double VITESSE_MAX = 178.6;
    static constexpr double RPM_MIN = 0;
    static constexpr double RPM_MAX = 8000;
    static constexpr double CHARGE_MIN = 0;
    static constexpr double CHARGE_MAX = 1;

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

    //Interpolation
    int echAxe;

    //autre
    int event = 0;

    /**Méthodes**/

    public:
    //Constructeur
    IntermediaireG(){
        recuperationDonnees();
        initRoues();
    };
    /*Méthodes publiques*/

    void majDonnees(){
        recupVitesseRoues();
    }
    /*getter*/
    //données véhicule
    bool ralentiMoteur(){
        bool res = true;
        if(vitesseVehicule > 5)
            res = false;
        return res;
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

    //Interpolation
    int getEchAxe(){
        return echAxe;
    };

    double* getAxeZ(int gear, ModeConduite mode){
        double* valeurs = new double[echAxe*echAxe];
        for(int i = 0; i < echAxe*echAxe; i++)
            valeurs[i] = (((i%echAxe)*(i%echAxe-10))*((i/echAxe)*(i/echAxe-10)))/625.0;
        return valeurs;
    };


    /*setter*/
    void setFreinageRoue(int i, double chrg){
        freinRoues[i] = chrg;
    };

    /**Méthodes privées**/

    private:
    void recuperationDonnees(){
        ralentiMot = false;
        currentGear = 2;
        nbRoues = 6;
        vitesseVehicule = 110;
        puissanceMoteur = 90;
        chargeMoteur = .3;
        chargeFrein = 0;
        consommation = 430;
        echAxe = 20;
    };

    void initRoues(){
        for(int i = 0; i < nbRoues ; i++){
            rayonRoues.push_back(0.29545);
            vitesseRoues.push_back(0);
            freinRoues.push_back(0);
        }
        recupVitesseRoues();
    }

    void recupVitesseRoues(){
        event++;
        for(unsigned int i = 0; i < vitesseRoues.size() ; i++){
            if(event != 5 && event !=41 && event != 26)
                vitesseRoues[i] = vitesseVehicule*(1-freinRoues[i])/(rayonRoues[i]*3.6);
        }
        if(event != 5 && event != 16 && event !=18 &&  event != 26)
            vitesseVehicule -= 0.01*vitesseVehicule*chargeFrein;
        if(event == 4){
            std::cout << "Event 1 -- freinage" << std:: endl;
            chargeFrein = 1;
            for(unsigned int i = 0; i < vitesseRoues.size() ; i++){
                vitesseRoues[i] = vitesseVehicule*(1-chargeFrein)/(rayonRoues[i]*3.6);
                freinRoues[i] = chargeFrein;
            }
        }
        if(event == 15){
            std::cout << "Event 1 -- relacher" << std:: endl;
            chargeFrein = 0.52;
            for(unsigned int i = 0; i < vitesseRoues.size() ; i++){
                vitesseRoues[i] += 2;
            }
        }
        if(event == 25){
            std::cout << "Event 2 -- appuie2" << std:: endl;
            chargeFrein = 0.9;
            for(unsigned int i = 0; i < vitesseRoues.size() ; i++){
                vitesseRoues[i] -= 5.1;
            }
        }
        if(event == 40){
            std::cout << "Event 2 -- lâcher" << std:: endl;
            chargeFrein = 0;
        }
    }
};
#endif
