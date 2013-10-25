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
        virtual void bound();       //todo
        virtual void prune();       //todo
        virtual bool isSol();       //todo
        virtual long long int getQuality();
        void addSpace(SpaceToFill* toAdd);      //todo
        void addObject(ObjectBox* toAdd);       //todo
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
