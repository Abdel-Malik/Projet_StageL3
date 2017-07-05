/**----------------------------------------------------------------------
 * \author Abdel-Malik Bouhassoun
 * \date : 31 Mai 2017
 * \file Ce header contient une classe intermédiaire
 * Cette classe permet de déplacer le problème de la réception de données
 * des classes principales pour pouvoir terminer la création de celles-ci.
 */

#ifndef _Intermediaire_h_
#define _Intermediaire_h_

#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>

class Intermediaire{

    //-**attributs**-//
    //Véhicule
    bool ralentiMot;
    double vitesseVehicule;
    double puissanceMoteur;
    double consommation;


    //-**Méthodes**-//

    public:
    //Constructeurs
    Intermediaire(){
        recuperationDonnees();
    };
    /*Méthodes publiques*/

    /*getter*/
    double getVitesse(){
        return vitesseVehicule;
    };
    bool ralentiMoteur(){
        bool res = true;
        if(vitesseVehicule > 5)
            res = false;
        return res;
    };
    double getPuissanceMoteur(){
        return puissanceMoteur;
    };
    double getConsommation(){
        return consommation;
    };
    /*setter*/

    //-**Méthodes privées**-//

    private:
    void recuperationDonnees(){
        ralentiMot = false;
        vitesseVehicule = 110;
        puissanceMoteur = 90;
        consommation = 430;
    };
};

//signatures autres

#endif
