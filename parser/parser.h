    #ifndef PARSER_H
    #define PARSER_H

    #include "../lexer/helpers/symbolTable.h"
    #include "../lexer/structs/structures.h"

    extern Node *current;

    void advance();

    void expect(TokenType type, char *value);

    void parse_projection();
    void parse_source();
    void parse_query();
    char* convert_to_lower(char *str);

    void start_parser();

    #endif