#ifndef __BUILTIN_H__
#define __BUILTIN_H__

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

void cdFunc(std::vector<std::string> & args);
void expFunc(std::vector<std::string> & args, std::map<std::string, std::string> vars);

#endif

