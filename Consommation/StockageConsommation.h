/**--------------------------------------------------------
 * \author Abdel-Malik Bouhassoun
 * \date 09 Mai 2017
 * \file StockageConsommation.h
 * \brief Ce header contient deux classes utilisées pour le calcul de
 * consommation d'un véhicule, (instantanée et moyenne)
 * ainsi que le calcul de ses émissions de CO2.
 */
#ifndef _StockageConsommation_h_
#define _StockageConsommation_h_

#include <math.h>
#include <vector>
#include <iostream>
#include <string.h>
#include "../Intermediaire/IntermediaireG.h"//<>

#define rhoDiesel (0.85)
#define CONST_RAPPORT_DIESEL_CO2 (2.65)

using namespace std;

//--**Classe 1 SCI**--//
/**\class StockageConsommationInstantanee
*\brief (SCI) sert à stocker les variables liées à la consommation d'un instant donnée
*/
class StockageConsommationInstantanee{

    //--**Attributs**--//
    /** \brief Q est un 'double' contenant la consommation 'en litre par heure'
    */
	double Q;
	/** \brief speed est un 'double' contenant la vitesse du véhicule 'en kilomètre par heure'
    */
	double speed;
	/** \brief ralenti est un 'booléen' à true si le moteur est au ralenti, false sinon
    */
    bool ralenti;

    //-**Méthodes**-//
    public:
    /*Constructeurs*/
    StockageConsommationInstantanee(){
        Q = 0;
        speed = 0;
        ralenti = false;
    }
    StockageConsommationInstantanee(double speed, bool ralenti){
        Q = 0;
        this->speed = speed;
        this->ralenti = ralenti;
    }

    void affichageConsommationInstantanee(){
        string res;
        double c = Q;
        if(isRalenti()||getSpeed()<1)
            res = " L/h";
        else{
                //conversion
            c = c*(100/getSpeed());
            res = " L/100km";
        }
        cout << "\n" << c << res << endl;
    }

    /*Getter*/
    /** \brief Retourne la consommation instantanée du véhicule.
     * \return Un 'double' contenant la consommation 'en litre par heure'
    */
    double getQ(){
        return Q;
    }
    /** \brief Retourne la vitesse du véhicule.
     * \return Un 'double' 'en km/h'
    */
    double getSpeed(){
        return speed;
    }
    /** \brief Retourne l'état du moteur
     * \return Un 'booléen' à true si le moteur est au ralenti, false sinon
    */
    bool isRalenti(){
        return ralenti;
    }

    /*Setter*/
    void setQ(double Q){
        this->Q = Q;
    }
    void setSpeed(double speed){
        this->speed = speed;
    }
    void setRalenti(bool ralenti){
        this->ralenti = ralenti;
    }
};

//--*Classe 2 -Principale- SCG*--//
/**\class StockageConsommationGeneral
*\brief (SCG) sert à stocker les informations liées à la consommation du véhicule
* \details Contient un tableau (historique) de consommations instantanées
* ainsi que les données moyennes
*/
class StockageConsommationGeneral{

    //-**Attributs**-//
    vector<StockageConsommationInstantanee> sci; //si l'intérêt de retenir toutes les sci est discuté. Il est possible d'employer vector comme une pile. dans ce cas "indexDebut" sera toujours 0 et "nbStock non lu" sera toujours vector.size()
    double consMoyenne, vitesse, rejetCO2, consoRalenti;
    int indexDebut, nbStockNonLu;
    IntermediaireG* intermediaire;//<>

    //-**Méthodes**-//
    public:
    /*Constructeurs*/
    StockageConsommationGeneral(IntermediaireG* i){
        intermediaire = i;
        consMoyenne = 0;
        vitesse = 0;
        indexDebut = 0;
        nbStockNonLu = 0;
        rejetCO2 = 0;
        consoRalenti = consommationRalenti();
    };

    /*Méthodes*/

    /** \brief Réalise une moyenne de la consommation du véhicule
    * \details on retransforme les anciennes moyennes en somme de valeurs (de poids 1)
    * on ajoute les nouveaux relevés et on divise de manière à obtenir une nouvelle moyenne
    */
    void calculConsommationMoyenne(){
        double moyenne = consMoyenne*(indexDebut+1);
        double v = vitesse*(indexDebut+1);
        int index = indexDebut;
        int nonLu = nbStockNonLu;
        for(int i = index; i< nonLu + index; i++){
            moyenne += sci[i].getQ();
            v += sci[i].getSpeed();
        }
        moyenne = moyenne/(nonLu+index);
        v = v/(nonLu+index);
        StockLu(nonLu);
        consMoyenne = moyenne;
        vitesse = v;
    }


