#include"algo.hpp"

long int SpaceBandBTree::highestLowBound = 0;

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
	AXIS allAxis[3];
	allAxis[0] = X;
	allAxis[1] = Y;
	allAxis[2] = Z;
	AXIS axe = X;
        for(int iAx = 0 ; iAx < 3 ; iAx++){                    //rotations (axe)
	    axe = allAxis[iAx];
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
                                for(uInt obj = 0 ; obj < objects.size() ; ++obj)
                                    if(objects[obj]->getId() != nextObj->getId())
                                        sons.back()->addObject(objects[obj]->getCopy());
                            }
                            nextObj->move(getNewVect(0, precision, 0));
                        }
                        nextObj->move(getNewVect(precision, 0, 0));
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
    if(!hasBranched)
        return;     //do nothing
    for(int s = 0 ; s < sons.size() ; ++s){
    	if(sons[s]->getQuality() < lowerBound)
		lowerBound = sons[s]->getQuality();
        else if(sons[s]->getQuality() > upperBound)
		upperBound = sons[s]->getQuality();
    }
    if(highestLowBound < lowerBound)
	highestLowBound == lowerBound;
}

void SpaceBandBTree::prune(){
    if(not hasBranched)
	return;
    for(uInt s = 0 ; s < getSonsLength() ; ++s){
    	if(sons[s]->getQuality() < highestLowBound){
	    delete sons[s];
	    sons.erase(sons.begin()+s);
        }
    }
}

bool SpaceBandBTree::isSol(){
    return not (not hasBranched or sons.size() > 0);
}

long long int SpaceBandBTree::getQuality(){
    long long int retour = 0;
    for(int i = 0 ; i < spaces.size() ; ++i)
	retour += spaces[i]->getQuality();
    return retour;
}

void SpaceBandBTree::addSpace(SpaceToFill* toAdd){
     if(toAdd != NULL)
	spaces.push_back(toAdd);
}

void SpaceBandBTree::addObject(ObjectBox* toAdd){
     if(toAdd != NULL)
	objects.push_back(toAdd);
}
