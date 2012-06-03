#ifndef XMLDIFF_FILE_H
#define XMLDIFF_FILE_H

#define BUF_SIZE 1024


/**
 * Gets the entire contents of the given (already open for reading) file as a string.
 * Caller must free the memory returned
 * Exits(halts) the program on error
 */
void file_get_contents( char **destination, FILE *file );




#endif
