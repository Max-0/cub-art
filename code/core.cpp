#include"core.hpp"

void outputTest(bool test, bool awaited, char* prefix){
    std::string output;
    if( ( test and awaited ) or ( (not test) and (not awaited) ) )	//test == awaited
        output = "OK"; 
    else
	output = "FAIL";
    output = prefix + output;
    std::cout << output << std::endl;
}

int main(){
    std::cout << "test instanciation/destruction ObjectBox" << std::endl;
    bool rotation[3];
    ObjectBox* testObject = new ObjectBox(0, getNewVect(0,0,0), getNewVect(20,20,20), 25, 15678, true, true, true, rotation, true); 
    delete testObject;
    std::cout << "test instanciation/destruction SpaceToFill" << std::endl;
    SpaceToFill *testSpace = new SpaceToFill(0, getNewVect(0,0,0), getNewVect(0,0,0), 0, 0, 0);
    delete testSpace;
    std::cout << "test collision 2 Box : " << std::endl;
    Box* test1 = new Box(0, getNewVect(0,0,0), getNewVect(50,50,50), 25); 
    Box* test2 = new Box(0, getNewVect(30,0,0), getNewVect(50,50,50), 25); 
    Box* test3 = new Box(0, getNewVect(0,120,0), getNewVect(50,50,50), 25); 
    Box* test4 = new Box(0, getNewVect(0,0,30), getNewVect(50,50,50), 25); 
    Box* test5 = new Box(0, getNewVect(180,180,180), getNewVect(50,50,50), 25); 
    Box* test6 = new Box(0, getNewVect(-30,-30,-30), getNewVect(50,50,50), 25); 
    std::cout << "	(1/5)" << std::endl;
    outputTest(test1->collide(test2) or test2->collide(test1), true, "        ");
    std::cout << "	(2/5)" << std::endl;
    outputTest(test1->collide(test3) or test3->collide(test1), false, "        ");
    std::cout << "	(3/5)" << std::endl;
    outputTest(test1->collide(test4) or test4->collide(test1), true, "        ");
    std::cout << "	(4/5)" << std::endl;
    outputTest(test1->collide(test5) or test5->collide(test1), false, "        ");
    std::cout << "	(5/5)" << std::endl;
    outputTest(test1->collide(test6) or test6->collide(test1), true, "        ");
    std::cout << "test internation et évaluation qualitée" << std::endl;
    

    return 0;    
}

