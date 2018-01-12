#ifndef __PARSE_H__
#define __PARSE_H__

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

void readInput(std::string str, std::vector<std::string> & args);
std::string replaceVar(std::string str, std::map<std::string, std::string> myMap);
std::string findDir(std::vector<std::string> & args);
std::string getValue(std::string key, std::string input);

#endif

