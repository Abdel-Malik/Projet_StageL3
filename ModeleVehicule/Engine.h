#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <vector>
#include <math.h>
#include <string.h>
#include <windows.h>

class Engine
{
    //constantes
    double M_PI = 3.1415926;
    //--
    double CONSO_RALENTI = 3.46; // l/h
    double RALENTI_MIN = 620;
    double ACTIVATION_RALENTI = 710;
    double ROT_MIN = 800; // (= RALENTI_MAX)
    double ROT_MAX = 2500;

    //variables
    double acceleration = 0;
    public:
    bool activationRalenti = false;
    double omega = 0; // tr/min
    double coeffMoindreCarreC[4];
    double coeffMoindreCarreCons[3];
    bool fonctionne = false; //permet de savoir si le moteur ne fonctionne plus (éteint <=> calage)
    bool rupteur = false;

    public:
        Engine(){
        };
        Engine(double coeffC[], double coeffCons[]){
            for(int i = 0; i <3 ; i++){
                coeffMoindreCarreC[i] = coeffC[i];
                coeffMoindreCarreCons[i] = coeffCons[i];
            }
            coeffMoindreCarreC[3] = coeffC[3];

        }

        void majOmegaEngine(double rotationRoue, double coeff, int gear){
            if(omega < RALENTI_MIN){
                if(gear > 1){
                    fonctionne = false; //calage
                    activationRalenti = false;
                    rupteur = false;
                }else{
                    fonctionne = true;
                    activationRalenti = true;
                    rupteur = false;
                }
            }else if(omega <= ACTIVATION_RALENTI){
                fonctionne = true;
                activationRalenti = true;
                rupteur = false;
            }else if(omega > ROT_MIN){
                fonctionne = true;
                activationRalenti = false;
                rupteur = false;
            }else if(omega > ROT_MAX){
                rupteur = true;
            }
            omega = rotationRoue;
            std::cout << omega << " rotation" << std::endl;
        }

        double getVitesse(){
            return omega;
        }
        //Watt
        double puissanceFournie(double coeff){
            double res = 0;
            if(fonctionne && accelerationB() && !rupteur)
                res = (omega*(M_PI/30)*coupleFourni(coeff));
            if(res < 0){
                res = 12.085;
            }
            return res;
        }
        //N.m
        double coupleFourni(double coeff){
            return coeff*calculPolynome(omega,coeffMoindreCarreC);
        }
        //g/kW.h
        double essenceConsommee(){
            return calculPolynome(omega,coeffMoindreCarreCons);
        }
        void setAcceleration(double acc){
            if(acc){
                if(activationRalenti){
                    activationRalenti = false;
                }
            }
            acceleration = acc;
        }
        void demarrerMoteur(double rotationRoue){
            if(!fonctionne){
                Sleep(10);
                activationRalenti = true;
                Sleep(10);
                if(rotationRoue > ACTIVATION_RALENTI)
                    omega = rotationRoue;
                else
                    omega = ACTIVATION_RALENTI;
                fonctionne = true;
            }
        }

        double puissanceResistante(double coeff){
            return 0*coeff;
        }
    protected:
    private:

        double augmentationRegime(double coeff){
            return 0*coeff;
        }

    bool accelerationB(){
        return (acceleration == 0);
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

#endif // ENGINE_H
