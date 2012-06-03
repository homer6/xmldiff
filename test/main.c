#include <stdio.h>

#include "array_test.h"
#include "xml_element_test.h"
#include "xml_parser_test.h"


//TEST executable

int main(){


    printf( "Test Suite for xmldiff:\n" );

    array_run_tests();
    xml_element_run_tests();
    xml_parser_run_tests();

    return 0;

}
