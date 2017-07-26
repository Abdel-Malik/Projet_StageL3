/**------------------------------
 * \Author Abdel-Malik Bouhassoun
 * \date 29 Mai 2017
 * Ce header contient une classe roue ayant pour but d'être manipulée par l'ABS
 */

#ifndef _Roue_h_
#define _Roue_h_

#include "../IntermediaireG/IntermediaireG.h"
#include "outilDichotomie.h"

//--************************************--//
//--******   definition Classe    ******--//
//--************************************--//

/** \class La classe Roue est la classe se raportant à une roue du véhicule simulé,
*cette roue contient les informations utiles pour une manipulation par l'ABS.
*/
class Roue{

    //--*attributs*--//
    IntermediaireG* inter;
    int noRoue;
    double vitesseAngulaire;
    double rayon;
    double glissement;
    BornesDichotomie bornes;

    //--*Méthodes*--//
    public:
    /*Constructeurs*/
    Roue(double r, IntermediaireG* i, int no){
        noRoue = no;
        inter = i;
        bornes = BornesDichotomie();
        vitesseAngulaire = 0;
        rayon = r;
        glissement = 0;
    };
    /*méthodes*/


    /** \brief calculer le glissement de la roue en fonction de la vitesse du véhicule.
     * \param[in] v_auto la vitesse du véhicule 'en m/s'
     */
    void glissementRoue(double v_auto){
        glissement = (v_auto-(2*inter->MATH_PI*vitesseAngulaire*rayon))/v_auto; //2PI*rayon*vitesseAngulaire en (tr/s) => vitesse (m/s)
    };

    /** \brief modifier la valeur de freinage par dichotomie
     * \param[in] choix l'évolution souhaité d'une étape de dichotomie (Serrer / Relacher)
     */
    void dichotomie(ChangementPression choix){
        bornes.dichotomie(choix);
    };

    /** \brief mettre à jour l'intervalle d'exécution de la dichotomie (zone réduisant l'intervalle de pressions applicable)
     */
    void bornesIntervalleFaible(){
        bornes.versIntervalleFaible();
    };

    /** \brief mettre à jour l'intervalle d'exécution de la dichotomie (zone augmentant l'intervalle de pressions applicable)
     */
    void bornesIntervalleFort(){
        bornes.versIntervalleFort();
    };

    /*getter*/

    double getVitesseAngulaire(){
        return vitesseAngulaire;
    };
    double getRayon(){
        return rayon;
    };
    double getGlissement(){
        return glissement;
    };

    double pressionAppliquee(){
        return bornes.getIndex();
    };

    /*setter*/

    void setVitesseAngulaire(double va){
        vitesseAngulaire = va;
    };
    void setRayon(double r){
        rayon = r;
    };
    void setGlissement(double g){
        glissement = g;
    };

    void setFreinageRoue(double f){
        bornes.setSup(f);
    }

    /** \brief mettre à jour l'intervalle d'exécution de la dichotomie (zone augmentant l'intervalle de pressions applicable)
     */
    void majDonnees(IntermediaireG* inter){
        vitesseAngulaire = (*inter).getVitesseAngulaire(noRoue);
    };

    void affichageRoue(double v_auto){
        std::cout << "Glissment Roue_"<<noRoue<<" : "<< glissement<<" vitesses : voiture_"<<v_auto<<"(m/s) ; roue_"<<inter->MATH_PI*2*vitesseAngulaire*rayon<< std:: endl;
    }
};

#endif
