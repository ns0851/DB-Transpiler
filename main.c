#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<stdlib.h>

typedef struct symbolTable {
    char *word;
    char *token;
} symbolTable;

bool isNum(char a) {
    return a >= '0' && a <= '9';
}

bool isOperator(char a) {
    return a == '+' || a == '>' || a == '<' ||
           a == '=' || a == '!' || a == '|' || a == '&';
}

bool isString(char a) {
    return a == '"' || a == '\'';
}

bool isWord(char a) {
    return (a >= 'a' && a <= 'z') ||
           (a >= 'A' && a <= 'Z') ||
           a == '_';
}

bool isSpace(char a) {
    return (a == ' ') || (a == '\t') || (a == '\n');
}

char *keywords[5] = {
    "get",
    "everything",
    "from",
    "when",
    "is"
};

void insertToSymbolTable(int initial, int final, char *str, char *category) {
    int len = final - initial;
    char *word = (char*) malloc(sizeof(char) * (len + 1));
    struct symbolTable *st = malloc(sizeof(struct symbolTable) * 20);

    // st[0].token = "Test 8 key!";
    int k = 0;
    int count = 0;

    for(int i = initial; i < final; i++) {
        word[k++] = str[i];
    }

    st[count].word = word;
    st[count++].token = category;
    
    word[k] = '\0';
    printf("%s", word);
    printf(" = %s\n", category);

    // free(word);
}

void categorize(int initial, int length, char *str, char *category) {
    char *temp = (char*) malloc(sizeof(char) * (length + 1));

    for(int k = 0; k < length; k++) {
        temp[k] = str[initial + k];
    }

    temp[length] = '\0';

    if(strcmp(category, "word") == 0) {
        category = "identifier";
        for(int j = 0; j < 5; j++) {
            if(strcmp(keywords[j], temp) == 0) {
                category = "keyword";
                break;
            }
        }
    }

    insertToSymbolTable(initial, length, str, category);
}

char* getFirstCategory(int i, char *str) {
    if(isNum(str[i])) return "number";
    if(isOperator(str[i])) return "operator";
    if(isString(str[i])) return "string";
    if(isWord(str[i])) return "word";
    if(isSpace(str[i])) return "space";

    return "unknown";
}

int getNext(int i, char *str) {
    char *category = getFirstCategory(i, str);
    char *errorMessage = "";
    int count = 0;
    for(int j = i; j < str[j] != '\0'; j++) {
        if(strcmp(category, getFirstCategory(j, str)) != 0) {
            break;
        }
        if(strcmp(category, "number") == 0) {
            if(!isNum(str[j])) {
                errorMessage = "Invalid number";
                break;
            }
        }
        count++;
    }

    categorize(i, count, str, category);
    return count;
}

int main() {
    int nextindex = 0;
    char *str = (char*) malloc(sizeof(char) * 100);
    strcpy(str, "fetch get post boom 67 7+ {");

    for (int i = 0; i < strlen(str); i++) {
        if(isSpace(str[i])) continue;
        nextindex = getNext(i, str);
        i += nextindex - 1;
    }
    
    free(str);

    return 0;
}
