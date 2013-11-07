#include"tests.hpp"

void outputTest(bool test, bool awaited, const char* prefix){
    std::string output;
    if( ( test and awaited ) or ( (not test) and (not awaited) ) )	//test == awaited
        output = "OK"; 
    else
	output = "FAIL";
    output = prefix + output;
    std::cout << output << std::endl;
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
    delete test1;
    delete test2;
    delete test3;
    delete test5;
    delete test6;
    delete coll1;
    delete coll2;
    delete coll3;
    delete coll4;
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
    delete ref;
    delete vtest1;
    delete vtest2;
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
    delete intObj;
    delete intSpace;
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
    delete Obj4;
    delete Obj3;
    delete Obj5;
    delete Obj7;
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
    ObjectBox* Obj6 = new ObjectBox(36, getNewVect(50, 50, 50), getNewVect(-102, -224, 0), 50, 500, true, true, true, rotations, true);
    ObjectBox* Obj7 = new ObjectBox(37, getNewVect(100, 50, 50), getNewVect(-78, -173, 0), 50, 500, true, true, true, rotations, true);
    outputTest(testSpace->isInternable(Obj1), true, "   1:");
    testSpace->intern(Obj1);
    outputTest(testSpace->isInternable(Obj2), false, "   2:");
    outputTest(testSpace->isInternable(Obj3), true, "   3:");
    testSpace->intern(Obj3);
    outputTest(testSpace->isInternable(Obj4), false, "   4:");
    outputTest(testSpace->isInternable(Obj5), true, "   5:");
    outputTest(testSpace->isInternable(Obj6), true, "   6:");
    testSpace->intern(Obj6);
    outputTest(testSpace->isInternable(Obj7), true, "   7:");
    delete Obj2; 
    delete Obj4;
    delete Obj5; 
    delete Obj7;
    delete testSpace;
}

void testQuality(){
    std::cout << "test changements de qualitée" << std::endl;
    bool rotations[3];
    SpaceToFill* lowSpace = new SpaceToFill(41, getNewVect(500, 500, 500), getNewVect(0, 0, 0), 500);
    SpaceToFill* highSpace = new SpaceToFill(42, getNewVect(500, 500, 500), getNewVect(0, 0, 0), 500);
    //  1->3 : highSpace ; 4->6 : lowSpace
    ObjectBox* Obj1 = new ObjectBox(43, getNewVect(50, 50, 50), getNewVect(-25, 0, -25), 50, 5000, true, true, true, rotations, true);
    ObjectBox* Obj2 = new ObjectBox(44, getNewVect(50, 50, 50), getNewVect(25, 0, 25), 50, 5000, true, true, true, rotations, true);
    ObjectBox* Obj3 = new ObjectBox(45, getNewVect(50, 50, 50), getNewVect(0, 0, 0), 50, 5000, true, true, true, rotations, true);
    ObjectBox* Obj4 = new ObjectBox(46, getNewVect(50, 50, 50), getNewVect(100, 0, 64), 50, 5000, true, true, true, rotations, true);
    ObjectBox* Obj5 = new ObjectBox(47, getNewVect(50, 50, 50), getNewVect(200, 0, -96), 50, 5000, true, true, true, rotations, true);
    ObjectBox* Obj6 = new ObjectBox(48, getNewVect(50, 50, 50), getNewVect(100, 0, -150), 50, 5000, true, true, true, rotations, true);
    highSpace->intern(Obj3);
    outputTest(highSpace->getQuality() > lowSpace->getQuality(), true, "    1:");
    lowSpace->intern(Obj6);
    outputTest(highSpace->getQuality() > lowSpace->getQuality(), false, "    2:");
    highSpace->intern(Obj1);
    outputTest(highSpace->getQuality() > lowSpace->getQuality(), true, "    3:");
    lowSpace->intern(Obj5);
    outputTest(highSpace->getQuality() > lowSpace->getQuality(), true, "    4:");
    highSpace->intern(Obj2);
    outputTest(highSpace->getQuality() > lowSpace->getQuality(), true, "    5:");
    lowSpace->intern(Obj4);
    outputTest(highSpace->getQuality() > lowSpace->getQuality(), true, "    6:");
    delete lowSpace;
    delete highSpace;
}

void testCore(){
    testCreation();
    testCollisions();
    testValidIntern();
    testComplexIntern();
    testConstraints();
    testQuality();
}

//Algo
void testCreationAlgo(){
    SpaceBandBTree* test = new SpaceBandBTree();
    std::cout << "    création SpaceBandBTree : Ok" << std::endl; 
    delete test;
    std::cout << "    suppression SpaceBandBTree : Ok" << std::endl; 
}

void testBranch(){
    std::cout << "    Test SpaceBandBTree::branch()" << std::endl;
    SpaceBandBTree* test = new SpaceBandBTree();
    SpaceToFill* testSpace = new SpaceToFill(0, getNewVect(50, 50, 50), getNewVect(0, 0, 0), 500);
    bool rotations[3] = {true, true, true};
    ObjectBox* testObject = new ObjectBox(1, getNewVect(25, 25, 25), getNewVect(0, 0, 0), 0, 5000, true, true, true, rotations, true);
    test->addSpace(testSpace);
    test->addObject(testObject);
    outputTest(test->getHasBranched(), false, "        1:");
    outputTest(test->getSonsLength() == 0, true, "        1.2:");
    test->branch();
    outputTest(test->getHasBranched(), true, "        2:");
    outputTest(test->getSonsLength() > 0, true, "        2.1:");
    std::cout << "    Nombre de sous-noeuds : " << test->getSonsLength() << std::endl;
    delete test;
}