Vect3D* getNewVect(uInt x, uInt y, uInt z){
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

uInt Box::getVol(){
    return dim->x*dim->y*dim->z;    
}

uInt Box::getWeight(){
    return weight;    
}

Vect3D* Box::getDim(){
    return dim;    
}

Vect3D* Box::getCenter(){
    return center;
}

void Box::setDim(uInt x, uInt y, uInt z){
    dim->x = x;
    dim->y = y;
    dim->z = z;
}

void Box::setCenter(uInt x, uInt y, uInt z){
    center->x = x;
    center->y = y;
    center->z = z;
}

bool Box::collide(Box* one){
    //cf log : algorithme collision AABB 3D
    //changement de repère pour les boites : p-e supprimer 
    //allocation dynamique
    bool retour=true;
    Vect3D* box1 = getNewVect(one->getCenter()->x-(one->getDim()->x/2),
                              one->getCenter()->y-(one->getDim()->y/2),
                              one->getCenter()->z-(one->getDim()->z/2);
    Vect3D* box2 = getNewVect(center->x-(dim->x/2),
                              center->y-(dim->y/2),
                              center->z-(dim->z/2));
    if(box2->x >= box1->x+one->getDim()->x ||
       box2->x+dim->x <= box1->x ||
       box2->y >= box1->y+one->getDim()->y ||
       box2->y + dim->y <= box1->y ||
       box2->z >= box1->z + one->getDim()->z ||
       box2->z + dim->z)
        retour=false;
    
    //eventuellement supprimer l'allocation dynamique car 
    //fonction sensible par exemple macro
    delete box1;
    delete box2;
    
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
    Vect3D top[8];
    //d'abord on remplit avec les coordonnées du centre
    for(int i = 0 ; i < 8 ; i++){
        top[i].x = mayInside->getCenter()->x;
        top[i].y = mayInside->getCenter()->y;
        top[i].z = mayInside->getCenter()->z;
    }
    //générer les 8 sommets
    for(int i = 0 ; i < 4 ; i++){
        top[i].z += mayInside->getDim()->z/2;
        top[i+4].z -= mayInside->getDim()->z/2;
        if(i<2){
            top[i].y += mayInside->getDim()->y/2;
            top[i+4].y += mayInside->getDim()->y/2;
        }
        else{
            top[i].y -= mayInside->getDim()->y/2;
            top[i+4].y -= mayInside->getDim()->y/2;
        }
        if(i%2==0){
            top[i].x += mayInside->getDim()->x/2;    
            top[i+4].x += mayInside->getDim()->x/2;    
        }
        else{
            top[i].x -= mayInside->getDim()->x/2;    
            top[i+4].x -= mayInside->getDim()->x/2;    
        }
    }
    //tester les 8 sommets
    bool retour=true;
    int index=0;
    while(retour and index<8){
        retour = retour and Box::isPointInside(&top[index], mayOutside);
        index++;
    }
    return retour;
}

Box* Box::getCopy(){
    Box* copy = new Box(id, dim, center, weight);
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
}

ObjectBox::~ObjectBox(){
    while(not boxList.empty()){
        delete boxList.back();
        boxList.pop_back();
    }
}

void ObjectBox::addBox(Box* newBox){
    if(newBox != NULL)
        boxList.push_front(newBox);    
    if(boxList.size() > 1)
        isSingleBox=false;
}
void ObjectBox::delBox(uInt boxIndex){     //delete la box
    if(boxIndex >= 0 and boxIndex < boxList.size()){
        delete boxList[boxIndex];
        boxList[boxIndex] = NULL;
        boxList.erase(boxList.begin()+boxIndex);
    }
}
uInt ObjectBox::getBoxNbOf(){
    return boxList.size();
}

Box* ObjectBox::getBoxNb(uInt nb){
       if(nb >= 0 and nb < boxList.size()) {
            return boxList[nb];    
       }
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
    if((newVal and boxList.size() <= 1) or (not newVal and not boxList.size() <=1))
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
    bool retour = true;
    for(int i = 0 ; retour and i < boxList.size() ; ++i)    
        retour = retour and not one->collide(boxList[i]);
    return retour;
}

ObjectBox* ObjectBox::getCopy(){
    ObjectBox* copy = new ObjectBox(id, dim, center, weight, 
    maxWeightOnTop, canMove, canBeTopped, canBeBottomed, canBeRotated, isSingleBox, chargingOrder, bindedSpace);

    for(int i = 0 ; i < boxList.size() ; ++i)
        copy->addBox(boxList[i]->getCopy());

    return copy;
}
    
SpaceToFill::SpaceToFill(uInt id, Vect3D* dim, Vect3D* center, uInt weight, 
                         double volume, double P_precision) : Box(id, dim, center, weight)
{
    volLeft = volume;
    volTot = volume;
    precision = P_precision; 
}

SpaceToFill::~SpaceToFill(){
    while(not boxStack.empty())
	delete pop();
}

bool SpaceToFill::collide(Box* one){     //désactiver
    return not Box::isBoxInside(one, this);
}

bool SpaceToFill::isEmpty(){     //(boxStack)
    return boxStack.empty();
}

uInt SpaceToFill::getVolLeft(){
    return volLeft;    
}

uInt SpaceToFill::getStackLen(){
    return boxStack.size();    
}

bool SpaceToFill::isInternable(ObjectBox* toIntern){
    return collide(toIntern) and  getVol() >= toIntern->getVol() and  isStable(toIntern) and isCompatible(toIntern);    
}

bool SpaceToFill::isStable(Box* box){   // si internée directement
    uInt DOWNSTEP = 5*cm;
    /*
        cf algorithme d'équilibre
        l'axe du haut est l'axe y
        cf repère.jpg
    */
    Box* section;
    /* 
        division en trois selon le plus grand axe(x, z) 
        -trouver plus grand axe
        -creer une boxe, de la taille voulue
        -la positioner au départ
        -trouver le pas pour chaque tests
    */
    AXIS longuest;
    if(box->getDim()->x > box->getDim()->z)
        longuest = X;
    else
        longuest = Z;
    Vect3D *newDim, *newCenter;
    uInt fdStep;
    if( longuest == X ){
        newDim = getNewVect(box->getDim()->x/3, box->getDim()->y, box->getDim()->z);
        newCenter = getNewVect(box->getCenter()->x - newDim->x, box->getCenter()->y-DOWNSTEP, box->getCenter()->z);
        fdStep = newDim->x;
    }
    else{
        newDim = getNewVect(box->getDim()->x, box->getDim()->y, box->getDim()->z/3);
        newCenter = getNewVect(box->getCenter()->x, box->getCenter()->y-DOWNSTEP, box->getCenter()->z - newDim->z);
        fdStep = newDim->z;
    }
    section = new Box(box->getId(), newDim, newCenter, box->getWeight());
    //tests de collision
    bool onTop[3];
    bool onBottom[3];
    for(int i = 0 ; i < 3 ; i++){
       onBottom[i] = false;
       //test collision tts objets (en bas)
       for( int j = 0 ; j  < boxStack.size() and not onBottom[i] ; ++j)
            onBottom[i] = boxStack[j]->collide(section) or section->getCenter()->y - section->getDim()->y/2 <= 0;   //repose sur une autre boite ou sur le sol
       //test collision tts objets(en haut)
       onTop[i] = false;
       section->move(getNewVect(0, 2*DOWNSTEP, 0));
       for(int j = 0 ; j <  boxStack.size() and not onTop[i] ; i++)
            onTop[i] = boxStack[i]->collide(section);
       section->move(getNewVect(0, -2*DOWNSTEP, 0));
       if(longuest == X)
           section->move(getNewVect(fdStep, 0, 0));
       else
           section->move(getNewVect(0, 0,fdStep));
    }
    //analyse des test
    bool retour = true;
    bool onBottomNb = 0;
    for(int i = 0 ; i < 3 and retour ; ++i){
        if(onBottom[i])
            onBottomNb++;
        if(onTop[i])
            retour = onBottom[i];
    }
    retour = retour and onBottomNb >= 2;
    //nettoyage
    delete section; // va aussi deleter les Vect3D*
    return retour;
}

bool SpaceToFill::isCompatible(ObjectBox* box){    //comparaison paramètres individuels de chaques boites
    bool retour= true;
    retour = retour and box->getChargingOrder() == boxStack.size()+1;   //ordre de chargement
    retour = retour and (box->getBindedSpace() == this->id or box->getBindedSpace() == -1);                //bon véhicule
    //test poid au dessus
    Box* testBox = new Box(0, getNewVect(box->getDim()->x, box->getDim()->y*5000*m, box->getDim()->z), getNewVect(box->getCenter()->x, box->getDim()->y+(box->getDim()->y*5000*m)/2, box->getCenter()->z), 0);
    uInt topWeight = 0;
    for(int i = 0 ; i < boxStack.size() ; i++){
        if(boxStack[i]->collide(box))
            topWeight += boxStack[i]->getWeight();
    }
    retour = retour and ((box->getCanBeTopped() and topWeight > 0 and topWeight < box->getMaxWeightOnTop()) or (not box->getCanBeTopped() and topWeight == 0));
    //test en dessous
    retour = retour and ((not box->getCanBeBottomed() and box->getCenter()->y - box->getDim()->y / 2 <= 5*cm) or box->getCanBeBottomed());
    delete testBox;
    return retour;
}

void SpaceToFill::intern(Box* box){
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

std::deque<Box*>* SpaceToFill::getBoxStack(){
    return &boxStack;    
}

uInt SpaceToFill::getDensity(){
    /*
        revoie la somme des sommes des volumes des boites adjacente à chaque boite
    */
    uInt density=0;
    uInt objectDensity=0;
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
    Box* test = new Box(0, getNewVect(dim->x, dim->y, 0), getNewVect(center->x, center->y, center->z), 0);
        bool noColl = true;
        while(noColl){
            for(int i = 0 ; i < boxStack.size() and noColl ; ++i)
                noColl = not boxStack[i]->collide(test);
            if(noColl){
                test->setDim(test->getDim()->x, test->getDim()->y, test->getDim()->z+dim->z+step);
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
    uInt totalMass = 0;
    uInt massTimesPos = 0; 
    for(uInt axe = 0 ; axe < 3 ; ++axe){     //uInt axe < 3 := AXIS axe
       massTimesPos = 0;
       for(uInt i = 0 ; i < boxStack.size() ; ++i){
            switch(axe){
                case 0:
                    totalMass += boxStack[i]->getWeight();
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

uInt SpaceToFill::getQuality(){
    uInt P_volLeft = getVolLeft();
    uInt density = getDensity();
    Vect3D* boxBehind = getLargestBoxBehind(cm);
    uInt behind = boxBehind->z;
    delete boxBehind;
    Vect3D* massCenter = getCenterOfMass();
    int equilibrium = massCenter->x - center->x;
    delete massCenter;
    if(equilibrium < 0)
        equilibrium *= -1;
    return( (P_volLeft + density + behind + equilibrium) * boxStack.size());
}
