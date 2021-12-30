#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[], char *envp[]) {
	// vars
	// String holding the user input from the command line.
	char command[1024];
	// String array holding parsed PATH.
	char *directories[1024];
	// int count of the number of directories in PATH.
	int pathcnt = 0;
	// String array argv holding command line arguments.
	char *myargv[1024];
	// int count of command line arguments.
	int argcnt = 0;
	// String pointer token used to parse strings with strtok().
	char *token;

	// String holding PATH returned from getenv().	
	char *path;
	path = strdup(getenv("PATH"));

	// Separate path into tokens by delimiter ":".
	token = strtok(path, ":");

	int i = 0;
	// Copy each individual path into directories.
	while (token != NULL) {
		directories[i] = token; 
		pathcnt++;
		i++;
		// Read the next path into token.
		token = strtok(NULL, ":");
	}		

	// Print prompt.
	printf("$ ");

	// Get input from standard input and store it in command.
	while(fgets(command, 1024, stdin) != NULL) {
		// Exit if there is an EOF.
		if (feof(stdin)) {
			break;
		}

		// Replace the newline at the end of the string with a null char.
		command[strlen(command)-1] = '\0';
		
		// Exit if the command string is "exit".
		if(strcmp(command, "exit") == 0) {
			break;
		}

		// Parse the command string and separate all flags.
		token = strtok(command, " ");

		i = 0;
		argcnt = 0;
		// Copy all command line arguments into the list of string arguments.
		while (token != NULL) {
			myargv[i] = token;
			argcnt++;
			i++;
			// Read the next path into token.
			token = strtok(NULL, " ");
		}
	
		// Check if the command string starts with "." or "/"
		if (command[0] == '/' || command[0] == '.') {
			// Check if command string is executable.
			int ok = access(command, X_OK);
			// If command string is not executable print a message and try again.
			if (ok != 0) {
				printf("unable to locate executable '%s'\n", command);
			} else {    // If Executable is found, run the executable in a child process.
				// Copy full path to command into argv[0].
				strcpy(myargv[0], command);		
				// Child process id.
				pid_t cpid;
				// Fork the process.
				cpid = fork();
				// If the fork succeeds.
				// CHILD PROCESS
				if (cpid == 0) {
					// Execute the executable.
					int success = execve(myargv[0], myargv, envp);
					// If the executable fails, print error. 
					if (success == -1) {
						perror("execve() failed");
					}
				} else if (cpid < 0) {    // If the call to fork() fails, print error.
					perror("fork() failed");
				} else {    // PARENT PROCESS
					wait(NULL);
				}
			}

		} else {
			// Initialize temporary string to hold the concatenated path.
			char temp[1024];
			// Variable used to check if concatenated command is an executable.
			int ok;

			i = 0;
			// Loop over the directories.
			while (i < pathcnt) {
				// Copy path into temp.
				strcpy(temp, directories[i]);
				// Concatenate a forward slash.
				strcat(temp, "/");
				// Conctenate the command.
				strcat(temp, command);
				// Check if concatednated command is executable.
				ok = access(temp, X_OK);
				// Stop looking if we find an executable.
				if (ok == 0) {
					// Variable to hold size of executable.
					struct stat st;
					// Handle errors.
					if (stat(temp, &st) != 0) {
						perror("incorrect file size.\n");
					}
					// If we did find an executable, break out.
					break;
				}	
				i++;
			}		
			// If no executable is found print message and try again.
			if (ok != 0) {
				printf("unable to locate executable '%s'\n", temp);
			} else {    // If Executable is found, run the executable in a child process.  
				// Set the first element of argv to be the full path to the command.
				myargv[0] = temp;
				// Child process id.
				pid_t cpid;
				// Fork the process. 
				cpid = fork();
				// If the fork succeeds.
				// CHILD PROCESS
				if (cpid == 0) {
					// Execute the executable.
					int success = execve(temp, myargv, envp);
					// If the executable fails, print error. 
					if (success == -1) {
						perror("execve() failed");
					}
				} else if (cpid < 0) {    // If the fork fails, print error.
					perror("fork() failed");
				} else {    // PARENT PROCESS
					wait(NULL);
				}
			}
		}
		// Reset vals in myargv.
		memset(myargv, 0, sizeof(myargv));
		// Print prompt.
		printf("$ ");
	}
	free(path);
	return(0);
}
