/**-----------------------------------------------
 * \author Abdel-Malik Bouhassoun
 * \date 26 Mai 2017
 * \file Ce header contient la classe intermédiaire Générale
 * Celle-ci permet de regroupper les informations utilisés par les différentes briques logicelles
 */

#ifndef _IntermediaireG_h_
#define _IntermediaireG_h_

#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "../BoiteDeVitesse/E_ModeConduite.h"

/**\class IntermediaireG
* Cette classe permet de faire la liaison entre le simulateur et différentes briques logicielles réalisées. [consommation/Boite de vitesse/ABS]
* \brief Regroupe les informations utilisés par différents briques logicielles.
*/
class IntermediaireG{

    //--**attributs**--//
    //Constantes_véhicule
    public:

    double coeffCouple[4] = {0.0000002,-0.0015774,2.8671445,191.44755};
    double coeffConso[3] = {0.0000304,-0.0812751,243.36373};
    double coefficientEquation[5] = {0.0003697,9852.1396,-0.0000597,-0.9573306,221.96138};
    static constexpr double MATH_PI = 3.1415926;
    static constexpr double EXP_MEMBRE1 = 1.5;
    static constexpr double EXP_MEMBRE2 = -1;
    static constexpr double EXP_MEMBRE3 = 0.9;
    static constexpr double EXP_MEMBRE4 = .45;

    static constexpr double CHARGE_MIN = 0;
    static constexpr double CHARGE_MAX = 1;

    private:

    //Données de l'instance du véhicule
    int gearMin;
    int gearMax;
    double vitesseMin;
    double vitesseMax;
    double regimeMin;
    double regimeMax;
    double consommationRalenti;

    enum ModeConduite mode = ModeConduite::ECO;
    bool ralentiMot = true;
    int currentGear;
    int nbRoues;
    std::vector<double> rayonRoues;
    std::vector<double> rapportTransmission; //gear : R - 1 - 2 ...

    //Interaction Véhicule
    double vitesseVehicule;
    double rotationMoteur;
    double puissanceMoteur;
    double consommation;
    double chargeMoteur;
    double chargeFrein;
    std::vector<double> vitesseRoues;
    std::vector<double> freinRoues;

    //--**Méthodes**--//

    public:
    //Constructeur
    IntermediaireG(){
        recuperationsCaracteristiquesVehicule();
    };

    //*!-----getter-----*!*//

    //données véhicule

    int getGearMax(){
        return gearMax;
    }

    int getGearMin(){
        return gearMin;
    }

    double getRegimeMax(){
        return regimeMax;
    }
    double getRegimeMin(){
        return regimeMin;
    }
    /** \brief Retourne si le moteur est au ralenti
     * \return Un boolean (true) si le moteur est au ralenti, (false) sinon.
     */
    bool ralentiMoteur(){
        return ralentiMot;
    };
    /** \brief Retourne le rapport courant
     * \return Le rapport actuellement utilisé
     */
    int getGear(){
        return currentGear;
    };
    /** \brief Retourne le mode d'utilisation de la boîte de vitesse
     * \return L'élément "ModeConduite" utilisé.
     */
    ModeConduite getMode(){
        return mode;
    };
    /** \brief Retourne le nombre de roue du véhicule
     * \return le nombre de roues
     */
    int getNbRoues(){
        return nbRoues;
    };

    /** \brief Retourne le rayon de la roue (i) du véhicule
     * \return Le rayon de la roue 'en mètre'
     */
    double getRayonRoues(int i){
        return rayonRoues[i];
    };

    //interaction véhicule
    /** \brief Retourne le rapport de conversion de la vitesse de rotation (avant-après) la boîte de vitesse
     * \param[in] le rapport de vitesse dont l'on veut connaître le ratio.
     * \return Le coefficient du rapport (vitesse_moteur / vitesse_roue)
     */
    double getRapportBoiteDeVitesse(int rapport){
        return rapportTransmission[rapport];
    }

    /** \brief Retourne la vitesse du véhicule
     * \return La vitesse du véhicule 'en mètre/seconde'
     */
    double getVitesse(){
        return vitesseVehicule;
    };

    /** \brief Retourne la vitesse de la roue
     * \Return La vitesse de la roue 'en ?WAIT'
     */
    double getVitesseAngulaire(int i){
        return vitesseRoues[i];
    };

    /** \brief Retourne la vitesse de rotation du moteur
     * \Return La vitesse du moteur 'en tour/minute'
     */
    double getRotationMoteur(){
        return rotationMoteur;
    };

    /** \brief Retourne la puissance fourni parle moteur
     * \return La puissance moteur 'en kW'
     */
    double getPuissanceMoteur(){
        return puissanceMoteur/1000;
    };

