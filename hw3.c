// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "shell.h"

#define DEBUG 0

// -----------------------------------
// Main function 
// Arguments:	argc = number of arguments suppled by user
//				argv = array of argument values
//
//
int main( int argc, char** argv ) {

	// ----------------------------------
	// Pseudocode
	// ----------------------------------
	// while ( !done )
	//		print the prompt (i.e. >, #, or $ ... your choice)
	//		read command line
	//		parse the command line
    //      if (parsed command is "exit")
    //      	done = true
	//  	else if ( is _builtin( ... ) )
	//			do_builtin( ... )
	//		else if ( find_fullpath( ... ) )
	//  		execute( ... )
	//		else
	//			error()
	// 		cleanup( ... )
	//
	int done = 0, i = 0, status;
	char line[256], path[256];
	command_t currentcmd;
	while(!done){
		printf(">");
		fgets(line, 256, stdin);
		// gets(line);
		if (DEBUG){
			printf("read in line: '%s'\n", line);
		}
		if (line[0] != '\n'){
			parse(line, &currentcmd);
		
			if (samestr(currentcmd.name, "exit")) {
				done = 1;
			}
			else if (is_builtin(&currentcmd)){
				status = do_builtin(&currentcmd);
				if (status == ERROR){
					fprintf(stderr, "There was an issue with your builtin: %s\n", currentcmd.name);
				}
			}
			else if (find_fullpath(path, &currentcmd)){
				if (DEBUG) { printf("executing with currentcmd.name = %s, argv[1]==NULL? %d\n", currentcmd.name, currentcmd.argv[1] == NULL); }
				execute(&currentcmd);
			}
			else{
				fprintf(stderr, "cmd %s not found in your $PATH :(\n", currentcmd.name);
			}
			cleanup(&currentcmd);
			for (i = 0; i < 256; i++){
				path[i] = '\0';
			}
		}
	}
	return 0;

} // end main function
