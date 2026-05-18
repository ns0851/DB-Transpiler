#ifndef GENERATOR_H
#define GENERATOR_H

#include "../parser/structure/treeStructures.h"

typedef struct {
    char *key;
    char *value;
} Pair;

void generate_sql();
void generate_mongo();
void generator();

#endif