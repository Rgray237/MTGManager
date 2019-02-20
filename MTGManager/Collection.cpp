//
//  Collection.cpp
//  MTGManager
//
//  Created by Robert Gray on 1/25/19.
//  Copyright © 2019 Robert Gray. All rights reserved.
//

#include "Collection.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>



mtgCollection::mtgCollection(std::string filePath,std::string CollectionName)
{
    mtgCollection::collectionName = CollectionName;
    mtgCollection::mfilePath = filePath;
    loadFromFile(filePath);
    createVectorOfCollection();
    generateStatistics();
    applyFilters();
}

mtgCollection::mtgCollection(std::string CollectionName)
{
    mtgCollection::collectionName = CollectionName;
    mtgCollection::mfilePath = "MTG_Data/Decks/"+CollectionName+".txt";
}

void mtgCollection::printFilters()
{
    std::cout<<"\n\ncolorFilters: "<<std::endl;
    for(int i=0;i<numColorFilters;i++)
    {
        std::cout<<colorFilters[i]<<std::endl;
    }
    std::cout<<"\ntypeFilters: "<<std::endl;
    for(int i=0;i<numTypeFilters;i++)
    {
        std::cout<<typeFilters[i]<<std::endl;
    }
}

void mtgCollection::applyFilters()
{
    filteredVectorOfCardDetailsAndQuantities.clear();
    filteredVectorOfCardQuantities.clear();
    
    //Check every card in full collection
    for(int i=0;i<vectorOfCardDetailsAndQuantities.size();i++)
    {
        bool cardFitsAColorFilter = false;//assume the card is not the correct color until verified.
        
        bool cardFitsATypeFilter = false;//assume the card is not the correct type until verified.
        
        Card crd = vectorOfCardDetailsAndQuantities[i].first;
        
        //TODO -- add all the other filters
        //check if the card matches any of the color filters
        for(int f=0;f<numColorFilters;f++)
        {
            switch (f) {
                case filtRed:
                    if (colorFilters[filtRed]&&crd.colors[clrR])
                        cardFitsAColorFilter = true;
                    break;
                case filtBlue:
                    if (colorFilters[filtBlue]&&crd.colors[clrU])
                        cardFitsAColorFilter = true;
                    break;
                case filtWhite:
                    if (colorFilters[filtWhite]&&crd.colors[clrW])
                        cardFitsAColorFilter = true;
                    break;
                case filtGreen:
                    if (colorFilters[filtGreen]&&crd.colors[clrG])
                        cardFitsAColorFilter = true;
                    break;
                case filtBlack:
                    if (colorFilters[filtBlack]&&crd.colors[clrB])
                        cardFitsAColorFilter = true;
                    break;
                case filtColorless:
                    if (colorFilters[filtColorless]&&crd.isColorless())
                        cardFitsAColorFilter = true;
                    break;
                default:
                    break;
            }
        }
        
        //check if the card matches any of the type filters
        for(int f=0;f<numTypeFilters;f++)
        {
            switch (f) {
                case filtCreatures:
                    if (typeFilters[filtCreatures]&&crd.isCreature())
                        cardFitsATypeFilter = true;
                    break;
                case filtSorcery:
                    if (typeFilters[filtSorcery]&&crd.isSorcery())
                        cardFitsATypeFilter = true;
                    break;
                case filtInstant:
                    if (typeFilters[filtInstant]&&crd.isInstant())
                        cardFitsATypeFilter = true;
                    break;
                case filtLand:
                    if (typeFilters[filtLand]&&crd.isLand())
                        cardFitsATypeFilter = true;
                    break;
                default:
                    break;
            }
        }
        
        //if card fits any color filter criteria AND any type filter, it is added to "filtered" vectors
        if (cardFitsAColorFilter&&cardFitsATypeFilter)
        {
            filteredVectorOfCardQuantities.push_back(std::make_pair(crd.getName(), vectorOfCardDetailsAndQuantities[i].second));
            filteredVectorOfCardDetailsAndQuantities.push_back(std::make_pair(crd, vectorOfCardDetailsAndQuantities[i].second));
        }
        else
        {
            //std::cout<<crd.getName()<<" doesn't fit filters because color is "<<cardFitsAColorFilter<<" and type is "<<cardFitsATypeFilter<<std::endl;
        }
    }
}

