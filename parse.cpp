#include "parse.h"

/*Extract command and arguments from input into the vector*/
void readInput(std::string str, std::vector<std::string> & args) {
  std::string temp;
  for(size_t curr = 0; curr < str.length(); ++curr) {
    /*Check any white space and other characters following a \ and put it into temp*/
    if(curr < str.length() - 1 && str.at(curr) == '\\') {
      temp.append(1, str.at(curr+1));
      ++curr;
      if(curr == str.length() - 1) {
        args.push_back(temp);
      }
    }
    /*Remove the white space and push temp into vector*/
    else if(str.at(curr) == ' ') {
      if(!temp.empty()) {
        args.push_back(temp);
        temp.clear();
      }
    }
    /*Put the characters into temp, also push the last argument into vector*/
    else {
      temp.append(1, str.at(curr));
      if(curr == str.length() - 1) {
        args.push_back(temp);
      }
    }
  }
}

/*Replace the variables with their values in the arguments*/
std::string replaceVar(std::string str, std::map<std::string, std::string> myMap) {
  //Seperate the string by characters which are not letters, underscores or numbers, and put the results into a vector
  std::vector<std::string> vec;
  std::string::iterator it = str.begin();
  while(it != str.end()) {
    if(!isalpha(*it) && !isdigit(*it) && *it != '_') {
      vec.push_back(str.substr(0, it-str.begin()));
      str.erase(str.begin(), it);
      it = str.begin()+1;
    }
    else {
      ++it;
    }
  }
  vec.push_back(str);
  //Replace the varialbe with its value, then put the string together again
  std::string ans;
  for(size_t i = 0; i < vec.size(); ++i) {
    if(*(vec[i].begin()) == '$' && vec[i].size() > 1) {
      vec[i].erase(vec[i].begin());
      std::map<std::string, std::string>::iterator mit = myMap.find(vec[i]);
      if(mit != myMap.end()) {
        vec[i] = mit->second;
      }
      else {
        std::cerr<<"variable: "<<vec[i]<<" not found"<<std::endl;
        break;
      }
    }
    ans = ans + vec[i];
  }
  return ans;
}

/*Search the PATH environment variable for commands*/
std::string findDir(std::vector<std::string> & args) {
  /*Put the environment paths seperated by colon into a vector*/
  std::string pathList = getenv("PATH");
  std::vector<std::string> envPaths;
  size_t begin = 0;
  size_t end = 0;
  do {
    if((begin = pathList.find_first_not_of(':', end)) == std::string::npos) {
      std::cout<<"environment PATH is empty\n"<<std::endl;
    }
    if((end = pathList.find_first_of(':', begin)) == std::string::npos) {
      envPaths.push_back(pathList.substr(begin, end));
    }
    else {
      envPaths.push_back(pathList.substr(begin, end-begin));
    }
  } while(end != std::string::npos);
  /*Search all the paths in the vector for the command*/
  for (size_t i = 0; i < envPaths.size(); ++i) {
    DIR * dirptr;
    struct dirent * entry;
    if(envPaths[i].empty()) {
      continue;
    }
    if((dirptr = opendir(envPaths[i].c_str())) == NULL){
      perror("opendir");
      exit(EXIT_FAILURE);
    }
    while((entry = readdir(dirptr)) != NULL){
      /*If the command is found under the path, update the command with its full path*/
      if(entry->d_name == args[0]) {
        args[0] = envPaths[i] + "/" + args[0];
        closedir(dirptr);
        return args[0];
      }
    }
    closedir(dirptr);
  }
  std::cout<<"Command "<<args[0]<<" not found"<<std::endl;
  return args[0];
}

/*Get the value for the set command*/
/*Remove "set" and "key" from input to get the "value"*/
std::string getValue(std::string key, std::string input) {
  std::string strLeft = input;
  strLeft.erase(0, strLeft.find("set")+3);//remove set
  strLeft.erase(0, strLeft.find(key)+key.length());//remove key
  strLeft.erase(0, strLeft.find_first_not_of(' '));//remove space
  return strLeft;
}

