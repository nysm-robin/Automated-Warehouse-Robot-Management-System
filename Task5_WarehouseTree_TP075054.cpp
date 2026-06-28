#include "Task5_WarehouseTree_TP075054.hpp"
#include <limits>


LocationNode::LocationNode(string id, string name, string type, int r, int c) {
    locationId   = id;
    locationName = name;
    locationType = type;
    row          = r;
    column       = c;
    parent       = NULL;
    FirstChild   = NULL;
    nextSibling  = NULL;
}

WarehouseTree::WarehouseTree() {
    root = new LocationNode("W0", "Warehouse", "Root", 0, 0);
}

WarehouseTree::~WarehouseTree() {
    deleteRecursive(root);
}

void WarehouseTree::deleteRecursive(LocationNode* current) {
    if (current == NULL) return;
    deleteRecursive(current->FirstChild);
    deleteRecursive(current->nextSibling);
    delete current;
}

//add new location node
bool WarehouseTree::AddLocation(string parentName, string id, string name,
                                string type, int row, int column) {
    LocationNode* parentNode = searchLocation(parentName);
    if (parentNode == NULL) {
        cout << "\n[ERROR] Parent location not found: " << parentName << "\n";
        return false;
    }
    if (searchLocation(name) != NULL) {
        cout << "\n[ERROR] Location already exists: " << name << "\n";
        return false;
    }

    LocationNode* newNode = new LocationNode(id, name, type, row, column);
    newNode->parent       = parentNode;

    //append as last sibling under parent's first child
    if (parentNode->FirstChild == NULL) {
        parentNode->FirstChild = newNode;
    } else {
        LocationNode* cur = parentNode->FirstChild;
        while (cur->nextSibling != NULL) cur = cur->nextSibling;
        cur->nextSibling = newNode;
    }
    return true;
}

//search by name
LocationNode* WarehouseTree::searchLocation(string targetName) {
    return SearchRecursive(root, targetName);
}

LocationNode* WarehouseTree::SearchRecursive(LocationNode* current,
                                              string targetName) {
    if (current == NULL) return NULL;
    if (current->locationName == targetName) return current;

    LocationNode* found = SearchRecursive(current->FirstChild, targetName);
    if (found != NULL) return found;

    return SearchRecursive(current->nextSibling, targetName);
}

//search by grid coordinate
LocationNode* WarehouseTree::searchLocationByCoordinate(int row, int column) {
    return searchByCoordinateRecursive(root, row, column);
}

LocationNode* WarehouseTree::searchByCoordinateRecursive(LocationNode* current,
                                                          int row, int column) {
    if (current == NULL) return NULL;
    if (current->row == row && current->column == column) return current;

    LocationNode* found = searchByCoordinateRecursive(current->FirstChild,
                                                       row, column);
    if (found != NULL) return found;

    return searchByCoordinateRecursive(current->nextSibling, row, column);
}

//display full warehouse layout tree
void WarehouseTree::displayWarehouseLayout() {
    cout << "\n-------------------- WAREHOUSE LAYOUT TREE --------------------\n";
    displayRecursive(root, 0);
    cout << "-----------------------------------------------------------------\n";
}

void WarehouseTree::displayRecursive(LocationNode* current, int level) {
    if (current == NULL) return;

    for (int i = 0; i < level; i++) cout << "  ";
    cout << "- " << current->locationName
         << " [" << current->locationType << "]"
         << " ID: " << current->locationId;

    if (current->row >= 0 && current->column >= 0 &&
        !(current->row == 0 && current->column == 0 && level == 0)) {
        cout << " (" << current->row << "," << current->column << ")";
    }
    cout << "\n";

    displayRecursive(current->FirstChild,  level + 1);
    displayRecursive(current->nextSibling, level);
}

//display path from root to a named location
void WarehouseTree::displayPathToLocation(string targetName) {
    LocationNode* targetNode = searchLocation(targetName);
    if (targetNode == NULL) {
        cout << "\n[ERROR] Location not found: " << targetName << "\n";
        return;
    }

    // Count depth
    int count = 0;
    LocationNode* cur = targetNode;
    while (cur != NULL) { count++; cur = cur->parent; }

    // Build path array (root → target)
    LocationNode** path = new LocationNode*[count];
    cur = targetNode;
    for (int i = count - 1; i >= 0; i--) {
        path[i] = cur;
        cur = cur->parent;
    }

    cout << "\n-------------------- PATH TO LOCATION --------------------\n";
    for (int i = 0; i < count; i++) {
        cout << path[i]->locationName;
        if (i != count - 1) cout << " -> ";
    }
    cout << "\nType      : " << targetNode->locationType
         << "\nCoordinate: (" << targetNode->row
         << ", " << targetNode->column << ")\n";

    delete[] path;
}

