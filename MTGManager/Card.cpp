//
//  Card.cpp
//  MTGManager
//
//  Created by Robert Gray on 1/25/19.
//  Copyright © 2019 Robert Gray. All rights reserved.
//

#include "Card.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>



//column definitions
//0-name,1-printings/sets,2-colors,3-convertedmanacost,4-type,5-types,6-power,7-toughness,8-mana cost
enum colPositions
{
    colName,colSet,colColors,colCMC,colType,colTypes,colPower,colToughness,colMana
};



Card::Card(std::string name)
{
    
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    Card::name = name;
    //std::cout<<name<<std::endl;
    figureOutDetailsFromAllCardsList();
    
    //figureOutDetailsFromCardImage();
}

void Card::figureOutDetailsFromAllCardsList()
{
    //std::cout<<"find details for "<<name<<std::endl;
    std::ifstream in("MTG_Data/AllCardDetails.csv");
    std::string line;
    
    
    while (std::getline(in,line,'\n'))
    {
        if (line.find(name)!=std::string::npos)//is name in the line at all
        {
            if (line.find("\""+name+"\";")!=std::string::npos)//see if it's a definite match
                break;
        }
    }
    //std::cout<<line<<std::endl;

    size_t pos = line.find(';');
    //std::cout<<pos<<std::endl;
    
    while(pos!=std::string::npos)
    {
        //std::cout<<line.substr(0,pos)<<std::endl;
        detailColumns.push_back(line.substr(0,pos));
        line = line.substr(pos+1,line.size()-pos-1);
        pos = line.find(';');
    }

    
    name = detailColumns[colName].substr(1,detailColumns[colName].size()-2);
    
        
    convManaCost =std::stoi(detailColumns[colCMC]);
    if (convManaCost>0)
        manaCost =detailColumns[colMana].substr(1,detailColumns[colMana].size()-2);
    else manaCost = "";
    type =detailColumns[colType].substr(1,detailColumns[colType].size()-2);
    sets =detailColumns[colSet].substr(1,detailColumns[colSet].size()-2);
    colorsStr = detailColumns[colColors];
    //std::cout<<"COLORS = "<<colorsStr<<std::endl;
    determineTypes(type);
    determineColors(colorsStr);
    
    if (types[typCreature])
    {
        //std::cout<<detailColumns[colToughness].substr(1,detailColumns[colToughness].size()-2)<<std::endl;
        power = std::stoi(detailColumns[colPower].substr(1,detailColumns[colPower].size()-2));
        tough = std::stoi(detailColumns[colToughness].substr(1,detailColumns[colToughness].size()-2));
    }
    else
    {
        power = -1;
        tough = -1;
    }
        
    //std::cout<<std::endl;
    in.close();
    
    //printAllMembers();
}


void Card::determineColors(std::string clrs)
{
    size_t g,w,b,u,r;
    g = clrs.find("g");
    w = clrs.find("w");
    b = clrs.find("b");
    u = clrs.find("u");
    r = clrs.find("r");
    
    if (g!=std::string::npos)
        colors[clrG]=true;
    if (w!=std::string::npos)
        colors[clrW]=true;
    if (b!=std::string::npos)
        colors[clrB]=true;
    if (u!=std::string::npos)
        colors[clrU]=true;
    if (r!=std::string::npos)
        colors[clrR]=true;
    
    bool doesItHaveAColor = false;
    
    for(int i=0;i<5;i++)
    {
        if (colors[i])
            doesItHaveAColor=true;
    }
    if(!doesItHaveAColor)
        colorless = true;
}

void Card::determineTypes(std::string type)
{
    //typArtifact,typConspiracy,typCreature,typEnchantment,typInstant,typLand,typPhenomenon,typPlane,typPlaneswalker,typScheme,typSorcery,typTribal,typVanguard

    std::string str = type;
    //std::cout<<"determine types..." <<str<<std::endl;
    types[typArtifact] = str.find("artifact")!=std::string::npos ? true : false;
    types[typConspiracy] = str.find("conspiracy")!=std::string::npos ? true : false;
    types[typCreature] = str.find("creature")!=std::string::npos ? true : false;
    types[typEnchantment] = str.find("enchantment")!=std::string::npos ? true : false;
    types[typInstant] = str.find("instant")!=std::string::npos ? true : false;
    types[typLand] = str.find("land")!=std::string::npos ? true : false;
    types[typPhenomenon] = str.find("phenomenon")!=std::string::npos ? true : false;
    types[typPlane] = str.find("plane")!=std::string::npos ? true : false;
    types[typPlaneswalker] = str.find("planeswalker")!=std::string::npos ? true : false;
    types[typScheme] = str.find("scheme")!=std::string::npos ? true : false;
    types[typSorcery] = str.find("sorcery")!=std::string::npos ? true : false;
    types[typTribal] = str.find("tribal")!=std::string::npos ? true : false;
    types[typVanguard] = str.find("vanguard")!=std::string::npos ? true : false;

    /*std::cout<<"Printing types array..."<<std::endl;
    for(int i=0;i<13;i++)
    {
        std::cout<<types[i]<<",";
    }
    std::cout<<std::endl;
     */
}

void Card::printAllMembers()
{
    std::cout<<"name "<<name<<std::endl;
    std::cout<<"mana "<<manaCost<<std::endl;
    std::cout<<"convManaCost "<<convManaCost<<std::endl;
    std::cout<<"type "<<type<<std::endl;
    std::cout<<"sets "<<sets<<std::endl;
    std::cout<<"power "<<power<<std::endl;
    std::cout<<"toughness "<<tough<<std::endl;
}

void Card::printColumns()
{
    std::cout<<"columns are:"<<std::endl;
    for(int i=0;i<detailColumns.size();i++)
    {
        std::cout<<detailColumns[i]<<",";
    }
    
    std::cout<<std::endl;
}


bool Card::findImageOfCard()
{
    //printAllMembers();
    
    if (sets.find("rna")!=std::string::npos)
        pathToCardImage ="MTG_Pics/rna/"+name+".jpg";
    else if (sets.find("grn")!=std::string::npos)
        pathToCardImage = "MTG_Pics/grn/"+name+".png";
    else if (sets.find("m19")!=std::string::npos)
        pathToCardImage = "MTG_Pics/m19/"+name+".png";
    std::ifstream f(pathToCardImage);

   // std::cout<<std::endl<<pathToCardImage<<std::endl;
    
    return f.good();
}

std::string Card::pathToImage()
{
    return pathToCardImage;
}

std::ostream& operator<<(std::ostream& os, const Card& crd)
{
    os<<crd.name<<','<<crd.manaCost<<','<<crd.convManaCost<<','<<crd.type<<','<<crd.power<<"/"<<crd.tough;
    return os;
}
