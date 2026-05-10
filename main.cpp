#include "Classes.h"

using namespace std;

void printMenu(const char* title = "SimpleDB Schema Builder");
bool getYesNo(const char* prompt, bool defaultValue = false);
void pauseScreen();
void loadSampleSchema(Database& database);
void importEntityFromCsvText(Database& database);
string guessDataType(const string& header, const string& sampleValue);

static string lowerText(string text) {
    for (int i = 0; i < (int)text.length(); i++) text[i] = (char)tolower(text[i]);
    return text;
}

static int splitLine(const string& line, string parts[], int maxParts) {
    stringstream ss(line);
    string item;
    int count = 0;
    while (count < maxParts && getline(ss, item, ',')) parts[count++] = item;
    return count;
}

static bool allDigits(const string& text) {
    if (text.length() == 0) return false;
    for (int i = 0; i < (int)text.length(); i++) {
        if (!isdigit(text[i])) return false;
    }
    return true;
}

static void readText(const char* prompt, string& value) {
    cout << prompt;
    getline(cin, value);
}

int main() {
    Database database(10, 10);
    SchemaExporter exporter;
    int choice;
    string input;

    do {
        printMenu();
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore(10000, '\n');

        if (choice == 1) {
            string entityName;
            readText("Enter entity name: ", entityName);
            database.addEntity(Entity(entityName.c_str()));
            cout << "Entity created.\n";
        } else if (choice == 2) {
            string entityName, attrName, type;
            readText("Enter entity name: ", entityName);
            int index = database.findEntityIndex(entityName.c_str());
            if (index == -1) {
                cout << "Entity not found.\n";
            } else {
                readText("Enter attribute name: ", attrName);
                readText("Enter data type: ", type);
                bool key = getYesNo("Is this a primary key? y/n ", false);
                bool required = key || getYesNo("Is this required? y/n ", false);
                database.getEntity(index) += Attribute(attrName.c_str(), type.c_str(), key, required);
                cout << "Attribute added.\n";
            }
        } else if (choice == 3) {
            string relName, fromE, fromA, toE, toA;
            readText("Enter relationship name: ", relName);
            readText("From entity: ", fromE);
            readText("From attribute: ", fromA);
            readText("To entity: ", toE);
            readText("To attribute: ", toA);
            if (database.validateRelationship(fromE.c_str(), fromA.c_str(), toE.c_str(), toA.c_str())) {
                database.addRelationship(Relationship(relName.c_str(), fromE.c_str(), fromA.c_str(), toE.c_str(), toA.c_str()));
                cout << "Relationship added.\n";
            } else {
                cout << "Invalid relationship. Check entity names, attribute names, and make sure the target attribute is a primary key.\n";
            }
        } else if (choice == 4) {
            database.showSchema();
        } else if (choice == 5) {
            exporter.saveText(database);
            cout << "schema.txt was created in the current working directory.\n";
        } else if (choice == 6) {
            database.showDiagram();
        } else if (choice == 7) {
            loadSampleSchema(database);
            cout << "Sample schema loaded.\n";
        } else if (choice == 8) {
            importEntityFromCsvText(database);
        } else if (choice != 0) {
            cout << "Invalid choice.\n";
        }

        if (choice != 0) pauseScreen();
    } while (choice != 0);

    return 0;
}

void printMenu(const char* title) {
    cout << "\n" << title << "\n\n";
    cout << "1. Create Entity\n";
    cout << "2. Add Attribute to Entity\n";
    cout << "3. Create Relationship\n";
    cout << "4. Show Schema Details\n";
    cout << "5. Save Schema as Text\n";
    cout << "6. Show Schema Diagram in Terminal\n";
    cout << "7. Load Sample Schema\n";
    cout << "8. Import Entity from CSV Text\n";
    cout << "0. Exit\n\n";
}

bool getYesNo(const char* prompt, bool defaultValue) {
    string answer;
    cout << prompt;
    getline(cin, answer);
    if (answer.length() == 0) return defaultValue;
    return answer[0] == 'y' || answer[0] == 'Y';
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    string temp;
    getline(cin, temp);
}