mtgCollection::~mtgCollection()
{
}

void mtgCollection::printCollection()
{
    
    for (auto iterator=cardQuantities.begin();iterator!=cardQuantities.end();iterator++)
    {
        std::cout<<iterator->first<<" qty. "<<iterator->second<<std::endl;
    }
    /*
    for(int i=0;i<vectorOfCardDetailsAndQuantities.size();i++)
    {
        std::cout<<vectorOfCardDetailsAndQuantities[i].first<<", Qty. "<<vectorOfCardDetailsAndQuantities[i].second<<std::endl;
    }
     */
}

void mtgCollection::exportCollection(std::ofstream & ofs)
{
    for (auto iterator=cardQuantities.begin();iterator!=cardQuantities.end();iterator++)
    {
        if (iterator->second<=0)
            continue;
        ofs<<"\""<<iterator->first<<"\""<<";"<<iterator->second<<std::endl;
    }
}



void mtgCollection::loadFromFile(std::string filePath)
{
    std::cout<<"loading collection from file "<<filePath<<std::endl;
    std::ifstream inFile(filePath);
    std::string line;
    std::string col1,col2;
    if (inFile.is_open())
    {
        while (std::getline(inFile,line,'\n') && line!="[My Decks];") {
            //std::cout<<line<<std::endl;
            size_t semiCol = line.find(";");
            col1 =line.substr(1,semiCol-2);
            col2 =line.substr(semiCol+1);
            cardQuantities[col1] = std::stoi(col2);
        }
    }
    else
        std::cout<<"ERROR"<<std::endl;
    //printCollection();
}

void mtgCollection::printDetails()
{
    for (auto iterator=cardDetails.begin();iterator!=cardDetails.end();iterator++)
    {
        std::cout<<iterator->first<<" details "<<iterator->second.pathToImage()<<std::endl;
    }
}

void mtgCollection::createVectorOfCollection()
{
    vectorOfCardDetailsAndQuantities.clear();
    for(auto it=cardQuantities.begin();it!=cardQuantities.end();it++)
    {
        Card crd(it->first);
        vectorOfCardDetailsAndQuantities.push_back(std::make_pair(crd,it->second));
    }
}


void mtgCollection::sortVectorBy(int i,std::vector<std::pair<Card,int> > & vec)//0-card name 1-- converted mana cost
{
    switch (i) {
        case 0:
            std::sort(vectorOfCardDetailsAndQuantities.begin(), vectorOfCardDetailsAndQuantities.end());
            break;
        case 1:
            
        default:
            std::sort(vectorOfCardDetailsAndQuantities.begin(), vectorOfCardDetailsAndQuantities.end());
            break;
    }
}

void mtgCollection::addCardToCollection(Card *crd)
{
    if (crd)
    {
    if (cardQuantities[crd->getName()]==0)
    {
        Card crdCpy = *crd;
        vectorOfCardDetailsAndQuantities.push_back(std::make_pair(crdCpy, 1));
        sortVectorBy(0, vectorOfCardDetailsAndQuantities);
    }
    else
    {
        //search for card in vector of card details
        for(int i=0;i<vectorOfCardDetailsAndQuantities.size();i++)
        {
            if (crd->getName()==vectorOfCardDetailsAndQuantities[i].first.getName())
            {
                vectorOfCardDetailsAndQuantities[i].second++;
                break;
            }
        }
    }
    cardQuantities[crd->getName()]++;

        
    //printCollection();
    //printDetails();
        
        for(int i=0;i<5;i++)
        {
            if(crd->colors[i])
                statsColorCount[i]++;
        }
    }
    applyFilters();
    generateStatistics();
}

int mtgCollection::numOfCards()
{
    int cnt=0;
    for(int i=0;i<vectorOfCardDetailsAndQuantities.size();i++)
    {
        cnt+=vectorOfCardDetailsAndQuantities[i].second;
    }
    return cnt;
}

