#include"core.hpp"

void printVect(Vect3D *toPrint){
    std::cout << "(" << toPrint->x << ", " << toPrint->y << ", " << toPrint->z << std::endl;    
}


Vect3D* getNewVect(long int x, long int y, long int z){
    Vect3D* newVect = new Vect3D;
    newVect->x=x;
    newVect->y=y;
    newVect->z=z;
    return newVect;
}


float getSqDst(Vect3D* pointOne, Vect3D* pointTwo){
   float onX = (pointOne->x-pointTwo->x);
   float onY = (pointOne->y-pointTwo->y);
   float onZ = (pointOne->z-pointTwo->z);
   onX *= onX;
   onY *= onY;
   onZ *= onZ;
   //retour toujours + grâce aux carrés
   return (onX+onY+onZ);
}

float getOnAxisDst(Vect3D* pointOne, Vect3D* pointTwo, AXIS which){
       float retour;
       switch(which){
            case X:
                retour=pointOne->x-pointTwo->x;
                break;
            case Y:
                retour=pointOne->y-pointTwo->y;
                break;
            case Z:
                retour=pointOne->z-pointTwo->z;
                break;
            // pas besoin de default
       }
       if(retour < 0)   //cf type de retour
           retour *= -1;
       return retour;
}

Box::Box(uInt P_id, Vect3D *P_dim, Vect3D *P_center, uInt P_weight){
    id=P_id;
    center=P_center;
    dim=P_dim;
    weight=P_weight;
}

Box::~Box(){
    delete dim;
    delete center;
}

/*virtual*/void Box::rotate(AXIS which){
    // cf log : algorithme rotation//axes centré sur centre, limité à 90°
    if(which==Z){
        uInt temp = dim->y;
        dim->y=dim->x;
        dim->x=temp;
    }
    else if(which==Y){
        uInt temp = dim->x;
        dim->x = dim->z;
        dim->z=temp;
    }
    else{
        uInt temp = dim->z;
        dim->z=dim->y;
        dim->y=temp;
    }
}

void Box::move(Vect3D* displacement){
    center->x += displacement->x;
    center->y += displacement->y;
    center->z += displacement->z;
    delete displacement;
}

uInt Box::getId(){
    return id;    
}

long long int Box::getVol(){
    return dim->x*dim->y*dim->z;    
}

long long int Box::getWeight(){
    return weight;    
}

Vect3D* Box::getDim(){
    return dim;    
}

Vect3D* Box::getCenter(){
    return center;
}

void Box::setDim(long int x, long int y, long int z){
    dim->x = x;
    dim->y = y;
    dim->z = z;
}

void Box::setCenter(long int x, long int y, long int z){
    center->x = x;
    center->y = y;
    center->z = z;
}

bool Box::collide(Box* one){
    //cf log : algorithme collision AABB 3D
    bool retour = true;
       //                         x                    >          x'         +            w'          ||
    if( this->getCenter()->x - (this->getDim()->x / 2) > one->getCenter()->x + (one->getDim()->x / 2) or
       //           x        +          w              <                     x'                       ||
        this->getCenter()->x + (this->getDim()->x / 2) < one->getCenter()->x - (one->getDim()->x / 2) or
       //                    y                         >          y'         +             w'         ||
        this->getCenter()->y - (this->getDim()->y / 2) > one->getCenter()->y + (one->getDim()->y / 2) or
       //          y         +              w          <                     y'                       ||
        this->getCenter()->y + (this->getDim()->y / 2) < one->getCenter()->y - (one->getDim()->y / 2) or
       //                    z                         >           z'        +             d'         ||
        this->getCenter()->z - (this->getDim()->z / 2) > one->getCenter()->z + (one->getDim()->z / 2) or
       //          z         +              d          <                     z'
        this->getCenter()->z + (this->getDim()->z / 2) < one->getCenter()->z - (one->getDim()->z / 2) ){
        //then
            retour = false;
       }//fi
    return retour;
}

/*static*/bool Box::isPointInside(Vect3D* point, Box* theBox){
/*
Un point est à l'intérieur d'une boite si
pour chaque dimension, dcp : distance centre(boite)-point,
dim : longueur/largeur/hauteur de la boite selon la dimension considérée,
dcp<dim/2
*/
    bool onX=getOnAxisDst(point, theBox->getCenter(), X)<theBox->getDim()->x/2;
    bool onY=getOnAxisDst(point, theBox->getCenter(), Y)<theBox->getDim()->y/2;
    bool onZ=getOnAxisDst(point, theBox->getCenter(), Z)<theBox->getDim()->z/2;
    return onX && onY && onZ;
}

