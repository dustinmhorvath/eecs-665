#include <iostream>
#include <string>
#include <fstream>
#include <functional> 
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm> 
#include <cctype>
#include <map>
#include <set>
#include <locale>
#include <stack>

struct Nfa{
  std::map<char, std::vector<int> > transitions;
  std::set<int> e_closure(int this_state, std::vector<Nfa> &states){
    std::stack<int> stack1;
    stack1.push(this_state);
    std::set<int> resultSet;
    resultSet.insert(this_state);
    while(!stack1.empty()) {
      int t = stack1.top();
      stack1.pop();
      for(int u : states[t].transitions['E']) {
        if(resultSet.find(u) == resultSet.end()) {
          resultSet.insert(u);
          stack1.push(u);
        }
      }
    }
    return resultSet;
  }
};

struct Dfa{   
  Dfa(std::set<int> NfaStates, std::vector<int> finalStates) {
    this->NfaStates = NfaStates;
    this->final = false;
    for(int i : finalStates) {
      for(int j : NfaStates) {
        if(i == j) {
          this->final = true;
        }
      }
    }
  }
  bool final;
  std::map<char, int> transitions;
  std::set<int> NfaStates;

  bool operator==(const Dfa &other) {
    return std::equal(this->NfaStates.begin(), this->NfaStates.end(), other.NfaStates.begin());
  }
};

std::set<int> move(Dfa &s, char c, std::vector<Nfa> &NfaStates) {
  std::set<int> returnValues;
  for(int i : s.NfaStates) {
    Nfa &temp = NfaStates[i];
    std::map<char, std::vector<int> >::iterator it = temp.transitions.find(c);
    if(it != temp.transitions.end()) {
      for(int s : it->second) {
        returnValues.insert(s);
      }
    }
  }
  return returnValues;
}

void printNFA(std::vector<Nfa> &s, std::vector<char> &a) {
  std::cout<<"State";
  for(int i = 0; i < a.size(); ++i) {
    std::cout<<'\t'<<a[i];
  }
  std::cout<<"\n";
  for(int i = 0; i < s.size(); ++i) {
    std::cout<<i+1<<'\t';
    for(char c: a) {
      std::cout<<"{";
      std::map<char, std::vector<int> >::iterator it = s[i].transitions.find(c);
      if(it != s[i].transitions.end()) {
        bool first = true;
        for(int i: it->second) {
          if(!first) {
            std::cout<<',';
          }
          std::cout<<i+1;
          first = false;
        }
      }
      std::cout<<"}"<<'\t';
    }
    std::cout<<"\n";
  }
}

void printOutput(std::vector<Dfa> &s, std::vector<char> &a) {
  std::cout<<"State";
  for(int i = 0; i < a.size() - 1; ++i) {
    std::cout<<'\t'<<a[i];
  }
  std::cout<<"\n";
  for(int i = 0; i < s.size(); ++i){
    std::cout<<i+1<<'\t';
    for(char c: a) {
      if(c == 'E') {
        continue;
      }
      std::cout<<"{";


      std::map<char, int>::iterator it = s[i].transitions.find(c);
      if(it != s[i].transitions.end()) {
        std::cout<<it->second;
      }
      std::cout<<"}"<<'\t';
    }

    std::cout<<"\n";
  }
}

void printSet(std::set<int> s) {
  std::cout<<"{";
  bool first = true;
  for( int i : s) {
    if(!first) {
      std::cout<<",";
    }
    std::cout<<i+1;
    first = false;
  }
  std::cout<<"}";
}

void printSet(std::vector<int> s) {
  std::cout<<"{";
  bool first = true;
  for( int i : s) {
    if(!first) {
      std::cout<<",";
    }
    std::cout<<i+1;
    first = false;
  }
  std::cout<<"}";
}

// trim from start
static std::string &ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}

// trim from end
static std::string &rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
        std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
  return s;
}

// trim from both ends
static std::string &trim(std::string &s) {
  return ltrim(rtrim(s));
}

static std::string getSubstring(std::string s, std::string firstCharacter, std::string secondCharacter) {
  std::string t_string = s;
  std::size_t leftBracketIndex = t_string.find(firstCharacter);
  std::size_t rightBracketIndex = t_string.find(secondCharacter);
  t_string = t_string.substr(leftBracketIndex + 1, rightBracketIndex - leftBracketIndex - 1);
  return trim(t_string);
}

static std::string getSubstring(std::string s, std::string firstCharacter) {
  std::string t_string = s;
  std::size_t leftBracketIndex = t_string.find(firstCharacter);
  std::size_t rightBracketIndex = s.length();
  t_string = t_string.substr(leftBracketIndex + 1, rightBracketIndex - leftBracketIndex - 1);
  return trim(t_string);
}

