#ifndef XMLDIFF_FILE_H
#define XMLDIFF_FILE_H

#define BUF_SIZE 1024


typedef signed int wide_char;




/**
 * Gets the entire contents of the given (already open for reading) file as a string.
 * The file contents may be any encoding, but they'll be converted to wide characters (4 bytes wide).
 * Returns the number of wide characters. So the length allocated for the wide_char* is
 * ( 4 bytes * number of characters + 1 null terminator ).
 *
 * Caller must free the memory returned.
 * Exits(halts) the program on error.
 */
size_t file_get_contents( wide_char **destination, FILE *file );




#endif