/*static*/bool Box::isBoxInside(Box* mayInside, Box* mayOutside){
/*
une box est dans l'autre si chaque sommet de la boite est dans une autre
*/  

    Vect3D tops[8];
    /* pour i 0->7
        [0, 3] : +x
        [4, 7] : -x

        [0, 1] u [6, 7] : +y
        [2, 5] : -y
        
        [i%2 == 0] : +z
        [i%2 == 1] : -z
    */
    for(int i = 0 ; i < 8 ; i++){
        if(i < 4)   //+x
            tops[i].x = mayInside->getCenter()->x + (mayInside->getDim()->x / 2); 
        else        //-x
            tops[i].x = mayInside->getCenter()->x - (mayInside->getDim()->x / 2); 
        
        if(i <= 1 or i >= 6)    //+y
            tops[i].y = mayInside->getCenter()->y + (mayInside->getDim()->y / 2); 
        else                    //-y
            tops[i].y = mayInside->getCenter()->y - (mayInside->getDim()->y / 2); 

        if(i%2 == 0)        //+z
            tops[i].z = mayInside->getCenter()->z + (mayInside->getDim()->z / 2); 
        else
            tops[i].z = mayInside->getCenter()->z - (mayInside->getDim()->z / 2);
    }

    bool retour = true;
    for(int i = 0 ; i  < 8 and retour ; ++i){
        retour = isPointInside(&tops[i], mayOutside);
    return retour;
    }

    return retour;
}

Box* Box::getCopy(){
    Box* copy = new Box(id, getNewVect(dim->x, dim->y, dim->z), getNewVect(center->x, center->y, center->z), weight);
    return copy;
}

//bool isPointInside est au dessus

ObjectBox::ObjectBox(uInt P_id,
          Vect3D* P_dim,
          Vect3D* P_center,
          uInt weight,
          uInt P_maxWeightOnTop, 
          bool P_canMove, 
          bool P_canBeTopped, 
          bool P_canBeBottomed,
          bool* P_canBeRotated,
          bool P_isSingleBox,
          int P_chargingOrder,
          int P_bindedSpace) : Box(P_id, P_dim, P_center, weight){
            maxWeightOnTop = P_maxWeightOnTop;
            canMove = P_canMove;
            canBeTopped = P_canBeTopped;
            canBeBottomed = P_canBeBottomed;
            for(int i = 0 ; i < 3 ; i++)
                canBeRotated[i] = P_canBeRotated[i];
            isSingleBox = P_isSingleBox;
            chargingOrder = P_chargingOrder;
            bindedSpace = P_bindedSpace;
            if(isSingleBox){
                boxList.push_back(new Box(P_id, getNewVect(dim->x, dim->y, dim->z), getNewVect(center->x, center->y, center->z), weight)); 
            }
}

ObjectBox::~ObjectBox(){
    while(not boxList.empty()){
        delete boxList.back();
        boxList.pop_back();
    }
}

void ObjectBox::addBox(Box* newBox){
    if(newBox != NULL and not isSingleBox)
        boxList.push_front(newBox);    
}
void ObjectBox::delBox(uInt boxIndex){     //delete la box
    if(boxIndex >= 0 and boxIndex < boxList.size()){
        delete boxList[boxIndex];
        boxList[boxIndex] = NULL;
        boxList.erase(boxList.begin()+boxIndex);
    }
}

long long int ObjectBox::getVol(){
    uInt retour = 0;
    for(int i = 0 ; i < boxList.size() ; ++i){
        retour += boxList[i]->getVol();    
    }
    return retour;
}

long long int ObjectBox::getWeight(){
    uInt retour = 0;
    for(int i = 0 ; i < boxList.size() ; ++i ){
        retour += boxList[i]->getWeight();
    }
    return retour;
}


uInt ObjectBox::getBoxNbOf(){
    return boxList.size();
}

Box* ObjectBox::getBoxNb(uInt nb){
       if(nb >= 0 and nb < boxList.size()) {
            return boxList[nb];    
       }
       else
           return NULL;
}

bool ObjectBox::getCanMove(){
    return canMove;
}

bool ObjectBox::getCanBeTopped(){
    return canBeTopped;    
}

