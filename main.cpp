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
    std::map<std::string, int> zodziuMap;

    // skaitome teksto eilute, o tada turint eilute skaitome po zodi
    std::string eilute;
    while (std::getline(inputFile, eilute)) {
        std::istringstream iss(eilute);
        std::string zodis;

        while (iss >> zodis) {
            // Istriname skyrybos zenklus
            zodis.erase(std::remove_if(zodis.begin(), zodis.end(), ispunct), zodis.end());
            
            // padidiname dabartiniam zodziui skaiciu
            if (YraZodis(zodis) ) zodziuMap[zodis]++;
        }
    }

    
    inputFile.close();

    // sukuriame ir atidarome surastu zodziu failax
    std::ofstream outputFile("rasti_zodziai.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Nepavyko atidaryti failo.\n";
        return 1;
    }
    
    outputFile << std::left<<std::setw(20)<< "Zodis" << std::left<<std::setw(10)<<"Kiek kartu pasikartojo"<< '\n';
    // jeigu zodis buvo parasytas daugiau nei viena karta ji spausdiname
    for (const auto& a : zodziuMap) {
        if (a.second > 1) {
            outputFile << std::left<<std::setw(20)<<a.first << std::left<<std::setw(10)<<a.second << '\n';
        }
    }

    outputFile.close();

    std::cout << "Rezultatai surasyti faile 'rasti_zodziai.txt'.\n";
    
    system( "read -n 1 -s -p \"Press any key to continue...\"" );
    return 0;
}

