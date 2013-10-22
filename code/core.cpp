#include"core.hpp"

void printVect(Vect3D *toPrint){
    std::cout << "(" << toPrint->x << ", " << toPrint->y << ", " << toPrint->z << std::endl;    
}

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
    testAll();
    return 0;    
}

Vect3D* getNewVect(long int x, long int y, long int z){
    Vect3D* newVect = new Vect3D;
    newVect->x=x;
    newVect->y=y;
    newVect->z=z;
    return newVect;
}

void testCreation(){
    std::cout << "test création+destruction ObjectBox" << std::endl;
    bool rotation[3];
    ObjectBox* testObject = new ObjectBox(0, getNewVect(0,0,0), getNewVect(20,20,20), 25, 15678, true, true, true, rotation, true); 
    delete testObject;
    outputTest(true, true, "    ");
    std::cout << "test creation/destruction SpaceToFill" << std::endl;
    SpaceToFill *testSpace = new SpaceToFill(0, getNewVect(0,0,0), getNewVect(0,0,0), 0 );
    delete testSpace;
    outputTest(true, true, "    ");
}
void testCollisions(){
    bool rotation[3];   
    //test collision strict
    std::cout << "test collision 2 Box : " << std::endl;
    Box* test1 = new Box(0, getNewVect(50,50,50), getNewVect(0,0,0), 25); 
    Box* test2 = new Box(0, getNewVect(50,50,50), getNewVect(0,0,-40), 25);  //true
    Box* test3 = new Box(0, getNewVect(50,50,50), getNewVect(60,0,60), 25);   //false
    Box* test4 = new Box(0, getNewVect(50,50,50), getNewVect(10,10,10), 25);   //true
    Box* test5 = new Box(0, getNewVect(50,50,50), getNewVect(120,100,100), 25);   //false
    Box* test6 = new Box(0, getNewVect(50,50,50), getNewVect(45,45,250), 25);   //false
    std::cout << "	(1/5)" << std::endl;
    outputTest(test1->collide(test2) or test2->collide(test1), true, "        ");
    std::cout << "	(2/5)" << std::endl;
    outputTest(test1->collide(test3) or test3->collide(test1), false, "        ");
    std::cout << "	(3/5)" << std::endl;
    outputTest(test1->collide(test4) or test4->collide(test1), true, "        ");
    std::cout << "	(4/5)" << std::endl;
    outputTest(test1->collide(test5) or test5->collide(test1), false, "        ");
    std::cout << "	(5/5)" << std::endl;
    outputTest(test1->collide(test6) or test6->collide(test1), false, "        ");
    std::cout << "test collision 2 ObjectBox : " << std::endl;
    ObjectBox* coll1 = new ObjectBox(1, getNewVect(50, 50, 50), getNewVect(test1->getCenter()->x, test1->getCenter()->y, test1->getCenter()->z), 25, 15678, true, true, true, rotation, true);
    coll1->addBox(test1->getCopy());
    ObjectBox* coll2 = new ObjectBox(1, getNewVect(50, 50, 50), getNewVect(test2->getCenter()->x, test2->getCenter()->y, test2->getCenter()->z), 25, 15678, true, true, true, rotation, true);
    coll2->addBox(test2->getCopy());
    ObjectBox* coll3 = new ObjectBox(1, getNewVect(50, 50, 50), getNewVect(test3->getCenter()->x, test3->getCenter()->y, test3->getCenter()->z), 25, 15678, true, true, true, rotation, true);
    coll3->addBox(test3->getCopy());
    ObjectBox* coll4 = new ObjectBox(3, getNewVect(40, 40, 40), getNewVect(0, 43, 0), 25, 15678, true, true, true, rotation, true);
    ObjectBox* coll5 = new ObjectBox(4, getNewVect(40, 40, 500), getNewVect(0, 48, 0), 25, 15678, true, true, true, rotation, true);
    outputTest(coll1->collide(coll2), true, "    ");
    outputTest(coll1->collide(coll3), false, "    ");
    outputTest(coll4->collide(coll1), true, "    ");
    coll4->move(getNewVect(0, 10, 0));
    outputTest(coll4->collide(coll1), false, "    ");
    coll4->setDim(500, 150, 150);
    coll4->setCenter(0, 0, 0);
    outputTest(coll4->collide(coll1), true, "    ");
    outputTest(coll4->collide(coll1), true, "    ");
    delete test1, test2, test3, test5, test6;
    delete coll1, coll2, coll3, coll4;
    //test géométrie
    std::cout << "tests géométrie" << std::endl;
    Box* test21 = new Box(20, getNewVect(50, 50, 50), getNewVect(0, 0, 0), 0);
    Box* test22 = new Box(21, getNewVect(60, 60, 60), getNewVect(0, 0, 0), 0);
    Box* test23 = new Box(22, getNewVect(20, 20, 20), getNewVect(5, 5, 5), 0);
    Vect3D* test24 = getNewVect(27, 27, -27);
    SpaceToFill* test25 = new SpaceToFill(24, getNewVect(60, 60, 60), getNewVect(0, 0, 0), 500);
    outputTest(Box::isBoxInside(test21, test22), true, "    1:");
    outputTest(Box::isBoxInside(test22, test21), false, "    not 1:");
    outputTest(Box::isBoxInside(test23, test22), true, "    2:");
    outputTest(Box::isBoxInside(test22, test23), false, "    not 2:");
    outputTest(Box::isPointInside(test24, test21), false, "     3:");
    outputTest(Box::isPointInside(test24, test22), true, "      4:");
    outputTest(Box::isBoxInside(test21, test25), test25->collide(test21), "         5:");
    outputTest(Box::isBoxInside(test23, test25), test25->collide(test23), "         6:");
    delete test21, test22, test23, test24, test25;
    //  test distances
    std::cout << "tests distances" << std::endl;
    Vect3D* ref = getNewVect(0,0,0);
    Vect3D* vtest1 = getNewVect(0, 5, 5);
    Vect3D* vtest2 = getNewVect(7, 8, 9);
    outputTest(getSqDst(ref, vtest1) == 50, true, "      1:");
    outputTest(getOnAxisDst(ref, vtest2, Y) == 8, true, "      2:");
    outputTest(getSqDst(ref, vtest2) == 7*7+8*8+9*9, true, "     3:");
    outputTest(getOnAxisDst(ref, vtest1, X) == 0, true, "       4:");
    delete ref, vtest1, vtest2;
}

