# DatabaseSchemaBuilderInC++

SimpleDB Schema Builder is a console-based C++ Object-Oriented Programming final project. It lets a user create, view, save, and import a simple database schema.


The program stores schema metadata only:

- entity/table names
- attribute/column names
- data types
- primary key flags
- required flags
- relationships between entities

It is not a full database engine and does not store real database records.

## Features

- Create entities
- Add attributes to entities
- Create validated relationships
- Show schema details
- Save the schema to `schema.txt`
- Show a readable schema diagram in the terminal
- Load a built-in sample schema
- Import one entity from simple CSV text

## Files

- `main.cpp`: console menu, helper functions, sample schema loading, and CSV import
- `Classes.h`: all class declarations
- `Classes.cpp`: all method implementations
- `README.txt`: project explanation and compile/run instructions
- `CMakeLists.txt`: CLion/CMake build file

## Compile And Run

Compile from the terminal:

```bash
g++ -std=c++11 main.cpp Classes.cpp -o SimpleDBSchemaBuilder
```

Run on macOS/Linux:

```bash
./SimpleDBSchemaBuilder
```

Run on Windows:

```text
SimpleDBSchemaBuilder.exe
```

## CLion

CLion uses `CMakeLists.txt` to build the project. The executable target is `SimpleDBSchemaBuilder`, and it compiles `main.cpp` and `Classes.cpp`.

## Visualization

This project uses terminal visualization only. It does not use Graphviz, DOT files, PDF export, GUI code, SQLite, or external libraries.
