#include "lexer/lexer.h"
#include "lexer/helpers/symbolTable.h"
#include <stdio.h>

int main() {
    createLexems();

    for(int i = 0; i < symbolCount; i++) {
        printf("%s -> %s\n",
            st[i].word,
            tokenNames[st[i].type]
        );
    }

    return 0;
}