﻿/**---------------------------------------------------------
 * \author : Abdel-Malik Bouhassoun
 * \date : 26 Mai 2017
 * \file Ce header contient la classe servant à stocker et traiter
 * les données relatives à l'interpolation
 *
 *A modifier. faire grille interpolation (pour le calcul de l'interpolation et un héritage pour l'interpolation BSFC!
 */

#ifndef _GrilleInterpolation_h_
#define _GrilleInterpolation_h_
#include "Point.h"
#include <windows.h>
#define X_AXE (11)
#define Y_AXE (11)
#define Z_AXE (11)

class GrilleInterpolation{

    //--**attributs**--//
    std::vector<std::vector<Point> > grillePoints;
    int echantillonnage;

    //-**Méthodes**-//

    public:
    /*Constructeurs*/
    GrilleInterpolation(){
    };
    GrilleInterpolation(ModeConduite mode, double* axeZ, int ech){
        echantillonnage = ech;
        initialisationTableau2DDePoints();
        if(mode == ModeConduite::ECO)
            initialisationPointsEco(axeZ);
        else
            initialisationPointsPerf(axeZ);
    };

    /*Méthodes publiques*/


    /** \brief Réaliser une interpolation à l'aide des données de grillesPoints
     * \param[in] vit La vitesse du véhicule
     * \param[in] chM la charge moteur appliquée
     * \param[out] double La valeur interpolée
     */
    //Avec les paramêtres d'entrées, deux points (extrémitées opposées du rectangle) sont récupérées, un calcul d'interpolation est appellé à l'aide de ces points
    double interpolerPoint(double vit, double chM){
        vit = vit/10;
        std::cout << "vit : " << vit << " -- p : " << chM << std::endl;
        bool infGauche = false;
        bool supDroit = false;
        int bas[]={-1,-1}, haut[]={-1,-1};
        int i =1, j=1;

        while(haut[0] == -1 && i<echantillonnage){
            if(grillePoints[i][0].get1eD() > vit){
                bas[0] = i-1;
                haut[0] = i;
                infGauche = true;
            }
            i++;
        }
        while(haut[1] == -1 && j<echantillonnage){std::cout << "test "<<grillePoints[0][j].get2eD()<<" et p " << chM << " " << echantillonnage<<std::endl;
            if(grillePoints[0][j].get2eD() > chM){
                bas[1] = j-1;
                haut[1] = j;
                supDroit = true;
            }
            j++;
        }
        if(!infGauche){
            bas[0] = echantillonnage-1;
            haut[0] = echantillonnage-1;
            if(bas[1] == -1){
                bas[1] = echantillonnage-1;
                haut[1] = echantillonnage-1;
            }
        }
        if(!supDroit){
            haut[1] = echantillonnage-1;
            if(haut[0] == -1){
                haut[0] = echantillonnage-1;
            }
        }
        std::cout << "bas : " << bas[0] <<" "<< bas[1] << " haut : " << haut[0]<<" "<< haut[1] << std::endl;
        return interpoler(bas,haut,vit,chM);
    };

    /*getter*/

    /*setter*/
    void setEchantillonnage(int e){
        echantillonnage = e;
    };

    private:

    //--**Méthodes privées**--//

    /** \brief initialisation de la structure avec des objets Point vides.
     */
    //La structure modélise un tableau à deux dimensions.
    //Pour ce faire on utilise un Vector contenant dans chacune de ses case un second Vector.
    void initialisationTableau2DDePoints(){
        grillePoints = std::vector< std::vector<Point> >(echantillonnage);
        for(int i = 0; i < echantillonnage; i++){
            grillePoints[i] = std::vector<Point>(echantillonnage);
        }
    }

    /** \brief Initialisation des Points de la grillePoints en vue d'une interpolation de consommation.
     * \param[in] axeZ un tableau de taille echantillonage². Ce tableau contient les valeur de consommation (g/kW)
     */
    //données organisés comme suit : [x0-y0;...;x0-yMax ;x1-y0..;x1-yMax ; xMax - yMax]
    void initialisationPointsEco(double* axeZ){
        double pasVitesse = (IntermediaireG::RPM_MAX-IntermediaireG::RPM_MIN)/echantillonnage;
        double pasCharge = (IntermediaireG::PUISSANCE_MAX-IntermediaireG::PUISSANCE_MIN)/echantillonnage;
        for(int i = 0; i < echantillonnage; i++){
            for(int j = 0; j < echantillonnage; j++){
                grillePoints[i][j] = Point(pasVitesse*i,pasCharge*j,axeZ[echantillonnage*i+j]);
            }
        }
    };


    /** \brief Initialisation des Points de la grillePoints en vue d'une interpolation d'accéleration.
     * \param[in] axeZ un tableau de taille echantillonage². Ce tableau contient les valeur d'accéleration (m.s^-2)
     * données organisés comme suit : [x0-y0;...;x0-yMax ;x1-y0..;x1-yMax ; xMax - yMax]
     */
    void initialisationPointsPerf(double axeZ[]){
        double pasVitesse = (IntermediaireG::VITESSE_MAX-IntermediaireG::VITESSE_MIN)/echantillonnage;
        double pasCharge = (IntermediaireG::CHARGE_MAX-IntermediaireG::CHARGE_MIN)/echantillonnage;
        for(int i = 0; i < echantillonnage; i++){
            for(int j = 0; j < echantillonnage; j++){
                grillePoints[i][j] = Point(pasVitesse*i,pasCharge*j,axeZ[echantillonnage*i+j]);
            }
        }
    };

    /** \brief réaliser une interpolation de la surface représentée par la grillePoints
     * \param[in] coinIG coin inférieur Gauche
     * \param[in] coinSD coin supérieur droit
     * ; permettant de connaître les 4 points utilisé pour l'interpolation bilinéaire.
     * \param[in] vit la vitesse du véhicule
     * \param[in] chM la charge moteur du véhicule
     * \param[out] double La valeur d'interpolation de la surface en fonction de 'vit' et 'chM'
     */
    double interpoler(int coinIG[], int coinSD[], double vit, double chM){
        double d = grillePoints[coinIG[0]][coinIG[1]].get3eD();
        double a = grillePoints[coinSD[0]][coinIG[1]].get3eD()-d;
        double b = grillePoints[coinIG[0]][coinSD[1]].get3eD()-d;
        double c = (grillePoints[coinSD[0]][coinSD[1]].get3eD())-(grillePoints[coinSD[0]][coinIG[1]].get3eD()+grillePoints[coinIG[0]][coinSD[1]].get3eD()+d);
        double dx = (vit-grillePoints[coinIG[0]][0].get1eD())/(grillePoints[coinSD[0]][0].get1eD()-grillePoints[coinIG[0]][0].get1eD());
        double dy = (chM-grillePoints[0][coinIG[1]].get2eD())/(grillePoints[0][coinSD[1]].get2eD()-grillePoints[0][coinIG[1]].get2eD());
        if(grillePoints[coinSD[0]][0].get1eD() == grillePoints[coinIG[0]][0].get1eD())
            dx = 0;
        if(grillePoints[0][coinSD[1]].get2eD() == grillePoints[0][coinIG[1]].get2eD())
            dy = 0;
        return ((a*dx)+(b*dy)+(c*dx*dy)+d);
    };

    void test(){
        for(int i = 0; i < echantillonnage; i++){ //Y_AXE
            for(int j = 0; j < echantillonnage; j++){ //X_AXE
                std::cout << grillePoints[i][j].get3eD() << "i "<< i<<"j "<<j<< std::endl;
            }
        }
        Sleep(100);
    };

};


//signatures autres

#endif

