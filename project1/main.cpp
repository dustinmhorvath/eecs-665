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
  
  std::sort(closure.begin(), closure.end());
  return closure;
}

vector<int> epsilonClosure(std::vector<int> states[][MAXSIZE], vector<int> tocheck, int epsilonCol){
  vector<int> closure = tocheck;
  for(int vectorindex = 0; vectorindex < tocheck.size(); vectorindex++){
    for(int i = 0; i < states[tocheck.at(vectorindex)][epsilonCol].size(); i++){
      if(!states[tocheck.at(vectorindex)][epsilonCol].empty()){
        closure.push_back(states[tocheck.at(vectorindex)][epsilonCol].at(i));
        vector<int> recurseClose = epsilonClosureHelper(states, states[tocheck.at(vectorindex)][epsilonCol].at(i), epsilonCol);
        closure.insert(closure.end(), recurseClose.begin(), recurseClose.end());
      }
    }
  }

  std::sort(closure.begin(), closure.end());
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

  std::sort(destinations.begin(), destinations.end());
  return destinations;
}

bool compareIntVector(vector<int> a, vector<int> b){
  bool equals = true;
  if(a.size() != b.size()){
    return false;
  }
  else{
    for(int i = 0; i < a.size(); i++){
      if(a.at(i) != b.at(i)){
        equals = false;
      }
    }
  }
  
  return equals;
}

bool checkVectorArray(std::vector<vector<int>> q, std::vector<int> v){
  for(int i = 0; i < q.size(); i++){
    if(compareIntVector(q.at(i), v)){
      return true;
    }

  }
  return false;


}

int getMark(std::vector<vector<int>> q, std::vector<int> v){
  for(int i = 0; i < q.size(); i++){
    if(compareIntVector(q.at(i), v)){
      return i;
    }

  }
  return -1;

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
  std::vector<int> finalstates;
  std::getline(infile, line);
  first = line.find('{');
  last = line.find('}');
  std::string tempString = line.substr(first + 1, last-first-1);
  std::stringstream commaparse(tempString);
  int finalstate;
  while(commaparse >> finalstate){
    finalstates.push_back(finalstate);
    
    if (commaparse.peek() == ','){
      commaparse.ignore();
    }
  }


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
  vector<int> start = {initialstate-1};
  vector<int> Iclose = epsilonClosure(NFAstates, start, lastcolumn);
  std::cout << "\nE-Closure(I0) = {" << initialstate;
  for(int i = 1; i < Iclose.size(); i++){
    std::cout << "," << Iclose.at(i)+1;
  }
  std::cout << "} = 1\n";
  //std::cout << printVector(Iclose) << "\n";

  int currentstate = 0;
  std::queue<vector<int>> inQueue;
  std::vector<vector<int>> outList;

  // Push it onto our working queue
  inQueue.push(Iclose);
  outList.push_back(Iclose);


  while(!inQueue.empty()){
    std::vector<int> currentVector = inQueue.front();
    //outList.push_back(currentVector);
    inQueue.pop();

      bool printedMark = false;
      for(int symbol = 0; symbol < lastcolumn; symbol++){
        // Check where we can go from currentVector on 'symbol'
        std::vector<int> tempVector = navigate(NFAstates, currentVector, symbol);

        // Don't print anything if there are no destinations for a symbol
        if(!tempVector.empty()){

            if(!printedMark){
            std::cout << "\nMark " << currentstate+1 << "\n";
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
            std::cout << tempVector.at(0)+1;
            for(int i = 1; i < tempVector.size(); i++){
            std::cout << "," << tempVector.at(i)+1;
            }
            std::cout << "}\n";

            std::cout << "E-closure{";
            std::cout << tempVector.at(0)+1;
            for(int i = 1; i < tempVector.size(); i++){
              std::cout << "," << tempVector.at(i)+1;
            }
            std::cout << "} = {";
            tempVector = epsilonClosure(NFAstates, tempVector, lastcolumn);
            if(!tempVector.empty()){
              // Push the new state onto the queue if we haven't been there already
              if(!checkVectorArray( outList, tempVector )){
                inQueue.push(tempVector);
                outList.push_back(tempVector);
              }
              std::cout << tempVector.at(0)+1;
              for(int i = 1; i < tempVector.size(); i++){
                std::cout << "," << tempVector.at(i)+1;
              }
              std::cout << "} = " << getMark(outList, tempVector)+1 << "\n";

              
            }
        }
      
    }


    currentstate++;
  }

  std::cout << "\nInitial State: {" << initialstate << "}\n";
  std::cout << "Final States: {" << finalstates.at(0);
  for(int i = 1; i < finalstates.size(); i++){
    std::cout << "," << finalstates.at(i)+1;
  }
  std::cout << "}\n";




}
