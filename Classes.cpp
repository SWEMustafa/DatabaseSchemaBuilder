#include "Classes.h"

using namespace std;

int SchemaElement::nextId = 1;
int SchemaElement::elementCount = 0;

static void copyText(char* dest, const char* src, int size) {
    if (src == 0) src = "";
    strncpy(dest, src, size - 1);
    dest[size - 1] = '\0';
}

SchemaElement::SchemaElement() : id(0) {
    copyText(name, "", NAME_SIZE);
}

SchemaElement::SchemaElement(const char* name) : id(nextId++) {
    copyText(this->name, name, NAME_SIZE);
    elementCount++;
}

SchemaElement::SchemaElement(const SchemaElement& other) {
    copyText(name, other.name, NAME_SIZE);
    if (other.id == 0) {
        id = 0;
    } else {
        id = nextId++;
        elementCount++;
    }
}

SchemaElement::~SchemaElement() {
}

int SchemaElement::getId() const {
    return id;
}

const char* SchemaElement::getName() const {
    return name;
}

void SchemaElement::setName(const char* name) {
    copyText(this->name, name, NAME_SIZE);
}

int SchemaElement::getElementCount() {
    return elementCount;
}

bool SchemaElement::operator==(const SchemaElement& other) const {
    return strcmp(name, other.name) == 0;
}

Attribute::Attribute() : SchemaElement(), primaryKey(false), required(false) {
    copyText(type, "", TYPE_SIZE);
}

Attribute::Attribute(const char* name, const char* type, bool key, bool required)
    : SchemaElement(name), primaryKey(key), required(required || key) {
    copyText(this->type, type, TYPE_SIZE);
}

Attribute::Attribute(const Attribute& other) : SchemaElement(other) {
    copyText(type, other.type, TYPE_SIZE);
    primaryKey = other.primaryKey;
    required = other.required;
}

Attribute::~Attribute() {
}

Attribute& Attribute::operator=(const Attribute& other) {
    if (this != &other) {
        id = other.id;
        copyText(name, other.name, NAME_SIZE);
        copyText(type, other.type, TYPE_SIZE);
        primaryKey = other.primaryKey;
        required = other.required;
    }
    return *this;
}

void Attribute::setType(const char* type) {
    copyText(this->type, type, TYPE_SIZE);
}

const char* Attribute::getType() const {
    return type;
}

void Attribute::setPrimaryKey(bool value) {
    primaryKey = value;
    if (value) required = true;
}

bool Attribute::isPrimaryKey() const {
    return primaryKey;
}

void Attribute::setRequired(bool value) {
    required = value;
}

bool Attribute::isRequired() const {
    return required;
}

void Attribute::show() const {
    cout << "  ";
    if (primaryKey) cout << "PK ";
    cout << name << " : " << type;
    if (required) cout << " REQUIRED";
    cout << endl;
}

string Attribute::toText() const {
    ostringstream out;
    out << name << " " << type;
    if (primaryKey) out << " PRIMARY KEY";
    if (required) out << " REQUIRED";
    return out.str();
}

Entity::Entity() : SchemaElement(), attributeCount(0), maxAttributes(10) {
    attributes = new Attribute[maxAttributes];
}

Entity::Entity(const char* name, int maxAttributes)
    : SchemaElement(name), attributeCount(0), maxAttributes(maxAttributes) {
    attributes = new Attribute[this->maxAttributes];
}

Entity::Entity(const Entity& other)
    : SchemaElement(other), attributeCount(other.attributeCount), maxAttributes(other.maxAttributes) {
    attributes = new Attribute[maxAttributes];
    for (int i = 0; i < attributeCount; i++) attributes[i] = other.attributes[i];
}

Entity::~Entity() {
    delete[] attributes;
}

Entity& Entity::operator=(const Entity& other) {
    if (this != &other) {
        id = other.id;
        copyText(name, other.name, NAME_SIZE);
        attributeCount = other.attributeCount;
        if (attributeCount > maxAttributes) attributeCount = maxAttributes;
        for (int i = 0; i < attributeCount; i++) attributes[i] = other.attributes[i];
    }
    return *this;
}

void Entity::addAttribute(const Attribute& attribute) {
    if (attributeCount < maxAttributes) {
        attributes[attributeCount++] = attribute;
    } else {
        cout << "Cannot add more attributes to " << name << "." << endl;
    }
}

int Entity::getAttributeCount() const {
    return attributeCount;
}

Attribute& Entity::getAttribute(int index) {
    return attributes[index];
}

const Attribute& Entity::getAttribute(int index) const {
    return attributes[index];
}

