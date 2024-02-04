package main

import (
	"bufio"
	"fmt"
	"os"
	"os/exec"
	"strings"
)

func main() {
	// String holding the user input from the command line.
	var command string
	// String array holding parsed PATH.
	directories := strings.Split(os.Getenv("PATH"), ":")
	// String array argv holding command line arguments.
	var myargv []string

	// Print prompt.
	fmt.Print("$ ")

	// Create new buffered reader to read input
	reader := bufio.NewReader(os.Stdin)

	// Get input from standard input and store it in command.
	for {
		// Read input from the user.
		line, err := reader.ReadString('\n')
		if err != nil {
			fmt.Errorf("reader.ReadString error")
			break
		}

		// Strip null terminator if existent
		command = strings.TrimSpace(line)

		// Exit if the command string is "exit".
		if command == "exit" {
			break
		}

		// Parse the command string and separate all flags.
		myargv = strings.Fields(command)

		// Check if the command string starts with "." or "/"
		if command[0] == '/' || command[0] == '.' {
			fmt.Printf("I AM INSIDE THE FIRST IF STATEMENT")
			// Check if the command string is executable.
			_, err := exec.LookPath(command)
			// If command string is not executable, print a message and try again.
			if err != nil {
				fmt.Printf("unable to locate executable '%s'\n", command)
			} else { // If Executable is found, run the executable.
				// Execute the command.
				cmd := exec.Command(myargv[0], myargv[1:]...)
				cmd.Stdout = os.Stdout
				cmd.Stderr = os.Stderr
				err := cmd.Run()
				// If the executable fails, print error.
				if err != nil {
					fmt.Printf("execve() failed: %v\n", err)
				}
			}
		} else {
			// Loop over the directories.
			var ok bool
			for _, dir := range directories {
				// Construct the full path to the command.
				// basically, we are trying to see if the binary
				// exists in any of the dirs in PATH
				temp := dir + "/" + myargv[0]
				// Check if the concatenated command is executable.
				_, err := exec.LookPath(temp)
				// Stop looking if we find an executable.
				if err == nil {
					ok = true
					break
				}
			}

			// If no executable is found, print a message and try again.
			if !ok {
				fmt.Printf("unable to locate executable '%s'\n", command)
			} else { // If Executable is found, run the executable.
				// Execute the command.
				cmd := exec.Command(myargv[0], myargv[1:]...)
				cmd.Stdout = os.Stdout
				cmd.Stderr = os.Stderr
				err := cmd.Run()
				// If the executable fails, print error.
				if err != nil {
					fmt.Printf("execve() failed: %v\n", err)
				}
			}
		}

		// Print prompt.
		fmt.Print("$ ")
	}
}
