#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <bits/stdc++.h>
#include <iostream>
#include <sys/types.h>
#include <stdlib.h>
#include <filesystem>
using namespace std;

bool exists_test1 (std::string name);
bool exists_test2 (std::string name);
bool exists_test3 (std::string name);
bool exists_test0 (std::string name);

int main(int argc, char** argv) {
  std::string name = "seed";
  cout << exists_test0(name);
  cout << exists_test1(name);
  cout << exists_test2(name);
  cout << exists_test3(name);
  if(exists_test0(name)){
    cout << "exist0"<<endl;
  }
  if(exists_test1(name)){
    cout << "exist1"<<endl;
  }
  if(exists_test2(name)){
    cout << "exist2"<<endl;
  }
  if(exists_test3(name)){
    cout << "exist3"<<endl;
  }

}

bool exists_test0 (std::string name) {
    ifstream f(name.c_str());
    return f.good();
}

bool exists_test1 (std::string name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

bool exists_test2 (std::string name) {
    return ( access( name.c_str(), F_OK ) != -1 );
}

bool exists_test3 (std::string name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

