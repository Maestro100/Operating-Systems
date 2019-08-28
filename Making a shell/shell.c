#include  <stdio.h>

#include  <sys/types.h>

#include<unistd.h>

#include <string.h>

void parse(char * line, char ** argv) {
  while ( * line != '\0') {
    /* if not the end of line ....... */
    while ( * line == ' ' || * line == '\t' || * line == '\n')
      *
      line++ = '\0'; /* replace white spaces with 0    */
    * argv++ = line; /* save the argument position     */
    while ( * line != '\0' && * line != ' ' &&
      *
      line != '\t' && * line != '\n')
      line++; /* skip the argument until ...    */
  }
  * argv = '\0'; /* mark the end of argument list  */
}

char root_dir[1024];
char cur_dir[1024];
char tmp_dir[1024];
void execute(char ** argv) {
  pid_t pid;
  int status;

  if ((pid = fork()) < 0) {
    /* fork a child process           */
    printf("*** ERROR: forking child process failed\n");
    exit(1);
  } else if (pid == 0) {
    /* for the child process:         */
    if (execvp( * argv, argv) < 0) {
      /* execute the command  */
      printf("*** ERROR: exec failed\n");
      exit(1);
    }
  } else {
    /* for the parent:      */
    while (wait( & status) != pid) /* wait for completion  */
    ;
  }
}

void docd(char ** argv) {
  pid_t pid;
  int status;
  char s[100];
  if ((pid = fork()) < 0) {
    /* fork a child process           */
    printf("*** ERROR: forking child process failed\n");
    exit(1);
  } else if (pid == 0) {
    /* for the child process:         */
    chdir(argv[1]);
    getcwd(s, 100);
    if (strlen(s) < strlen(root_dir))
      chdir(root_dir);
  } else {
    /* for the parent:      */
    while (wait( & status) != pid) /* wait for completion  */
    ;
  }
}
int cur_h = 0;
char history[35][1024];

void print_history() {
  // return;
  pid_t pid;
  int status;
  char s[100];
  if ((pid = fork()) < 0) {
    /* fork a child process           */
    printf("*** ERROR: forking child process failed\n");
    exit(1);
  } else if (pid == 0) {
    /* for the child process:         */
    int i = 0;
    // printf("%d afhi\n",i);
    while (i < cur_h) {
      printf("%d %s\n", i, history[i]);
      i++;
    }
  } else {
    /* for the parent:      */
    while (wait( & status) != pid) /* wait for completion  */
    ;
  }
}
void main(void) {

  getcwd(root_dir, 100);
  cur_dir[0] = '/';
  cur_dir[1] = '\0';
  char line[1024]; /* the input line                 */
  char * argv[64]; /* the command line argument      */
  printf("%s\n",root_dir);
  while (1) {
    /* repeat until done ....         */
    printf("MTL458(%s)> ", cur_dir); /*   display a prompt             */
    gets(line); /*   read in the command line     */
    // printf("%s\n",line );
    if (strcmp(line, "\0") == 0) continue;
    strcpy(history[cur_h++], line);
    parse(line, argv); /*   parse the line               */
    if (strcmp(argv[0], "exit") == 0) /* is it an "exit"?     */
      exit(0); /*   exit if it is                */
    else if (strcmp(argv[0], "cd") == 0)
      docd(argv);
    else if (strcmp(argv[0], "history") == 0) {
      print_history(history);
    } else execute(argv); /* otherwise, execute the command */
    getcwd(tmp_dir, 100);
    int ln = strlen(tmp_dir);
    int ln1 = strlen(root_dir);
    int j = 1;
    cur_dir[0] = '/';
    for (int i = ln1+1; i <= ln; i++) {
      cur_dir[j++] = tmp_dir[i];
    }
  }
}