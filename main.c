#include "lexer/lexer.h"
#include "parser/parser.h"
#include <stdio.h>

int main() {
    create_lexems();
    start();
    printf("Query parsed successfully!\n");


    return 0;
}