#include<iostream>
#include<deque>
#include<cstdlib>
#include<ctime>

int findIn(std::deque<int>, std::deque<int>, int);

int main(){
    std::deque<int> list;
    std::deque<int> qualList;
    list.push_back(5);
    qualList.push_back(5);
    list.push_back(6);
    qualList.push_back(6);
    list.push_back(6);
    qualList.push_back(6);
    list.push_back(6);
    qualList.push_back(6);
    list.push_back(6);
    qualList.push_back(6);
    list.push_back(8);
    qualList.push_back(8);
    list.push_back(12);
    qualList.push_back(12);
    std::cout << findIn(list, qualList, 6) << " = 4" << std::endl;

}

int findIn(std::deque<int> list, std::deque<int> qualList, int quality){
    unsigned int insertIndex = 0;
    long long int nodeQuality = quality;
    //liste triée de façon croissante en partant du début
    //trouver emplacement (recherche dichotomique)
    //insérer
    //si trop long supprimer arrière
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
    return (lowBound+upBound)/2;
}
