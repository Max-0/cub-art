#include"core.hpp"

#define LARGEST_BOUND 999999999999999;
#define LOWEST_BOUND 0;

struct SemiSol{
    std::vector<SpaceToFill*> spaces;
    std::vector<ObjectBox*> objectsLefts ;
};

class SpaceBandBTree;

class SpaceNodeList{
    public:
	SpaceNodeList(uInt maxLen = 100);
    ~SpaceNodeList();
        uInt size();
        bool empty();
        SpaceBandBTree* back();
	void push_back(SpaceBandBTree* toPush);
	SpaceBandBTree* at(uInt index);
	void setMaxLen(uInt P_maxLen);
	void erase(uInt index);
    void cutFront(int index);
    uInt begin();

    int findIn(int quality);

    long long int getBestQuality();
    long long int getLowestQuality();
    protected:
	std::deque<SpaceBandBTree*> list;
	std::deque<long long int> qualList;
	uInt maxLength;
};

class SpaceBandBTree{
    public:
        SpaceBandBTree(float precision = 5, uInt maxToStudy = 200, uInt maxKept = 200);
        virtual ~SpaceBandBTree();
        virtual void branch();
        virtual void bound();       
        virtual void prune();       
        virtual bool isSol();       
        virtual long long int getQuality();	
        void addSpace(SpaceToFill* toAdd);     
        void addObject(ObjectBox* toAdd);     
        uInt getSonsLength();
        SpaceBandBTree* getSonNb(uInt nb);       //peut renvoyer NULL
        bool getHasBranched();
        static long int highestLowBound;
    protected:
        std::vector<SpaceToFill*> spaces;
        std::vector<ObjectBox*> objects;
        float precisionStep;         //utilisé dans branch()
        bool hasBranched;
        SpaceNodeList sons;
        long long unsigned int lowerBound, upperBound;
        uInt maxToStudy;
        uInt maxToKeep;
};

SemiSol SpaceBandB(std::vector<ObjectBox*> objects, std::vector<SpaceToFill*> spaces);