    /** \brief (fonction principale) : Réalise un calcul de la consommation du véhicule (à un instant t)
     * \details on récupère les informations nécessaire à la création d'un SCI,
     * on ajout celui-ci au vecteur déjà existant.
     * On modifie les indices de parcours pour la moyenne et,
     * on calcule et stocke Q : la consommation en (l/h) dans le nouveau (SCI)
    */
    void calculConsommationInstantanee(){
        bool ralenti = ralentiMoteur();
        sci.push_back(StockageConsommationInstantanee(vitesseVehicule()*3.6,ralenti));
        ajoutStockage();
        if(ralenti)
            sci[sci.size()-1].setQ(consoRalenti);
        else
            sci[sci.size()-1].setQ((puissanceMoteur()*consommation())/(1000*rhoDiesel));
    }

    /** \brief affichage de la consommation du véhicule
    * \details l'affichage envoie une chaîne sur la sortie standard
    * si la vitesse moyenne est très faible, le résultat est retourné en 'L/h'
    * sinon la valeur est retournée en 'L/100km'
    */
    void affichageConsommationMoyenne(){
        string str = "consommation moyenne : ";
        if(vitesse < 2)
            cout << str << consMoyenne << " L/h" << endl;
        else
            cout << str << consMoyenne*(100/vitesse) << " L/100km" << endl;
    }

    /** \brief affichage de la consommation intantannée du véhicule
    * \details l'affichage envoie une chaîne sur la sortie standard
    */
    void affichageConsommationInstantanee(){
        sci[sci.size()-1].affichageConsommationInstantanee();
    }

    /** \brief Réaliser un calcul du rejet moyen de CO2
    * \details si la vitesse moyenne est très faible, le résultat est retourné en 'kg/h'
    * sinon la valeur est retournée en 'kg/100km'
    */
    void calculRejetCO2(){
        double vMoy = vitesse;
        if(vMoy < 2)
            rejetCO2 = consMoyenne*CONST_RAPPORT_DIESEL_CO2;
        else
            rejetCO2 = consMoyenne*(100/vMoy)*CONST_RAPPORT_DIESEL_CO2;
    }

    /*getter*/
    /** \brief Retourne un vector<StockageConsommationInstantanee>.
    * \return Un 'vector<SCI>' contenant les consommations instantanées déjà relevés
    */
    vector<StockageConsommationInstantanee> getTabStockage(){
        return sci;
    }

    /** \brief Retourne un élément de l'attribut vector<StockageConsommationInstantanee>.
    * \return Un 'SCI'
    */
    StockageConsommationInstantanee getSCI(int index){
        return sci[index];
    }

    /** \brief Retourne la consommation moyenne actuelle
    * \return Un 'double', la consommation moyenne calculé
    * ou (0) si aucune consommation moyenne n'a été calculée
    */
    double getConsMoyenne(){
        return consMoyenne;
    }

    /** \brief Retourne la vitesse moyenne du véhicule
    * \return Un 'double', la vitesse moyenne calculé du véhicule
    * ou (0) si aucune vitesse moyenne n'a été calculée
    */
    double getVitesseMoyenne(){
        return vitesse;
    }

    /** \brief Retourne le rojet de OC2 moyen du véhicule
    * \return Un 'double', le rejet moyen de CO2 calculé
    * ou (0) si aucun rejet moyen n'a été calculée
    */
    double getRejetCO2(){
        return rejetCO2;
    }
    int getStockNonLu(){
        return nbStockNonLu;
    }
    int getIndex(){
        return indexDebut;
    }

    void ajoutStockage(){   //ajout à faire ici à la fin
        nbStockNonLu = nbStockNonLu+1;
    }
    void StockLu(int k){    //ajout à faire ici à la fin
        nbStockNonLu -= k;
        indexDebut +=k;
    }

    private:

    /* Fonction lié à l'utilisation d'une classe intermediaire */

    double puissanceMoteur(){
        return intermediaire->getPuissanceMoteur();
    }
    double consommation(){
        return intermediaire->getConsommation();
    }

    //m/s
    double vitesseVehicule(){
        return intermediaire->getVitesse();
    }
    bool ralentiMoteur(){
        return intermediaire->ralentiMoteur();
    }
    double consommationRalenti(){
        return intermediaire->getConsoRalenti();
    }
};

#endif
