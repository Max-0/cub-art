#include"algo.hpp"

static long int SpaceBandBTree::highestLowBound = 0;

SpaceBandBTree::SpaceBandBTree(float precision){
     precisionStep = precision;
     hasBranched = false;
     lowerBound=999999999999;
     upperBound=0;
}

uInt SpaceBandBTree::getSonsLength(){
    return sons.size();    
}

SpaceBandBTree* SpaceBandBTree::getSonNb(uInt nb){
    SpaceBandBTree* retour = NULL;
    if(nb < getSonsLength())
        retour = sons[nb];
    return retour;
}

bool SpaceBandBTree::getHasBranched(){
    return hasBranched;    
}

SpaceBandBTree::~SpaceBandBTree(){
    for(uInt i = 0 ; i < spaces.size() ; ++i)
        delete spaces[i];
    for(uInt j = 0 ; j < objects.size() ; ++j)
        delete objects[j];
}

void SpaceBandBTree::branch(){
    float precision = precisionStep;
    if(hasBranched)
        return;
    else
        hasBranched = true;
    ObjectBox* nextObj = objects.back();
    objects.pop_back();
    for(uInt aSpace = 0 ; aSpace < spaces.size() ; ++aSpace){   //test insertion dans nimporte quel espace
        for(uInt axe = 0 ; axe < 3 ; ++axe){                    //rotations (axe)
            for(uInt rot = 0 ; rot <= 4 ; ++rot){               //rotations (suite)
                uInt nbOnX = (spaces[aSpace]->getDim()->x - nextObj->getDim()->x)/precision;
                uInt nbOnY = (spaces[aSpace]->getDim()->y - nextObj->getDim()->y)/precision;
                uInt nbOnZ = (spaces[aSpace]->getDim()->z - nextObj->getDim()->z)/precision;
                Vect3D* startPos = getNewVect(spaces[aSpace]->getCenter()->x - (spaces[aSpace]->getDim()->x)/2 + (nextObj->getDim()->x)/2,
                                              spaces[aSpace]->getCenter()->y - (spaces[aSpace]->getDim()->y)/2 + (nextObj->getDim()->y)/2,
                                              spaces[aSpace]->getCenter()->z - (spaces[aSpace]->getDim()->z)/2 + (nextObj->getDim()->z)/2);
                nextObj->move(getNewVect(startPos->x - nextObj->getCenter()->x, startPos->y - nextObj->getCenter()->y, startPos->z - nextObj->getCenter()->z));
                for(uInt z = 0 ; z < nbOnZ ; ++z){  //tts pos en z
                    nextObj->move(getNewVect(startPos->x-nextObj->getCenter()->x, 0, 0));
                    for(uInt x = 0 ; x < nbOnX ; ++x){  //tts pos en x
                        nextObj->move(getNewVect(0, startPos->y-nextObj->getCenter()->y, 0));
                        for(uInt y = 0 ; y < nbOnY ; ++y){  //tts pos en y
                            if(spaces[aSpace]->isInternable(nextObj)){       //on branche
                                sons.push_back(new SpaceBandBTree(precision));
                                for(uInt sp = 0 ; sp < spaces.size() ; sp++){
                                    if(sp != aSpace)
                                        sons.back()->addSpace(spaces[sp]->getCopy());
                                    else{
                                        SpaceToFill* copy = spaces[sp]->getCopy();
                                        copy->intern(nextObj->getCopy());
                                        sons.back()->addSpace(copy);
                                    }
                                }
                                for(uInt obj = 0 ; obj < objectsLefts.size() ; ++obj)
                                    if(objectsLefts[obj]->getId() != nextObj->getId())
                                        (SpaceBandBTree*) sons.back()->addObject(objectsLefts[obj]->getCopy());
                            }
                        }
                        nextObj->move(getNewVect(0, precision, 0));
                    }
                nextObj->move(getNewVect(0, 0, precision));    
                }
                delete startPos;
            }
            nextObj->rotate(axe);
        }            
    }
}

void SpaceBandBTree::bound(){
    if(!branched)
        return;     //do nothing
    long long unsigned int temp = 0;        //stockage qualitée du noeud à l'étude
    for(int s = 0 ; s < sons.size() ; ++s){
        temp=0;
        for(int sq = 0 ; sq < 
    }
}