    /** WAIT
     */
    double getConsommation(){
        return consommation;
    };
    double getConsoRalenti(){
        return 3.48;
    };

    /** \brief calcul la puissance théorique du moteur en fonction du régime
     * \param[in] Régime moteur 'en tour/minute'
     * \return La puissance moteur 'en kW'
     */
    double getPuissanceMoteur(double r){
         return ((MATH_PI/30)*r*getCoupleMoteur(r))/1000;
    };

    /** \brief calcul le couple théorique du moteur en fonction du régime
     * \param[in] Régime moteur 'en tour/minute'
     * \return Le couple moteur 'en N.m'
     */
    double getCoupleMoteur(double r){
        return chargeMoteur*calculPolynome(r,coeffCouple);
    };

    /** \brief calcul la consommation théorique du moteur en fonction du régime
     * \param[in] Régime moteur 'en tour/minute'
     * \return La consommation spécifique'en g/kW.h'
     */
    double getConsommation(double r){
        double y = getCoupleMoteur(r);
        return calculConsommationSpecifique(r,y);
    };


    /** \brief Retourne une valeur de l'appuie sur la pédale d'accélération
     * \return Une valeur dans l'intervalle [0 ; 1]
     */
    double getChargeMoteur(){
        return chargeMoteur;
    };

    /** \brief Retourne une valeur de l'appuie sur la pédale de frein
     * \return Une valeur dans l'intervalle [0 ; 1]
     */
    double getChargeFrein(){
        return chargeFrein;
    };

    /*-----fin_getter-----*/

    //*!-----setter-----*!*//
    void setFreinageRoue(int i, double chrg){
        freinRoues[i] = chrg;
    };

    void setGear(int newGear){
        currentGear = newGear;
    };
    /*-----fin_setter-----*/


    //utilisée lors des tests avec le mdv simpliste (à finir par supprimer)
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

    private:

    /** \brief !TODO!
    * utiliser ces méthodes pour récupérer toutes les informations utilisés par les autres classes
    */
    void recuperationsCaracteristiquesVehicule(){
        recuperationsInformationsRoues();
        recuperationsInformationsMoteur();
        recuperationsInformationsBoiteDeVitesse();
        recuperationsInformationsVehicule();
    }

    void recuperationsInformationsRoues(){
        nbRoues = 4;
        double rayonRoue = .4719; //rayon de la roue (en mètre)
        for(int i = 0; i < nbRoues ; i++){
            rayonRoues.push_back(rayonRoue);
            vitesseRoues.push_back(0);
            freinRoues.push_back(0);
        }
    }
    void recuperationsInformationsMoteur(){
        vitesseMin = 0;
        vitesseMax = 123.26;
        regimeMin = 800;
        regimeMax = 2500;
        consommationRalenti = 3.48;
    }
    void recuperationsInformationsBoiteDeVitesse(){
        gearMin = 0;
        gearMax = 6;
        double ratioT[] = {4.24,3.36,1.91,1.42,1,0.72,0.62};
        for(int i = 0; i <= gearMax ; i++){
            rapportTransmission.push_back(ratioT[i]);
        }
    }
    void recuperationsInformationsVehicule(){
        mode = ModeConduite::ECO;
        ralentiMot = true;
        currentGear = 1;
        vitesseVehicule = 0;
        rotationMoteur = 0;
        puissanceMoteur = 0;
        consommation = 0;
    }

     /** \brief Automatisation du calcul d'un polynôme (une variable) quelque soit le nombre du coefficients
     * Le polynôme est représenté par un tableau de coefficients du degré n au degré 0.
     * \param[in] val La variable pour laquelle on calcul la valeur du polynôme
     * \param[in] coeffs un tableau contenant les coefficients du polynôme
     * \return La valeur du polynôme
     */
    double calculPolynome(double val, double coeffs[]){
        double res = 0;
        double x = 1;
        for(int i = sizeof(coeffs)-1; i>=0;i--){
            res += (x*coeffs[i]);
            x = x*val;
        }
        return res;
    }

    /** \brief Implémentation de la formule du calcul de la consommation spécifique
     * \param[in] rotation le régime moteur 'en tr/min'
     * \param[in] couple le couple fourni par la moteur 'en N.m'
     * \return La valeur de la consommation spécifique
     */
    double calculConsommationSpecifique(double rotation, double couple){
        return coefficientEquation[0]*(pow(rotation,EXP_MEMBRE1))+coefficientEquation[1]*pow((rotation+1),EXP_MEMBRE2)+coefficientEquation[2]*pow(rotation*couple,EXP_MEMBRE3)+coefficientEquation[3]*pow(couple,EXP_MEMBRE4)+coefficientEquation[4];
    }
};
#endif
