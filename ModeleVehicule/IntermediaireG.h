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
    static const int GEAR_MIN = 0;
    static const int GEAR_MAX = 6;

    static constexpr double VITESSE_MIN = 0;
    static constexpr double VITESSE_MAX = 123.26;
    static constexpr double PUISSANCE_MIN = 0;
    static constexpr double PUISSANCE_MAX = 266;
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

    //Interpolation
    int getEchAxe(){
        return echAxe;
    };

    double* getAxeZ(int gear, ModeConduite mode){
        double* valeurs = new double[echAxe*echAxe];
        double ci = (RPM_MAX/echAxe);
        double cj = (COUPLE_MAX/echAxe);
        double x,y;
        for(int i = 0; i < echAxe; i++){
            x = i*RPM_MAX/echAxe;
            for(int j = 0; j < echAxe; j++){
                y = j*COUPLE_MAX/echAxe;
                valeurs[i*echAxe+j] = 188+215*n(n((x-1300)*(x-1300),maxX) + 3.7*n((y-1600)*(y-1600),maxY) + n((x-1300)*(y-1600),maxX*maxY),maxZ);
            }
        }
        return valeurs;
    };
    double* getAxeZ(){
        double* valeurs = new double[(echAxe+1)*(echAxe+1)];
        double ci = (RPM_MAX/echAxe);
        double cj = (PUISSANCE_MAX/echAxe);
        double x,y;
        for(int i = 0; i <= echAxe; i++){
            x = i*RPM_MAX/echAxe;
            x=x/10;
            for(int j = 0; j <= echAxe; j++){
                y = j*PUISSANCE_MAX/echAxe;
                y=y;
                valeurs[i*echAxe+j] = 188+208.5*n(3.14*n((x-121)*(x-121),maxX) + 10.3*n((y-278)*(y-278),maxY) -6.5*n((x-145)*(y-271),maxM),maxZ);
                //std::cout << "x :"<<x<<" y :"<<y<<"val = " <<valeurs[i*echAxe+j] << "(i,j)" <<  i << ","<<j << std::endl;
            }
        }
        return valeurs;
    };

    double n(double x, double maxi){
        return x/maxi;
    }


    /*setter*/
    void setFreinageRoue(int i, double chrg){
        freinRoues[i] = chrg;
    };

    void majMDV(bool a, int g, double v, double p, double cM, double cF, double conso){
        ralentiMot = a;
        currentGear = g;
        nbRoues = 6;
        vitesseVehicule = v;
        puissanceMoteur = p;
        chargeMoteur = cM;
        chargeFrein = cF;
        consommation = conso;
        echAxe = 20;
    }

    /**Méthodes privées**/

    private:
    void recuperationDonnees(){
        ralentiMot = false;
        nbRoues = 6;
        echAxe = 21;
    };

    void initRoues(){
        for(int i = 0; i < nbRoues ; i++){
            rayonRoues.push_back(0.4719);
            vitesseRoues.push_back(0);
            freinRoues.push_back(0);
        }
        //recupVitesseRoues();
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
