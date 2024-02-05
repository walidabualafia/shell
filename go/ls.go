/*
 * ls.go (C) 2024
 *
 *
 * author: Walid Abu Al-Afia
 *
 * description: space and time efficient implementation of good ole' ls.
 *
 */

package main

import (
	"fmt"
	"os"
	"sort"
	"strings"
)

const (
	BLUE  = "\x1b[1m\033[34m"
	CYAN  = "\x1b[1m\033[36m"
	GREEN = "\x1b[1m\033[32m"
	WHITE = "\x1b[37m"
)

/*
 * HELPER FUNCTIONS
 */
func compareNames(str1, str2 string) bool {
	return strings.Compare(str1, str2) < 0
}

/*
 * Usage - this function prints the proper usage instructions to the user.
 *
 * parameters: None.
 */
func usage() {
	fmt.Fprintf(os.Stderr, "\nUsage: %s [OPTION]... [DIR]...\n", os.Args[0])
	fmt.Fprintf(os.Stderr, "List current working directory contents.\n")
	fmt.Fprintf(os.Stderr, "\nOptions:\n")
	fmt.Fprintf(os.Stderr, "\t-R\tlist subdirectories recursively\n\n")
}

func simple(path string) {
	// Get the entries in the CWD
	entries, err := os.ReadDir(path)
	if err != nil {
		fmt.Printf("Error reading dir %s: %v\n", path, err)
		return
	}

	// Sort the entries alpha-numerically
	sort.Slice(entries, func(i, j int) bool {
		return compareNames(entries[i].Name(), entries[j].Name())
	})

	for _, entry := range entries {
		// Skip dotfiles
		if strings.HasPrefix(entry.Name(), ".") {
			continue
		}

		fileInfo, err := os.Stat(path + "/" + entry.Name())
		if err != nil {
			fmt.Println("Error:", err)
			return
		}

		if entry.IsDir() {
			// Print blue for directories
			fmt.Printf(BLUE+"%s  "+WHITE, entry.Name())
		} else if entry.Type() == os.ModeSymlink {
			// Print cyan for symbolic links
			fmt.Printf(CYAN+"%s  "+WHITE, entry.Name())
		} else if fileInfo.Mode()&0111 != 0 {
			// Print green for executables
			fmt.Printf(GREEN+"%s  "+WHITE, entry.Name())
		} else {
			// Print white for files
			fmt.Printf("%s  ", entry.Name())
		}
	}
	fmt.Println()

}

func main() {
	switch len(os.Args) {
	case 1:
		cwd, err := os.Getwd()
		if err != nil {
			fmt.Println("Error getting CWD:", err)
			os.Exit(1)
		}
		simple(cwd)
	case 2:
		simple(os.Args[1])
	default:
		usage()
	}
}
