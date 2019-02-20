//
//  Card.hpp
//  MTGManager
//
//  Created by Robert Gray on 1/25/19.
//  Copyright © 2019 Robert Gray. All rights reserved.
//

#ifndef Card_hpp
#define Card_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

enum colors
{
    clrW,clrU,clrB,clrR,clrG
};

enum cardTypes
{typArtifact,typConspiracy,typCreature,typEnchantment,typInstant,typLand,typPhenomenon,typPlane,typPlaneswalker,typScheme,typSorcery,typTribal,typVanguard
};

class Card{
public:
    Card(std::string name);
    
    bool findImageOfCard();
    std::string pathToImage();
    
    const std::string getName() {return name;}
    int getconvManaCost(){return convManaCost;}
    bool colors[5]={0,0,0,0,0};

    friend std::ostream& operator<<(std::ostream& os, const Card& crd);
    std::string name;

    //TODO -- fix these to be accurate
    bool isCreature() {return types[typCreature];}
    bool isInstant() {return types[typInstant];}
    bool isSorcery() {return types[typSorcery];}
    bool isEnchantment() {return types[typEnchantment];}
    bool isLand() {return types[typLand];}
    bool isColorless() {return colorless;}
    bool isMultiColored() {int cnt=0;for(int i=0;i<5;i++){if(colors[i])cnt++;} return cnt>1?true:false;}
    
    bool operator < (const Card& str) const
    {
        return (name < str.name);
    }
    
private:
    void figureOutDetailsFromAllCardsList();
    std::vector<std::string> detailColumns;
    void printColumns();
    void printAllMembers();
    void determineTypes(std::string type);
    void determineColors(std::string colors);

    std::string type;
    std::string colorsStr;
    bool types[13]= {0,0,0,0,0,0,0,0,0,0,0,0,0};
    int convManaCost;
    std::string manaCost;
    int power;
    int tough;
    std::string sets;
    bool colorless=false;
    std::string pathToCardImage;
    
};


struct less_than_card_names
{
    inline bool operator() (const Card& crd1,const Card& crd2)
    {
        return (crd1.name<crd2.name);
    }
};

#endif /* Card_hpp */
