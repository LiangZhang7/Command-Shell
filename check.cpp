#include "check.h"

/*Check the command existence in the specific directory*/
int existCheck(std::string arg) {
    size_t pos = arg.find_last_of('/');
    std::string dir = arg.substr(0, pos);
    std::string cmd = arg.substr(pos+1);
    DIR * dirptr;
    struct dirent * entry;
    if((dirptr = opendir(dir.c_str())) == NULL){
      perror("opendir");
      exit(EXIT_FAILURE);
    }
    while((entry = readdir(dirptr)) != NULL){
      if(entry->d_name == cmd) {
        closedir(dirptr);
        return 1;
      }
    }
    closedir(dirptr);
    return 0;
}

/*Update the command with its full path and check its existence*/
int cmdCheck(std::string & cmd, std::vector<std::string> & args) {
  if(cmd.find('/') == std::string::npos) {
    cmd = findDir(args);/*Update the full path of the command*/
  }
  if(cmd.find('/') != std::string::npos) {
    /*check whether the command exists in the specific directory*/
    if(existCheck(cmd) == 0) {
      std::cout<<"Command "<<cmd<<" not found"<<std::endl;
      return 0;
    }
  }
  else {
    return 0;
  }
  return 1;
}

/*Check whether the string is valid for a variable name*/
int validCheck(std::string var) {
  for(size_t i = 0; i < var.size(); ++i) {
    if(!isdigit(var[i]) && !isalpha(var[i]) && var[i] != '_') {
      return 0;
    }
  }
  return 1;
}

