//
//  MTGCardsInExistence.hpp
//  MTGManager
//
//  Created by Robert Gray on 1/27/19.
//  Copyright © 2019 Robert Gray. All rights reserved.
//

#ifndef MTGCardsInExistence_hpp
#define MTGCardsInExistence_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <set>

class cardsInExistence
{
public:
    cardsInExistence();
    bool doesCardExist(std::string name);
    std::string setOfCard(std::string name);
    std::vector<std::string> allCardNamesVector;
    std::vector<std::string> findTop10Matches(std::string str);
    int getSizeOfTop10();
    
    void writeJSONFileToCSVAllCards();
    
private:
    void processCSVFileToBeMoreReadable();

    std::vector<std::string> top10;
    std::set<std::string> allCardNames;
    std::set<std::string> RNANames;
    std::set<std::string> GRNNames;
    void writeSetToFile(std::set<std::string> set);
};
#endif /* MTGCardsInExistence_hpp */
