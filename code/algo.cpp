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
        retour = sons.at(nb);
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
    bool keepTrying = true;
    if(hasBranched)
        return;
    else if(objects.empty() or spaces.empty()){
	hasBranched = true;
        return;
    }
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
        for(int iAx = 0 ; iAx < 3 and keepTrying ; iAx++){                    //rotations (axe)
	    axe = allAxis[iAx];
            for(uInt rot = 0 ; rot <= 4 and keepTrying  ; ++rot){               //rotations (suite)
                uInt nbOnX = (spaces[aSpace]->getDim()->x - nextObj->getDim()->x)/precision;
                uInt nbOnY = (spaces[aSpace]->getDim()->y - nextObj->getDim()->y)/precision;
                uInt nbOnZ = (spaces[aSpace]->getDim()->z - nextObj->getDim()->z)/precision;
                Vect3D* startPos = getNewVect(spaces[aSpace]->getCenter()->x - (spaces[aSpace]->getDim()->x)/2 + (nextObj->getDim()->x)/2,
                                              spaces[aSpace]->getCenter()->y - (spaces[aSpace]->getDim()->y)/2 + (nextObj->getDim()->y)/2,
                                              spaces[aSpace]->getCenter()->z - (spaces[aSpace]->getDim()->z)/2 + (nextObj->getDim()->z)/2);
                nextObj->move(getNewVect(startPos->x - nextObj->getCenter()->x, startPos->y - nextObj->getCenter()->y, startPos->z - nextObj->getCenter()->z));
                for(uInt z = 0 ; z < nbOnZ and keepTrying ; ++z){  //tts pos en z
                    nextObj->move(getNewVect(startPos->x-nextObj->getCenter()->x, 0, 0));
                    for(uInt x = 0 ; x < nbOnX and keepTrying ; ++x){  //tts pos en x
                        nextObj->move(getNewVect(0, startPos->y-nextObj->getCenter()->y, 0));
                        for(uInt y = 0 ; y < nbOnY and keepTrying ; ++y){  //tts pos en y
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
    	if(sons.at(s)->getQuality() < lowerBound){
		lowerBound = sons.at(s)->getQuality();
        }
        else if(sons.at(s)->getQuality() > upperBound)
		upperBound = sons.at(s)->getQuality();
    }
    if(highestLowBound < lowerBound and not sons.empty()){
	highestLowBound = lowerBound;
    }
}

void SpaceBandBTree::prune(){
    if(not hasBranched)
	return;
    for(uInt s = 0 ; s < getSonsLength() ; ++s){
    	if(sons.at(s)->getQuality() < highestLowBound){
	    delete sons.at(s);
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
    if(retour < 0)
         retour = 0;
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

SpaceNodeList::SpaceNodeList(){
    maxLength = 1;
}

uInt SpaceNodeList::size(){
    return list.size();
}

bool SpaceNodeList::empty(){
    return list.empty();
}

SpaceBandBTree* SpaceNodeList::back(){
    return list.back();
}

void SpaceNodeList::pop_back(){
    list.pop_back();
    qualList.pop_back();
}

void SpaceNodeList::push_back(SpaceBandBTree* toPush){
    uInt insertIndex = 0;
    long long int nodeQuality = toPush->getQuality();
    while(insertIndex < size() and nodeQuality < qualList[insertIndex])
	++insertIndex;
    if(insertIndex < size()-1){
    	list.insert(list.begin()+insertIndex, toPush);
        qualList.insert(qualList.begin()+insertIndex, nodeQuality);
    }
    else{
	list.push_back(toPush);
        qualList.push_back(nodeQuality);
    }
    if(list.size() > maxLength){
        delete list.at(0);
	list.pop_front();
    }
}

SpaceBandBTree* SpaceNodeList::at(uInt index){
    return list[index];
}

void SpaceNodeList::setMaxLen(uInt P_maxLen){
    maxLength = P_maxLen;
}

void SpaceNodeList::erase(uInt index){
    list.erase(list.begin()+index);
    qualList.erase(qualList.begin()+index);
}

uInt SpaceNodeList::begin(){
    return 0;
}