void testValidIntern(){
    bool rotation[3];    
    std::cout << "test internation et évaluation qualitée" << std::endl;
    Box* intBox = new Box(0, getNewVect(50, 50, 50), getNewVect(0, -474, 0), 50);
    ObjectBox* intObj = new ObjectBox(1, getNewVect(50, 50, 50), getNewVect(0, -224, 0), 50, 500, true, true, true, rotation, true);
    intObj->addBox(intBox);
    SpaceToFill* intSpace = new SpaceToFill(2, getNewVect(500, 500, 500), getNewVect(0, 0, 0), 500);
    std::cout << "      test SpaceToFill::Collide" << std::endl;
    outputTest(intSpace->collide(intObj), true, "      ");
    std::cout << "      test SpaceToFill->getVolLeft() >= toIntern->getVol()" << std::endl;
    outputTest(intSpace->getVolLeft() >= intObj->getVol(), true, "      ");
    std::cout << "      test SpaceToFill::isStable" << std::endl;
    outputTest(intSpace->isStable(intObj), true, "      ");
    std::cout << "      test SpaceToFill::isCompatible" << std::endl;
    outputTest(intSpace->isCompatible(intObj), true, "      ");
    std::cout << "      test internation" << std::endl;
    outputTest(intSpace->isInternable(intObj), true, "      ");
    delete intObj, intSpace;
}


