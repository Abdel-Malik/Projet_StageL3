/**-----------------------------------------------
 * \author Abdel-Malik Bouhassoun
 * \date 26 Mai 2017
 * \file [GearBoxAI.h]Ce header contient l'implémentation d'une boîte de vitesses
 */

#ifndef _GearAI_h_
#define _GearAI_h_

#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>
#include "E_ModeConduite.h"
#include "Point.h"
#include "../Intermediaire/IntermediaireG.h" //<>

class GearBoxAI{

    //--**attributs**--//
    enum ModeConduite mode;
    std::vector<double> demultiplication;
    IntermediaireG* informations; // <>
    double rotation_moteur, rotationT, puissance, consommation;
    bool marcheArriere;
    int gear;

    //--**Méthodes**--//
    public:
    //Constructeurs
    GearBoxAI(IntermediaireG* i){
        informations = i;
        recuperationDesRapportsDeTransmissions();
        recuperationDonnees();
    };

    /*Méthodes publiques*/

    /** \brief Fonction global réalisant l'appel de sous-fonctions
    * le but final est d'obtenir la meilleur vitesse en fonction du mode effectif
    * Cas spécial : Si la marche arrière est enclenchée, aucun test n'est effectué
    */
    int optimiserRapport(){
        recuperationDonnees();
        if(!marcheArriere){}
            rapportCoherent();
            if((mode == ModeConduite::ECO && chargeMoteur() <= 0.95) || (mode == ModeConduite::PERF && chargeMoteur() < 0.2)){
                optimiserConsommation();
            }else{
                optimiserPuissance();
            }
        }
        return gear;
    };

    /*getter*/

    /*setter*/

    //--**Méthodes privées**--//

    private:

    /** \brief Copie les 'rapports de transmissions' dans un vector
    * l'élément [0] correspond au rapport de la marche arrière
    */
    void recuperationDesRapportsDeTransmissions(){
        for(int i = 0 ; i <=(informations->GEAR_MAX-informations->GEAR_MIN); i++) //<>
            demultiplication.push_back(informations->getRapportBoiteDeVitesse(i));
    }

    /** \brief Vérifie que la vitesse engagée entraine une rotation moteur dans la plage de rotation dans laquelle il est foncitonnel.
    * Si la rotation moteur devient trop faible/importante un changement de vitesse sera opéré avant tout autre test.
    */
    void rapportCoherent(){
        double gearT = gear;
        while(nouvelleRotationMoteur(gearT) > informations->RPM_MAX && gearT < 6) // <>
            gearT++;
        while(nouvelleRotationMoteur(gearT) < informations->RPM_MIN && gearT > 1) // <>
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
        mode = informations->getMode();
        if(gear == 0)
            marcheArriere = true;
        else
            marcheArriere = false;
        rotation_moteur = rotationMoteurCourante();
        puissance = puissanceCourante();
        consommation = consommationCourante();
    };


    /** \brief Modifier le rapport de vitesse courant du véhicule
     * \param[in] ng Le rapport de vitesse à appliquer
     */
    void changeGear(int ng){
        gear = ng;
    }

    /** \brief trouver le rapport optimisant l'accéleration du véhicule
     */
    //Appel à une boucle externe
    void optimiserPuissance(){
        puissance = puissanceCourante();
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
     * \return Le rapport[supérieur/inférieur] optimal
     */
    //Si aucun rapport optimal n'est trouvé, le rapport courant est retourné
    int boucleMeilleurPuissance(int delta){
        double pCalculee = 0;
        int gearT = gear+delta;
        int ng = gear;
        rotationT = nouvelleRotationMoteur(gearT);
        while(gearT > informations->GEAR_MIN && gearT <= informations->GEAR_MAX && (rotationT >= informations->RPM_MIN && rotationT <= informations->RPM_MAX)){ //<>
            pCalculee = puissanceTheorique();
            std::cout << "pour g = "<<gearT<<" => "<<rotationT<<" pC = "<<pCalculee<<" | " << "cur'G "<<gear<<" => "<<rotation_moteur<<" p = " << puissance << std::endl;
            if(puissance < 0.99*pCalculee){
                ng = gearT;
                puissance = pCalculee;
            }
            gearT += delta;
            rotationT = nouvelleRotationMoteur(gearT);
        }
        return ng;
    };


    /** \brief Retourner le rapport optimal
     * \param[in] delta incrémente/décrémente le rapport de vitesse testé
     * \return Le rapport[supérieur/inférieur] optimal
     */
    //Si aucun rapport optimal n'est trouvé, le rapport courant est retourné
    int boucleMeilleurConso(int delta){
        double consoCalculee;
        int gearT = gear+delta;
        int ng = gear;
        rotationT = nouvelleRotationMoteur(gearT);
        while(gearT > informations->GEAR_MIN && gearT <= informations->GEAR_MAX && (rotationT >= informations->RPM_MIN && rotationT <= informations->RPM_MAX)){ // <>
            consoCalculee = consommationTheorique();
            std::cout << "pour g = "<<gearT<<" => "<<rotationT<<" cC = "<<consoCalculee<<" | " << "cur'G "<<gear<<" => "<<rotation_moteur<<" conso = " << consommation << std::endl;
            if(consommation > (1.01*consoCalculee)){ //101% afin de laisser une zone dans laquelle il n'y a pas de changement (pour eviter le cas de changement répétitif dû à deux valeurs identiques)
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
