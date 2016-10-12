#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

#define MAXSYMBOLS 26
#define MAXSIZE MAXSYMBOLS+1


vector<int> epsilonClosureHelper(std::vector<int> states[][MAXSIZE], int row, int epsilonCol){
  vector<int> closure;
  for(int i = 0; i < states[row][epsilonCol].size(); i++){
    if(!states[row][epsilonCol].empty()){
      closure.push_back(states[row][epsilonCol].at(i));
      vector<int> recurseClose = epsilonClosureHelper(states, states[row][epsilonCol].at(i), epsilonCol);
      closure.insert(closure.end(), recurseClose.begin(), recurseClose.end());
    }
  }
  return closure;
}

vector<int> epsilonClosure(std::vector<int> states[][MAXSIZE], int row, int epsilonCol){
  vector<int> closure;
  closure.push_back(row);
  for(int i = 0; i < states[row][epsilonCol].size(); i++){
    if(!states[row][epsilonCol].empty()){
      closure.push_back(states[row][epsilonCol].at(i));
      vector<int> recurseClose = epsilonClosureHelper(states, states[row][epsilonCol].at(i), epsilonCol);
      closure.insert(closure.end(), recurseClose.begin(), recurseClose.end());
    }
  }
  return closure;
}

vector<int> navigate(std::vector<int> states[][MAXSIZE], vector<int> tocheck, int col){
  vector<int> destinations;
  for(int vectorindex = 0; vectorindex < tocheck.size(); vectorindex++){
    for(int i = 0; i < states[tocheck.at(vectorindex)][col].size(); i++){
      if(!states[tocheck.at(vectorindex)][col].empty()){
        destinations.push_back(states[tocheck.at(vectorindex)][col].at(i));
      }
    }
  }
  return destinations;
}




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
  std::vector<int> NFAstates[totalstates][MAXSIZE];
  std::vector<int> DFAstates[totalstates][MAXSIZE];

  // Parse table of inputs
  std::string word;
  int row = 0;
  int lastcolumn = 0;
  while (std::getline(infile, line)){
    std::stringstream ss(line);
    // Consume row number
    ss >> word;

    // Tokenize states
    int column = 0;
    while(ss >> word){
      first = word.find('{');
      last = word.find('}');
      std::string nocurlies = word.substr(first + 1, last-first-1);
      std::stringstream commaparse(nocurlies);
      int i;
      while (commaparse >> i){
        // NOTE decrementing indeces within the table by 1. This will convert
        // the whole table to 0-index, so will need ++ for any cout's
        NFAstates[row][column].push_back(i-1);

        if (commaparse.peek() == ',')
          commaparse.ignore();
      }
     

      column++;
    }
    if(column > lastcolumn){
      lastcolumn = column;
    }
    
    row++;   
  }
  lastcolumn--;

  /* // Print the read-in values
  for(int i = 0; i < totalstates; i++){
    for(int j = 0; j < 3; j++){
      for(int k = 0; k < NFAstates[i][j].size(); k++){
        std::cout << NFAstates[i][j].at(k) << " ";
      }
      std::cout << "    ";
    }
    std::cout << "\n";
  }
  */

  // Get epsilon closure of start state
  int start = initialstate-1;
  vector<int> Iclose = epsilonClosure(NFAstates, start, lastcolumn);
  std::cout << "E-Closure(I0) = {" << start+1;
  for(int i = 1; i < Iclose.size(); i++){
    std::cout << "," << Iclose.at(i)+1;
  }
  std::cout << "} = 1\n";
  //std::cout << printVector(Iclose) << "\n";

  int currentstate = 0;
  std::queue<vector<int>> inQueue;
  std::vector<vector<int>> outList;

  inQueue.push(Iclose);
  while(!inQueue.empty()){
    std::vector<int> currentVector = inQueue.front();
    inQueue.pop();

    bool printedMark = false;
    for(int symbol = 0; symbol < lastcolumn; symbol++){
      // Get a new vector off the front of the queue
      std::vector<int> tempVector = navigate(NFAstates, currentVector, symbol);
       
      // Don't print anything if there are no destinations for a symbol
      if(!tempVector.empty()){
        if(!printedMark){
          std::cout << "Mark " << currentstate+1 << "\n";
          printedMark = true;
        }
        
        // Print start
        std::cout << "{";
        if(currentVector.size() > 0){
          std::cout << currentVector.at(0)+1;
        }
        for(int i = 1; i < currentVector.size(); i++){
          std::cout << "," << currentVector.at(i)+1;
        }
        std::cout << "} --" << (char)((int)'a'+symbol) << "--> {";

        // Print possible terminals on symbol
        if(tempVector.size() > 0){
          std::cout << tempVector.at(0)+1;
        }
        for(int i = 1; i < tempVector.size(); i++){
          std::cout << "," << tempVector.at(i)+1;
        }
        std::cout << "}\n";

        inQueue.push(tempVector);

      }
    }






    



    currentstate++;
  }








}
