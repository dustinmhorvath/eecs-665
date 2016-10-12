#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <vector>

using namespace std;


int main(){

  std::ifstream infile("input.txt");
  std::string line;

  // Read in initial state
  std::getline(infile, line);
  unsigned first = line.find("{");
  unsigned last = line.find("}");
  int initialstate = std::stoi(line.substr(first + 1, last-first - 1));

  // Read in final states
  std::getline(infile, line);
  first = line.find('{');
  last = line.find('}');
  int finalstates = std::stoi(line.substr(first + 1, last-first-1));

  // Read in total states
  std::getline(infile, line);
  //int totalstates = std::stoi(gensubstr(line, ":"));
  int totalstates = std::strtol(&line[line.find(':') + 1], nullptr, 10);

  // Consume header line
  std::getline(infile, line);

  // Assuming we don't use more than the alphabet's worth of states, plus E
  std::vector<int> states[totalstates][27];

  std::string word;
  int row = 0;
  while (std::getline(infile, line)){
    std::stringstream ss(line);
    // Consume row number
    ss >> word;

    int column = 0;
    while(ss >> word){
      first = word.find('{');
      last = word.find('}');
      std::string nocurlies = word.substr(first + 1, last-first-1);
      std::stringstream commaparse(nocurlies);
      int i;
      while (commaparse >> i){
        states[row][column].push_back(i);

        if (commaparse.peek() == ',')
          commaparse.ignore();
      }
     

      column++;
    }
    
    row++;   
  }

  for(int i = 0; i < totalstates; i++){
    for(int j = 0; j < 3; j++){
      for(int k = 0; k < states[i][j].size(); k++){
        std::cout << states[i][j].at(k) << " ";
      }
      std::cout << "   ";
    }
    std::cout << "\n";
  }












}
