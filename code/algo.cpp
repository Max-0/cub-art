#include"algo.hpp"

long int SpaceBandBTree::highestLowBound = 0;

SpaceBandBTree::SpaceBandBTree(float precision, uInt P_maxToStudy, uInt P_maxToKeep){
     precisionStep = precision;
     hasBranched = false;
     lowerBound = LARGEST_BOUND;
     upperBound= LOWEST_BOUND;
     maxToStudy = P_maxToStudy;
     maxToKeep = P_maxToKeep;
     sons = SpaceNodeList(maxToKeep);
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
    Vect3D* startPos = getNewVect(0, 0, 0);
	AXIS axe = X;
    unsigned long int comptTest = 0;
        for(int iAx = 0 ; iAx < 3 and keepTrying ; iAx++){                    //rotations (axe)
	    axe = allAxis[iAx];
            for(uInt rot = 0 ; rot <= 4 and keepTrying  ; ++rot){               //rotations (suite)
                uInt nbOnX = (spaces[aSpace]->getDim()->x - nextObj->getDim()->x)/precision;
                uInt nbOnY = (spaces[aSpace]->getDim()->y - nextObj->getDim()->y)/precision;
                uInt nbOnZ = (spaces[aSpace]->getDim()->z - nextObj->getDim()->z)/precision;
                startPos->x = spaces[aSpace]->getCenter()->x - (spaces[aSpace]->getDim()->x)/2 + (nextObj->getDim()->x)/2;
                startPos->y = spaces[aSpace]->getCenter()->y - (spaces[aSpace]->getDim()->y)/2 + (nextObj->getDim()->y)/2;
                startPos->z = spaces[aSpace]->getCenter()->z - (spaces[aSpace]->getDim()->z)/2 + (nextObj->getDim()->z)/2;
                nextObj->move(getNewVect(startPos->x - nextObj->getCenter()->x, startPos->y - nextObj->getCenter()->y, startPos->z - nextObj->getCenter()->z));
                for(uInt z = 0 ; z < nbOnZ and keepTrying ; ++z){  //tts pos en z
                    nextObj->move(getNewVect(startPos->x-nextObj->getCenter()->x, 0, 0));
                    for(uInt x = 0 ; x < nbOnX and keepTrying ; ++x){  //tts pos en x
                        nextObj->move(getNewVect(0, startPos->y-nextObj->getCenter()->y, 0));
                        for(uInt y = 0 ; y < nbOnY and keepTrying ; ++y){  //tts pos en y
                            //ceci représente un cas étudié
                            ++comptTest;
                            if(spaces[aSpace]->isInternable(nextObj)){       //on branche
                                SpaceBandBTree* newSon = new SpaceBandBTree(precision, maxToStudy, maxToKeep);
                                for(uInt sp = 0 ; sp < spaces.size() ; sp++){
                                    if(sp != aSpace)
                                        newSon->addSpace(spaces[sp]->getCopy());
                                    else{
                                        SpaceToFill* copy = spaces[sp]->getCopy();
                                        copy->intern(nextObj->getCopy());
                                        newSon->addSpace(copy);
                                    }
                                }
                                for(uInt obj = 0 ; obj < objects.size() ; ++obj)
                                    if(objects[obj]->getId() != nextObj->getId())
                                        newSon->addObject(objects[obj]->getCopy());
                                sons.push_back(newSon);
                            }   //on branche plus
                            if(comptTest >= maxToStudy)
                                keepTrying = false;
                            nextObj->move(getNewVect(0, precision, 0));
                        }
                        nextObj->move(getNewVect(precision, 0, 0));
                    }
                nextObj->move(getNewVect(0, 0, precision));    
                }
            }
            nextObj->rotate(axe);
        }            
    delete startPos;
    }
    delete nextObj;
}

void SpaceBandBTree::bound(){
    if(!hasBranched)
        return;     //do nothing
    lowerBound = sons.getLowestQuality();
    if(lowerBound > highestLowBound)
        highestLowBound = lowerBound;
    upperBound = sons.getBestQuality();
}

