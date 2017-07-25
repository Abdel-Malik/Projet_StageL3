#ifndef MDV_H
#define MDV_H

#include "Engine.h"
#include "../Intermediaire/IntermediaireG.h"
class MDV
{
    //temp
    double TEMPS_CYCLE = 0.005; // s (5ms)

    //constantes
    double M_PI = 3.1415926;
    double M_MIN_TO_M_S = 1.0/60;
    double MASSE_VOLUMIQUE_AIR = 1.22; //kg/m^3

    //IntermediaireG
    IntermediaireG i = IntermediaireG();

    //transmission
    double RATIO_T_PONT = 5.82;
    double rendementTransmission = 0.92;
    double rapportTransmission[7] = {4.24,3.36,1.91,1.42,1,0.72,0.62}; // gear : R - 1 - 2 - 3 - 4 - 5 - 6
    double plageVitesse[7][2] = {{0,0},{1.643,6.318},{2.89,11.114},{3.887,14.949},{5.519,21.227},{7.665,29.482},{8.902,34.238}};
    double coefficientEquation[5] = {0.000000002351,10422.766,-0.0000069,-0.0094361,207.25751};

    //moteur
    Engine moteur;
    double pFrein;
    double pAcc;
    double pEmb;

    //structure
    double surfaceFrontale = 5; //m^2
    double trainee = 0.8; //Cx
    double coeffResistanceRoulement = 0.008;
    double masse = 16000; // kg

    //roue
    double rayonRoue = 0.4719; // mètre

    //vehicule
    double v_auto = 15; // m/s
    int gear = 2;
    bool pointMort = true;

    public:
        MDV(){
            double coeffCouple[4] = {0.0000002,-0.0015774,2.8671445,191.44755};
            double coeffConso[3] = {0.0000304,-0.0812751,243.36373};
            moteur = Engine(coeffCouple,coeffConso);
        }

        void fct(){
            v_auto = 0;
            moteur.demarrerMoteur(WheelToRpm(v_auto));
            pFrein = 0;
            pAcc = .81;
            majInter();
        }

        double getRotationMoteur(){
            return moteur.getVitesse();
        }
        double getPuissanceMoteur(){
            return moteur.puissanceFournie(pAcc);
        }
        double getCoupleMoteur(){
            return moteur.coupleFourni(pAcc);
        }
        double getConsommationMoteur(double rotation){
            double y = getCoupleMoteur();
            return coefficientEquation[0]*(rotation*rotation*rotation)+coefficientEquation[1]*(1/(rotation+1))+coefficientEquation[2]*rotation*y+coefficientEquation[3]*y+coefficientEquation[4];
        }

        double getVitesse_vehicule(){
            return v_auto;
        }
        double getVitesseKMh(){
            return v_auto*3.6;
        }
        void setpAcc(double a){
            pAcc = a;
        }
    void avancer(int t){
        if(t%1 == 0){
            std::cout <<"\n"<< v_auto << "m/s || "<<getVitesseKMh() <<"km/h - temps :"<<TEMPS_CYCLE*t<<"s || gear ="<<gear<<std::endl;
        }
        accelerationGagnee();
        moteur.majOmegaEngine(WheelToRpm(v_auto),pAcc,gear);
        majInter();
    }
        double vitMotToWheel(){
            std::cout << moteur.getVitesse() << std::endl;
            return (moteur.getVitesse()/(RATIO_T_PONT*rapportTransmission[gear])*(2*M_PI*rayonRoue)*M_MIN_TO_M_S);
        }
        double WheelToRpm(double Wheel){
            return (Wheel*(RATIO_T_PONT*rapportTransmission[gear])/(2*M_PI*rayonRoue)*(1/M_MIN_TO_M_S));
        }

        IntermediaireG* getI(){
            return &i;
        }

        void setGear(int g){
            gear = g;
        }

    protected:
    private:
    double n(double x, double maxi){
        return x/maxi;
    }

    double resistanceAir(double v){
        return (.5*MASSE_VOLUMIQUE_AIR*surfaceFrontale*trainee*v*v);
    }
    double resistanceRoulement(){
        return coeffResistanceRoulement*masse*9.81;
    }


    void accelerationGagnee(){
        double v = plageVitesse[gear][1]*pAcc+plageVitesse[gear][0]*(1-pAcc);
        double Fmoteur = 0.5*masse*((v*v)-(v_auto*v_auto))*rendementTransmission;
        if(Fmoteur < 0){
            Fmoteur = Fmoteur*(0.6*(moteur.getVitesse()/2500));
        }
        double Fres = resistanceAir(v_auto)+resistanceRoulement();
        v_auto += ((Fmoteur-Fres)/masse)*TEMPS_CYCLE;
    }


    //Voisin inf' gauche
    void majInter(){
        i.majMDV(moteur.activationRalenti, gear, v_auto,moteur.getVitesse(), moteur.puissanceFournie(pAcc), pAcc, pFrein, getConsommationMoteur(moteur.getVitesse()));
    }
};

#endif // MDV_H