void loadSampleSchema(Database& database) {
    database.clear();

    Entity schemaElement("SchemaElement");
    schemaElement += Attribute("element_id", "AUTONUMBER", true, true);
    schemaElement += Attribute("name", "TEXT", false, true);
    schemaElement += Attribute("element_type", "TEXT", false, true);

    Entity entity("Entity");
    entity += Attribute("entity_id", "AUTONUMBER", true, true);
    entity += Attribute("element_id", "INT", false, true);
    entity += Attribute("max_attributes", "INT");

    Entity attribute("Attribute");
    attribute += Attribute("attribute_id", "AUTONUMBER", true, true);
    attribute += Attribute("element_id", "INT", false, true);
    attribute += Attribute("data_type", "TEXT");
    attribute += Attribute("primary_key", "BOOL");
    attribute += Attribute("required", "BOOL");

    Entity relationship("Relationship");
    relationship += Attribute("relationship_id", "AUTONUMBER", true, true);
    relationship += Attribute("element_id", "INT", false, true);
    relationship += Attribute("from_entity_id", "INT");
    relationship += Attribute("to_entity_id", "INT");

    database.addEntity(schemaElement);
    database.addEntity(entity);
    database.addEntity(attribute);
    database.addEntity(relationship);

    database.addRelationship(Relationship("EntityToSchemaElement", "Entity", "element_id", "SchemaElement", "element_id"));
    database.addRelationship(Relationship("AttributeToSchemaElement", "Attribute", "element_id", "SchemaElement", "element_id"));
    database.addRelationship(Relationship("RelationshipToSchemaElement", "Relationship", "element_id", "SchemaElement", "element_id"));
    database.addRelationship(Relationship("RelationshipFromEntity", "Relationship", "from_entity_id", "Entity", "entity_id"));
    database.addRelationship(Relationship("RelationshipToEntity", "Relationship", "to_entity_id", "Entity", "entity_id"));
}

void importEntityFromCsvText(Database& database) {
    const int MAX_COLS = 20;
    const int MAX_ROWS = 20;
    string entityName, line;
    string headers[MAX_COLS];
    string firstValues[MAX_ROWS];
    string sampleValues[MAX_COLS];

    readText("Enter entity name: ", entityName);
    cout << "Paste CSV text below. First row must be headers.\n";
    cout << "Type END_CSV on its own line when finished.\n";

    getline(cin, line);
    if (line == "END_CSV" || line.length() == 0) {
        cout << "No CSV data entered.\n";
        return;
    }

    int colCount = splitLine(line, headers, MAX_COLS);
    int rowCount = 0;

    while (getline(cin, line) && line != "END_CSV") {
        string cols[MAX_COLS];
        int count = splitLine(line, cols, MAX_COLS);
        if (count > 0 && rowCount < MAX_ROWS) firstValues[rowCount++] = cols[0];
        for (int i = 0; i < count && i < colCount; i++) {
            if (sampleValues[i].length() == 0) sampleValues[i] = cols[i];
        }
    }

    bool firstColumnIsKey = colCount > 0 && lowerText(headers[0]).find("id") != string::npos;
    for (int i = 0; i < rowCount && firstColumnIsKey; i++) {
        if (firstValues[i].length() == 0) firstColumnIsKey = false;
        for (int j = i + 1; j < rowCount; j++) {
            if (firstValues[i] == firstValues[j]) firstColumnIsKey = false;
        }
    }

    Entity entity(entityName.c_str());
    if (!firstColumnIsKey) {
        string newKey = entityName + "ID";
        entity += Attribute(newKey.c_str(), "AUTONUMBER", true, true);
    }

    for (int i = 0; i < colCount; i++) {
        string type = guessDataType(headers[i], sampleValues[i]);
        bool key = firstColumnIsKey && i == 0;
        entity += Attribute(headers[i].c_str(), type.c_str(), key, key);
    }

    database.addEntity(entity);
    cout << "CSV entity imported.\n";
}

string guessDataType(const string& header, const string& sampleValue) {
    string h = lowerText(header);
    string v = lowerText(sampleValue);
    if (h.find("date") != string::npos) return "DATE";
    if (h.find("phone") != string::npos) return "TEXT";
    if (v == "true" || v == "false" || v == "yes" || v == "no") return "BOOL";
    if (allDigits(v) && v.length() < 8) return "INT";
    return "TEXT";
}
