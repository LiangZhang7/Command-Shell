#include "execution.h"

/*Implement input redirection (<), output redirection (>), and error output redirection (2>)*/
void redirection(std::string fname, int tagRed, std::vector<std::string> & args, size_t & i) {
  int fd;
  mode_t f_attrib = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
  switch(tagRed) {
  case 0://input redirection
    if((fd = open(fname.c_str(), O_RDONLY)) == -1) {
      std::cerr<<"input redirection failed"<<std::endl;
    }
    break;
  case 1://output redirection
    if((fd = open(fname.c_str(), O_CREAT | O_RDWR | O_TRUNC, f_attrib)) == -1) {
      std::cerr<<"output redirection failed"<<std::endl;
    }
    break;
  case 2://error output redirection
    if((fd = open(fname.c_str(), O_CREAT | O_RDWR | O_APPEND, f_attrib)) == -1) {
      std::cerr<<"error output redirection failed"<<std::endl;
    }
    break;
  default:
    break;
  }
  /*Change the current process's file descriptor 0 from stdin to the file descriptor of the input file following the "<" redirection sign.*/
  /*Or change the current process's file descriptor 1 from stdout to the file descriptor of the output file following the ">" redirection sign.*/
  /*Or change the current process's file descriptor 2 from stderr to the file descriptor of the output file following the "2>" redirection sign.*/
  dup2(fd, tagRed);
  close(fd);
  /*After redirect the command's input, delete "<" and input file name from the vector*/
  /*Or after redirect the command's output, delete ">" and output file name from the vector*/
  /*Or after redirect the command's error output, delete "2>" and output file name from the vector*/
  args.erase(args.begin()+i,args.begin()+i+2);
  --i;
}

/*Execute one single command seperated by '|'*/
void perExe(std::vector<std::string> args, size_t limit) {
  /*Change arguments from string to char ** to do the execve*/
  char ** lArgs = new char * [limit+1];
  for(size_t j = 0; j < limit; ++j) {
    lArgs[j] = new char[args[j].size()+1];
    strcpy(lArgs[j], args[j].c_str());
  }
  lArgs[limit] = NULL;
  execve(lArgs[0], lArgs, environ);
  for(size_t k = 0; k < limit; ++k) {
    delete [] lArgs[k];
  }
  delete [] lArgs;
}

/*Execute the command*/
void execute(std::vector<std::string> & args, std::map<std::string, std::string> vars) {
  /*Builtin commands*/
  if(!strcmp(args[0].c_str(), "cd")) {
    cdFunc(args);
    return;
  }
  if(!strcmp(args[0].c_str(), "export")) {
    expFunc(args, vars);
    return;
  }
  /*Check whether the command is executable*/
  int cmdStat = cmdCheck(args[0], args);
  if(cmdStat == 0) {
    return;
  }
  else {
  /*Execute the command using multi processes*/
  /*And dealing with pipes and redirection*/
  /*Man page waitpid example as reference*/
    pid_t cpid, wpid;
    int status;
    cpid = fork();
    if(cpid == -1) {
      perror("fork");
      exit(EXIT_FAILURE);
    }
    if(cpid == 0) {/*Execute child process*/
      for(size_t i = 0; i < args.size(); ++i) {
        /*Pipes and redirection inspection*/
        if(!strcmp(args[i].c_str(), "<")) {
          redirection(args[i+1], 0, args, i);
          continue;
        }
        if(!strcmp(args[i].c_str(), ">")) {
          redirection(args[i+1], 1, args, i);
          continue;
        }
        if(!strcmp(args[i].c_str(), "2>")) {
          redirection(args[i+1], 2, args, i);
          continue;
        }
        if(!strcmp(args[i].c_str(), "|")) {
          /*Do the pipe by order*/
          int pfd[2], status;
          if(pipe(pfd) < 0) {//Open the pipe
            perror("pipe");
            continue;
          }
          else {
            pid_t ccpid = fork();/*Creat the second child process to execute the command before the pipe sign*/
            if(ccpid == -1) {
              perror("2nd fork");
              exit(EXIT_FAILURE);
            }
            if(ccpid == 0) {/*Execute the 2nd child process*/
              close(pfd[0]);/*Close the read port of the pipe*/
              dup2(pfd[1], 1);/*Change the current process's file descriptor 1 from stdout to the write port of the pipe*/
              close(pfd[1]);
              /*Execute the command before '|'*/
              perExe(args, i);
            }
            else {/*Return to the 1st child process*/
              if(waitpid(ccpid, &status, WUNTRACED | WCONTINUED) == -1) {
                perror("pipe waitpid");
                exit(EXIT_FAILURE);
              }
              close(pfd[1]);/*Close the write port of the pipe*/
              dup2(pfd[0], 0);/*Change the current process's file descriptor 0 from stdin to the read port of the pipe*/
              close(pfd[0]);
              /*After finishing the pipe, delete the arguments string before the pipe sign from the vector*/
              args.erase(args.begin(), args.begin()+i+1);
              /*Check whether the next command is executable*/
              int cmdStat = cmdCheck(args[0],args);
              if(cmdStat == 0) {
                return;
              }
              else {
                i = 0;
                continue;
              }
            }
          }
        }
      }
      /*After finishing all the commands before the last pipe, execute the last commnad*/
      perExe(args, args.size());
    }
    else {/*Execute the parent process*/
      do {
        wpid = waitpid(cpid, &status, WUNTRACED | WCONTINUED);
        if (wpid == -1) {
          perror("waitpid");
          exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status)) {
          std::cout<<"Program exited with status "<<WEXITSTATUS(status)<<std::endl;
        }
        else if (WIFSIGNALED(status)) {
          std::cout<<"Program was killed by signal "<<WTERMSIG(status)<<std::endl;
        }
        else if (WIFSTOPPED(status)) {
          std::cout<<"Program was stopped by signal "<<WSTOPSIG(status)<<std::endl;
        }
        else if (WIFCONTINUED(status)) {
          std::cout<<"Program continued"<<std::endl;
        }
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
  }
}

