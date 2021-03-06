﻿/**------------------------------
 * \author Abdel-Malik Bouhassoun
 * \date 29 Mai 2017
 * \file outilDichotomie.h
 * \brief Ce header contient une énumération et une classe permettant de réaliser une méthode de dichotomie
 */

#ifndef _outilDichotomie_h_
#define _outilDichotomie_h_

//--************************************--//
//--****** definition énumération ******--//
//--************************************--//
/** \enum ChangementPression
* \brief Référence les évolutions de pression possible
*/
enum ChangementPression{
    SERRER,
    RELACHER
};

//--************************************--//
//--******   definition Classe    ******--//
//--************************************--//

/** \class BornesDichotomie
* \brief permet de réaliser une dichotomie dans un intervalle normalisé ([0;1])
*/
class BornesDichotomie{

    //--*attributs*--//
    static constexpr double ZERO_DICHOTOMIE_REALISEE = -1;
    double inf = 0;
    double sup = 1;
    double index = ZERO_DICHOTOMIE_REALISEE;

    //--*Méthodes*--//
    public:
    /*Constructeurs*/
    BornesDichotomie(){
    };

    /*méthodes*/

    /** \brief  modifie la valeur de freinage par dichotomie
     * \param[in] choix l'évolution souhaité d'une étape de dichotomie (Serrer / Relacher)
     */
    void dichotomie(ChangementPression choix){
        if(index == ZERO_DICHOTOMIE_REALISEE)
            index = (inf+sup)/2;
        else{
            if(choix == ChangementPression::SERRER){
                inf = index;
                index = (inf+sup)/2;
            }
            if(choix == ChangementPression::RELACHER){
                sup = index;
                index = (inf+sup)/2;
            }
        }
    };

    /** \brief  mettre à jour l'intervalle d'exécution de la dichotomie (zone réduisant l'intervalle de pressions applicable)
     */
    void versIntervalleFaible(){
        sup = index;
        inf = 0;
        index = ZERO_DICHOTOMIE_REALISEE;
    };

    /** \brief  mettre à jour l'intervalle d'exécution de la dichotomie (zone augmentant l'intervalle de pressions applicable)
     */
    void versIntervalleFort(){
        sup = 1;
        inf = index;
        index = ZERO_DICHOTOMIE_REALISEE;
    };

    /*getter*/
    double getInf(){
        return inf;
    };
    double getSup(){
        return sup;
    };
    double getIndex(){
        return index;
    };

    /*setter*/
    void setInf(double i){
        inf = i;
    };
    void setSup(double s){
        sup = s;
    };
    void setIndex(double ind){
        index = ind;
    };

};

#endif
