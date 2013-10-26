#include"core.hpp"

struct SemiSol{
    std::vector<SpaceToFill*> spaces;
    std::vector<ObjectBox*> objectsLefts ;
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
    protected:
        std::vector<SpaceToFill*> spaces;
        std::vector<ObjectBox*> objects;
        static long int highestLowBound;
        float precisionStep;         //utilisé dans branch()
        bool hasBranched;
        std::vector<SpaceBandBTree*> sons;
        long long unsigned int lowerBound, upperBound;
};