void mtgCollection::removeCardFromCollection(Card *crd)
{
    if (crd)
    {
    if (cardQuantities[crd->getName()]>1)
    {
        cardQuantities[crd->getName()]--;

        for(int i=0;i<5;i++)
        {
            if(crd->colors[i])
                statsColorCount[i]--;
        }
    }
    else
    {
        cardQuantities[crd->getName()]=0;
        for(int i=0;i<vectorOfCardDetailsAndQuantities.size();i++)
        {
            if (crd->getName()==vectorOfCardDetailsAndQuantities[i].first.getName())
            {
                std::swap(vectorOfCardDetailsAndQuantities[i],vectorOfCardDetailsAndQuantities[vectorOfCardDetailsAndQuantities.size()-1]);
                vectorOfCardDetailsAndQuantities.pop_back();
                sortVectorBy(0, vectorOfCardDetailsAndQuantities);
                break;
            }

        }
    }
    
        for(int i=0;i<vectorOfCardDetailsAndQuantities.size();i++)
        {
            if (crd->getName()==vectorOfCardDetailsAndQuantities[i].first.getName())
            {
                vectorOfCardDetailsAndQuantities[i].second = cardQuantities[crd->getName()];
            }
        }
    
        
    //createVectorOfCollection();
    //printCollection();
    //printDetails();
    }
    
    applyFilters();
    generateStatistics();
}

void mtgCollection::generateStatistics()
{
    for(int i=0;i<20;i++)
        cmcHistogram[i]=0;
    for(int i=0;i<5;i++)
        statsColorCount[i]=0;
    int max=0;
    maxQtyInHistogram=0;
    for(int i=0;i<vectorOfCardDetailsAndQuantities.size();i++)
    {
        Card crd = vectorOfCardDetailsAndQuantities[i].first;
        //std::cout<<crd.getName()<<" "<<vectorOfCardDetailsAndQuantities[i].second<<std::endl;
        for(int j=0;j<5;j++)
        {
            if (crd.colors[j])
            {
                //std::cout<<statsColorCount[j]<<","<<vectorOfCardDetailsAndQuantities[i].second;
                statsColorCount[j]+=vectorOfCardDetailsAndQuantities[i].second;
            }
        }
        if (crd.getconvManaCost()>max)
            max = crd.getconvManaCost();
        cmcHistogram[crd.getconvManaCost()]+=vectorOfCardDetailsAndQuantities[i].second;
    }
    
    //find peak of histogram
    for(int i=0;i<max;i++)
    {
        if (cmcHistogram[i]>maxQtyInHistogram)
            maxQtyInHistogram=cmcHistogram[i];
    }
    mtgCollection::maxCmc = max;
}

void mtgCollection::printHistogram(int maxCmc)
{
    std::cout<<"\nhistogram of Collection:"<<std::endl;
    for(int i=0;i<=maxCmc;i++)
    {
        std::cout<<cmcHistogram[i]<<std::endl;
    }
}

std::pair<std::string, int>  mtgCollection::pairAtIndex(int i)
{
    std::pair<std::string,int> pr = std::make_pair(vectorOfCardDetailsAndQuantities[i].first.getName(),vectorOfCardDetailsAndQuantities[i].second);
    return pr;
}


std::pair<std::string, int>  mtgCollection::filteredPairAtIndex(int i)
{
    std::pair<std::string,int> pr = std::make_pair(filteredVectorOfCardDetailsAndQuantities[i].first.getName(),filteredVectorOfCardDetailsAndQuantities[i].second);
    return pr;
}

std::string mtgCollection::nameAtIndex(int i)
{
    return vectorOfCardDetailsAndQuantities[i].first.getName();
}

std::string mtgCollection::filteredNameAtIndex(int i)
{
    return filteredVectorOfCardDetailsAndQuantities[i].first.getName();
}

int mtgCollection::qtyAtIndex(int i)
{
    return vectorOfCardDetailsAndQuantities[i].second;
}

int mtgCollection::filteredQtyAtIndex(int i)
{
    return filteredVectorOfCardDetailsAndQuantities[i].second;
}

int mtgCollection::getMaxQtyFromHistogram()
{
    int max =0;
    for(int i=0;i<10;i++)
    {
        if(cmcHistogram[i]>max)
            max = cmcHistogram[i];
    }
    int numberOf10Plus = 0;
    for(int i=10;i<20;i++)
    {
        numberOf10Plus+=cmcHistogram[i];
    }
    if (numberOf10Plus>max)
        max = numberOf10Plus;
    
    return max;
}


