#include <cstdio>
#include <cstdlib>
#include <limits.h>  //PATH_MAX
#include <unistd.h>  //dup2(), pipe()
#include <iostream>
#include <string>
#include <vector>
#include <string.h>  //strcmp()
#include <sys/types.h>  //open()
#include <dirent.h>  //opendir(), readdir(), closedir()
#include <sys/wait.h>  //waitpid()
#include <sys/stat.h>  //open()
#include <fcntl.h> //open()
#include <map>
#include <ctype.h> //isdigit(), isalpha

#include "parse.h"
#include "check.h"
#include "execution.h"
#include "builtin.h"

#define TRUE 1

/*Print a prompt while waiting for input*/
void inputPrompt() {
  char currPath[PATH_MAX];
  if(getcwd(currPath, PATH_MAX) == NULL) {
    perror("getcwd");
    exit(EXIT_FAILURE);
  }
  std::cout<<"myShell:"<<currPath<<" $ ";
}

int main(int argc, char * argv[], char ** envptr) {
  /*Ceate a map for shell variables*/
  std::map<std::string, std::string> vars;
  /*Put the environment variables into the map*/
  std::string total, key, value;
  for(int e = 0; envptr[e] != NULL; ++e) {
    total = envptr[e];
    size_t found = total.find('=');
    key = total.substr(0, found);
    value = total.substr(found+1);
    vars.insert(std::pair<std::string, std::string>(key,value));
  }
  while(TRUE) {
    inputPrompt();/*Print a prompt while waiting for input*/
    std::vector<std::string> inputArgs;
    std::string input;
    getline(std::cin, input);
    /*Exit the shell when EOF is encountered*/
    if(!std::cin.good()) {
      std::cout<<"EOF is encountered reading from stdin"<<std::endl;
      return EXIT_SUCCESS;
    }
    readInput(input, inputArgs);/*Read input command and arguments and put them in a vector*/
    if(inputArgs.empty()) {
      continue;
    }
    /*Update variables with their values*/
    for(size_t i = 0; i < inputArgs.size(); ++i) {
      inputArgs[i] = replaceVar(inputArgs[i], vars);
    }
    /*Exit the shell when user types "exit"*/
    if(inputArgs[0] == "exit") {
      return EXIT_SUCCESS;
    }
    /*"set var value" command*/
    if(inputArgs[0] == "set") {
      if(inputArgs.size() < 2) {
        std::cerr<<"Missing variable name for 'set'"<<std::endl;
        continue;
      }
      /*Check whether the variable name is valid*/
      if(validCheck(inputArgs[1]) == 0) {
        std::cerr<<"Invalid variable name: "<<inputArgs[1]<<std::endl;
        continue;
      }
      std::string sKey = inputArgs[1];
      std::string sValue = getValue(inputArgs[1], input);
      std::map<std::string, std::string>::iterator sIt = vars.find(sKey);
      if(sIt != vars.end()) {
        vars.erase(sIt);//If the key exists in the map, erase it
      }
      vars.insert(std::pair<std::string, std::string>(sKey, sValue));
      continue;
    }
    /*Execute other commands*/
    execute(inputArgs, vars);
  }
  return EXIT_SUCCESS;
}

