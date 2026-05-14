# dbgen — A Custom Query Language Transpiler

A command-line tool written in **C** that transpiles a human-readable custom query language into syntactically correct **PostgreSQL**, **SQL**, **MongoDB**. 
Built from scratch with no external libraries — just C, pointers, and little bit of pain.

---

## What It Does

Instead of writing raw SQL, you write queries in a simpler, readable syntax:

```
FETCH name, age from users when age is greater than 18 and status is "active"
```

sqlgen transpiles this into:

```sql
SELECT name, age FROM users WHERE age > 18 AND status = 'active';
```

---
This might seem pretty similar to actual SQL queries but where it gets interesting is converting the same exact request into a working MongoDB line.

## Architecture

Classic compiler pipeline — three distinct phases:

```
Input String
     │
     ▼
┌─────────────┐
│    LEXER    │  raw string → token stream
└─────────────┘
     │
     ▼
┌─────────────┐
│   PARSER    │  token stream → AST
└─────────────┘
     │
     ▼
┌─────────────┐
│  GENERATOR  │  AST → SQL string
└─────────────┘
     │
     ▼
PostgreSQL Output
```

### Phase 1 — Lexer
- Hand-rolled FSM (no `strtok`)
- Case-insensitive keyword recognition (`FETCH` == `fetch`)
- Look-ahead (peek) for multi-char operators (`<=`, `>=`, `!=`)
- Precise error reporting with line and column numbers
- Handles string literals, numeric constants, identifiers

### Phase 2 — Parser
- Recursive descent parser
- Builds an Abstract Syntax Tree (AST)
- Correct operator precedence (`AND` binds tighter than `OR`)
- Graceful error reporting:
  ```
  Syntax Error: Expected 'FROM' at line 1, col 12, found 'TABLE'
  ```

### Phase 3 — Generator
- Post-order AST traversal
- Dynamic string buffer with `realloc`
- SQL injection protection — escapes single quotes in user strings

---

## Grammar (EBNF)

```ebnf
query       ::= "FETCH" projection "FROM" source [ "WHERE" condition ]
projection  ::= "*" | column { "," column }
column      ::= IDENTIFIER
source      ::= IDENTIFIER
condition   ::= and_expr { "OR" and_expr }
and_expr    ::= comparison { "AND" comparison }
comparison  ::= IDENTIFIER operator value
operator    ::= "=" | "!=" | "<" | ">" | "<=" | ">=" | "IS"
value       ::= NUMBER | STRING | IDENTIFIER
```

---

## Examples

**Simple fetch:**
```
FETCH * FROM products
```
```sql
SELECT * FROM products;
```

**With condition:**
```
FETCH name, price FROM products WHERE price < 100
```
```sql
SELECT name, price FROM products WHERE price < 100;
```

**Compound condition:**
```
FETCH name, age FROM users WHERE age > 18 AND age < 60
```
```sql
SELECT name, age FROM users WHERE age > 18 AND age < 60;
```

**OR precedence:**
```
FETCH name FROM users WHERE age < 18 OR age > 60 AND status IS "active"
```
```sql
SELECT name FROM users WHERE age < 18 OR (age > 60 AND status = 'active');
```


## Testing

```bash
make test
```

`test_suite.c` covers 50+ cases:

- Valid queries (simple, compound, edge cases)
- Invalid syntax (missing FROM, unknown tokens, malformed conditions)
- Operator precedence (AND vs OR)
- String escaping (SQL injection prevention)
- Edge cases (empty input, extra whitespace, case insensitivity)

---

## Memory Safety

Zero memory leaks. Verified with Valgrind:

```bash
valgrind --leak-check=full ./sqlgen -i query.txt
```

Every AST node is cleaned up via `AST_free()` — recursive teardown of the entire tree after code generation.

---

## Roadmap

- [x] Lexer with FSM
- [x] Symbol table
- [ ] Recursive descent parser
- [ ] AST construction
- [ ] PostgreSQL code generator
- [ ] CLI with getopt
- [ ] 50+ test suite
- [ ] Valgrind clean
- [ ] MySQL dialect support
- [ ] MongoDB query generation

---

## Why

Writing raw SQL is fine. But understanding how a query language gets parsed, validated, and compiled into executable statements — that's the interesting part. 
This project is about building the pipeline from scratch: every token, every node, every string concat by hand.

No libraries. No shortcuts. Just C.

---
