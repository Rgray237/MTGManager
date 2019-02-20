//
//  CollectionManager.hpp
//  MTGManager
//
//  Created by Robert Gray on 2/8/19.
//  Copyright © 2019 Robert Gray. All rights reserved.
//

#ifndef CollectionManager_hpp
#define CollectionManager_hpp
#include "Collection.hpp"
#include <vector>
#include <string>
#include <stdio.h>
#include <fstream>

class CollectionManager
{
public:
    CollectionManager(std::string firstCollectionFilePath);
    ~CollectionManager();
    mtgCollection * mainCollection;
    void exportCollection();
    
    void addDeck(std::string deckName);
    int numDecksInCollection(){return (int)decks.size();}
    std::string deckNameAtIndex(int i) {return decks[i].getCollectionName();}
    mtgCollection collectionAtIndex(int i) {return decks[i];}
    void setSelectedCollection(int i){selectedCollection = &decks[i];}
    /*
    int numDecksInCollection(){return (int)allCollections.size()-1;}
    std::string deckNameAtIndex(int i) {return allCollections[i+1].getCollectionName();}
    mtgCollection collectionAtIndex(int i) {return allCollections[i+1];}
    void setSelectedCollection(int i){selectedCollection = &allCollections[i+1];}
     */
    mtgCollection * getSelectedCollection() {return selectedCollection;}
    int numCardsInSelectedDeck();
    bool isSelectedCollectionADeck() {return (selectedCollection==mainCollection)||(selectedCollection==NULL) ? false : true;}
    bool canAddThisCardToSelectedDeck(Card * crd);
    bool canRemoveThisCardFromSelectedDeck(Card*crd);
    bool canRemoveCardFromMainCollection(Card *crd);
    bool doesDeckWithNameAlreadyExist(std::string deckName);
    void addCardToSelectedDeck(Card * crd);
    void removeCardFromSelectedDeck(Card *crd);
    void deleteSelectedDeck();
    std::string getExplanationString();
    std::string cardNameInSelectedDeckAtIndex(int i){return selectedCollection->nameAtIndex(i);}
    std::string getPathToCardBackImage() {return pathToCardBackImage;}
    int histogramValueOfSelectedDeckAtIndex(int i);
    float ratioOfHistogramValueWithPeakOfHistogram(int i);
private:
    mtgCollection *selectedCollection;
    std::string filePath;
    void findDecksFromCollectionFile();
    std::vector<mtgCollection> decks;//0 deck 1, 1 deck 2, etc. //OLD//0 - mainCollection , 1.... decksInCollection
    int errorCode=0;
    std::string pathToCardBackImage = "MTG_Pics/cardBack.jpg";

};

#endif /* CollectionManager_hpp */
