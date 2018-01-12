#include "builtin.h"

/*cd function*/
void cdFunc(std::vector<std::string> & args) {
  if(args.size() == 1) {
    if(chdir("/home/lz139") == -1) {
      perror("chdir");
    }
  }
  else if(chdir(args[1].c_str()) == -1) {
    perror("chdir");
  }
}

/*export variables fuction*/
void expFunc(std::vector<std::string> & args, std::map<std::string, std::string> vars) {
  if(args.size() < 2) {
    std::cerr<<"Wrong argument numbers for export command"<<std::endl;
    return;
  }
  std::map<std::string, std::string>::iterator it = vars.find(args[1]);
  if(it == vars.end()) {
    std::cerr<<"variable "<<args[1]<<" not exists"<<std::endl;
    return;
  }
  if(setenv(args[1].c_str(), (it->second).c_str(), 1) == -1) {
    perror("setenv");
  }
}