uInt ObjectBox::getMaxWeightOnTop(){
    return maxWeightOnTop;    
}

bool ObjectBox::getCanBeBottomed(){
    return canBeBottomed;    
}

bool* ObjectBox::getCanBeRotated(){    //bool[3]
    return canBeRotated;
}

bool ObjectBox::getIsSingleBox(){
    return isSingleBox;    
}

int ObjectBox::getBindedSpace(){
    return bindedSpace;    
}

int ObjectBox::getChargingOrder(){
    return chargingOrder;    
}

/*virtual*/void ObjectBox::setDim(long int x, long int y, long int z){
    dim->x = x;
    dim->y = y;
    dim->z = z;
    if(isSingleBox and boxList.size() > 0)
            boxList[0]->setDim(x, y, z); 
}

/*virtual*/void ObjectBox::setCenter(long int x, long int y, long int z){
    center->x=x;
    center->y=y;
    center->z=z;
    for(int i = 0 ; i < boxList.size() ; ++i)
        boxList[i]->setCenter(x, y, z);    
}

void ObjectBox::setCanMove(bool newVal){
    canMove = newVal;    
}

void ObjectBox::setCanBeTopped(bool newVal){
    canBeTopped = newVal;    
}

void ObjectBox::setMaxWeightOnTop(uInt newVal){
    maxWeightOnTop = newVal;    
}

void ObjectBox::setCanBeBottomed(bool newVal){
    canBeBottomed = newVal;    
}

void ObjectBox::setCanBeRotated(bool* newVal){    //bool[3]
    for(int i = 0 ; i < 3 ; ++i)
        canBeRotated[i] = newVal[i];
}

void ObjectBox::setIsSingleBox(bool newVal){
        isSingleBox=newVal;
}

void ObjectBox::move(Vect3D* displacement){
    if(canMove){
        center->x += displacement->x;
        center->y += displacement->y;
        center->z += displacement->z;
        for(int i = 0 ; i < boxList.size() ; ++i){
            boxList[i]->move(displacement);    
        }
    }
}

void ObjectBox::rotate(AXIS axe){
    if(canBeRotated[axe] and isSingleBox ){
        // cf log : algorithme rotation//axes centré sur centre, limité à 90°
        if(axe==Z){
            uInt temp = dim->y;
            dim->y=dim->x;
            dim->x=temp;
        }
        else if(axe==Y){
            uInt temp = dim->x;
            dim->x = dim->z;
            dim->z=temp;
        }
        else{
            uInt temp = dim->z;
            dim->z=dim->y;
            dim->y=temp;
        }
        for(int i = 0 ; i < boxList.size() ; ++i)
            boxList[i]->rotate(axe);
        }
}

bool ObjectBox::collide(Box* one){
    bool retour = false;
    for(int i = 0 ; (not retour) and i < boxList.size() ; ++i)    
        retour =  one->collide(boxList[i]);
    return retour;
}

ObjectBox* ObjectBox::getCopy(){
    ObjectBox* copy = new ObjectBox(id, dim, center, weight, 
    maxWeightOnTop, canMove, canBeTopped, canBeBottomed, canBeRotated, isSingleBox, chargingOrder, bindedSpace);

    for(int i = 0 ; i < boxList.size() ; ++i)
        copy->addBox(boxList[i]->getCopy());

    return copy;
}
    
SpaceToFill::SpaceToFill(uInt id, Vect3D* dim, Vect3D* center, uInt weight) : Box(id, dim, center, weight)
{
}

SpaceToFill::~SpaceToFill(){
    while(not boxStack.empty())
	delete pop();
}

bool SpaceToFill::collide(Box* one){     
    return Box::isBoxInside(one, this);
}

bool SpaceToFill::isEmpty(){     //(boxStack)
    return boxStack.empty();
}

long long int SpaceToFill::getVolLeft(){
    long long int volLeft = getVol();
    for(uInt i = 0 ; i < boxStack.size() ; ++i)
        volLeft -= boxStack[i]->getVol();
    return volLeft;
}

uInt SpaceToFill::getStackLen(){
    return boxStack.size();    
}

