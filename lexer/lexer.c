#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include <ctype.h>

#include "helpers/categorySelectors.h"
#include "helpers/symbolTable.h"
#include "structs/structures.h"
#include "lexer.h"

char *keywords[7] = {
    "get",
    "everything",
    "from",
    "when",
    "is",
    "and",
    "or"
};

void categorize(int initial, int final, char *str, TokenType type) {
    int length = final + initial;
    char *temp = malloc(final + 1);

    for(int k = 0; k < final; k++) {
        temp[k] = tolower(str[initial + k]);
    }

    temp[final] = '\0';

    if(type == TOKEN_IDENTIFIER) {
        for(int j = 0; j < 7; j++) {
            if(strcmp(keywords[j], temp) == 0) {
                type = TOKEN_KEYWORD;
                break;
            }
        }
    }

    if(type == TOKEN_STRING) {
        ++initial;
        --length;
    }
    
    insert_to_symbol_table(initial, length, str, type);
}


Result get_next(int i, char *str, int len) {
    Result r;
    bool closingFound = false;

    bool isDouble=false;
    TokenType type = get_first_category(i, str);
    char *errorMessage = "";
    int count = 0;
    if(type!=TOKEN_STRING) {

        for(int j = i; str[j] != '\0'; j++) {
            if(type == TOKEN_NUMBER) {
                if(!is_space(str[j]) && is_word(str[j])) {
                    errorMessage = "Invalid number";
                    r.count = count;
                    r.line = j;
                    r.error = errorMessage;
                    r.token = str[j];
                    return r;
                }
            }

            if(type == TOKEN_OPERATOR) {
                if(j+2 < len && (is_space(str[j+1]) && is_operator(str[j+2]))) {
                    errorMessage = "Invalid operator";
                    r.count = count;
                    r.line = j;
                    r.error = errorMessage;
                    r.token = str[j];
                    return r;
                }
                if(is_operator(str[j+1])) {
                    isDouble=true;
                }
            }

            if(type != get_first_category(j, str)) {
                break;
            }
            if(isDouble) {
                count+=2;
                isDouble=false;
            } 
            else count++;
        }
    } else {

        if(is_string(str[i+1])) {
            errorMessage = "Invalid string";
            r.count = count;
            r.line = i;
            r.error = errorMessage;
            r.token = str[i];
            return r;
        }

        count++; 
        for(int k = i + 1; k < len; k++) {
            count++;
            if(is_string(str[k])) {
                closingFound = true;
                r.line = k+1;
                break;
            }
        }
    }

    if(type==TOKEN_STRING && !closingFound) {
        errorMessage = "\' mismatch";
        r.count = count;
        r.error = errorMessage;
        r.token = '\'';
        return r;
    }

    categorize(i, count, str, type);
    r.count = count;
    r.line = i;
    r.error = errorMessage;
    r.token = str[i];
    return r;
}

void create_lexems() {
    int nextindex = 0;
    char *str = (char*) malloc(sizeof(char) * 100);
    strcpy(str, "GET name, age FROM users WHEN age>25 AND age<28 AND age=30");

    int len = strlen(str);

    for (int i = 0; i < len; i++) {
        if(is_space(str[i])) {
            continue;
        } 
        Result r = get_next(i, str, len);

        if(r.error[0] != '\0') {
            if(get_first_category(i, str) == TOKEN_STRING) {
                printf("Error: %s at line %d\nNot Found: %c\n", r.error, r.line, r.token);
            } else {
                printf("Error: %s at line %d\nFound: %c", r.error, r.line, r.token);
            }
            break;
        }

        nextindex = r.count;
        i += nextindex-1;
    }
    
    free(str);
}