void SpaceBandBTree::prune(){
    if(not hasBranched)
	    return;
    int index = sons.findIn(highestLowBound);
    if(index >= sons.size())
        index == sons.size()-1;
    sons.cutFront(index);
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

SpaceNodeList::SpaceNodeList(uInt maxLen){
    maxLength = maxLen;
}

SpaceNodeList::~SpaceNodeList(){
    while(not list.empty()){
        delete list[list.size()-1];
        list.pop_back();
    }
}

uInt SpaceNodeList::size(){
    return list.size();
}

bool SpaceNodeList::empty(){
    return list.empty();
}

SpaceBandBTree* SpaceNodeList::back(){
    if(not empty()){
        return list.back();
    }
    else
        return NULL;
}

void SpaceNodeList::push_back(SpaceBandBTree* toPush){
    uInt insertIndex = 0;
    long long int nodeQuality = toPush->getQuality();
    //liste triée de façon croissante en partant du début
    //trouver emplacement (recherche dichotomique)
    //insérer devant emplacement (sauf last : derrière)
    //si trop long supprimer arrière
    int lowBound, upBound;
    lowBound = 0;
    upBound = list.size();
    bool trouve = false;
    while(trouve and lowBound + 1 < upBound ){
        if(qualList[ (lowBound+upBound) / 2 ] < nodeQuality  )
            (lowBound = (lowBound+upBound) / 2);   
        else if(qualList[ (lowBound+upBound) / 2 ] > nodeQuality ) 
            (upBound = (lowBound+upBound) / 2) ;   
        else
        trouve = true;
    }
    int index = (lowBound+upBound)/2;
    if( index >= list.size() ){
        list.push_back(toPush);
        qualList.push_back(nodeQuality);
    }
    else{
        list.insert(list.begin() + index, toPush);
        qualList.insert(qualList.begin() + index, nodeQuality);
    }

    while(list.size() > maxLength){
        delete list.front();
        list.pop_front();
        qualList.pop_front();
    }
}

SpaceBandBTree* SpaceNodeList::at(uInt index){
    return list[index];
}

void SpaceNodeList::setMaxLen(uInt P_maxLen){
    maxLength = P_maxLen;
}

void SpaceNodeList::erase(uInt index){
    delete list[index];
    list.erase(list.begin()+index);
    qualList.erase(qualList.begin()+index);
}

void SpaceNodeList::cutFront(int index){
    int newSize = list.size()-(index+1);
    while(list.size() > newSize and newSize > 0){
       delete list[0];
       list.pop_front();
       qualList.pop_front();
    }
}

uInt SpaceNodeList::begin(){
    return 0;
}

int SpaceNodeList::findIn(int quality){     //peut renvoyer int == len(l) (erreurs de seg)
    unsigned int insertIndex = 0;
    long long int nodeQuality = quality;
    //liste triée de façon croissante en partant du début
    //trouver emplacement (recherche dichotomique)
    //le plus à la fin
    unsigned int lowBound, upBound;
    lowBound = 0;
    upBound = list.size();
    bool trouve = false;
    while( (not trouve) and  lowBound + 1 < upBound ){
        if( qualList[ (lowBound+upBound) / 2 ] < nodeQuality )
            (lowBound = (lowBound+upBound) / 2);   
        else if( qualList[ (lowBound+upBound) / 2 ] > nodeQuality )
            (upBound = (lowBound+upBound) / 2)  ;   
        else 
            trouve = true;
    }
    if(trouve){
        int index =  (lowBound+upBound)/2;
        for(; index < list.size()+1 and qualList[index] == nodeQuality ; ++index);
        return index;
    }
    else
        return -1;
}

long long int SpaceNodeList::getBestQuality(){
    if(empty()){
        return LOWEST_BOUND;
    }
    else{
        return qualList.back();
    }
}

long long int SpaceNodeList::getLowestQuality(){
    if(empty()){
        return LOWEST_BOUND;
    }
    else{
        return qualList.front();
    }
}

SemiSol SpaceBandB(std::vector<ObjectBox*> objects, std::vector<SpaceToFill*> spaces){
    SemiSol ret;
    if( not(  ret.spaces.empty() and ret.objectsLefts.empty() ) )
        std::cout << "algo.cpp:207" << std::endl;
    return ret;
}
