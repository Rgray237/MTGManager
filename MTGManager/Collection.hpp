//
//  Collection.hpp
//  MTGManager
//
//  Created by Robert Gray on 1/25/19.
//  Copyright © 2019 Robert Gray. All rights reserved.
//

#ifndef Collection_hpp
#define Collection_hpp

#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include <set>
#include <vector>

#include "Card.hpp"

enum typeFilters
{
    filtCreatures,filtInstant,filtSorcery,filtLand,filtEnchantment
};

enum colorFilters
{
    filtWhite,filtBlue,filtBlack,filtRed,filtGreen,filtColorless,filtMulti
};

#define numTypeFilters 5
#define numColorFilters 7

class mtgCollection{
public:
    mtgCollection(std::string filePath,std::string CollectionName);
    mtgCollection(std::string CollectionName);
    ~mtgCollection();
    void addCardToCollection(Card* crd);
    void removeCardFromCollection(Card* crd);
    void printCollection();
    void printDetails();
    void printFilters();
    void applyFilters();
    void exportCollection(std::ofstream & ofs);
    std::map<std::string,int> cardQuantities;
    std::pair<std::string, int> pairAtIndex(int i);

    std::pair<std::string,int> filteredPairAtIndex(int i);
    std::vector<std::pair<std::string, int> > filteredVectorOfCardQuantities;
    std::vector<std::pair<Card,int> > filteredVectorOfCardDetailsAndQuantities;

    int filteredNumOfUniqueCards() {return (int) filteredVectorOfCardDetailsAndQuantities.size();}
    
    std::string nameAtIndex(int i);
    std::string filteredNameAtIndex(int i);
    int     qtyAtIndex(int i);
    int filteredQtyAtIndex(int i);

    int numOfUniqueCards() {return (int)vectorOfCardDetailsAndQuantities.size();}
    int numOfCards();
    int getGreenCount(){return statsColorCount[clrG];}
    int getBlueCount(){return statsColorCount[clrU];}
    int getBlackCount(){return statsColorCount[clrB];}
    int getWhiteCount(){return statsColorCount[clrW];}
    int getRedCount(){return statsColorCount[clrR];}
    
    std::string getCollectionName(){return collectionName;}
    std::string getCollectionFilePath() {return mfilePath;}

    void setColorFilter(int i,bool onoff) {colorFilters[i]= onoff; applyFilters();}
    void setTypeFilter(int i,bool onoff) {typeFilters[i]= onoff; applyFilters();}

    bool operator < (const mtgCollection& collection) const
    {
        return (collectionName < collection.collectionName);
    }
    
    int getHistogramValueAtIndex(int i) {if (i<10) return cmcHistogram[i];
    else {
        int cnt=0;
        for(int j=10;j<20;j++)
            cnt+=cmcHistogram[j];
        return cnt;}}
    int getMaxQtyFromHistogram();
private:
    int statsColorCount[5]={0,0,0,0,0};
    bool typeFilters[numTypeFilters]={1,1,1,1,1};
    bool colorFilters[numColorFilters] = {1,1,1,1,1,1,1};
    std::string collectionName;
    std::string mfilePath;
    void sortVectorBy(int i,std::vector<std::pair<Card,int> > & vec);//0-card name 1-- converted mana cost
    void updateStatistics(Card * crd);
    std::vector<std::pair<std::string, int> > vectorOfCardQuantities;
    std::vector<std::pair<Card,int> > vectorOfCardDetailsAndQuantities;
    void createVectorOfCollection();
    void generateStatistics();
    std::map<std::string,Card> cardDetails;
    
    void loadFromFile(std::string filePath);
    int cmcHistogram[20]={0};
    void printHistogram(int maxCmc);
    int maxCmc = 0;
    int maxQtyInHistogram=0;
};



#endif /* Collection_hpp */
