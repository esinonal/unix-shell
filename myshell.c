#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
//#define _POSIX_SOURCE
#include <fcntl.h>
//#undef _POSIX_SOURCE
//-check all the error things work properly
//-get advanced redirection to work (read into buffer)
//-An empty command line. (and related things should run properly)
//- implement tabs in middle
//-tabs at end
void myPrint(char *msg)
{
  write(STDOUT_FILENO, msg, strlen(msg));
}
int checkIfShouldPrint(char* string){
  //if new line or actual content,  return 1
  //else if "" or "       " return 0.
  int length=strlen(string);
  int isValid=1;
  //printf("New word\n");
  for(int i=0; i<length; i++){
    if ((string[i] != ' ')&&(string[i] != '\n')&&(string[i] != '\t')){
      isValid=0;
      //printf("The char:  %c\n", string[i]);
      //int godddamnit= string[i];
      //printf("Char in ascii:   %d\n", godddamnit);
      //printf("isValid %d\n", isValid);
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
  //For all the spaces in between words, return a single string with only one space between words.
/*
  char * temp[600];
  temp[0] = malloc(600);
  temp[0] = strtok(*string, " ");
  int i=0;
  while (temp[i] != NULL) {
    i++;
    temp[i] = malloc(600);
    temp[i] = strtok(NULL, " ");
  }
  int length=i; //i= NULL.
  temp[length] = NULL;
  char* middleTrimmed=(char*)malloc(600);
  i=0;
  while(temp[i] != NULL) {
    strcat(middleTrimmed, temp[i]);
    strcat(middleTrimmed, " \0");
    temp[i] = strtok(NULL, " ");
    i++;
  }
  middleTrimmed[strlen(middleTrimmed)-1] = '\0';
  //myPrint("MIDDLE TRIMMED IS: ");
  //myPrint(middleTrimmed);
  *string=middleTrimmed;


*/
////____________________________________________
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
  //myPrint("MIDDLE TRIMMED IS: ");
  //myPrint(middleTrimmed);
  *string=middleTrimmed;
/*


  //myPrint("\n");
  char* ptr = strtok(*string, " ");  //First do SPACES
  char* middleTrimmed=(char*)malloc(513);


   //myPrint(*string);
   ///myPrint("\n");

   ptr = strtok(*string, "\t"); //Then, do TABS.
   middleTrimmed=(char*)malloc(513);
    while(ptr != NULL) {
      strcat(middleTrimmed, ptr);
      strcat(middleTrimmed, " \0");
      ptr = strtok(NULL, "\t");
    }
    middleTrimmed[strlen(middleTrimmed)-1] = '\0';
    *string=middleTrimmed;

    //myPrint(*string);
    //myPrint("\n");
    */
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
    //printf("Value of i: %d \n", i);
    //printf("Value of command i: %c \n", command[i]);
    if(command[i]=='>'){
      //printf("Value of command i inside IF statement.: %c \n", command[i]);
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
  //printf("%d\n", wasSuccessful);
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
    //printf("val of i: %d\n", i);
    //printf("command at i: %c\n", command[i]);
  }
  return command;
}
char* getRightOfRedir(char* command){
  //myPrint("Command is: ");
  //myPrint(command);
  char*ptr=command;
  int length= strlen(command);
  //myPrint("Length is: ");
  //printf("%d\n",length );
  int i=0;
  while((*ptr != '>')&&(i<length-1)){
    ptr++;
    i++;
    //printf("%c ", *ptr);
  }
  ptr++;
  //myPrint("Rightofredir in function(filename): ");
  //myPrint(ptr);
  //myPrint("command is: ");
  //myPrint(command);
  return ptr;
}
char* getRightOfRedirPlus(char* command){
  char*ptr=command;
  int length= strlen(command);
  int i=0;
  while((*ptr != '+')&&(i<length-1)){
    ptr++;
    i++;
    //printf("%c ", *ptr);
  }
  ptr++;
  return ptr;
}
int exists(char *fname)
{
    //Check if special case is happening. :
    int length= strlen(fname);
    if(length>=2){
      if((fname[0]=='.')&&(fname[1]=='/')){
        //we are cd ing into a folder so that folder most prob wont exist. ret 2
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
  int length= strlen(string); //copy over command to new spot in memory where it cant be munched by nasty strtok func.
  char* thestring= (char*)malloc(length*sizeof(char));
  for(int i=0; i<length; i++){
    thestring[i]= string[i]; //so now we've copied our values before they get destroyed by nasty other func.
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
  // char * args[]= {command, (char*)0};
  //myPrint(command);
  pid = fork();
  if (pid < 0){
    exit(0);
  }
  else if (pid == 0){ //Child.
    //for(int)
    if (execvp(*wordsInCommand,wordsInCommand) < 0){
    //Only comes here if exec fails (this program is replaced by what is in exec)
    //perror("exec");
      printErrorMessage();
      exit(0);
    }
  }
  else{
    wait(NULL);
  }
}
void executeIntoFile(char **wordsInRedirCommand, char*fileName){  //left,right
  //myPrint("\n");
  //myPrint(command); //print what command and filename are ???
  //myPrint("\n");
  //myPrint(fileName);
  int pid;
  //char * args[]= {command, (char*)0}; //FIX THIS COMMAND to be accessible to multiple words.
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
      //perror("exec");
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
  //char * args[]= {command, (char*)0};
  //1: read the initial file (so we SAVE our initial data.) with fread()
  FILE* storedElements=fopen("filetosaveourdatatoxxxxxxxxxxxxxxxxxxxxxxx.txt", "w");
  char arr[1];
  FILE * filePtr= fopen(fileName, "r");
  while( fread(&arr, sizeof(arr), 1, filePtr) == 1 ){
    fwrite(&arr, sizeof(arr), 1, storedElements);
  }
  //fread(buffer, 514, 1, filePtr);
  fclose(storedElements);
  fclose(filePtr);
  //2: Erase the file contents of file by opening with O_TRUNC.
  //Also use O_APPEND so it points to the end, so what gets entered is entered at end.
  int fileDescriptor = creat(fileName, S_IRUSR | S_IWUSR | O_APPEND| O_TRUNC);
  //printf("fileDescriptor: %d\n", fileDescriptor);
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
      //perror("exec");
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
  //myPrint("here3");
  char* pwdString="pwd";
  char* cdString="cd";
  int containsRedirection = checkIfContainsRedirection(command);
  int containsPlus = checkIfContainsPlus(command);
  int numRedir= countRedir(command);
  int numPlus=countPlus(command);
  //printf("Command: %s \n", command);
  //printf("containsRedirection %d \n", containsRedirection);
  //printf("containsPlus %d \n", containsPlus);
  //printf("numRedir %d \n", numRedir);
  //printf("numPlus %d \n", numPlus);
  //printf("length %d \n", length);
  //myPrint("2:the command is: ");
  //myPrint(command);

  if (length !=0 ){
  //printf("wordsInCommand[0] %s\n", wordsInCommand[0]);
  //printf("*wordsInCommand %s\n", *wordsInCommand);
  if ((length==1)&&(strcmp(pwdString,wordsInCommand[0])==0)){ //PWD command.
    //myPrint("here1");
    //printf("here?????1\n" );
    //char* pwd = getPWD();
    //myPrint("here5");
    char pwd[4100];
    myPrint(getcwd(pwd, 4100));
    myPrint("\n");
  }
  else if((length>1)&&(strcmp(pwdString,wordsInCommand[0])==0)){
    //printf("here iam!\n
    //myPrint("i should b here.");
    printErrorMessage();
    //myPrint("done...");
  }
  else if ((strcmp(cdString, *wordsInCommand)==0)&&((length==1)|(length==2))){ //CD commands. cd, or cd directory.
    //myPrint("here2");
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
    //Notes because its gotten messy: left is the command as a string.
    //However we also need WordsIncommand, the left of redir part of it, separately, to call the command.
    //Right is the file name as a string (single pointer), which is fine.
    //So keep everything as is, but just have a separate **pointer that gets the words in the command(left of >.)
    //myPrint("here3");
    //printf("command at redir: %s\n", command );
    //printf("this far \n");
    //myPrint("command: " );
    //myPrint(command);
    //for(int i=0; i<length; i++){
    //  myPrint("word: " );
    //  myPrint(wordsInCommand[i]);
    //}
    char* command1= "cd>output";
    char* command2= "cd >output";
    char* command3= "cd> output";
    char* command4= "cd > output";
    char* pwdComm= "pwd";
    char* cdComm= "cd";
    char* exitComm= "exit";
    //myPrint("COMMAND: ");
    //myPrint(command);
    int lengthofcomm= strlen(command);
    //printf("%d\n", lengthofcomm);
    char* thecommand= (char*)malloc(lengthofcomm*sizeof(char)); //Save it before it gets edited.
    int i=0;
    for(; i<lengthofcomm; i++){
      thecommand[i]= command[i];
      //myPrint("\n");
    }
    thecommand[i]='\0';
    //myPrint("THECOMMAND: ");
    //myPrint(thecommand);
    char* left=getLeftOfRedir(command);
    //myPrint("A");
    rightTrim(left);
    //myPrint("\nleft: ");
    //command=thecommand;//command has been edited so replace it.
    //myPrint(left);
    //myPrint("\n");
    //myPrint("B");
    //myPrint("Left is : ");
    //myPrint(left);
    //command=thecommand;//command has been edited so replace it.
    //myPrint("1:the command is: ");
    //myPrint(command);
    if((numRedir>1)|(numPlus>1)){ //we aren't allowed multiple of them
      printErrorMessage();
      //myPrint("C");
    }
    else if ((strcmp(command, command1)==0)| //yes
             (strcmp(command, command2)==0)| //NO: runs in other section.
             (strcmp(command, command3)==0)| //yes
             (strcmp(command, command4)==0)| //yes (length=3.)
             (strcmp(pwdComm, left)==0)| //yes
            (strcmp(cdComm, left)==0)| //cd one word; yes.
            (strcmp(exitComm, left)==0)){//yes
      //printf("here\n" );
      //printf("%s\n", left);
      printErrorMessage();
      //myPrint("D");
    }
    else{
      command=thecommand;//Reclean because you lost it. Why? Idk.
      thecommand= (char*)malloc(lengthofcomm*sizeof(char)); //if lose again can reuse this.
      i=0;
      for(; i<lengthofcomm; i++){
        thecommand[i]= command[i];
        //myPrint("\n");
      }
      thecommand[i]='\0';
      //myPrint("1.5:the command is: ");
    //  myPrint(command);
      //
      //
      /*
      myPrint("here1");
      int i=0;
      while ( strcmp(wordsInCommand[i], ">")!=0 ) {
        i++;
      }
      wordsInCommand[i]=NULL;
      myPrint("here2");
      */
      //char** wordsInLeft=NULL;
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
        //myPrint("E");
      }
      else if(containsPlus==0){// Simple redirection. >
        //myPrint("In redirection! ");
        //myPrint(command);
        //myPrint("here3");
        char* right=getRightOfRedir(command);
        //myPrint("here4");
        leftTrim(&right);
        //myPrint("\nright: ");
        //command=thecommand;//command has been edited so replace it.
        //myPrint(right);
        //myPrint("\n");
        //myPrint("3:the command is: ");
        //myPrint(command);
        //myPrint("File name is: ");
        //myPrint(right);
        int fileExists= exists(right);
        //printf("%d\n", fileExists);
        //myPrint("4:the command is: ");
        //myPrint(command);
        if (fileExists == 1){ //If file exists, error.
          //myPrint("File exists!");
          printErrorMessage();
          //myPrint("G");
        }
        else if(fileExists==2){//file does not exist, but invalid promt.
          printErrorMessage();
        }
        else { //File does not exist yet. Its a valid prompt.
          //myPrint("H");
          //myPrint("File does not exist.");
          /////XXXXX First have to check if the folder exists of not.
          if ((strcmp(left, "")!=0)&&(strcmp(right, "")!=0)){
            executeIntoFile(wordsInLeft, right);
          }
          else{//We have an empty command so error.
            printErrorMessage();
          }
          //This is edited wordsInCommand(shortened!) //XXXXXXXXBut also we never execute into the file.
          //myPrint("I");
        }
      }
      else if(containsPlus==1){// Advanced redirection. >+
        char* right=getRightOfRedirPlus(command);
        //myPrint("J");
        leftTrim(&right);
        //myPrint("K");
        command=thecommand;//command has been edited so replace it.
        int fileExists= exists(right);
        if (fileExists == 1){//If the file exists, then we have to do the ADVANCED redirection.
          if ((strcmp(left, "")!=0)&&(strcmp(right, "")!=0)){
            executeIntoFileAppend(wordsInLeft,right);//This is edited wordsInCommand(shortened!)
          }
          else{//We have an empty command so error.
            printErrorMessage();
          }



          //myPrint("L");
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

          //myPrint("M");
        }
      }
    }
  }
  else{   //Do an exec general call.
    //myPrint("In general exec call!");
    //myPrint("\nENERAL EXEC\n");
    execute(wordsInCommand);
    //myPrint("P");
  }
  }
}
int checkIfOkay(char* command){
  //myPrint(command);
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
  //myPrint(&command[0]);
  //myPrint(&command[1]);
  //myPrint(&command[2]);
  //myPrint(&command[3]);
  //myPrint(&command[4]);
  return 1;//okay.
}
void cleanCommand(char* command){
  //myPrint("\nCommand at cleancommand: ");
  //myPrint(command);
  //myPrint(command);//2
  if (strcmp(command, "")!=0){ //If not empty.
    //myPrint(command);//3
    rightTrim(command);
    //myPrint(command);//4
    //myPrint("\nAfter right trim: ");
    //myPrint(command);
    leftTrim(&command);
    //myPrint(command);//5
    //myPrint("\nAfter left trim: ");
    //myPrint(command);
    middleTrim(&command);
    //myPrint(command);//6







    int isOkay=checkIfOkay(command);
    if (isOkay==1){

    //printf("cleanCommand: command%s\n", command);
    /*
    int length=0;
    int i=0;
    while(command[i] != '\0'){
      length++;
      i++;
    }
    char* thecommand= (char*)malloc(length*sizeof(char));
    for(int i=0; i<length; i++){
      thecommand[i]= command[i]; //so now we've copied our values before they get destroyed by nasty other func.
    }
    thecommand[i]= '\0';
    //printf("cleanCommand: thecommand%s\n", thecommand);
    */

    int lengthofcomm= strlen(command);
    char* thecommand= (char*)malloc(lengthofcomm*sizeof(char)); //if lose again can reuse this.
    int i=0;
    for(; i<lengthofcomm; i++){
      thecommand[i]= command[i];
      //myPrint("\n");
    }
    thecommand[i]='\0';
    //myPrint(command);//7
    //myPrint(thecommand);//7
    //myPrint("\n\n\n\n\n ");

    i=0;

    char * wordsInCommand[600];
    memset(wordsInCommand, 600, 600*sizeof(wordsInCommand[0]));
    wordsInCommand[0] = malloc(600);
    wordsInCommand[0] = strtok(command, " ");
    //myPrint(wordsInCommand[0]);//7
    //myPrint("A");
    while (wordsInCommand[i] != NULL) {
      i++;

      wordsInCommand[i] = malloc(600);
      wordsInCommand[i] = strtok(NULL, " ");

    }
    int length=i; //i= NULL.
    wordsInCommand[length] = NULL;
    //myPrint("B");
    //myPrint(wordsInCommand[0]);//7
    //myPrint(wordsInCommand[2]);//7
  //  if (strcmp("exit argv2 argv3",thecommand)!=0){ //OPTION1:we exit.

    runSingleCommand(thecommand, wordsInCommand, length);
    }


    else{

      //myPrint("im here....");
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
      //myPrint(input);
      char *exitstring = "exit";
      //myPrint("here1");

      if (strcmp(exitstring,input)==0){ //OPTION1:we exit.
        exit(0);
      }
      else if ((orinigalInputLength>513)) { //OPTION2:is wrong size.
        //myPrint(originalInput);
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
          //myPrint("goes directly/");
          //myPrint(input);//1
          if (strcmp("exit>",input)!=0){////XXXXXXXXXXXXX
          cleanCommand(input);
          }
          else{
            printErrorMessage();
          }
        }
        else if(containsSemiColon==1){ //Evidently this isnt working so replace by strgtok.
          /*
          //if at least one ; happens.
          char* currentCommand=(char*)malloc(600);
          int currentCommIndex=0;
          for(int i=0; i<trimmedLength; i++){
            if(input[i]==';'){//start new comm
              if((i==trimmedLength-1)|(input[i+1]==';')){
                continue;
              }
              else{
                currentCommand[i]='\0';
                cleanCommand(currentCommand);
                currentCommIndex=0;
              }
            }
            else{
              currentCommand[currentCommIndex]=input[i];
              currentCommIndex++;
            }
          }
          //one last command to execute.
          currentCommand[currentCommIndex]='\0';
          cleanCommand(currentCommand);



//////////////
char * temp[600];
temp[0] = malloc(600);
temp[0] = strtok(*string, " ");
int i=0;
while (temp[i] != NULL) {
  i++;
  temp[i] = malloc(600);
  temp[i] = strtok(NULL, " ");
}
int length=i; //i= NULL.
temp[length] = NULL;










          */
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
            //printf("i is: %d\n", i);
            //printf("%s\n", commandCleanedOfSemiColon[i]);
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
      //myPrint("got line!\n");
      int shouldPrint=checkIfShouldPrint(line);
      //myPrint("\n");
      //myPrint("this is my line:");
      //myPrint(line);
      //printf("This is my shouldPrint: %d\n", shouldPrint);
      if (shouldPrint==1){
        myPrint(line);
      }
      doEverything(line);
      //myPrint("getting next line!");
    //  myPrint("\nhjkhkjh!!!!!");
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
