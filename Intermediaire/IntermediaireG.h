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
#include <math.h>

#include "../BoiteDeVitesse/E_ModeConduite.h"

class IntermediaireG{

    //--**attributs**--//

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
    enum ModeConduite mode;
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

    //--**Méthodes**--//

    public:
    //Constructeur
    IntermediaireG(int nbRoues, double rayonRoue){
        mode = ModeConduite::ECO;
        ralentiMot = false;
        nbRoues = nbRoues;
        for(int i = 0; i < nbRoues ; i++){
            rayonRoues.push_back(rayonRoue);
            vitesseRoues.push_back(0);
            freinRoues.push_back(0);
        }
    };

    /*-----getter-----*/
    //données véhicule
    /** \brief Retourner si le moteur est au ralenti
     * \return Un boolean (true) si le moteur est au ralenti, (false) sinon.
     */
    bool ralentiMoteur(){
        return ralentiMot;
    };
    /** \brief Retourner le rapport courant
     * \return Le rapport actuellement utilisé
     */
    int getGear(){
        return currentGear;
    };
    /** \brief Retourner le mode d'utilisation de la boîte de vitesse
     * \return L'élément "ModeConduite" utilisé.
     */
    ModeConduite getMode(){
        return mode;
    };
    /** \brief Retourner le nombre de roue du véhicule
     * \return le nombre de roues
     */
    int getNbRoues(){
        return nbRoues;
    };

    /** \brief Retourner le rayon de la roue (i) du véhicule
     * \return Le rayon de la roue 'en mètre'
     */
    double getRayonRoues(int i){
        return rayonRoues[i];
    };

    //interaction véhicule
    /** \brief Retourner le rapport de conversion de la vitesse de rotation (avant-après) la boîte de vitesse
     * \param[in] le rapport de vitesse dont l'on veut connaître le ratio.
     * \return Le coefficient du rapport (vitesse_moteur / vitesse_roue)
     */
    double getRapportBoiteDeVitesse(int rapport){
        return rapportTransmission[rapport];
    }

    /** \brief Retourner la vitesse du véhicule
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

    /** \brief Retourner la vitesse de rotation du moteur
     * \Return La vitesse du moteur 'en tour/minute'
     */
    double getRotationMoteur(){
        return rotationMoteur;
    };

    /** \brief Retourner la puissance fourni parle moteur
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
         return ((M_PI/30)*r*getCoupleMoteur(r))/1000;
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
        return coefficientEquation[0]*(r*r*r)+coefficientEquation[1]*(1/(r+1))+coefficientEquation[2]*r*y+coefficientEquation[3]*y+coefficientEquation[4];
    };

    /*wait
    */
    double consoAvecCoeff(double r){
        double y = getPuissanceMoteur(r);
        double yTh = y/chargeMoteur;
        double conso = coefficientEquation[0]*(r*r)+coefficientEquation[1]*y+coefficientEquation[2];
        double consoTh = coefficientEquation[0]*(r*r)+coefficientEquation[1]*yTh+coefficientEquation[2];
        return ((conso)*(1+ALPHA*(1-((coeffCouple[0]*r*r+coeffCouple[1]*r+coeffCouple[2])/consoTh))));
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

    private:

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
    double n(double x, double maxi){
        return x/maxi;
    }
};
#endif
