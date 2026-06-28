#include "task1_Order_TP079583.hpp"
#include "shared_data.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

//share inventory storage
InventoryItem warehouseInventory[MAX_ITEMS];
int totalItemsInWarehouse = 0;

//load from csv
bool loadInventoryFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    string line;
    getline(file, line); //skip CSV header row (ItemID,ItemName,X,Y)

    while (getline(file, line) && totalItemsInWarehouse < MAX_ITEMS) {
        if (line.empty()) continue;
        //strip windows style
        if (!line.empty() && line.back() == '\r') line.pop_back();

        stringstream ss(line);
        string id, name, xStr, yStr;
        getline(ss, id,    ',');
        getline(ss, name,  ',');
        getline(ss, xStr,  ',');
        getline(ss, yStr,  ',');

        if (id.empty() || xStr.empty() || yStr.empty()) continue;

        warehouseInventory[totalItemsInWarehouse].itemID   = id;
        warehouseInventory[totalItemsInWarehouse].itemName = name;
        warehouseInventory[totalItemsInWarehouse].x        = stoi(xStr);
        warehouseInventory[totalItemsInWarehouse].y        = stoi(yStr);
        totalItemsInWarehouse++;
    }

    file.close();
    return true;
}

//lLinear search in inventory
bool findItemInInventory(const string& searchID, InventoryItem& foundItem) {
    for (int i = 0; i < totalItemsInWarehouse; i++) {
        if (warehouseInventory[i].itemID == searchID) {
            foundItem = warehouseInventory[i];
            return true;
        }
    }
    return false;
}

//implementation
OrderQueue::OrderQueue() : front(NULL), rear(NULL), orderCounter(1) {}
OrderQueue::~OrderQueue() {
    while (!isEmpty()) {
        QueueNode* temp = front;
        front = front->next;
        delete temp;
    }
    rear = NULL;
}

bool OrderQueue::isEmpty() {
    return (front == NULL);
}

void OrderQueue::enqueue(Order newOrder) {
    //generate padded order ID
    stringstream ss;
    ss << "ORD-";
    if      (orderCounter < 10)  ss << "00";
    else if (orderCounter < 100) ss << "0";
    ss << orderCounter;
    newOrder.orderID = ss.str();
    orderCounter++;

    //allocate new node and append to rear
    QueueNode* newNode  = new QueueNode();
    newNode->data       = newOrder;
    newNode->next       = NULL;

    if (isEmpty()) {
        front = newNode;
        rear  = newNode;
    } else {
        rear->next = newNode;
        rear       = newNode;
    }

    cout << "\n[SUCCESS] Order created: " << newOrder.orderID << "\n";
    cout << "Item    : " << newOrder.itemName
         << "(ID: " << newOrder.itemID << ")\n";
    cout << "Location: (" << newOrder.targetX
         << ", " << newOrder.targetY << ")\n";
}

//peek at front order without removing it
bool OrderQueue::getFrontOrder(Order& out) {
    if (isEmpty()) return false;
    out = front->data;
    return true;
}

//remove front order n print dispatch info
void OrderQueue::dequeue() {
    if (isEmpty()) {
        cout << "\n[WARNING] Queue is empty! No orders to process.\n";
        return;
    }
    QueueNode* temp = front;
    Order processed  = front->data;

    cout << "\n[DISPATCHING] Order: " << processed.orderID << "\n";
    cout << "  Item  : " << processed.itemName << "\n";
    cout << "  Target: (" << processed.targetX
         << ", " << processed.targetY << ")\n";

    front = front->next;
    if (front == NULL) rear = NULL;
    delete temp;
}

//remove front order silently
void OrderQueue::dequeueSilent() {
    if (isEmpty()) return;
    QueueNode* temp = front;
    front = front->next;
    if (front == NULL) rear = NULL;
    delete temp;
}

void OrderQueue::displayQueue() {
    if (isEmpty()) {
        cout << "\n---------- ORDER QUEUE IS EMPTY ----------\n";
        return;
    }

    cout << "\n"
         << "------------------------------------------------------------------------------\n"
         << "                     CURRENT WAREHOUSE ORDER QUEUE\n"
         << "------------------------------------------------------------------------------\n"
         << "No.  Order ID     Item ID  Item Name                    Location\n"
         << "------------------------------------------------------------------------------\n";

    QueueNode* current = front;
    int no = 1;
    while (current != NULL) {
        cout << no << ".   "
             << current->data.orderID << "   "
             << current->data.itemID;

        // padding so columns align
        if ((int)current->data.itemID.length() < 4) cout << "\t\t";
        else                                         cout << "\t";

        cout << current->data.itemName;
        if      ((int)current->data.itemName.length() < 16) cout << "\t\t";
        else if ((int)current->data.itemName.length() < 24) cout << "\t";

        cout << "(" << current->data.targetX
             << ", " << current->data.targetY << ")\n";

        current = current->next;
        no++;
    }
    cout << "------------------------------------------------------------------------------\n";
}