#include <stdio.h>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/structure/treeStructures.h"
#include "generator/generator.h"

int main() {
    create_lexems();
    start_parser();
    generator();
    
    return 0;
}