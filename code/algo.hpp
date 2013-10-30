#include"core.hpp"

struct SemiSol{
    std::vector<SpaceToFill*> spaces;
    std::vector<ObjectBox*> objectsLefts ;
};

class SpaceBandBTree;

class SpaceNodeList{
    public:
	SpaceNodeList();
        uInt size();
        bool empty();
        SpaceBandBTree* back();
	void pop_back();
	void push_back(SpaceBandBTree* toPush);
	SpaceBandBTree* at(uInt index);
	void setMaxLen(uInt P_maxLen);
	void erase(uInt index);
	uInt begin();
    protected:
	std::deque<SpaceBandBTree*> list;
	std::deque<long long int> qualList;
	uInt maxLength;
};

class SpaceBandBTree{
    public:
        SpaceBandBTree(float precision = 5);
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
        std::vector<SpaceBandBTree*> sons;
        long long unsigned int lowerBound, upperBound;
};