void testBound(){
    SpaceBandBTree::highestLowBound = 0;
    std::cout << "    Test SpaceBandBTree::bound()" << std::endl;
    SpaceBandBTree* test = new SpaceBandBTree();
    SpaceToFill* testSpace = new SpaceToFill(0, getNewVect(50, 50, 50), getNewVect(0, 0, 0), 500);
    bool rotations[3] = {true, true, true};
    ObjectBox* testObject = new ObjectBox(1, getNewVect(5, 5, 5), getNewVect(0, 0, 0), 0, 5000, true, true, true, rotations, true);
    test->addSpace(testSpace);
    test->addObject(testObject);
    test->branch();
    test->bound();
    std::cout << "        Tout s'est bien passer" << std::endl;
    delete test;
}

void testPrune(){
    SpaceBandBTree::highestLowBound = 0;
    std::cout << "    Test SpaceBandBTree::prune()" << std::endl;
    SpaceBandBTree* test = new SpaceBandBTree(5, 3000, 550);
    SpaceToFill* testSpace = new SpaceToFill(0, getNewVect(50, 10, 30), getNewVect(0, 0, 0), 500);
    bool rotations[3] = {true, true, true};
    ObjectBox* testObject = new ObjectBox(1, getNewVect(5, 5, 5), getNewVect(0, 0, 0), 0, 5000, true, true, true, rotations, true);
    ObjectBox* testO2 = new ObjectBox(2, getNewVect(5, 5, 5), getNewVect(0, 0, 0), 0, 5000, true, true, true, rotations, true);
    test->addSpace(testSpace);
    test->addObject(testObject);
    test->addObject(testO2);
    test->branch();
    test->bound();
    long long int sonsLength = test->getSonsLength();
    for(int i = 0 ; i < test->getSonsLength() ; ++i){
	test->getSonNb(i)->branch();
        test->getSonNb(i)->bound();
    }
    test->prune();
    outputTest(sonsLength > test->getSonsLength(), true, "        Réduction : ");
    std::cout << "	" << sonsLength-test->getSonsLength() << " noeuds déffaussés" << " sur " << sonsLength << std::endl;
    delete test;
}

void testAlgo(){
    std::cout << "Test Algorithme" << std::endl;
    testCreationAlgo();
    testBranch();
    testBound();
    testPrune();
    bool rotations[3];
    for(int i = 0 ; i < 3 ; i++)
        rotations[i] = true;
    //sujets du test
    std::vector<ObjectBox*> testObjects;
    testObjects.push_back(new ObjectBox(1, getNewVect(10, 10, 10), getNewVect(0, 0, 0), 4, 5000, true, true, true, rotations, true));
    testObjects.push_back(new ObjectBox(2, getNewVect(15, 20, 25), getNewVect(0, 0, 0), 10, 3000, true, true, true, rotations, true));
    testObjects.push_back(new ObjectBox(4, getNewVect(25, 25, 15), getNewVect(0, 0, 0), 10, 2500, true, true, true, rotations, true));
    testObjects.push_back(new ObjectBox(5, getNewVect(20, 15, 10), getNewVect(0, 0, 0), 10, 2500, true, true, true, rotations, true));
    testObjects.push_back(new ObjectBox(6, getNewVect(10, 10, 25), getNewVect(0, 0, 0), 10, 2500, true, true, true, rotations, true));
    testObjects.push_back(new ObjectBox(7, getNewVect(15, 25, 20), getNewVect(0, 0, 0), 10, 2500, true, true, true, rotations, true));
    testObjects.push_back(new ObjectBox(8, getNewVect(20, 25, 10), getNewVect(0, 0, 0), 10, 2500, true, true, true, rotations, true));
        //space test
    std::vector<SpaceToFill*> testSpaces;
    testSpaces.push_back(new SpaceToFill(11, getNewVect(100, 100, 100), getNewVect(0, 0, 0), 450));
    testSpaces.push_back(new SpaceToFill(12, getNewVect(100, 75, 100), getNewVect(0, 0, 0), 450)); 
    //calcul
    SemiSol result = SpaceBandB(testObjects, testSpaces);
    //suppression sujets
    while( not testObjects.empty() ){
        delete testObjects.back();
        testObjects.pop_back();
    }
    while( not testSpaces.empty() ){
        delete testSpaces.back();
        testSpaces.pop_back();
    }
    //affichage solutions
    std::cout << "     Test complet" << std::endl;
    for(int i = 0 ; i  < result.spaces.size() ; ++i){
        std::stringstream currAnalysis;
        currAnalysis << "    Containers" << std::endl;
        currAnalysis << "        ID, Stack Size, Quality, Inside..." << std::endl;
        currAnalysis << "        ";
        currAnalysis << result.spaces[i]->getId() << ", " ;
        currAnalysis << result.spaces[i]->getStackLen() << "," ;
        currAnalysis << result.spaces[i]->getQuality() << ", " << std::endl;
        currAnalysis << "            ID, Size(x, y, z), pos(x, y, z)" << std::endl;
        std::deque<ObjectBox*> *inBoxes = result.spaces[i]->getBoxStack();
        for(int j = 0 ; j < result.spaces[i]->getStackLen() ; j++){
            currAnalysis << "            ";
            currAnalysis << inBoxes->at(j)->getId() << ", " << "(";
            currAnalysis << inBoxes->at(j)->getDim()->x << ", " << inBoxes->at(j)->getDim()->y << ", " << inBoxes->at(j)->getDim()->z << "), (";
            currAnalysis << inBoxes->at(j)->getCenter()->x << ", " << inBoxes->at(j)->getCenter()->y << ", " << inBoxes->at(j)->getCenter()->z << std::endl;
        }

        std::cout << currAnalysis;
    }
}

void testMain(){
    testCore(); 
    testAlgo();   
}
