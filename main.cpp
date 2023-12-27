#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

bool YraZodis(const std::string& str) {
    // Define conditions for considering a string as a word
    return !str.empty() && str != "—" && str != "„" && str != "•" && str != " “ " &&str != "–";
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

    
    inputFile.close();

    // sukuriame ir atidarome surastu zodziu failax
    std::ofstream outputFile("rasti_zodziai.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Nepavyko atidaryti failo.\n";
        return 1;
    }
    
    outputFile << std::left<<std::setw(20)<< "Zodis" << std::left<<std::setw(15)<<"Kartu sk."<< std::left<<std::setw(10)<<"Eilutes nr."<<'\n';
    
    
    // jeigu zodis buvo parasytas daugiau nei viena karta ji spausdiname, kartu su skaiciumi, kiek kartu buvo parasytas, bei eiluciu sk., kur galima rasti zodi
  
    for (const auto& a : zodziuMap) {
         if (a.second.first > 1) {
             outputFile << std::left << std::setw(20) << a.first << std::left << std::setw(15) << a.second.first;

             for (int eilutesnr : a.second.second) {
                 outputFile << eilutesnr<< ", ";
             }

             outputFile << '\n';
         }
     }

    outputFile.close();

    std::cout << "Rezultatai surasyti faile 'rasti_zodziai.txt'.\n";
    
    system( "read -n 1 -s -p \"Press any key to continue...\"" );
    return 0;
}

