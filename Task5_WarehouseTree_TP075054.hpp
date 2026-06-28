#ifndef WAREHOUSE_HPP
#define WAREHOUSE_HPP

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct LocationNode {
    string locationId;
    string locationName;
    string locationType;

    int row;
    int column;

    LocationNode* parent;
    LocationNode* FirstChild;
    LocationNode* nextSibling;

    LocationNode(string id, string name, string type, int r, int c);
};

class WarehouseTree {
private:
    LocationNode* root;

    LocationNode* SearchRecursive(LocationNode* current, string targetName);
    LocationNode* searchByCoordinateRecursive(LocationNode* current, int row, int column);

    void displayRecursive(LocationNode* current, int level);
    void deleteRecursive(LocationNode* current);

public:
    WarehouseTree();
    ~WarehouseTree();

    bool AddLocation(string parentName, string id, string name, string type, int row, int column);
    bool loadLayoutFromGridFile(string filename);

    LocationNode* searchLocation(string targetName);
    LocationNode* searchLocationByCoordinate(int row, int column);

    void displayWarehouseLayout();
    void displayPathToLocation(string targetName);
    void displayPathToCoordinate(int row, int column);

    LocationNode* getRoot();
};


void runWarehouseTreeMenu(WarehouseTree& wt);
#endif //warehous hpp