#include <stdio.h>

#include "array_test.h"
#include "xml_element_test.h"
#include "xml_parser_test.h"
#include "file_test.h"
#include "tree_test.h"


//TEST executable

int main(){


    printf( "Test Suite for xmldiff:\n" );

    array_run_tests();
    xml_element_run_tests();
    xml_parser_run_tests();
    file_run_tests();
    tree_run_tests();

    return 0;

}
