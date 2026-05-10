#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cctype>

using namespace std;

class SchemaElement {
protected:
    int id;
    char name[30];
    static int nextId;
    static int elementCount;
    static const int NAME_SIZE = 30;

public:
    SchemaElement();
    SchemaElement(const char* name);
    SchemaElement(const SchemaElement& other);
    virtual ~SchemaElement();

    int getId() const;
    const char* getName() const;
    void setName(const char* name);
    static int getElementCount();

    virtual void show() const = 0;
    virtual string toText() const = 0;

    bool operator==(const SchemaElement& other) const;
};

class Attribute : public SchemaElement {
private:
    char type[20];
    bool primaryKey;
    bool required;
    static const int TYPE_SIZE = 20;

public:
    Attribute();
    Attribute(const char* name, const char* type, bool key = false, bool required = false);
    Attribute(const Attribute& other);
    ~Attribute();

    Attribute& operator=(const Attribute& other);

    void setType(const char* type);
    const char* getType() const;
    void setPrimaryKey(bool value = true);
    bool isPrimaryKey() const;
    void setRequired(bool value = true);
    bool isRequired() const;

    void show() const;
    string toText() const;
};

class Entity : public SchemaElement {
private:
    Attribute* attributes;
    int attributeCount;
    const int maxAttributes;

public:
    Entity();
    Entity(const char* name, int maxAttributes = 10);
    Entity(const Entity& other);
    ~Entity();

    Entity& operator=(const Entity& other);

    void addAttribute(const Attribute& attribute);
    int getAttributeCount() const;
    Attribute& getAttribute(int index);
    const Attribute& getAttribute(int index) const;
    bool hasAttribute(const char* attributeName) const;
    int findAttributeIndex(const char* attributeName) const;
    bool hasPrimaryKey() const;

    void show() const;
    string toText() const;

    Entity& operator+=(const Attribute& attribute);
    Attribute& operator[](int index);
    const Attribute& operator[](int index) const;
};

class Relationship : public SchemaElement {
private:
    char fromEntity[30];
    char fromAttribute[30];
    char toEntity[30];
    char toAttribute[30];

public:
    Relationship();
    Relationship(const char* name, const char* fromE, const char* fromA,
                 const char* toE, const char* toA);
    Relationship(const Relationship& other);
    ~Relationship();

    Relationship& operator=(const Relationship& other);

    const char* getFromEntity() const;
    const char* getFromAttribute() const;
    const char* getToEntity() const;
    const char* getToAttribute() const;

    void show() const;
    string toText() const;
};

class Database {
private:
    Entity* entities;
    Relationship* relationships;
    int entityCount;
    int relationshipCount;
    const int maxEntities;
    const int maxRelationships;

public:
    Database(int maxEntities = 10, int maxRelationships = 10);
    Database(const Database& other);
    ~Database();

    Database& operator=(const Database& other);

    void clear();
    void addEntity(const Entity& entity);
    void addRelationship(const Relationship& relationship);
    int getEntityCount() const;
    int getRelationshipCount() const;
    Entity& getEntity(int index);
    const Entity& getEntity(int index) const;
    Relationship& getRelationship(int index);
    const Relationship& getRelationship(int index) const;
    int findEntityIndex(const char* entityName) const;
    bool hasEntity(const char* entityName) const;
    bool validateRelationship(const char* fromEntity, const char* fromAttribute,
                              const char* toEntity, const char* toAttribute) const;

    void showSchema() const;
    void showDiagram() const;
    string toText() const;
    string diagramText() const;
};

class SchemaExporter {
private:
    char fileName[50];
    static const int FILE_NAME_SIZE = 50;

public:
    SchemaExporter();
    SchemaExporter(const char* fileName);
    ~SchemaExporter();

    void setFileName(const char* fileName);
    const char* getFileName() const;
    void saveText(const Database& database, const char* outputFile = "schema.txt") const;
};

#endif
