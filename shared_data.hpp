#ifndef SHARED_DATA_HPP
#define SHARED_DATA_HPP

#include <string>

struct InventoryItem {
    std::string itemID;
    std::string itemName;
    int x;
    int y;
};

const int MAX_ITEMS = 700;

//actual storage define in Task1
extern InventoryItem warehouseInventory[MAX_ITEMS];
extern int totalItemsInWarehouse;

//inventory functions implemented in Task1
bool loadInventoryFromFile(const std::string& filename);
bool findItemInInventory(const std::string& searchID, InventoryItem& foundItem);

#endif //share data hpp
