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

vector<int> epsilonClosureHelper(std::vector<int> states[][27], int row, int epsilonCol){
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

vector<int> epsilonClosure(std::vector<int> states[][27], int row, int epsilonCol){
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

vector<int> navigate(std::vector<int> states[][27], vector<int> tocheck, int col){
  vector<int> destinations;
  for(int vectorindex = 0; vectorindex < tocheck.size(); vectorindex++){
    for(int i = 0; i < states[vectorindex][col].size(); i++){
      if(!states[vectorindex][col].empty()){
        destinations.push_back(states[vectorindex][col].at(i));
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
  std::vector<int> NFAstates[totalstates][27];
  std::vector<int> DFAstates[totalstates][27];

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
        NFAstates[row][column].push_back(i);

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

  for(int i = 0; i < totalstates; i++){
    for(int j = 0; j < 3; j++){
      for(int k = 0; k < NFAstates[i][j].size(); k++){
        std::cout << NFAstates[i][j].at(k) << " ";
      }
      std::cout << "    ";
    }
    std::cout << "\n";
  }

  // Get epsilon closure of start state
  int start = initialstate-1;
  vector<int> Iclose = epsilonClosure(NFAstates, start, lastcolumn);
  std::cout << "E-Closure(I0) = {" << start+1;
  for(int i = 1; i < Iclose.size(); i++){
    std::cout << "," << Iclose.at(i);
  }
  std::cout << "}\n";

  int currentstate = 0;
  std::stack<vector<int>> inStack;
  std::vector<vector<int>> outList;

  inStack.push(Iclose);
  while(!inStack.empty()){
    std::cout << "Mark " << currentstate+1 << "\n";
    std::vector<int> currentVector = inStack.top();
    std::vector<int> tempVector = navigate(NFAstates, currentVector, 0);
    inStack.pop();


    




  }








}
