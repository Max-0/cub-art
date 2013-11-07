#ifndef core
#define core

#include<iostream>
#include<string>
#include<deque>
#include<vector>
#include<sstream>

#define meters 10000     //précision au 10ème de mm
#define m meters
#define cm meters/100
#define m3 m*m*m

#define kg 10000  //précision au 10ème de gramme
#define t 1000*kg   //pour tonne

typedef unsigned int uInt;

//lisibilitée du code
enum AXIS {X, Y, Z};

void outputTest(bool, bool, char*);

//structure de donnée pour vecteur de taille 3
struct Vect3D {
    long int x;
    long int y;
    long int z;
};
//création dynamique+initialisation
Vect3D* getNewVect(long int x=0, long int y=0, long int z=0);

//get squarred distance, calcul de la racine carrée long
//fonction très utilisée
float getSqDst(Vect3D* pointOne, Vect3D* pointTwo);
//get on 1 axis distance
float getOnAxisDst(Vect3D* pointOne, Vect3D* pointTwo, AXIS which);

class Box{
    public:
    Box(uInt P_id, Vect3D *P_dim=NULL, Vect3D *P_center=NULL, uInt P_weight = 0);
    virtual ~Box();
    virtual void rotate(AXIS which);
    virtual void move(Vect3D* displacement);
    
    uInt getId();
    virtual long long int getVol();
    virtual long long int getWeight();
    Vect3D* getDim();
    Vect3D* getCenter();

    virtual void setDim(long int x, long int y, long int z);
    virtual void setCenter(long int x, long int y, long int z);

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
    virtual long long int getVol();
    virtual long long int getWeight();
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
    virtual void setDim(long int x, long int y, long int z);
    virtual void setCenter(long int x, long int y, long int z);
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
    SpaceToFill(uInt id, Vect3D* dim, Vect3D* center, uInt weight);
    virtual ~SpaceToFill();
    virtual bool collide(Box* one);     //désactiver
    bool isEmpty();     //(boxStack)
    long long int getVolLeft();
    uInt getStackLen();
    bool isInternable(ObjectBox* toIntern);
    bool isStable(Box* box); // si internée directement
    bool isCompatible(ObjectBox* box);    //comparaison paramètres individuels de chaques boites
    void intern(ObjectBox* box);
    Box* pop();   //pop sur le stack
    Box* back();
    std::deque<ObjectBox*>* getBoxStack();
    //filtres
    long long int getDensity();
    Vect3D* getLargestBoxBehind(float step);    //step : pourcentage 
                                                //de la dimension z, z/step = nb test
    Vect3D* getCenterOfMass();
    Vect3D* getGeoCenter(); //retourne sens géo 
    //espace pour comparaison avec centre de masse
    long long int getQuality();
    
    SpaceToFill* getCopy();

    protected:
    std::deque<ObjectBox*> boxStack;
    double volLeft;
    double volTot;
    double precision;

};

#endif

