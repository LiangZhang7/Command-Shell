#ifndef __EXECUTION_H__
#define __EXECUTION_H__

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

#include "check.h"
#include "builtin.h"

extern char ** environ;/*User environment*/

void redirection(std::string fname, int tagRed);
void perExe(std::vector<std::string> args, size_t limit);
void execute(std::vector<std::string> & args, std::map<std::string, std::string> vars);

#endif