void testComplexIntern(){
    bool rotation[3];
    std::cout << "   internations complexes" << std::endl;
    SpaceToFill* intSpace = new SpaceToFill(2, getNewVect(500, 500, 500), getNewVect(0, 0, 0), 500);
    ObjectBox* Obj1 = new ObjectBox(10, getNewVect(50, 50, 50), getNewVect(0 , -224, 0), 50, 500, true, true, true, rotation, true);
    ObjectBox* Obj2 = new ObjectBox(11, getNewVect(50, 50, 50), getNewVect(0 , -173, 0), 50, 500, true, true, true, rotation, true);
    ObjectBox* Obj3 = new ObjectBox(12, getNewVect(50, 50, 50), getNewVect(0 , -333, 0), 50, 500, true, true, true, rotation, true);
    ObjectBox* Obj4 = new ObjectBox(13, getNewVect(50, 50, 50), getNewVect(0 , -122, 0), 50000, 500, true, true, true, rotation, true);
    ObjectBox* Obj5 = new ObjectBox(14, getNewVect(50, 50, 50), getNewVect(0 , -122, 40), 50, 500, true, true, true, rotation, true);
    ObjectBox* Obj6 = new ObjectBox(14, getNewVect(50, 50, 50), getNewVect(0 , -122, 5), 50, 500, true, true, true, rotation, true);
    ObjectBox* Obj7 = new ObjectBox(14, getNewVect(50, 50, 50), getNewVect(0 , -71, 0), 50, 500, true, true, true, rotation, true);
    //test posé sur sol
    outputTest(intSpace->isInternable(Obj1), true, "     1:");
    outputTest(intSpace->getBoxStack()->size() == 0, true, "        ");
    intSpace->intern(Obj1);
    outputTest(intSpace->getBoxStack()->size() == 1, true, "        ");
    //test posé sur caisse
    outputTest(intSpace->isInternable(Obj2), true, "     2:");
    outputTest(intSpace->getBoxStack()->size() == 1, true, "        ");
    intSpace->intern(Obj2);
    outputTest(intSpace->getBoxStack()->size() == 2, true, "        ");
    //test en collision
    outputTest(intSpace->isInternable(Obj3), false, "     3:");
    //test trop lourd
    outputTest(intSpace->isInternable(Obj4), false, "     4:");
    //test équilibre précaire
    outputTest(intSpace->isInternable(Obj5), false, "     5:");
    //test équilibre 2
    outputTest(intSpace->isInternable(Obj6), true, "     6:");
    intSpace->intern(Obj6);
    //test seconde colonne
    outputTest(intSpace->isInternable(Obj7), true, "     7:");
    delete Obj4, Obj3, Obj5, Obj6, Obj7;
    delete intSpace;
}

void testConstraints(){
    std::cout << "Test contraintes " << std::endl;
    bool rotations[3];
    SpaceToFill* testSpace = new SpaceToFill(30, getNewVect(500, 500, 500), getNewVect(0, 0, 0), 500);
    ObjectBox* Obj1 = new ObjectBox(31, getNewVect(50, 50, 50), getNewVect(0, -224, 0), 50, 500, true, false, false, rotations, true); 
    ObjectBox* Obj2 = new ObjectBox(32, getNewVect(50, 50, 50), getNewVect(0, -173, 0), 50, 500, true, false, true, rotations, true); 
    ObjectBox* Obj3 = new ObjectBox(33, getNewVect(50, 50, 50), getNewVect(-51, -224, 0), 50, 500, true, true, false, rotations, true, 2, 30);
    ObjectBox* Obj4 = new ObjectBox(34, getNewVect(50, 50, 50), getNewVect(0, -173, 0), 50, 500, true, true, true, rotations, true, 3, 29); 
    ObjectBox* Obj5 = new ObjectBox(35, getNewVect(50, 50, 50), getNewVect(-51, -173, 0), 50, 500, true, true, true, rotations, true, 3); 
    outputTest(testSpace->isInternable(Obj1), true, "   1:");
    testSpace->intern(Obj1);
    outputTest(testSpace->isInternable(Obj2), false, "   2:");
    outputTest(testSpace->isInternable(Obj3), true, "   3:");
    testSpace->intern(Obj3);
    outputTest(testSpace->isInternable(Obj4), false, "   4:");
    outputTest(testSpace->isInternable(Obj5), true, "   5:");
    delete Obj2, Obj4, Obj5, testSpace;
}

void testQuality(){
    /*
        A completer
    */
}

void testAll(){
    testCreation();
    testCollisions();
    testValidIntern();
    testComplexIntern();
    testConstraints();
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

uInt ObjectBox::getVol(){
    uInt retour = 0;
    for(int i = 0 ; i < boxList.size() ; ++i){
        retour += boxList[i]->getVol();    
    }
    return retour;
}

uInt ObjectBox::getWeight(){
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

uInt SpaceToFill::getVolLeft(){
    uInt volLeft = getVol();
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
    Vect3D* boxBehind = getLargestBoxBehind(5);
    uInt behind = boxBehind->z;
    delete boxBehind;
    Vect3D* massCenter = getCenterOfMass();
    int equilibrium = massCenter->x - center->x;
    if(equilibrium < 0)
        equilibrium *= -1;
    delete massCenter;
    if(equilibrium < 0)
        equilibrium *= -1;
    return( (P_volLeft + density + behind - equilibrium ) * boxStack.size());
}
