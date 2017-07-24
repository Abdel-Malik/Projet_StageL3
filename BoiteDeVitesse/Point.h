/**-------------------------------------------------------------
 * \author Abdel-Malik Bouhassoun
 * \date 26 Mai 2017
 * \file Ce header contient une implémentation d'une classe point (3D)
 */
#ifndef _Point_h_
#define _Point_h_

class Point{

    //-**attributs**-//
    double x;
    double y;
    double z;

    //-**Méthodes**-//
    public:

    /*Constructeurs*/
    Point(){
        x = 0;
        y = 0;
        z = 0;
    };
    Point(double X, double Y, double Z){
        x = X;
        x = Y;
        z = Z;
    };

    /*getter*/
    double get1eD(){
        return x;
    }
    double get2eD(){
        return y;
    }
    double get3eD(){
        return z;
    }
};

#endif