bool Entity::hasAttribute(const char* attributeName) const {
    return findAttributeIndex(attributeName) != -1;
}

int Entity::findAttributeIndex(const char* attributeName) const {
    for (int i = 0; i < attributeCount; i++) {
        if (strcmp(attributes[i].getName(), attributeName) == 0) return i;
    }
    return -1;
}

bool Entity::hasPrimaryKey() const {
    for (int i = 0; i < attributeCount; i++) {
        if (attributes[i].isPrimaryKey()) return true;
    }
    return false;
}

void Entity::show() const {
    cout << "[" << name << "]" << endl;
    for (int i = 0; i < attributeCount; i++) attributes[i].show();
}

string Entity::toText() const {
    ostringstream out;
    out << "[" << name << "]\n";
    for (int i = 0; i < attributeCount; i++) out << "  " << attributes[i].toText() << "\n";
    return out.str();
}

Entity& Entity::operator+=(const Attribute& attribute) {
    addAttribute(attribute);
    return *this;
}

Attribute& Entity::operator[](int index) {
    return attributes[index];
}

const Attribute& Entity::operator[](int index) const {
    return attributes[index];
}

Relationship::Relationship() : SchemaElement() {
    copyText(fromEntity, "", NAME_SIZE);
    copyText(fromAttribute, "", NAME_SIZE);
    copyText(toEntity, "", NAME_SIZE);
    copyText(toAttribute, "", NAME_SIZE);
}

Relationship::Relationship(const char* name, const char* fromE, const char* fromA,
                           const char* toE, const char* toA) : SchemaElement(name) {
    copyText(fromEntity, fromE, NAME_SIZE);
    copyText(fromAttribute, fromA, NAME_SIZE);
    copyText(toEntity, toE, NAME_SIZE);
    copyText(toAttribute, toA, NAME_SIZE);
}

Relationship::Relationship(const Relationship& other) : SchemaElement(other) {
    copyText(fromEntity, other.fromEntity, NAME_SIZE);
    copyText(fromAttribute, other.fromAttribute, NAME_SIZE);
    copyText(toEntity, other.toEntity, NAME_SIZE);
    copyText(toAttribute, other.toAttribute, NAME_SIZE);
}

Relationship::~Relationship() {
}

Relationship& Relationship::operator=(const Relationship& other) {
    if (this != &other) {
        id = other.id;
        copyText(name, other.name, NAME_SIZE);
        copyText(fromEntity, other.fromEntity, NAME_SIZE);
        copyText(fromAttribute, other.fromAttribute, NAME_SIZE);
        copyText(toEntity, other.toEntity, NAME_SIZE);
        copyText(toAttribute, other.toAttribute, NAME_SIZE);
    }
    return *this;
}

const char* Relationship::getFromEntity() const {
    return fromEntity;
}

const char* Relationship::getFromAttribute() const {
    return fromAttribute;
}

const char* Relationship::getToEntity() const {
    return toEntity;
}

const char* Relationship::getToAttribute() const {
    return toAttribute;
}

void Relationship::show() const {
    cout << "  " << fromEntity << "." << fromAttribute
              << " -> " << toEntity << "." << toAttribute << endl;
}

string Relationship::toText() const {
    ostringstream out;
    out << fromEntity << "." << fromAttribute << " -> " << toEntity << "." << toAttribute;
    return out.str();
}

Database::Database(int maxEntities, int maxRelationships)
    : entityCount(0), relationshipCount(0),
      maxEntities(maxEntities), maxRelationships(maxRelationships) {
    entities = new Entity[this->maxEntities];
    relationships = new Relationship[this->maxRelationships];
}

Database::Database(const Database& other)
    : entityCount(other.entityCount), relationshipCount(other.relationshipCount),
      maxEntities(other.maxEntities), maxRelationships(other.maxRelationships) {
    entities = new Entity[maxEntities];
    relationships = new Relationship[maxRelationships];
    for (int i = 0; i < entityCount; i++) entities[i] = other.entities[i];
    for (int i = 0; i < relationshipCount; i++) relationships[i] = other.relationships[i];
}

Database::~Database() {
    delete[] entities;
    delete[] relationships;
}

Database& Database::operator=(const Database& other) {
    if (this != &other) {
        entityCount = other.entityCount;
        relationshipCount = other.relationshipCount;
        if (entityCount > maxEntities) entityCount = maxEntities;
        if (relationshipCount > maxRelationships) relationshipCount = maxRelationships;
        for (int i = 0; i < entityCount; i++) entities[i] = other.entities[i];
        for (int i = 0; i < relationshipCount; i++) relationships[i] = other.relationships[i];
    }
    return *this;
}

