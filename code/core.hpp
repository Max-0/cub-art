#ifndef core
#define core

#include<iostream>
#include<string>
#include<deque>
#include<vector>

#define meters 10000     //précision au 10ème de mm
#define m meters
#define cm meters/100
#define m3 m*m*m

#define kg 10000  //précision au 10ème de gramme
#define t 1000*kg   //pour tonne

typedef unsigned int uInt;

//lisibilitée du code
enum AXIS {X=0, Y=1, Z=2};

//structure de donnée pour vecteur de taille 3
struct Vect3D {
    uInt x;
    uInt y;
    uInt z;
};
//création dynamique+initialisation
Vect3D* getNewVect(uInt x=0, uInt y=0, uInt z=0);

//get squarred distance, calcul de la racine carrée long
//fonction très utilisée
float getSqDst(Vect3D* pointOne, Vect3D* pointTwo);
//get on 1 axis distance
float getOnAxisDst(Vect3D* pointOne, Vect3D* pointTwo, AXIS which);

class Box{
    public:
    Box(uInt P_id, Vect3D *P_dim, Vect3D *P_center, uInt P_weight);
    virtual ~Box();
    virtual void rotate(AXIS which);
    virtual void move(Vect3D* displacement);
    
    uInt getId();
    uInt getVol();
    uInt getWeight();
    Vect3D* getDim();
    Vect3D* getCenter();

    void setDim(uInt x, uInt y, uInt z);
    void setCenter(uInt x, uInt y, uInt z);

    virtual bool collide(Box* one);
    static bool isBoxInside(Box* mayInside, Box* mayOutside);
    static bool isPointInside(Vect3D* point, Box* theBox);

    Box* getCopy();

    protected:
    uInt id;
    Vect3D* dim;
    Vect3D* center;
    uInt weight;

};


class ObjectBox : public Box
{
    public:
    ObjectBox(uInt P_id,
              Vect3D* P_dim,
              Vect3D* P_center,
              uInt weight,
              uInt P_maxWeightOnTop, 
              bool P_canMove, 
              bool P_canBeTopped, 
              bool P_canBeBottomed,
              bool* P_canBeRotated,
              bool P_isSingleBox,
              int P_chargingOrder = -1,
              int P_bindedSpace = -1);
    virtual ~ObjectBox();
    void addBox(Box* newBox);
    void delBox(uInt boxIndex);     //delete la box
    uInt getBoxNbOf();
    Box* getBoxNb(uInt nb);
    bool getCanMove();
    bool getCanBeTopped();
    uInt getMaxWeightOnTop();
    bool getCanBeBottomed();
    bool* getCanBeRotated();    //bool[3]
    bool getIsSingleBox();
    int getBindedSpace();
    int getChargingOrder();
    void setCanMove(bool newVal);
    void setCanBeTopped(bool newVal);
    void setMaxWeightOnTop(uInt newVal);
    void setCanBeBottomed(bool newVal);
    void setCanBeRotated(bool* newVal);    //bool[3]
    void setIsSingleBox(bool newVal);
   
    virtual void move(Vect3D* displacement);
    virtual void rotate(AXIS axe);
    virtual bool collide(Box* one);
    ObjectBox* getCopy();

    protected:
    std::deque<Box*> boxList;
    bool canMove;
    bool canBeTopped;
    uInt maxWeightOnTop;
    bool canBeBottomed;
    bool canBeRotated[3];   //1 pour chaque axe
    bool isSingleBox;       //pas de rotation pour objets complexes
    int chargingOrder;
    int bindedSpace;
};

class SpaceToFill : public Box
{
    public:
    SpaceToFill(uInt id, Vect3D* dim, Vect3D* center, uInt weight, 
                double volume, double P_precision);
    virtual ~SpaceToFill();
    virtual bool collide(Box* one);     //désactiver
    bool isEmpty();     //(boxStack)
    uInt getVolLeft();
    uInt getStackLen();
    bool isInternable(ObjectBox* toIntern);
    bool isStable(Box* box); // si internée directement
    bool isCompatible(ObjectBox* box);    //comparaison paramètres individuels de chaques boites
    void intern(Box* box);
    void empty();   //destructrice
    Box* pop();   //pop sur le stack
    Box* back();
    std::deque<Box*>* getBoxStack();
    //filtres
    uInt getDensity();
    Vect3D* getLargestBoxBehind(float step);    //step : pourcentage 
                                                //de la dimension z, z/step = nb test
    Vect3D* getCenterOfMass();
    Vect3D* getGeoCenter(); //retourne sens géo 
    //espace pour comparaison avec centre de masse
    uInt getQuality();

    protected:
    std::deque<Box*> boxStack;
    double volLeft;
    double volTot;
    double precision;

};


/*
    Début algo remplissage
*/

#endif

