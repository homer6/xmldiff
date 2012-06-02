#ifndef XMLDIFF_FILE_H
#define XMLDIFF_FILE_H

#define BUF_SIZE 1024


/*
//gets the entire contents of the given (already open for reading) file as a string
//caller must free the memory returned
//exits(halts) the program on error
*/
char *file_get_contents( FILE *file );




#endif
