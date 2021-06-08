#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <chrono>
#include <algorithm>


#include <set>
#include <map>
#include <stack>
#include <queue>


using namespace std;

void removeUnnecessaryElements(string &line){
    //Removing symbols
    set<char> symbols {'!','@','#','$','%','^','&','*','(',')','-','_','=','+','{','}','[',']','\\','|',':',';','\'','"','<','>',',','.','/','?','1','2','3','4','5','6','7','8','9','0'};
    for(char symbol : symbols){
        line.erase(std::remove(line.begin(), line.end(), symbol), line.end());
    }

    //Turning all letters into lower case
    for(int i=0; i<line.length(); i++){
        line[i] = tolower(line[i]);
    }
}

void findUrls(string line, vector<string> &urlList){
    stringstream ss(line);
    string word;

    const regex pattern("((http|https)://)(www.)?[a-zA-Z0-9@:%._\\+~#?&//=]{2,256}\\.[a-z]{2,6}\\b([-a-zA-Z0-9@:%._\\+~#?&//=]*)");
    const regex pattern2("(^www\\.[a-zA-Z0-9]+\\.[a-zA-Z0-9]+)");

    while(ss >> word){
        if(regex_match(word, pattern)) urlList.push_back(word);
        if(regex_match(word, pattern2)) urlList.push_back(word);
    }
}

int main(){
    //setlocale(LC_ALL,"Lithuanian");

    ifstream file("duomLT.txt");
    ofstream out("output.txt");

    map<string, set<int>> wordsList;
    vector<string> urlList;

    chrono::steady_clock sc;
    auto start = sc.now(); // Start timer
    cout << "Programa pradeta" << endl;
    
    string line;
    int lineNr = 1;
    while(getline(file, line)){
        findUrls(line, urlList);

        removeUnnecessaryElements(line);
        stringstream ss(line);

        string word;
        while(ss >> word){
            auto match = wordsList.find(word);
            if(match == wordsList.end()){
                set<int> emptySet;
                emptySet.insert(lineNr);
                wordsList.insert(pair <string, set<int>> (word, emptySet));
            }
            else match->second.insert(lineNr);
        }
        lineNr++;
    }

    //Printing all url addresses
    if(urlList.size() >= 1){
        out << "Url's:" << endl;
        for(int i=0; i<urlList.size(); i++){
            out << urlList[i] << endl;
        }
        out << "---------------------------------------------------" << endl;
    }
    

    for(auto it = wordsList.begin(); it != wordsList.end(); it++){
        if(it->second.size() > 1) {
            out << "'" << it->first << "' pasikartoja " << it->second.size() << " kartus, eilutese: ";
            for(int i : it->second){
                out << i << " ";
            }
            out << endl;
        }
        
    }

    auto end = sc.now();       // end timer
    auto time_span = static_cast<chrono::duration<double>>(end - start);   // measure time span between start & end
    cout<<"Programos veikimo laikas: "<<time_span.count()<<"s";

    return 0;
}