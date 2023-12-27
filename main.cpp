#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

bool isWord(const std::string& str) {
    // Define conditions for considering a string as a word
    return !str.empty() && str != "—" && str != "„" && str != "•" && str != " “ " &&str != "–";
}

std::string removeQuotationMarks(const std::string& word) {
    std::string cleanedWord;
    for (wchar_t c : word) {
        if (c != L'„' && c != L'“') {
            cleanedWord += c;
        }
    }
    return cleanedWord;
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
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string word;

        while (iss >> word) {
            // Istriname skyrybos zenklus
            word.erase(std::remove_if(word.begin(), word.end(), ispunct), word.end());
            std::string cleanedWord = removeQuotationMarks(word);
            // padidiname dabartiniam zodziui skaiciu
            if (isWord(cleanedWord) ) zodziuMap[cleanedWord]++;
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
    for (const auto& entry : zodziuMap) {
        if (entry.second > 1) {
            outputFile << std::left<<std::setw(20)<<entry.first << std::left<<std::setw(10)<<entry.second << '\n';
        }
    }

    outputFile.close();

    std::cout << "Rezultatai surasyti faile 'rasti_zodziai.txt'.\n";
    
    system( "read -n 1 -s -p \"Press any key to continue...\"" );
    return 0;
}

