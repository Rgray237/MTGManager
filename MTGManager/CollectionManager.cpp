//
//  CollectionManager.cpp
//  MTGManager
//
//  Created by Robert Gray on 2/8/19.
//  Copyright © 2019 Robert Gray. All rights reserved.
//

#include "CollectionManager.hpp"


CollectionManager::CollectionManager(std::string firstCollectionFilePath)
{
    
    mainCollection = new mtgCollection(firstCollectionFilePath,"mainCollection");
    //decks.push_back(*mainCollection);

    selectedCollection = NULL;
    filePath = firstCollectionFilePath;
    findDecksFromCollectionFile();
    
    
    for(int i=0;i<decks.size();i++)
    {
        //std::cout<<"printing collection number "<<i<<std::endl;
        //allCollections[i].printCollection();
    }
}

bool CollectionManager::doesDeckWithNameAlreadyExist(std::string deckName)
{
    for(int i=0;i<decks.size();i++)
    {
        if (decks[i].getCollectionName()==deckName)
            return true;
    }
    return false;
}

void CollectionManager::addDeck(std::string deckName)
{
    if (doesDeckWithNameAlreadyExist(deckName))
    {
        int i=1;
        while (doesDeckWithNameAlreadyExist(deckName+"("+std::to_string(i)+")"))
            i++;
        deckName = deckName +"("+std::to_string(i)+")";
    }
        
    mtgCollection clc(deckName);
    decks.push_back(clc);
    selectedCollection = &decks[decks.size()-1];
}

int CollectionManager::numCardsInSelectedDeck()
{
    if (selectedCollection!=NULL && selectedCollection!=mainCollection)
        return selectedCollection->numOfUniqueCards();
    else
        return 0;
    
}

void CollectionManager::findDecksFromCollectionFile()
{
   // std::cout<<"searching for deck names in "<<filePath<<std::endl;
    std::ifstream inFile(filePath);
    std::string line;
    std::string col1,col2;
    if (inFile.is_open())
    {
        while (std::getline(inFile,line,'\n') && line!="[My Decks];")
        {
            }
        if (line=="[My Decks];")
        {
            while (std::getline(inFile, line, '\n'))
            {
               // std::cout<<"original line = "<<line<<std::endl;
                std::string newline = line.substr(0,line.size()-1);
                mtgCollection newclct = mtgCollection("MTG_Data/Decks/"+newline+".txt", newline);
                decks.push_back(newclct);
            }
        }
    }

}

CollectionManager::~CollectionManager()
{
   // std::cout<<"we doin it"<<std::endl;

    //exportCollection();
    
}

void CollectionManager::exportCollection()
{
    std::ofstream of;
    of.open("MTG_Data/myCollection.csv",std::ofstream::trunc);
    of.close();
    of.open("MTG_Data/myCollection.csv",std::ios_base::app);
    
    if (of.is_open())
    {
        mainCollection->exportCollection(of);

        of<<"[My Decks];"<<std::endl;
        
        for(int i=0;i<decks.size();i++)
        {
            //output deck name in main collection file.
            of<<decks[i].getCollectionName()<<";"<<std::endl;
            
            //output deck collection in appropriate deck file.
            std::ofstream deckOutStream;
            deckOutStream.open(decks[i].getCollectionFilePath());
            if (deckOutStream.is_open())
            {
                decks[i].exportCollection(deckOutStream);
            }
            else
                std::cout<<"error exporting deck: "<<decks[i].getCollectionName()<<std::endl;
            deckOutStream.close();
        }
    }
    else
        std::cout<<"Error opening"<<std::endl;
    of.close();
}

bool CollectionManager::canAddThisCardToSelectedDeck(Card * crd)
{
    if (isSelectedCollectionADeck() && crd)
    {
    int numInMainCollection = mainCollection->cardQuantities[crd->getName()];
    //std::cout<<numInMainCollection<<" of card "<<crd->getName()<<" in main collection"<<std::endl;
    int numInSelectedDeck = selectedCollection->cardQuantities[crd->getName()];
    //std::cout<<numInSelectedDeck<<" of card "<<crd->getName()<<" in selected deck"<<std::endl;
        
        if (numInSelectedDeck >= numInMainCollection)
        {
            errorCode=2;
            return false;
        }
        else
            return true;
    }
    else
    {
        if (crd)
            errorCode=3;
        else
            errorCode=4;
        return false;
    }
}

bool CollectionManager::canRemoveThisCardFromSelectedDeck(Card *crd)
{
    if (isSelectedCollectionADeck()&&crd)
        return selectedCollection->cardQuantities[crd->getName()]>=1 ? true : false;
    else
        return false;
}

void CollectionManager::addCardToSelectedDeck(Card *crd)
{
    selectedCollection->addCardToCollection(crd);
}


void CollectionManager::removeCardFromSelectedDeck(Card *crd)
{
    selectedCollection->removeCardFromCollection(crd);
}

void CollectionManager::deleteSelectedDeck()
{
    if (isSelectedCollectionADeck())
    {
        for(int i=0;i<decks.size();i++)
        {
            if (&decks[i]==selectedCollection)
            {
                std::remove(decks[i].getCollectionFilePath().c_str());
                std::swap(decks[i],decks[decks.size()-1]);
                decks.pop_back();
                std::sort(decks.begin(), decks.end());
                break;
            }
        }
        selectedCollection = NULL;
    }
}

bool CollectionManager::canRemoveCardFromMainCollection(Card *crd)
{
    
    int copiesInDeck=0;
    int copiesInMainCollection = mainCollection->cardQuantities[crd->getName()];
    if (copiesInMainCollection<1)
    {
        errorCode=0;
        return false;
    }
    //check all decks to see if any of them use more cards than we would have if we removed this card
    for(int i=0;i<decks.size();i++)
    {
        copiesInDeck = decks[i].cardQuantities[crd->getName()];
        std::cout<<"in main collection: "<<copiesInMainCollection<<", in deck: "<<copiesInDeck<<std::endl;
        
        if (copiesInDeck>=copiesInMainCollection)
        {
            errorCode=1;
            return false;
        }
    }
    return true;
}

std::string CollectionManager::getExplanationString()
{
    switch (errorCode) {
        case 0:
            return "Cannot remove card. Less than 1 cards in collection.";
            break;
        case 1:
            return "Cannot remove card. Decks in your collection use too many copies of this card.";
            break;
        case 2:
            return "Cannot add card to deck. Not enough copies in main collection.";
            break;
        case 3:
            return "Cannot add card to deck. No deck selected.";
        case 4:
            return "Cannot add card to deck. No card selected.";
        default:
            return "Cannot remove card from colleciton.";
            break;
    }
}

int CollectionManager::histogramValueOfSelectedDeckAtIndex(int i)
{
    return selectedCollection->getHistogramValueAtIndex(i);
}

float CollectionManager::ratioOfHistogramValueWithPeakOfHistogram(int i)
{
    if (selectedCollection->getMaxQtyFromHistogram()!=0)
    return (float)selectedCollection->getHistogramValueAtIndex(i)*0.8/(float)selectedCollection->getMaxQtyFromHistogram();
    else
        return 0;
}