void WarehouseTree::displayPathToCoordinate(int row, int column) {
    LocationNode* targetNode = searchLocationByCoordinate(row, column);
    if (targetNode == NULL) {
        cout << "\n[ERROR] No location at (" << row << "," << column << ")\n";
        return;
    }
    displayPathToLocation(targetNode->locationName);
}

LocationNode* WarehouseTree::getRoot() {
    return root;
}

// P = Parking station (dispatch/packing area)
// S = Shelf (item storage location)
//C = Charging station
//R = Robot road/path
//. = Open space (ignored, not added to tree)
//rows = Y coordinates 
//columns = X coordinates.

bool WarehouseTree::loadLayoutFromGridFile(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "\n[ERROR] Failed to open layout file: " << filename << "\n";
        return false;
    }

    bool parkingCreated  = false;
    bool shelfCreated    = false;
    bool chargingCreated = false;

    string line;
    int row = 0;

    while (getline(file, line)) {
        //strip carriage return for cross platform safety
        if (!line.empty() && line.back() == '\r') line.pop_back();

        for (int col = 0; col < (int)line.length(); col++) {
            char symbol = line[col];

            if (symbol == 'P') {
                if (!parkingCreated) {
                    AddLocation("Warehouse", "A-P", "Parking Area",
                                "Area", -1, -1);
                    parkingCreated = true;
                }
                string id   = "P-" + to_string(row) + "-" + to_string(col);
                string name = "Parking (" + to_string(row) + ","
                              + to_string(col) + ")";
                AddLocation("Parking Area", id, name, "Parking", row, col);

            } else if (symbol == 'S') {
                if (!shelfCreated) {
                    AddLocation("Warehouse", "A-S", "Shelf Area",
                                "Area", -1, -1);
                    shelfCreated = true;
                }
                string id   = "S-" + to_string(row) + "-" + to_string(col);
                string name = "Shelf (" + to_string(row) + ","
                              + to_string(col) + ")";
                AddLocation("Shelf Area", id, name, "Shelf", row, col);

            } else if (symbol == 'C') {
                if (!chargingCreated) {
                    AddLocation("Warehouse", "A-C", "Charging Area",
                                "Area", -1, -1);
                    chargingCreated = true;
                }
                string id   = "C-" + to_string(row) + "-" + to_string(col);
                string name = "Charging (" + to_string(row) + ","
                              + to_string(col) + ")";
                AddLocation("Charging Area", id, name, "Charging", row, col);

            } else if (symbol == 'R') {
                (void)col; //suppress unused warning
            }
            //'.' = open space, ignored
        }
        row++;
    }

    file.close();
    cout << "[SUCCESS] Warehouse layout loaded from " << filename
         << " (" << row << " rows processed)\n";
    return true;
}

//Task 5 menu
void runWarehouseTreeMenu(WarehouseTree& wt) {
    int choice;
    do {
        cout << "\n---------- WAREHOUSE LAYOUT TREE MODULE (Task 5) ----------\n";
        cout << "1. Display Full Warehouse Layout Tree\n";
        cout << "2. Search Location by Name\n";
        cout << "3. Search Location by Coordinate (row, col)\n";
        cout << "4. Display Path to Named Location\n";
        cout << "5. Display Path to Coordinate\n";
        cout << "0. Back to Main Menu\n";
        cout << "-------------------------------------------------------------\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            wt.displayWarehouseLayout();

        } else if (choice == 2) {
            string name;
            cout << "Enter location name (e.g. Shelf (3,2)): ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, name);

            LocationNode* loc = wt.searchLocation(name);
            if (loc) {
                cout << "\n[FOUND] " << loc->locationName
                     << " [" << loc->locationType << "]"
                     << " at (" << loc->row << ", " << loc->column << ")\n";
            } else {
                cout << "[NOT FOUND] No location named '" << name << "'\n";
            }

        } else if (choice == 3) {
            int r, c;
            cout << "Enter row   (Y): "; cin >> r;
            cout << "Enter column(X): "; cin >> c;

            LocationNode* loc = wt.searchLocationByCoordinate(r, c);
            if (loc) {
                cout << "\n[FOUND] " << loc->locationName
                     << " [" << loc->locationType << "]\n";
            } else {
                cout << "[NOT FOUND] No location at (" << r << ", " << c << ")\n";
            }

        } else if (choice == 4) {
            string name;
            cout << "Enter location name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, name);
            wt.displayPathToLocation(name);

        } else if (choice == 5) {
            int r, c;
            cout << "Enter row   (Y): "; cin >> r;
            cout << "Enter column(X): "; cin >> c;
            wt.displayPathToCoordinate(r, c);

        } else if (choice != 0) {
            cout << "[ERROR] Invalid choice.\n";
        }

    } while (choice != 0);

    cout << "Returning to main menu...\n";
}