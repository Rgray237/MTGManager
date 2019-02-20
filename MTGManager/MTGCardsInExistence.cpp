//
//  MTGCardsInExistence.cpp
//  MTGManager
//
//  Created by Robert Gray on 1/27/19.
//  Copyright © 2019 Robert Gray. All rights reserved.
//

#include "MTGCardsInExistence.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include "json.hpp"


void toLowerCase(std::string& instr)
{
    std::transform(instr.begin(), instr.end(), instr.begin(), ::tolower);
}

void cardsInExistence::writeSetToFile(std::set<std::string> st)
{
    std::ofstream of("AllCards.txt");

    for(auto it = st.begin();it!=st.end();it++)
    {
        of<<*it<<std::endl;
    }
    of.close();
}


cardsInExistence::cardsInExistence()
{
    std::cout<<"Loading all cards in existence from file."<<std::endl;
    std::string filePath = "MTG_Data/AllCards.txt";
    std::ifstream myfile(filePath);
    std::string line;
    
    if(myfile.is_open())
    {
        while (std::getline(myfile,line,'\n')) {
            toLowerCase(line);
            allCardNamesVector.push_back(line);
            allCardNames.insert(line);
        }
    }
    myfile.close();
    /*
    filePath = "MTG_Data/RNANames.txt";
    myfile.open(filePath);
    if(myfile.is_open())
    {
        while (std::getline(myfile,line,'\n')) {
            toLowerCase(line);
            RNANames.insert(line);
        }
    }
    myfile.close();
    
    filePath = "MTG_Data/GRNNames.txt";
    myfile.open(filePath);
    if(myfile.is_open())
    {
        while (std::getline(myfile,line,'\n')) {
            toLowerCase(line);
            GRNNames.insert(line);
        }
    }
    myfile.close();
    
    std::cout<<GRNNames.size()<<std::endl;
    std::cout<<RNANames.size()<<std::endl;*/
    
    
    //writeSetToFile(allCardNames);
    /*
    std::ifstream mf("MTG_Data/GRNNames.txt");
    std::ofstream of("MTG_Data/GRNNamesNoExt.txt");
    std::string sub;

    if (mf.is_open())
    {
        while (std::getline(mf,line,'\r'))
        {
            sub = line.substr(0,line.length()-4);
            of<<sub<<std::endl;
            std::cout<<sub<<std::endl;
        }
        mf.close();
        of.close();
    }*/
    
}

std::vector<std::string> cardsInExistence::findTop10Matches(std::string str)
{
    size_t pos;
    std::vector<std::string> top10;
    for(int i=0;i<allCardNamesVector.size();i++)
    {
        
        pos = allCardNamesVector[i].find(str);
        if (pos!=std::string::npos)
            top10.push_back(allCardNamesVector[i]);
        if (top10.size()>=10)
        {
            break;
        }
    }
    std::cout<<std::endl;
    for(size_t i=top10.size();i<10;i++)
    {
        top10.push_back("");
    }
    cardsInExistence::top10 = top10;

    return top10;
}

int cardsInExistence::getSizeOfTop10()
{
    return (int)top10.size();
}

std::string cardsInExistence::setOfCard(std::string name)
{
    std::string set;

    std::set<std::string>::iterator it = GRNNames.find(name);
    if (it!=GRNNames.end())
        return "grn";
    it = RNANames.find(name);
    if (it!=RNANames.end())
        return "rna";
    return "none";
}

bool cardsInExistence::doesCardExist(std::string name)
{
    
    size_t cnt = allCardNames.count(name);
    //dfdxdxstd::cout<<cnt<<std::endl;
    if (cnt>0)
        return true;
     
    return false;
}

void cardsInExistence::writeJSONFileToCSVAllCards()
{
    
    std::ifstream myfile("MTG_Data/AllCards.json");
    std::ofstream of("MTG_Data/AllCardsTemp.csv");
    nlohmann::json j;
    
    myfile>>j;
    for(nlohmann::json::iterator it=j.begin();it!=j.end();it++)
    {
        if (it->at("layout")=="normal")
        {
            of<<it->at("name")<<";"<<it->at("printings")<<";"<<it->at("colors")<<";"<<it->at("convertedManaCost")<<";"<<it->at("type")<<";"<<it->at("types")<<";";
            //If creature, display power and toughness
            nlohmann::json j_string = it->at("type");
            std::string temp = j_string.get<std::string>();
            std::size_t i = temp.find("Creature");
            if (i!=std::string::npos)
                of<<it->at("power")<<";"<<it->at("toughness")<<";";
            else
                of<<""<<";"<<""<<";";
            if (it->at("convertedManaCost")!=0.0)
                of<<it->at("manaCost")<<";"<<std::endl;
            else
                of<<""<<";"<<std::endl;
        }
        
        else if (it->at("layout") == "split")
        {
            of<<it->at("name")<<";"<<it->at("printings")<<";"<<it->at("colors")<<";"<<it->at("faceConvertedManaCost")<<";"<<it->at("type")<<";"<<it->at("types")<<";;;";
            if (it->at("faceConvertedManaCost")!=0.0)
                of<<it->at("manaCost")<<";"<<std::endl;
            else
                of<<""<<";;;"<<std::endl;
        }
    }
    myfile.close();
    of.close();
    processCSVFileToBeMoreReadable();
}

void cardsInExistence::processCSVFileToBeMoreReadable()
{
    std::ifstream infile("MTG_Data/AllCardsTemp.csv");
    std::ofstream outfile("MTG_Data/AllCardDetails.csv");
    std::string line;
    
    while (std::getline(infile, line, '\n'))
    {
        toLowerCase(line);
        std::cout<<line<<std::endl;
        outfile<<line<<std::endl;
    }
    infile.close();
    outfile.close();
}