bool SpaceToFill::isInternable(ObjectBox* toIntern){
    bool goodChargingOrder = (toIntern->getChargingOrder() == -1 or toIntern->getChargingOrder() == boxStack.size()+1);
    bool compatibility = isCompatible(toIntern);
    boxStack.push_back(toIntern);
    for(int i = 0 ; i < boxStack.size() - 1 and compatibility; ++i){
        compatibility = isCompatible(boxStack[i]);
    }
    boxStack.pop_back();
    bool validPos = collide(toIntern);
    for(int i = 0 ; i < boxStack.size() and validPos ; ++i)
        validPos = not (toIntern->collide(boxStack[i]));
    return goodChargingOrder and getVolLeft() >= toIntern->getVol() and validPos and isStable(toIntern) and compatibility;    
}

bool SpaceToFill::isStable(Box* box){   // si internée directement
    uInt DOWNSTEP = 5;
    uInt NBOFDS = 10;   //par (x,z)
    /*
        l'axe du haut est l'axe y
        cf repère.jpg
    */
    /*
        découpage surface intérieur de la box en n*m petites surface ds,
        voir si nb surfaces ds en contact avec d'autres boites réprésentent
        une surface équivalente à celle de la box
    */
    float fractionX, fractionZ;     //taille d'une ds
    fractionX = box->getDim()->x / NBOFDS;
    fractionZ = box->getDim()->z / NBOFDS;
    Box* ds = new Box(0, getNewVect(fractionX, box->getDim()->y, fractionZ), getNewVect(box->getCenter()->x, box->getCenter()->y, box->getCenter()->z), 0);
    std::vector<Box*> objectsPool;
    uInt collidedDs = 0;
    Vect3D* startPos = getNewVect(box->getCenter()->x-(box->getDim()->x/2)+fractionX, box->getCenter()->y-DOWNSTEP, box->getCenter()->z-(box->getDim()->z/2));
    box->move(getNewVect(0, -DOWNSTEP, 0));
    for(uInt b = 0 ;  b < boxStack.size() ; ++b){
        if(boxStack[b]->collide(box))
            objectsPool.push_back(boxStack[b]);
    }
    box->move(getNewVect(0, DOWNSTEP, 0));
    for(uInt x = 0 ; x < NBOFDS ; ++x){
        for(uInt z = 0 ; z < NBOFDS ; ++z){
           ds->setCenter(startPos->x + x * fractionX, startPos->y, startPos->z + z * fractionZ);
           bool collided = startPos->y-(ds->getDim()->y/2) <= center->y-(dim->y/2);     //repose sur le sol
           for(uInt b = 0 ; not collided and b < objectsPool.size() ; b++)
               collided = objectsPool[b]->collide(ds);                                  //repose sur une boite
           if(collided)
               collidedDs += 1;
        }    
    }
    delete ds, startPos;
    return collidedDs >= (NBOFDS*NBOFDS)/2;
}

bool SpaceToFill::isCompatible(ObjectBox* box){    //comparaison paramètres individuels de chaques boites
    bool retour= true;
    uInt DOWNSTEP = 5;
    retour = retour and (box->getBindedSpace() == this->id or box->getBindedSpace() == -1);                //bon véhicule
    //test poid au dessus
    Box* testBox = new Box(0, getNewVect(box->getDim()->x, this->dim->y, box->getDim()->z), getNewVect(box->getCenter()->x, box->getCenter()->y+(box->getDim()->y/2) + (this->dim->y/2), box->getCenter()->z), 0);
    uInt topWeight = 0;
    for(int i = 0 ; i < boxStack.size() ; i++){
        if(boxStack[i]->getId() != box->getId() and boxStack[i]->collide(testBox)){
            topWeight += boxStack[i]->getWeight();
        }
    }
    retour = retour and ((box->getCanBeTopped() and topWeight < box->getMaxWeightOnTop()) or (not box->getCanBeTopped() and topWeight == 0));
    //test en dessous
    retour = retour and ((not box->getCanBeBottomed() and box->getCenter()->y - (box->getDim()->y / 2) - DOWNSTEP <= center->y - (dim->y / 2)) or box->getCanBeBottomed());
    delete testBox;
    return retour;
}

void SpaceToFill::intern(ObjectBox* box){
    boxStack.push_back(box);    
}

void SpaceToFill::empty(){
    while(not boxStack.empty()){
        delete boxStack.back();
        boxStack.pop_back();
    }
}

Box* SpaceToFill::pop(){   //pop sur le stack
    Box* retour = boxStack.back();
    boxStack.pop_back();
    return retour;
}
Box* SpaceToFill::back(){
    if(not boxStack.empty())
        return boxStack[boxStack.size()-1];
    else
        return NULL;
}

