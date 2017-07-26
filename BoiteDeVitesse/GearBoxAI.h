/**-----------------------------------------------
 * \author Abdel-Malik Bouhassoun
 * \date 26 Mai 2017
 * \file GearBoxAI.h
 * \brief Ce header contient l'implémentation d'une boîte de vitesses
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

/**\class GearBoxAI
* \brief Cette classe modélise le fonctionnement d'une boîte de vitesses automatisée
*/
class GearBoxAI{

    //--**attributs**--//
    enum ModeConduite mode;

    /**\brief contient le rapport (vitesse_moteur / vitesse_roue) des différentes vitesses
    */
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

    /** \brief (Fonction principale) : Fonction global réalisant l'appel de sous-fonctions
    * \details e but final est d'obtenir la meilleure vitesse en fonction du mode effectif
    * Cas spécial : Si la marche arrière est enclenchée, aucun test n'est effectué
    */
    void optimiserRapport(){
        recuperationDonnees();
        if(!marcheArriere){
            rapportCoherent();
            if((mode == ModeConduite::ECO && chargeMoteur() <= 0.9) || (mode == ModeConduite::PERF && chargeMoteur() < 0.2)){
                optimiserConsommation();
            }else{
                optimiserPuissance();
            }
        }
        informations->setGear(gear);
    };

    /*getter*/

    /*setter*/

    //--**Méthodes privées**--//

    private:

    /** \brief Copie les 'rapports de transmissions' dans un vector
    * \details l'élément [0] correspond au rapport de la marche arrière
    */
    void recuperationDesRapportsDeTransmissions(){
        for(int i = 0 ; i <=informations->getGearMax(); i++) //<>
            demultiplication.push_back(informations->getRapportBoiteDeVitesse(i));
    }

    /** \brief Récupère les données du véhicule prmettant le bon fonctionnement de cette classe
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
    }

    /** \brief Vérifie que la vitesse engagée entraine une rotation moteur dans la plage de rotation dans laquelle il est fonctionnel.
    * \details Si la rotation moteur devient trop faible/importante un changement de vitesse sera opéré avant tout autre test.
    */
    void rapportCoherent(){
        double gearT = gear;
        while(nouvelleRotationMoteur(gearT) > informations->getRegimeMax() && gearT < informations->getGearMax())
            gearT++;
        while(nouvelleRotationMoteur(gearT) < informations->getRegimeMin() && gearT > 1)
            gearT--;
        gear = gearT;
    }

    /** \brief Modifie le rapport de vitesse courant du véhicule
     * \param[in] ng Le rapport de vitesse à appliquer
     */
    void changeGear(int ng){
        gear = ng;
    }

    /** \brief trouve le rapport optimisant l'accéleration du véhicule
     */
    //Appel à une boucle externe
    void optimiserPuissance(){
        puissance = puissanceCourante();
        int ng = boucleMeilleurPuissance(-1);
        if(ng == gear)
            ng = boucleMeilleurPuissance(1);
        changeGear(ng);
    }


    /** \brief trouve le rapport optimisant la consommation du véhicule
     */
    //Appel à une boucle externe
    void optimiserConsommation(){
        consommation = consommationCourante();
        int ng = boucleMeilleurConso(-1);
        if(ng == gear)
            ng = boucleMeilleurConso(1);
        changeGear(ng);
    }


    /** \brief Retourne le rapport optimal maximisant la puissance fournie par le moteur
     * \param[in] delta incrémente/décrémente le rapport de vitesse testé
     * \return Le rapport[supérieur/inférieur] optimal
     */
    //Si aucun rapport optimal n'est trouvé, le rapport courant est retourné
    int boucleMeilleurPuissance(int delta){
        double pCalculee = 0;
        int gearT = gear+delta;
        int ng = gear;
        rotationT = nouvelleRotationMoteur(gearT);
        while(gearT > informations->getGearMin() && gearT <= informations->getGearMax() && (rotationT >= informations->getRegimeMin() && rotationT <= informations->getRegimeMax())){
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
    }


    /** \brief Retourne le rapport optimal minimisant la consommation du moteur
     * \param[in] delta incrémente/décrémente le rapport de vitesse testé
     * \return Le rapport[supérieur/inférieur] optimal
     */
    //Si aucun rapport optimal n'est trouvé, le rapport courant est retourné
    int boucleMeilleurConso(int delta){
        double consoCalculee;
        int gearT = gear+delta;
        int ng = gear;
        rotationT = nouvelleRotationMoteur(gearT);
        while(gearT > informations->getGearMin() && gearT <= informations->getGearMax() && (rotationT >= informations->getRegimeMin() && rotationT <= informations->getRegimeMax())){
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
    }

    /** \brief Retourne le régime hypothétique du moteur si le rapport n'était plus la vitesse courante mais celle passé en paramètre
     * \param[in] gear le nouveau rapport de vitesse utilisé
     * \return le régime moteur avec le rapport "gear"
     */
    double nouvelleRotationMoteur(int gear){
        return (demultiplication[gear]/demultiplication[this->gear])*rotation_moteur;
    }

    /* Fonction lié à l'utilisation d'une classe intermediaire */

    /** \brief retourne le régime moteur actuel
     * \return Un 'double', le régime moteur actuel
     */
    double rotationMoteurCourante(){
        return informations->getRotationMoteur();
    }

    /** \brief retourne la puissance fournie actuel
     * \return Un 'double', la puissance fournie par le moteur
     */
    double puissanceCourante(){
        return informations->getPuissanceMoteur();
    }

    /** \brief retourne la consommation actuel
     * \return Un 'double', la consommation du moteur 'en g/kW.h'
     */
    double consommationCourante(){
        return informations->getConsommation();
    }

    /** \brief retourne la puissance fournie pour un régime donnée
     * \return Un 'double', la puissance fournie par le moteur
     */
    double puissanceTheorique(){
        return informations->getPuissanceMoteur(rotationT);
    }

    /** \brief retourne la consommation du moteur pour un régime donnée
     * \return Un 'double', la consommation du moteur 'en g/kW.h'
     */
    double consommationTheorique(){
        return informations->getConsommation(rotationT);
    }
    double chargeMoteur(){
        return informations->getChargeMoteur();
    }
};

//signatures autres

#endif