void Database::clear() {
    entityCount = 0;
    relationshipCount = 0;
}

void Database::addEntity(const Entity& entity) {
    if (entityCount < maxEntities) {
        entities[entityCount++] = entity;
    } else {
        cout << "Database is full. Cannot add entity." << endl;
    }
}

void Database::addRelationship(const Relationship& relationship) {
    if (relationshipCount < maxRelationships) {
        relationships[relationshipCount++] = relationship;
    } else {
        cout << "Database is full. Cannot add relationship." << endl;
    }
}

int Database::getEntityCount() const {
    return entityCount;
}

int Database::getRelationshipCount() const {
    return relationshipCount;
}

Entity& Database::getEntity(int index) {
    return entities[index];
}

const Entity& Database::getEntity(int index) const {
    return entities[index];
}

Relationship& Database::getRelationship(int index) {
    return relationships[index];
}

const Relationship& Database::getRelationship(int index) const {
    return relationships[index];
}

int Database::findEntityIndex(const char* entityName) const {
    for (int i = 0; i < entityCount; i++) {
        if (strcmp(entities[i].getName(), entityName) == 0) return i;
    }
    return -1;
}

bool Database::hasEntity(const char* entityName) const {
    return findEntityIndex(entityName) != -1;
}

bool Database::validateRelationship(const char* fromEntity, const char* fromAttribute,
                                    const char* toEntity, const char* toAttribute) const {
    int fromIndex = findEntityIndex(fromEntity);
    int toIndex = findEntityIndex(toEntity);
    if (fromIndex == -1 || toIndex == -1) return false;
    int fromAttr = entities[fromIndex].findAttributeIndex(fromAttribute);
    int toAttr = entities[toIndex].findAttributeIndex(toAttribute);
    if (fromAttr == -1 || toAttr == -1) return false;
    return entities[toIndex].getAttribute(toAttr).isPrimaryKey();
}

void Database::showSchema() const {
    cout << toText();
}

void Database::showDiagram() const {
    cout << diagramText();
}

string Database::toText() const {
    ostringstream out;
    out << "SimpleDB Schema Builder\n\nEntities:\n";
    if (entityCount == 0) out << "  No entities.\n";
    for (int i = 0; i < entityCount; i++) out << entities[i].toText() << "\n";
    out << "Relationships:\n";
    if (relationshipCount == 0) out << "  No relationships.\n";
    for (int i = 0; i < relationshipCount; i++) out << "  " << relationships[i].toText() << "\n";
    return out.str();
}

string Database::diagramText() const {
    ostringstream out;
    out << "==============================\n";
    out << " SimpleDB Schema Visualization\n";
    out << "==============================\n\n";
    for (int i = 0; i < entityCount; i++) {
        out << "[" << entities[i].getName() << "]\n";
        for (int j = 0; j < entities[i].getAttributeCount(); j++) {
            const Attribute& a = entities[i].getAttribute(j);
            bool fk = false;
            for (int r = 0; r < relationshipCount; r++) {
                if (strcmp(relationships[r].getFromEntity(), entities[i].getName()) == 0 &&
                    strcmp(relationships[r].getFromAttribute(), a.getName()) == 0) {
                    fk = true;
                }
            }
            out << "  ";
            if (a.isPrimaryKey()) out << "PK ";
            else if (fk) out << "FK ";
            else out << "   ";
            out << a.getName() << " : " << a.getType();
            if (a.isRequired()) out << " REQUIRED";
            out << "\n";
        }
        out << "\n";
    }
    out << "Relationships:\n";
    if (relationshipCount == 0) out << "  No relationships.\n";
    for (int i = 0; i < relationshipCount; i++) {
        out << "  " << relationships[i].toText() << "\n";
    }
    return out.str();
}

SchemaExporter::SchemaExporter() {
    copyText(fileName, "schema.txt", FILE_NAME_SIZE);
}

SchemaExporter::SchemaExporter(const char* fileName) {
    copyText(this->fileName, fileName, FILE_NAME_SIZE);
}

SchemaExporter::~SchemaExporter() {
}

void SchemaExporter::setFileName(const char* fileName) {
    copyText(this->fileName, fileName, FILE_NAME_SIZE);
}

const char* SchemaExporter::getFileName() const {
    return fileName;
}

void SchemaExporter::saveText(const Database& database, const char* outputFile) const {
    ofstream out(outputFile);
    if (out) out << database.toText();
}