std::deque<ObjectBox*>* SpaceToFill::getBoxStack(){
    return &boxStack;    
}

uInt SpaceToFill::getDensity(){
    /*
        revoie la somme des sommes des volumes des boites adjacente à chaque boite
    */
    uInt density=0;
    Box* testBox = new Box(0, getNewVect(0, 0, 0), getNewVect(0,0,0), 0);
    for(int i = 0 ; i < boxStack.size() ; ++i){
        uInt objectDensity=boxStack[i]->getVol();
        testBox->setDim( boxStack[i]->getDim()->x*1.2, boxStack[i]->getDim()->y*1.2, boxStack[i]->getDim()->z*1.2);
        testBox->setCenter(boxStack[i]->getCenter()->x, boxStack[i]->getCenter()->y, boxStack[i]->getCenter()->z);
        for(int j = 0 ; j < boxStack.size() ; ++j){
            if( i!=j and boxStack[j]->collide(testBox) )
                objectDensity += boxStack[j]->getVol();
        }
        density+=objectDensity;
    }
    return density;
}

Vect3D* SpaceToFill::getLargestBoxBehind(float step){
    Box* test = new Box(0, getNewVect(dim->x, dim->y, 0), getNewVect(center->x, center->y, dim->z), 0);
        bool noColl = true;
        for(int j = 0 ; j < (dim->z)/step and noColl ; ++j){
            for(int i = 0 ; i < boxStack.size() and noColl ; ++i)
                noColl = not boxStack[i]->collide(test);
            if(noColl){
                test->setDim(test->getDim()->x, test->getDim()->y, j*step);
                test->move(getNewVect(0, 0, -(j/2)*step));
            }
        } 
        Vect3D* retour = getNewVect(test->getDim()->x, test->getDim()->y, test->getDim()->z);
        delete test;
        return retour;
}

Vect3D* SpaceToFill::getCenterOfMass(){
    /*
        formule : décomposition sur axe, somme des positions 
        pondérées par les masses divisé par masse totale
        ex : m1, m2 ; x1, x1 -> Gx = (m1.x1+m2.x2)/'m1+m2)
    */  
    Vect3D *retour = getNewVect(0,0,0);
    long int totalMass = 0;
    long int massTimesPos = 0; 
    for(uInt axe = 0 ; axe < 3 ; ++axe){     //uInt axe < 3 := AXIS axe
       massTimesPos = 0;
       for(uInt i = 0 ; i < boxStack.size() ; ++i){
            totalMass += boxStack[i]->getWeight();
            switch(axe){
                case 0:
                    massTimesPos += boxStack[i]->getCenter()->x*boxStack[i]->getWeight();
                    break;
                case 1:
                    massTimesPos += boxStack[i]->getCenter()->y*boxStack[i]->getWeight();
                    break;
                case 2:
                    massTimesPos += boxStack[i]->getCenter()->z*boxStack[i]->getWeight();
                    break;
            }
       }
       if(totalMass == 0){
           totalMass = 1;
           massTimesPos = 0;    //vide = 0
       }
       switch(axe){
        case 0:
            retour->x = massTimesPos/totalMass;
            break;
        case 1:
            retour->y = massTimesPos/totalMass;
            break;
        case 2:
            retour->z = massTimesPos/totalMass;
            break;
       }
    }
    return retour;
}

Vect3D* SpaceToFill::getGeoCenter(){
    return getNewVect(center->x, center->y, center->z);    
}

long long int SpaceToFill::getQuality(){
    long long int P_volLeft = getVolLeft();
    long long int density = getDensity();
    Vect3D* boxBehind = getLargestBoxBehind(5);
    uInt behind = boxBehind->z;
    delete boxBehind;
    Vect3D* massCenter = getCenterOfMass();
    int equilibrium = massCenter->x - center->x;
    if(equilibrium < 0)
        equilibrium *= -1;
    delete massCenter;
    //P_volLeft + density + behind ?> equilibrium
    long long int retour = ( (P_volLeft + density + behind - equilibrium ) * boxStack.size());
    if(retour < 0)
        retour *= -1;
    return retour;
}

SpaceToFill* SpaceToFill::getCopy(){
    SpaceToFill* retour = new SpaceToFill(id, getNewVect(dim->x, dim->y, dim->z), getNewVect(center->x, center->y, center->z), weight);
    return retour;
}
