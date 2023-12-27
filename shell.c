#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

// declaring functions
char** get_input();
int execute_cmd(char**);


int main(int argc, char* argv[])
{
    char **input;
    int status;
    char path[256];

    do
    {
        // get current working directory for prompt
        getcwd(path, 256);

        // continuously print prompt with current directory
        printf("%s $: ", path);

        // getting string from user and splitting into tokens
        input = get_input();

        // passing the split tokens to execute command
        status = execute_cmd(input);

        // freeing memory used by malloc for token array
        free(input);


    } while (status);

    return EXIT_SUCCESS;
}



char** get_input()
{
    // char to store user input
    char input[256];
    
    // get input from the user
    fgets(input, 256, stdin);

    int index = 0;

    // delimter for 'space', 'tab', 'return', and 'new line'
    const char delimeter[] = " \t\r\n";

    char *token;

    // declaring array to store tokens
    char** listOfTokens = malloc(256 * sizeof(input));

    // splitting the input by the delimeter defined earlier
    token = strtok(input, delimeter);

    while (token != NULL)
    {

        // append the token to the array
        listOfTokens[index] = token;


        // increment the index counter by one
        index++;

        // move to next token
        token = strtok(NULL, delimeter);
    }

    // setting the last value in the array to NULL 
    listOfTokens[index] = NULL;

    return listOfTokens;   
}


int execute_cmd(char** args)
{

    // if no input is passed we return 1 and prompt for input again
    if(args[0] == NULL)
    {
        return 1;
    }

    // if user types 'exit' we return 0 ane exit the shell
    if(strcmp(args[0], "exit") == 0)
    {
        return 0;
    }

    // checking if a user types 'cd'
    if(strcmp(args[0], "cd") == 0)
    {
        // passing the directory in args[1] to chdir()
        chdir(args[1]);
        // prompt for input again and don't exit
        return 1;
    }

    // calling fork funtion
    pid_t pid = fork();


    // output error is system call returns 0 or a negative number
    if (pid < 0)
    {
        perror("Error forking");
        return EXIT_FAILURE;
    }

    // child process
    else if (pid == 0)
    {
        // passing execvp the command arg to be executed
        if (execvp(args[0], args) == -1)
        {
            perror("Error executing command");
        }

        exit(EXIT_FAILURE);
    }

    else
    {
        // used to store the state of the process
        int status;

        // parent process waits for child before exiting
        if (waitpid(pid, &status, 0) < 0)
        {
            perror("Error waitpid");
            return EXIT_FAILURE;
        }
    }
    // when the process completes with no erroes 1 is returned
    return 1;
}
