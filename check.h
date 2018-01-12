#ifndef __CHECK_H__
#define __CHECK_H__

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

int existCheck(std::string arg);
int cmdCheck(std::string & cmd, std::vector<std::string> & args);
int validCheck(std::string var);

#endif
