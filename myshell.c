#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fcntl.h>

void myPrint(char *msg)
{
  write(STDOUT_FILENO, msg, strlen(msg));
}
int checkIfShouldPrint(char* string){

  int length=strlen(string);
  int isValid=1;
  
  for(int i=0; i<length; i++){
    if ((string[i] != ' ')&&(string[i] != '\n')&&(string[i] != '\t')){
      isValid=0;
    }
  }
  if (isValid==1){
    return 0;
  }
  else if (strcmp(string, "")==0){
    return 0;
  }
  else{
    return 1;
  }
}
void leftTrim(char** string){
  char* ptr=*string;
  while((*ptr==' ')|(*ptr=='\t')){
     ptr++;
  }
  *string=ptr;
}
void rightTrim(char * string){
  char *endOfString;
  int length= strlen(string);
  int done= 0;
  endOfString= &string[length-1]; //point end of string to end of string.
  //Then going backwards, if space char or \n char, that spot becomes a \c location.
  for(int i=length-1; i>=0; i--){
    if ((string[i]!=' ')&&(string[i]!= '\n')){
      done=1;
    }
    if ((done==0) && ((string[i]== ' ')|(string[i]== '\n')|(string[i]== '\t'))){
      *endOfString='\0';
    }
    endOfString--;
  }
}
void middleTrim(char** string){
  char * temp[600];
  temp[0] = malloc(600);
  temp[0] = strtok(*string, "\t");
  int i=0;
  while (temp[i] != NULL) {
    i++;
    temp[i] = malloc(600);
    temp[i] = strtok(NULL, "\t");
  }
  int length=i; //i= NULL.
  temp[length] = NULL;
  char* middleTrimmed=(char*)malloc(600);
  i=0;
  while(temp[i] != NULL) {
    strcat(middleTrimmed, temp[i]);
    strcat(middleTrimmed, " \0");
    temp[i] = strtok(NULL, "\t");
    i++;
  }
  middleTrimmed[strlen(middleTrimmed)-1] = '\0';
  *string=middleTrimmed;
}
void printEnter(char *msg)
{
  char* enter="\n";
  write(STDOUT_FILENO, msg, strlen(msg));
  write(STDOUT_FILENO, enter, strlen(enter));
}
void printErrorMessage(){
  char error_message[30] = "An error has occurred\n";
  write(STDOUT_FILENO, error_message, strlen(error_message));
}
int checkIfContainsRedirection(char* command){
  int length=strlen(command);
  for (int i=0; i<length;i++){
    if(command[i]=='>'){
      return 1;
    }
  }
  return 0;
}
int checkIfContainsPlus(char* command){
  int length=strlen(command);
  for (int i=0; i<length;i++){
    if(command[i]=='+'){
      return 1;
    }
  }
  return 0;
}
int countRedir(char* command){
  int counter=0;
  int length=strlen(command);
  for (int i=0; i<length;i++){
    if(command[i]=='>'){
      counter++;
    }
  }
  return counter;
}
int countPlus(char* command){
  int counter=0;
  int length=strlen(command);
  for (int i=0; i<length;i++){
    if(command[i]=='+'){
      counter++;
    }
  }
  return counter;
}
int toCD(){
  char* cd = getenv ("HOME");
  int wasSuccessful=chdir(cd); //On success, zero is returned.
  //On error, -1 is returned, and errno is set appropriately.
  return wasSuccessful;
}
int toCDDIRECTORY(char* directory){
  int wasSuccessful=chdir(directory); //On success, zero is returned.
  //On error, -1 is returned, and errno is set appropriately.
  return wasSuccessful;
}
char* getLeftOfRedir(char* command){
  char*ptr=command;
  while(*ptr != '>'){
    ptr++;
  }
  *ptr='\0';
  int length= strlen(command);
  for (int i=0; i<length; i++){
  }
  return command;
}
char* getRightOfRedir(char* command){
  char*ptr=command;
  int length= strlen(command);
  int i=0;
  while((*ptr != '>')&&(i<length-1)){
    ptr++;
    i++;
  }
  ptr++;
  return ptr;
}
char* getRightOfRedirPlus(char* command){
  char*ptr=command;
  int length= strlen(command);
  int i=0;
  while((*ptr != '+')&&(i<length-1)){
    ptr++;
    i++;
  }
  ptr++;
  return ptr;
}
int exists(char *fname)
{
    int length= strlen(fname);
    if(length>=2){
      if((fname[0]=='.')&&(fname[1]=='/')){
        return 2;
      }
    }

    FILE *file;
    if ((file = fopen(fname, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}
char ** getWordsInString(char*string){
  int length= strlen(string); //copy over command to new spot in memory
  char* thestring= (char*)malloc(length*sizeof(char));
  for(int i=0; i<length; i++){
    thestring[i]= string[i];
  }
  int i=0;
  static char * wordsInString[600];
  wordsInString[0] = strtok(string, " ");
  while (wordsInString[i] != NULL) {
    i++;
    wordsInString[i] = strtok(NULL, " ");
  }
  string= thestring; //revert our old one back.
  return wordsInString;
}
void execute( char** wordsInCommand){
  int pid;
  pid = fork();
  if (pid < 0){
    exit(0);
  }
  else if (pid == 0){ //Child.
    if (execvp(*wordsInCommand,wordsInCommand) < 0){
    //Only comes here if exec fails (this program is replaced by what is in exec)
      printErrorMessage();
      exit(0);
    }
  }
  else{
    wait(NULL);
  }
}
void executeIntoFile(char **wordsInRedirCommand, char*fileName){  //left,right
  int pid;
  //Create file.(outside of child, as has to be accessible outside of child too)
  int fileDescriptor = creat(fileName, S_IRUSR | S_IWUSR);
  if(fileDescriptor != -1){ //Do this only if the file pointer is valid.
    pid = fork();
    if (pid < 0){
      exit(1);
    }
    else if (pid == 0){ //Child.
      //Rewrite the file pointers with dup().(only do this inside child.)
      dup2(fileDescriptor,1);
      if (execvp(*wordsInRedirCommand,wordsInRedirCommand) < 0){
      //Only comes here if exec fails (this program is replaced by what is in exec)
        printErrorMessage();
        exit(0);
      }
    }
    else{
      wait(NULL);
      close(fileDescriptor); //Close the file descriptor, as currently its no longer needed.
    }
  }
}
void executeIntoFileAppend(char **wordsInRedirCommand, char*fileName){
  int pid;
  //1: read the initial file (so we SAVE our initial data.) with fread()
  FILE* storedElements=fopen("filetosaveourdatatoxxxxxxxxxxxxxxxxxxxxxxx.txt", "w");
  char arr[1];
  FILE * filePtr= fopen(fileName, "r");
  while( fread(&arr, sizeof(arr), 1, filePtr) == 1 ){
    fwrite(&arr, sizeof(arr), 1, storedElements);
  }
  fclose(storedElements);
  fclose(filePtr);
  //2: Erase the file contents of file by opening with O_TRUNC.
  //Also use O_APPEND so it points to the end, so what gets entered is entered at end.
  int fileDescriptor = creat(fileName, S_IRUSR | S_IWUSR | O_APPEND| O_TRUNC);
  if(fileDescriptor != -1){ //Do this only if the file pointer is valid.
    pid = fork();
    if (pid < 0){
      exit(1);
    }
    else if (pid == 0){ //Child.
      //3: execute into the file.
      dup2(fileDescriptor,1);
      if (execvp(*wordsInRedirCommand,wordsInRedirCommand) < 0){
      //Only comes here if exec fails (this program is replaced by what is in exec)
        printErrorMessage();
        exit(0);
      }
    }
    else{
      wait(NULL);
      //4: write the previously saved data to where the buffer currently is in the file.
      FILE* filePtr= fopen(fileName, "a");
      FILE* storedElements=fopen("filetosaveourdatatoxxxxxxxxxxxxxxxxxxxxxxx.txt", "r");
      while( fread(&arr, sizeof(arr), 1, storedElements) == 1 ){
        fwrite(&arr, sizeof(arr), 1, filePtr);
      }
      fclose(storedElements);
      fclose(filePtr);
      close(fileDescriptor); //Close the file descriptor, as currently its no longer needed.
      remove("filetosaveourdatatoxxxxxxxxxxxxxxxxxxxxxxx.txt");
    }
  }
}
void runSingleCommand(char* command, char ** wordsInCommand, int length){
  char* pwdString="pwd";
  char* cdString="cd";
  int containsRedirection = checkIfContainsRedirection(command);
  int containsPlus = checkIfContainsPlus(command);
  int numRedir= countRedir(command);
  int numPlus=countPlus(command);
  if (length !=0 ){
  if ((length==1)&&(strcmp(pwdString,wordsInCommand[0])==0)){ //PWD command.
    char pwd[4100];
    myPrint(getcwd(pwd, 4100));
    myPrint("\n");
  }
  else if((length>1)&&(strcmp(pwdString,wordsInCommand[0])==0)){
    printErrorMessage();
  }
  else if ((strcmp(cdString, *wordsInCommand)==0)&&((length==1)|(length==2))){ //CD commands. cd, or cd directory.
    if (length == 1){//only one word, we have cd.
      int wasSuccessful= toCD();
      if (wasSuccessful==-1){
        printErrorMessage();
      }
    }
    else if((length == 2)&&(containsRedirection == 0)){ //cd directory. We can only have one word for directory.
      int wasSuccessful= toCDDIRECTORY(wordsInCommand[1]);
      if (wasSuccessful==-1){
        printErrorMessage();
      }
    }
    else if((length == 2)&&(containsRedirection == 1)){
      printErrorMessage();
    }
  }
  else if((length>1)&&(strcmp("exit",wordsInCommand[0])==0)){ //exit.
    printErrorMessage();
  }
  else if (containsRedirection==1){ //REDIRECTION command.
    char* command1= "cd>output";
    char* command2= "cd >output";
    char* command3= "cd> output";
    char* command4= "cd > output";
    char* pwdComm= "pwd";
    char* cdComm= "cd";
    char* exitComm= "exit";
    int lengthofcomm= strlen(command);
    char* thecommand= (char*)malloc(lengthofcomm*sizeof(char)); //Save it before it gets edited.
    int i=0;
    for(; i<lengthofcomm; i++){
      thecommand[i]= command[i];
    }
    thecommand[i]='\0';
    char* left=getLeftOfRedir(command);
    rightTrim(left);
    if((numRedir>1)|(numPlus>1)){ //we aren't allowed multiple of them
      printErrorMessage();
    }
    else if ((strcmp(command, command1)==0)| //yes
             (strcmp(command, command2)==0)| //NO: runs in other section.
             (strcmp(command, command3)==0)| //yes
             (strcmp(command, command4)==0)| //yes (length=3.)
             (strcmp(pwdComm, left)==0)| //yes
            (strcmp(cdComm, left)==0)| //cd one word; yes.
            (strcmp(exitComm, left)==0)){//yes
      printErrorMessage();
    }
    else{
      command=thecommand;
      thecommand= (char*)malloc(lengthofcomm*sizeof(char)); 
      i=0;
      for(; i<lengthofcomm; i++){
        thecommand[i]= command[i];
      }
      thecommand[i]='\0';
      char * wordsInLeft[600];
      wordsInLeft[0] = malloc(600);
      wordsInLeft[0] = strtok(left, " ");
      int i=0;
      while (wordsInLeft[i] != NULL) {
        i++;
        wordsInLeft[i] = malloc(600);
        wordsInLeft[i] = strtok(NULL, " ");
      }
      length=i; //i= NULL.
      wordsInLeft[length] = NULL;
      if ((strcmp(cdString, *wordsInCommand)==0)&&(length>=2)){//cd xx > x.txt. (bad).
        printErrorMessage();
      }
      else if(containsPlus==0){// Simple redirection. >
        char* right=getRightOfRedir(command);
        leftTrim(&right);
        int fileExists= exists(right);
        if (fileExists == 1){ //If file exists, error.
          printErrorMessage();
        }
        else if(fileExists==2){//file does not exist, but invalid promt.
          printErrorMessage();
        }
        else { //File does not exist yet. Its a valid prompt.
          if ((strcmp(left, "")!=0)&&(strcmp(right, "")!=0)){
            executeIntoFile(wordsInLeft, right);
          }
          else{//We have an empty command so error.
            printErrorMessage();
          }
        }
      }
      else if(containsPlus==1){// Advanced redirection. >+
        char* right=getRightOfRedirPlus(command);
        leftTrim(&right);
        command=thecommand;//command has been edited so replace it.
        int fileExists= exists(right);
        if (fileExists == 1){//If the file exists, then we have to do the ADVANCED redirection.
          if ((strcmp(left, "")!=0)&&(strcmp(right, "")!=0)){
            executeIntoFileAppend(wordsInLeft,right);//This is edited wordsInCommand(shortened!)
          }
          else{//We have an empty command so error.
            printErrorMessage();
          }
        }
        else if(fileExists==2){ //false command.(folder that doesnt exist. )
          printErrorMessage();
        }
        else if (fileExists ==0){//If the file doesn't exist, then we can do redirection (create file...etc.)
          if ((strcmp(left, "")!=0)&&(strcmp(right, "")!=0)){
              executeIntoFile(wordsInLeft, right);
          }
          else{//We have an empty command so error.
            printErrorMessage();
          }
        }
      }
    }
  }
  else{   //Do an exec general call.
    execute(wordsInCommand);
  }
  }
}
int checkIfOkay(char* command){
  int length= strlen(command);
  int isOkay0=1;
  int isOkay1=1;
  int isOkay2=1;
  int isOkay3=1;
  int isOkay4=1;
  if(length>=5){
    if(command[0]=='e'){
      isOkay0=0;
    }
    if(command[1]=='x'){
      isOkay1=0;
    }
    if(command[2]=='i'){
      isOkay2=0;
    }
    if(command[3]=='t'){
      isOkay3=0;
    }
    if(command[4]==' '){
      isOkay4=0;
    }
  }
  if ((isOkay0==0)&&(isOkay1==0)&&(isOkay2==0)&&(isOkay3==0)&&(isOkay4==0)){
    return 0;//not okay.
  }
  return 1;//okay.
}
void cleanCommand(char* command){
  if (strcmp(command, "")!=0){ //If not empty.
    rightTrim(command);
    leftTrim(&command);
    middleTrim(&command);
    int isOkay=checkIfOkay(command);
    if (isOkay==1){
    int lengthofcomm= strlen(command);
    char* thecommand= (char*)malloc(lengthofcomm*sizeof(char)); //if lose again can reuse this.
    int i=0;
    for(; i<lengthofcomm; i++){
      thecommand[i]= command[i];
    }
    thecommand[i]='\0';
    
    i=0;
    char * wordsInCommand[600];
    memset(wordsInCommand, 600, 600*sizeof(wordsInCommand[0]));
    wordsInCommand[0] = malloc(600);
    wordsInCommand[0] = strtok(command, " ");
    while (wordsInCommand[i] != NULL) {
      i++;
      wordsInCommand[i] = malloc(600);
      wordsInCommand[i] = strtok(NULL, " ");
    }
    int length=i; //i= NULL.
    wordsInCommand[length] = NULL;
    runSingleCommand(thecommand, wordsInCommand, length);
    }
    else{
      printErrorMessage();
    }
  }
}

void doEverything(char* originalInput){
    //  myPrint("Command at original input:");
      //myPrint(originalInput);
      int orinigalInputLength= strlen(originalInput); //includes spaces and newline char.
      char* input= originalInput; //edit only a new var.
      rightTrim(input);
      leftTrim(&input);
      char *exitstring = "exit";

      if (strcmp(exitstring,input)==0){ //OPTION1:we exit.
        exit(0);
      }
      else if ((orinigalInputLength>513)) { //OPTION2:is wrong size.
        printErrorMessage();
      }
      else{ // OPTION3: is correct sized input,will run in some sort of way.
        //keep running through the commands on the list onebyone.
        int containsSemiColon=0;
        int trimmedLength= strlen(input);
        for(int i=0; i<trimmedLength; i++){
          if (input[i]==';'){
            containsSemiColon=1;
          }
        }
        //if no ";"happens ever.
        if (containsSemiColon==0){ //this means there is just ONE job.
          if (strcmp("exit>",input)!=0){////XXXXXXXXXXXXX
          cleanCommand(input);
          }
          else{
            printErrorMessage();
          }
        }
        else if(containsSemiColon==1){ //Evidently this isnt working so replace by strgtok.
          int i=0;
          char * commandCleanedOfSemiColon[600];
          commandCleanedOfSemiColon[0] = malloc(600);
          commandCleanedOfSemiColon[0] = strtok(input, ";");
          while (commandCleanedOfSemiColon[i] != NULL) {
            i++;
            commandCleanedOfSemiColon[i] = malloc(600);
            commandCleanedOfSemiColon[i] = strtok(NULL, ";");
          }
          int length=i; //i= NULL.
          commandCleanedOfSemiColon[length] = NULL;
          i=0;
          while(commandCleanedOfSemiColon[i] != NULL){
            cleanCommand(commandCleanedOfSemiColon[i]);
            i++;
          }
      }
  }
}
void runInInteractive(){
  char cmd_buff[1600];
  char *originalInput;
  while (1) {
    myPrint("myshell> ");
    originalInput = fgets(cmd_buff, 1600, stdin);
    if (!originalInput) {
        exit(0);
    }
    doEverything(originalInput);
  }
}
void runinBatch(char * fileName){
  int fileExists= exists(fileName);
  if (fileExists== 0){//file doesn't exist.
    printErrorMessage();
    exit(1);
  }
  else{//file does exist.
    FILE* filePtr= fopen(fileName, "r");
    char line[1600];
    while(fgets(line,sizeof(line), filePtr)!= NULL){
      int shouldPrint=checkIfShouldPrint(line);
      if (shouldPrint==1){
        myPrint(line);
      }
      doEverything(line);
    }
  }
}
int main(int argc, char *argv[])
{
  if (argc==1){
    //printf("here1\n" );
    runInInteractive();
  }
  else if (argc==2){
    runinBatch(argv[1]);
  }
  else{
    printErrorMessage();
    exit(1);
  }
}
