/**-----------------------------------------------
 * \author Abdel-Malik Bouhassoun
 * \date 26 Mai 2017
 * \file Ce header contient ...todo...
 */

#ifndef _GearAI_h_
#define _GearAI_h_

#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include "E_ModeConduite.h"
#include "Point.h"
#include "../Intermediaire/IntermediaireG.h"

/** \def Constante PI
 */
#define M_PI (3.1415926)

class GearBoxAI{

    //--**attributs**--//
    enum ModeConduite mode = ModeConduite::ECO;
    std::vector<double> demultiplication;
    IntermediaireG* informations;
    double rotation_moteur, rotationT, puissance, consommation;
    double rapportTransmission[7] = {4.24,3.36,1.91,1.42,1,0.72,0.62}; // gear : R - 1 - 2 - 3 - 4 - 5 - 6 A récupérer au final par intermediaire
    bool marcheArriere;
    int gear;

    //--**Méthodes**--//
    public:
    //Constructeurs
    GearBoxAI(IntermediaireG* i){
        informations = i;
        for(int i = 0 ; i <(informations->GEAR_MAX-informations->GEAR_MIN); i++)
            demultiplication.push_back(rapportTransmission[i]);
        recuperationDonnees();
    };

    /*Méthodes publiques*/


    int optimiserRapport(){
        recuperationDonnees();
        rapportCoherent();
        if((mode == ModeConduite::ECO && chargeMoteur() <= 1.7) || (mode == ModeConduite::PERF && chargeMoteur() < 0.3)){
            optimiserConsommation();
        }else{
            optimiserPuissance();
        }
        return gear;
    };

    /*getter*/

    /*setter*/

    //--**Méthodes privées**--//

    private:

    void rapportCoherent(){
        double gearT = gear;
        while(nouvelleRotationMoteur(gearT) > informations->RPM_MAX && gearT < 6)
            gearT++;
        while(nouvelleRotationMoteur(gearT) < informations->RPM_MIN && gearT > 1)
            gearT--;
        gear = gearT;
    }
    double rotationMoteurCourante(){
        return informations->getRotationMoteur();
    }
    double puissanceCourante(){
        return informations->getPuissanceMoteur();
    }
    double consommationCourante(){
        return informations->getConsommation();
    }

    double puissanceTheorique(){
        return informations->getPuissanceMoteur(rotationT);
    }
    double consommationTheorique(){
        return informations->getConsommation(rotationT);
    }

    double chargeMoteur(){
        return informations->getChargeMoteur();
    }

    /** \brief Récupérer les données du véhicule prmettant le bon fonctionnement de cette classe
     */
    void recuperationDonnees(){
        gear = informations->getGear();
        if(gear == 0)
            marcheArriere = true;
        else
            marcheArriere = false;
        rotation_moteur = rotationMoteurCourante();
        puissance = puissanceCourante();
        consommation = consommationCourante();
    };


    /** \brief Modifier le rapport de vitesse courant du véhicule
     * \param[in] gear Le rapport de vitesse à appliquer
     */
    void changeGear(int ng){
        gear = ng;
    }

    /** \brief trouver le rapport optimisant l'accéleration du véhicule
     */
    //Appel à une boucle externe
    void optimiserPuissance(){
        int ng = boucleMeilleurPuissance(-1);
        if(ng == gear)
            ng = boucleMeilleurPuissance(1);
        changeGear(ng);
    };


    /** \brief trouver le rapport optimisant la consommation du véhicule
     */
    //Appel à une boucle externe
    void optimiserConsommation(){
        consommation = consommationCourante();
        int ng = boucleMeilleurConso(-1);
        if(ng == gear)
            ng = boucleMeilleurConso(1);
        changeGear(ng);
    };


    /** \brief Retourner le rapport optimal
     * \param [in] delta incrémente/décrémente le rapport de vitesse testé
     * \param[out] Le rapport[supérieur/inférieur] optimal
     */
    //Si aucun rapport optimal n'est trouvé, le rapport courant est retourné
    int boucleMeilleurPuissance(int delta){
        double pCalculee;
        int gearT = gear+delta;
        int ng = gear;
        rotationT = nouvelleRotationMoteur(gearT);
        while(gearT > informations->GEAR_MIN && gearT <= informations->GEAR_MAX && (rotationT >= informations->RPM_MIN && rotationT <= informations->RPM_MAX)){
            pCalculee = puissanceTheorique();
            if(puissance < pCalculee){
                ng = gearT;
                puissance = pCalculee;
            }
            gearT += delta;
        }
        return ng;
    };


    /** \brief Retourner le rapport optimal
     * \param[in] delta incrémente/décrémente le rapport de vitesse testé
     * \param[out] Le rapport[supérieur/inférieur] optimal
     */
    //Si aucun rapport optimal n'est trouvé, le rapport courant est retourné
    int boucleMeilleurConso(int delta){
        double consoCalculee;
        int gearT = gear+delta;
        int ng = gear;
        rotationT = nouvelleRotationMoteur(gearT);
            std::cout << "roT : " << rotationT << std::endl;
        while(gearT > informations->GEAR_MIN && gearT < informations->GEAR_MAX && (rotationT >= informations->RPM_MIN && rotationT <= informations->RPM_MAX)){
            consoCalculee = consommationTheorique();
            std::cout << "pour g = "<<gearT<<" => "<<rotationT<<" cC = "<<consoCalculee<<" | " << "cur'G "<<gear<<" => "<<rotation_moteur<<" conso = " << consommation << std::endl;
            if(consommation > consoCalculee){
                ng = gearT;
                consommation = consoCalculee;
            }
            gearT += delta;
            rotationT = nouvelleRotationMoteur(gearT);
        }
        return ng;
    };

    double nouvelleRotationMoteur(int gear){
        return (demultiplication[gear]/demultiplication[this->gear])*rotation_moteur;
    }
};

//signatures autres

#endif
