# dbgen — A Custom Query Language Transpiler

A command-line tool written in **C** that transpiles a human-readable custom query language into **SQL** and **MongoDB** queries.  
Built from scratch — no external libraries. Just C, pointers, and a bit of pain.

---

## What It Does

Instead of writing raw SQL or MongoDB, you write queries in a simpler, readable syntax:

```
get name, age from users when age>18
```

And dbgen transpiles it into:

**SQL:**
```sql
SELECT name, age FROM users WHERE age > 18
```

**MongoDB:**
```js
db.users.find({ age: { $gt: 18 } }, { name: 1, age: 1 })
```

Same input. Two targets. One pipeline.

---

## Demo

```
C:\Users\Admin> transpiler listen

>> get name from user when age>20 -both
Query parsed successfully!
Generating SQL
SELECT name FROM user WHERE age > 20

Generating MongoDB
db.user.find({ age: { $gt: 20 } }, { name: 1 })

>> get name from user when age>20 and age<30 -sql
Query parsed successfully!
Generating SQL
SELECT name FROM user WHERE age > 20 and age < 30

>> get name from user when age>20 and age<30 -mongo
Query parsed successfully!
Generating MongoDB
db.user.find({ $and: [ { age: { $gt: 20 } }, { age: { $lt: 30 } } ] }, { name: 1 })

>> exit
```

---

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
│  GENERATOR  │  AST → SQL / MongoDB
└─────────────┘
```

### Phase 1 — Lexer
- Hand-rolled, no `strtok`
- Case-insensitive keyword recognition (`GET` == `get`)
- Look-ahead for multi-char operators (`<=`, `>=`, `!=`)
- Handles string literals, numeric constants, identifiers
- Error reporting for invalid tokens

### Phase 2 — Parser
- Recursive descent parser
- Builds a binary AST where condition nodes are operators and leaves are values
- `AND` chains handled via recursion — naturally produces correct tree structure

### Phase 3 — Generator
- Walks the AST and emits the target query string
- SQL: standard `SELECT ... FROM ... WHERE` syntax
- MongoDB: idiomatic `$and`, `$gt`, `$lt`, `$gte`, `$lte` operators with projection

---

## Query Syntax

```
get <fields> from <table> when <condition>
```

| Clause | Example |
|--------|---------|
| Select all | `get everything from users` |
| Select fields | `get name, age from users` |
| Single condition | `get name from users when age>18` |
| Multiple conditions | `get name from users when age>18 and age<60` |

**Supported operators:** `>`, `<`, `>=`, `<=`, `=`, `!=`

**Flags:**
- `-sql` → generate SQL only
- `-mongo` → generate MongoDB only
- `-both` → generate both (default if no flag)

---

## How to Build

```bash
make
```

```bash
# REPL mode
./transpiler listen

# or on Windows
transpiler listen
```

---

## Project Structure

```
dbgen/
├── main.c
├── lexer/
│   ├── lexer.c / lexer.h
│   ├── helpers/
│   │   ├── categorySelectors.c/h
│   │   └── symbolTable.c/h
│   └── structs/
│       └── structures.h
├── parser/
│   ├── parser.c / parser.h
│   └── structure/
│       └── treeStructures.h
└── generator/
    └── generator.c / generator.h
```

---

## Roadmap

- [x] Lexer with look-ahead
- [x] Symbol table
- [x] Recursive descent parser
- [x] Binary AST construction
- [x] SQL code generator
- [x] MongoDB code generator
- [x] REPL mode (`transpiler listen`)
- [x] `-sql`, `-mongo`, `-both` flags
- [ ] `OR` condition support
- [ ] `JOIN` support
- [ ] `GROUP BY` / `ORDER BY` / `LIMIT`
- [ ] MySQL / PostgreSQL dialect toggle
- [ ] Test suite

---

## Why

Writing raw SQL or MongoDB is fine. But understanding how a query language gets **lexed, parsed, and compiled** into executable statements — that's the interesting part.

Every token, every AST node, every string by hand. No libraries. No shortcuts. Just C.

---
