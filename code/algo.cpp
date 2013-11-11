#include"algo.hpp"

long int SpaceBandBTree::highestLowBound = 0;

long long int getTreeNbOfNode(uInt m_ary, int treeDepth){
    long long int ret = 0;
    if(m_ary > 0 and treeDepth > 0){
            if(m_ary == 1)
                ret = treeDepth;
            else{
                uInt m_ary_exponented = 1;
                for(int i = 0 ; i < treeDepth+1 ; ++i)
                    m_ary_exponented *= m_ary;
                ret = (m_ary_exponented - 1) / (m_ary - 1);
            }
    }
    if(ret < 0)
        ret *= -1;
    return ret;
}

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
                            if(spaces[aSpace]->isInternable(nextObj)){       //on branche
                                //ceci représente un cas étudié
                                ++comptTest;
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
    objects.push_back(nextObj);
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

SemiSol SpaceBandBTree::getContent(){
    SemiSol res;
    for(int i = 0 ; i < spaces.size() ; ++i)
        res.spaces.push_back(spaces[i]->getCopy());
    for(int j = 0 ; j < objects.size() ; ++j)
        res.objectsLefts.push_back(objects[j]->getCopy());
    return res;
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

long long int BandBCallback::actualCount = 0;

void BandBCallback::operator()(uInt depth, int lenPrune, uInt maxSons, uInt boxNb){
    //cas n°1 : noeud traité sans prune
    //cas n°2 : noeud traité sans enfants (lenPrune = maxSons)
    //cas n°3 : noeud traité avec maxSons > lenPrune > 0
    long long int pruned = 1 + getTreeNbOfNode(maxSons, boxNb-(depth+1))*lenPrune;
    actualCount += pruned;
    long long int total = getTreeNbOfNode(maxSons, boxNb);
    std::cout << "-----waiting-----" << (double) 100 -  (( (double) (total- actualCount) / (total) )*100)  << "%-------------------" << (actualCount) << " / " << total  << std::endl;
}

SemiSol *SpaceBandB(std::vector<ObjectBox*> objects, std::vector<SpaceToFill*> spaces, float precision, uInt maxStudy, uInt maxKeep, BandBCallback* callback){
    SemiSol *ret = new SemiSol;
    uInt nbObj = objects.size();
    uInt nbMaxSons = maxKeep;
    SpaceBandBTree::highestLowBound = LOWEST_BOUND;
    //départ
    SpaceBandBTree* root = new SpaceBandBTree(5, maxStudy, maxKeep);
    for(int i = 0 ; i < objects.size() ; ++i )
        root->addObject(objects[i]->getCopy());
    for(int j = 0 ; j < spaces.size() ; ++j)
        root->addSpace(spaces[j]->getCopy());
    //récursivitée
    std::vector<SpaceBandBTree*> nodeStack;
    std::vector<SpaceBandBTree*> nodeSonsStack;
    //garder la solution en mémoire
    std::vector<SemiSol> sols;
    //traitement
    nodeStack.push_back(root);
    int depth = 0;
    uInt lenBefore, lenAfter;
    
        //
            uInt compt = 0;
        //

    while(not nodeStack.empty()){
        SpaceBandBTree* actNode = nodeStack.back();
        nodeStack.pop_back();
        compt ++;
        // 1 branch - 2 bound - 3 prune - 4 push sur le stack
        //1
        actNode->branch();
        //2
        lenBefore = nbMaxSons;
        actNode->bound();
        //3
        actNode->prune();
        lenAfter = actNode->getSonsLength();
        if(actNode->getSonsLength() > 0)
            (*callback)(depth, lenBefore-lenAfter, nbMaxSons, nbObj);
        //4
        for(int i = 0 ; i < actNode->getSonsLength() ; ++i){
            nodeSonsStack.push_back(actNode->getSonNb(i));
        }
        //si noeud sans enfants, solution possible 
        if(actNode->getSonsLength() == 0){
            sols.push_back(actNode->getContent());
            (*callback)(depth, nbMaxSons, nbMaxSons, nbObj);
        }
        //on oublie actNode
        actNode = NULL;
        //si touts les noeuds du niveau ont été traités : swap avec prochain niveau
        if(nodeStack.empty()){
            nodeStack.swap(nodeSonsStack);
            depth++;
        }
    }
    std::cout << compt << " noeuds traités, " << getTreeNbOfNode(nbMaxSons, nbObj)-compt << " noeuds déffaussés, " << (double) (getTreeNbOfNode(nbMaxSons, nbObj)-compt) / ( (double)  getTreeNbOfNode(nbMaxSons, nbObj)/100) << " \% noeuds déffaussés " << std::endl;
    //nettoyage
    delete root;
    //prendre la meilleur solution
    std::cout << sols.size() << " solutions" << std::endl;
    long long int bestQuality = 0;
    uInt bestIndex = -1;
    for(uInt i = 0 ; i < sols.size() ; ++i){
        long long int quality = 0;
        for(uInt j = 0 ; j < sols[i].spaces.size() ; ++j){
            quality += sols[i].spaces[j]->getQuality();    
        }
        if(quality > bestQuality){
            bestQuality = quality;
            bestIndex = i;
        }
    }
    if(bestIndex != -1){
        for(uInt i = 0 ; i < sols[bestIndex].spaces.size() ; ++ i)
            ret->spaces.push_back(sols[bestIndex].spaces[i]);
        for(uInt i = 0 ; i < sols[bestIndex].objectsLefts.size() ; ++ i)
            ret->objectsLefts.push_back(sols[bestIndex].objectsLefts[i]);
        for(int i = 0 ; i < sols.size() ; ++i){
            if(i != bestIndex){
                for(uInt j = 0 ; j < sols[i].spaces.size() ; ++j)
                    delete sols[i].spaces[j];
                for(uInt j = 0 ; j < sols[i].objectsLefts.size() ; ++j)
                    delete sols[i].objectsLefts[j];
            }
        }
    }    
    delete callback;
    return ret;
}