static std::vector<int> splitstr(std::string s, char character) {
  std::vector<int> iv;
  trim(s);
  std::stringstream ss(s);
  while(ss.good()) {
    std::string substring = "";
    getline(ss, substring, character);
    trim(substring);
    if(substring == "") {
    } else {
      int i = stoi(substring) - 1;
      iv.push_back(i);
    }
  }
  return iv;
}

int terminals(std::string const &str)
{
  std::stringstream stream(str);

  return std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
}

int main(int argc, char* argv[]) {


  std::cout << "Please provide an input file via stdin.\nExample: \"./Nfa2Dfa < input.txt\"\n\n";

  std::ifstream infile(argv[1]);


  std::string line;
  std::vector<std::string> fileLines;
  std::vector<char> symbolList;

  std::vector<Nfa> states;
  std::vector<Dfa> dStates;

  std::vector<int> finalStates;
  std::vector<int> d_finalStates;
  int n_terminals = 0;
  int n_states = 0;
  int initial_state = 0;





  while(std::getline(std::cin, line)) {
    fileLines.push_back(line);
  }


  // Read in the initial state
  std::string t_string = getSubstring(fileLines[0], "{", "}");
  initial_state = stoi(t_string);

  // Read in the final state
  t_string = getSubstring(fileLines[1], "{", "}");

  // Read in number of final states
  finalStates = splitstr(t_string, ',');

  //Get number of States
  n_states = stoi(getSubstring(fileLines[2], ":"));

  //Get number of terminals
  t_string = fileLines[3];
  t_string = t_string.substr(5, t_string.length());
  for(char c : t_string) {
    if(c != ' ' && c != '\t') {
      symbolList.push_back(c);
    }
  }
  n_terminals = terminals(t_string) - 1;

  // Resize states
  states.resize(n_states);

  for(int i = 4; i <= n_states + 3; ++i) {

    std::string first = fileLines[i];
    std::string string1;
    std::string string2;
    std::stringstream ss(first);


    ss >> string1;
    size_t size1;
    size_t size2;
    for(int k = 0; k <= n_terminals; ++k){

      ss >> string1;
      size1 = string1.find("{");
      size2 = string1.find("}");

      string2 = string1.substr(size1+1, size2 - size1 - 1);

      states[i-4].transitions[symbolList[k]] = splitstr(string2, ',');

    }
  }   


  std::cout<<"Initial State: {"<<initial_state<<"}\n";
  std::cout<<"Final States: ";
  printSet(finalStates);
  std::cout<<"\n";


  std::cout<<"Total States: "<<n_states<<"\n";
  printNFA(states,symbolList);
  std::cout<<"\n";


  std::cout<<"E-closure(I0) = ";

  std::set<int> resultSet;
  resultSet = states[initial_state - 1].e_closure(initial_state - 1, states);
  Dfa initial(resultSet, finalStates);
  dStates.push_back(initial);
  printSet(resultSet);
  for(char c : symbolList) {
    move(initial, c, states);
  }
  std::cout<<" = 1\n\n";
  for(int i = 0; i < dStates.size(); ++i) {
    char c;
    std::cout<<"Mark "<< i+1<<"\n";

    for(char c: symbolList) {
      if(c == 'E') {
        continue;
      }
      resultSet = move(dStates[i], c, states);
      if(resultSet.size() == 0) {
        continue;
      }
      printSet(dStates[i].NfaStates);
      std::cout<<" --"<<c<<"--> ";
      printSet(resultSet);

      std::set<int> closureCoords;
      for(int i : resultSet) {
        std::set<int> tmp = states[i].e_closure(i, states);
        closureCoords.insert(tmp.begin(), tmp.end());
      }


      std::cout<<"\nE-closure";
      printSet(resultSet);
      std::cout << " = ";
      printSet(closureCoords);

      int flag = -1;
      for(int k = 0; k < dStates.size(); ++k) {
        if(std::equal(closureCoords.begin(), closureCoords.end(), dStates[k].NfaStates.begin())) {
          flag = k;
        }
      }
      if(flag == -1) {

        Dfa newState(closureCoords, finalStates);
        dStates.push_back(newState);
        dStates[i].transitions[c] = dStates.size();
        std::cout<<" = "<< dStates.size();

        std::cout<<"\n";
      } 
      else {
        std::cout<<" = " << flag+1;
        dStates[i].transitions[c] = flag + 1;
        std::cout<<"\n";
      }
    }
    std::cout<<"\n";
  }
  for(int i = 0; i < dStates.size(); ++i) {
    if(dStates[i].final == true) {
      d_finalStates.push_back(i);
    }
  }
  std::cout<<"Initial State: {1}\n";

  std::cout<<"Final States: ";
  printSet(d_finalStates);
  std::cout<<"\n";
  printOutput(dStates,symbolList);


  return 0;
}
