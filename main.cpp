#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_set>
#include <algorithm>
#include <string>


bool YraZodis(const std::string& str) {
    // Salygos, kada string yra zodis
    return !str.empty() && str != "—" && str != "„" && str != "•" && str != " “ " &&str != "–";
}

// Funkcija, skirta patikrinti ar string turi savyje TLD
bool turiTLD(const std::string& str, const std::unordered_set<std::string>& tlds) {
    // Check if any TLD is present in the input string
    for (const auto& tld : tlds) {
        if (str.find(tld) != std::string::npos) {
            return true;
        }
    }
    return false;
}

//funkcija, skirta surinkti URL ir laikyti jas unordered_set'e
void extractLinks(const std::string& text, const std::unordered_set<std::string>& tlds, std::unordered_set<std::string>& links)
{
    std::istringstream iss(text);
    std::string word;
    while (iss >> word) 
    {
        // tikrinam ar string turi tld, jei turi, pridedame
        if (turiTLD(word, tlds))
        {links.insert(word);}
    }
}

int main() {
   
    std::string failo_kelias;
    std::cout<<"Iveskite failo kelia: ";
    std::cin>>failo_kelias;
    
    //atidarome faila, kuriame yra tekstas
    std::ifstream inputFile(failo_kelias);
    if (!inputFile.is_open()) {
        std::cerr << "Nepavyko atidaryti failo.\n";
        return 1;
    }
    
    std::ifstream inputFilelinkams(failo_kelias);
    if (!inputFilelinkams.is_open()) {
        std::cerr << "Nepavyko atidaryti failo.\n";
        return 1;
    }
    
    // perskaitome TLD faila
    std::ifstream tldFile("/Users/emilijabareikaite/Desktop/isankstine/isankstine/TLD.txt");
       if (!tldFile.is_open()) {
           std::cerr << "Error opening TLD.txt" << std::endl;
           return 1;
       }
    //tlds kaups visus galimus tld's, o tld skirta, kai skaitys TLD.txt faila ir ims po viena string
    std::unordered_set<std::string> tlds;
    std::string tld;
    
    while (tldFile >> tld) {tlds.insert(tld);}
    tldFile.close();

    // sukuriame map, skirta kaupti zodzius ir skaiciu, kiek kartu buvo naudotas zodis
    std::map<std::string, std::pair<int, std::vector<int>>> zodziuMap;
 
    // skaitome teksto eilute, o tada turint eilute skaitome po zodi
    std::string eilute;
    int lineNumber = 0;
    
    while (std::getline(inputFile, eilute)) {
        ++lineNumber;
        std::istringstream iss(eilute);
        std::string zodis;

        while (iss >> zodis) {
            // Istriname skyrybos zenklus
            zodis.erase(std::remove_if(zodis.begin(), zodis.end(), ispunct), zodis.end());
            
            // padidiname dabartiniam zodziui skaiciu
            if (YraZodis(zodis)) {auto& info = zodziuMap[zodis];
                info.first++;
                info.second.push_back(lineNumber);}
        }
    }
    
    //Sukuriame links, kuris kaups savyje URL linkus
        std::unordered_set<std::string> links;

        std::string eilute2;
    while (std::getline(inputFilelinkams, eilute2)) {extractLinks(eilute2, tlds, links);}

    inputFile.close();
    inputFilelinkams.close();

    // sukuriame ir atidarome surastu zodziu faila
    std::ofstream outputFile("rasti_zodziai.txt");
  
    outputFile << std::left<<std::setw(20)<< "Zodis" << std::left<<std::setw(15)<<"Kartu sk."<< std::left<<std::setw(10)<<"Eilutes nr."<<'\n';
    
    // jeigu zodis buvo parasytas daugiau nei viena karta ji spausdiname, kartu su skaiciumi, kiek kartu buvo parasytas, bei eiluciu sk., kur galima rasti zodi
  
    for (const auto& a : zodziuMap) 
    {
         if (a.second.first > 1)
         {
             outputFile << std::left << std::setw(20) << a.first << std::left << std::setw(15) << a.second.first;

             for (int eilutesnr : a.second.second) 
             {outputFile << eilutesnr<< ", ";}
             
             outputFile << '\n';
         }
     }
    
    outputFile.close();
   
    std::cout << "Surasti zodziai, kurie kartojasi daugiau nei viena karta, surasyti faile 'rasti_zodziai.txt'.\n";
    
    // Isspausdiname rastus URL; jei URL gale turi taska, ji panaikiname
        std::cout << "Rasti URL:" << std::endl;
        for (const auto& link : links) {
            if (link.back()=='.') {
                std::string link_pakeistas = link.substr(0, link.size() - 1);
                std::cout<<link_pakeistas<<std::endl;
            }
            else {std::cout << link << std::endl;}
        }
    
    system( "read -n 1 -s -p \"Press any key to continue...\"" );
    return 0;
}

