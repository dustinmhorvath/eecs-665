/*
 * Dustin Horvath
 * nfa2dfa
 *
 * EECS 665 Compilers
 * Project 1
 *
 * Takes an nfa table as input and produces a simplified dfa rule list and
 * state table as output. See file 'input.txt' for an example input format.

 *
 */


#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>
#include <iomanip>

using namespace std;

#define MAXSYMBOLS 26
#define MAXSIZE MAXSYMBOLS+1
#define MAXDFASTATES 30


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

// Takes an NFA table, start state, and last column, and returns the epsilon
// closure of that particular pseudo-state. Uses episolonClosureHelper for
// recursive calls
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

// Returns a list of possible output states for a particular input state,
// given a symbol on which to move.
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

// Returns true if two vectors are equivalent. Does not check for
// combinations, so take care to sort first.
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

// Returns true if a particular vector is in an array of vectors.
bool checkVectorArray(std::vector<vector<int>> q, std::vector<int> v){
  for(int i = 0; i < q.size(); i++){
    if(compareIntVector(q.at(i), v)){
      return true;
    }

  }
  return false;


}

// Returns the index of a particular vector in a list of vectors.
int getMark(std::vector<vector<int>> q, std::vector<int> v){
  for(int i = 0; i < q.size(); i++){
    if(compareIntVector(q.at(i), v)){
      return i;
    }

  }
  return -1;

}

// Returns true if a particular value is in a vector<int>
bool contains(vector<int> v, int state){
  for(int i = 0; i < v.size(); i++){
    if(v.at(i) == state){
      return true;
    }
  }
  return false;
}

// Returns true if ANY value in a vector is in another vector.
bool checkVectorElementInVector(vector<int> container, vector<int> in){
  for(int i = 0; i < in.size(); i++){
    if(contains(container, in.at(i))){
      return true;
    }
  }
  return false;
}

int main( int argc, char *argv[] ){
  if(argc<2){
    std::cout << "Please provide an input file via the command line.\nExample: \"./nfa2dfa input.txt\"\n";
    exit(0);
  }

  std::ifstream infile(argv[1]);
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
    finalstates.push_back(finalstate-1);

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

  // Parse table of inputs, tokenize on whitespace with stringstream
  std::string word;
  int row = 0;
  int lastcolumn = 0;
  while (std::getline(infile, line)){
    std::stringstream ss(line);
    // Consume row number
    ss >> word;

    // Tokenize states by parsing between commas
    int column = 0;
    while(ss >> word){
      // Discard everything outside curly braces
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
  // Needs decremented, because it gets incremented one extra time.
  lastcolumn--;

  /* // Print the read-in values for testing
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

  int currentstate = 0;

  // INQUEUE: stores the list of states that still need to be described, using
  // a BFS mannerism.
  std::queue<vector<int>> inQueue;
  // OUTLIST: list of all the states that have been evaluated so far. This
  // list must be consulted before pushing any new states onto inQueue, in
  // order to ensure uniqueness
  std::vector<vector<int>> outList;
  // DFATABLE: stores the actual, new DFA transition table in its simplified
  // form. lastcolumn is the number of symbols.
  int DFAtable[MAXDFASTATES][lastcolumn];

  // Holds the list of new final states for the DFA
  std::vector<int> newFinals;

  // Push initial closure onto our working queue
  inQueue.push(Iclose);
  outList.push_back(Iclose);

  while(!inQueue.empty()){
    // Pull a new working vector off the queue
    std::vector<int> currentVector = inQueue.front();
    inQueue.pop();

    bool printedMark = false;
    // Perform this block for every possible input symbol
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
          int mark = getMark(outList, tempVector)+1;
          std::cout << "} = " << mark << "\n";

          DFAtable[currentstate][symbol] = mark;


        }
      }
      else{
        // Use zero so we don't have null values in table
        DFAtable[currentstate][symbol] = 0;
      }

    }

    // Check if our current state contains a final state
    std::cout << "\n";
    if(checkVectorElementInVector(finalstates, currentVector)){
      newFinals.push_back(getMark(outList, currentVector));
    }

    currentstate++;
  }

  if(newFinals.empty()){
    newFinals.push_back(0);
  }

  // Print nice table at end
  std::cout << "\nInitial State: {" << initialstate << "}\n";
  std::cout << "Final States: {" << newFinals.at(0)+1;
  for(int i = 1; i < newFinals.size(); i++){
    std::cout << "," << newFinals.at(i)+1;
  }
  std::cout << "}\n";

  // Print column headers
  int width = 8;
  std::cout << std::setw(width) << left << "State";
  for(int i = 0; i < lastcolumn; i++){
    std::cout << std::setw(width) << left << (char)((int)'a' + i);
  }
  std::cout << "\n";

  // Print the block table
  for(int row = 0; row < outList.size(); row++){
    std::cout << std::setw(width) << left << row+1;

    for(int column = 0; column < lastcolumn; column++){
      if(DFAtable[row][column] != 0){
        std::stringstream concat;
        concat << "{" << DFAtable[row][column] << "}";
        std::cout << std::setw(width) << left << concat.str();
      }
      else{
        std::cout << std::setw(width) << left << "{}";
      }
    }


    std::cout << "\n";
  }

  std::cout << "\n";
}
