
#include <iostream>
#include <string>
#include <limits>

#include "shared_data.hpp"
#include "task1_Order_TP079583.hpp"
#include "Task2_Robot_TP079957.hpp"
#include "Task3_Nav_TP077593.hpp"
#include "task4_menu.hpp"
#include "Task5_WarehouseTree_TP075054.hpp"

using namespace std;

//integration
static void processOrderWithNavigation(OrderQueue& q, RobotNavigator& nav, RobotAssignmentModule& robotModule) {
    Order frontOrder;
    if (!q.getFrontOrder(frontOrder)) {
        cout << "\n[WARNING] Order queue is empty - nothing to process.\n";
        return;
    }

    cout << "\n---------------------------------------------------------\n";
    cout << "[TASK 1] DISPATCHING ORDER : " << frontOrder.orderID << "\n";
    cout << "Item    : " << frontOrder.itemName
         << "(ID: " << frontOrder.itemID << ")\n";
    cout << "Target  : (" << frontOrder.targetX
         << "," << frontOrder.targetY << ")\n";
    cout << "----------------------------------------------------------\n";

    //Task 2 = assign robot
    int assignedRobotId = -1;
    cout << "\n[TASK 2] Requesting available robot for delivery...\n";
    if (!robotModule.assignTask(("Deliver " + frontOrder.orderID).c_str(), assignedRobotId)) {
        cout << "[WARNING] No robots available! Please wait or free a robot in Task 2 menu.\n";
        return; //abort process
    }

    //Task 3 = Navigate from dispatch base (0,0) to item location
    cout << "\n[TASK 3] Robot " << assignedRobotId << " departing from base (0, 0)...\n";
    nav.navigateToItem(0, 0, frontOrder.targetX, frontOrder.targetY);

    //remove order from queue
    q.dequeueSilent();

    //Task 3 = return robot
    nav.returnToPackingStation();

    cout << "\n---------------------------------------------------------\n";
    cout << "[ORDER COMPLETE] " << frontOrder.orderID
         << " - " << frontOrder.itemName << " delivered.\n";
    cout << "----------------------------------------------------------\n";

    //Task 2 = free robot
    robotModule.completeTask(assignedRobotId);
}

//Task1
static void runOrderMenu(OrderQueue& q, RobotNavigator& nav, RobotAssignmentModule& robotModule) {
    int choice;
    do {
        cout << "\n------------- ORDER MANAGEMENT MODULE -------------\n";
        cout << "1. Create New Order\n";
        cout << "2. Process Next Order\n";
        cout << "3. Display All Pending Orders\n";
        cout << "0. Back to Main Menu\n";
        cout << "----------------------------------------------------\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string inputID;
                cout << "Enter Item ID to order (1-680): ";
                cin >> inputID;

                InventoryItem matched;
                if (findItemInInventory(inputID, matched)) {
                    Order newOrder;
                    newOrder.itemID   = matched.itemID;
                    newOrder.itemName = matched.itemName;
                    newOrder.targetX  = matched.x;
                    newOrder.targetY  = matched.y;
                    q.enqueue(newOrder);
                } else {
                    cout << "[ERROR] Item ID '" << inputID
                         << "' not found in inventory.\n";
                }
                break;
            }
            case 2:
                //integration (Task 1 + Task 2 + Task 3)
                processOrderWithNavigation(q, nav, robotModule);
                break;
            case 3:
                q.displayQueue();
                break;
            case 0:
                cout << "Returning to main menu...\n";
                break;
            default:
                cout << "[ERROR] Invalid choice. Enter 0-3.\n";
        }
    } while (choice != 0);
}

//Task 3
static void runNavigationMenu(RobotNavigator& nav) {
    int choice;
    do {
        cout << "\n---------- ROBOT NAVIGATION MODULE ----------\n";
        cout << "  1. Navigate from coordinates to target\n";
        cout << "  2. Return robot to base (packing station)\n";
        cout << "  0. Back to Main Menu\n";
        cout << "------------------------------------------------\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int sx, sy, tx, ty;
                cout << "Start X (column): "; cin >> sx;
                cout << "Start Y (row)   : "; cin >> sy;
                cout << "Target X        : "; cin >> tx;
                cout << "Target Y        : "; cin >> ty;
                nav.navigateToItem(sx, sy, tx, ty);
                break;
            }
            case 2:
                nav.returnToPackingStation();
                break;
            case 0:
                cout << "Returning to main menu...\n";
                break;
            default:
                cout << "[ERROR] Invalid choice. Enter 0-2.\n";
        }
    } while (choice != 0);
}

//Main
int main() {
    cout << "------------------------------------------------\n";
    cout << " WAREHOUSE G17 ROBOT MANAGEMENT SYSTEM\n";
    cout << "------------------------------------------------\n\n";

    //load shared inventory (Task 1 uses this for order lookup)
    const string INVENTORY_FILE = "Warehouse_G17.txt";
    cout << "Loading inventory  : " << INVENTORY_FILE << " ... ";
    if (!loadInventoryFromFile(INVENTORY_FILE)) {
        cout << "\n[FATAL] Cannot open '" << INVENTORY_FILE
             << "'. Make sure the file is in the same folder.\n";
        return 1;
    }
    cout << "OK (" << totalItemsInWarehouse << " items loaded)\n";

    //load warehouse layout tree (Task 5)
    const string GRID_FILE = "grid-DSTR.txt";
    WarehouseTree warehouseTree;
    cout << "Loading grid layout: " << GRID_FILE << " ... ";
    if (!warehouseTree.loadLayoutFromGridFile(GRID_FILE)) {
        cout << "[WARNING] Grid file not found. Task 5 tree will be empty.\n";
    }

    cout << "\nSystem ready. All modules loaded.\n";

    //share module
    OrderQueue   orderQueue; //Task 1
    RobotAssignmentModule robotModule; //Task 2 
    RobotNavigator navigator; //Task 3 

    // Load Task 2 robots at startup
    if (robotModule.loadRobotsFromFile("task2_robots.txt")) {
        cout << "[SUCCESS] Task 2 robots loaded from file.\n";
    } else {
        cout << "[WARNING] task2_robots.txt not found. Loading sample robots.\n";
        robotModule.addRobot(101, "Alpha");
        robotModule.addRobot(102, "Bravo");
        robotModule.addRobot(103, "Charlie");
        robotModule.addRobot(104, "Delta");
    }

    //Loop Main Menu
    int choice;
    do {
        cout << "\n-------------------------------------------------\n";
        cout << "WAREHOUSE G17 ROBOT MANAGEMENT SYSTEM\n";
        cout << "---------------------------------------------------\n";
        cout << "  1. Order Management         (Task 1 - TP079583)\n";
        cout << "  2. Robot Assignment         (Task 2 - TP079957)\n";
        cout << "  3. Manual Robot Navigation  (Task 3 - TP077593)\n";
        cout << "  4. Item Search & Management (Task 4 - TP078856)\n";
        cout << "  5. Warehouse Layout Tree    (Task 5 - TP075054)\n";
        cout << "  0. Exit\n";
        cout << "---------------------------------------------------\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                //auto triger task 3
                runOrderMenu(orderQueue, navigator, robotModule);
                break;
            case 2:
                runRobotAssignmentMenu(robotModule);
                break;
            case 3:
                runNavigationMenu(navigator);
                break;
            case 4:
                task4_menu::showMainMenu();
                break;
            case 5:
                runWarehouseTreeMenu(warehouseTree);
                break;
            case 0:
                cout << "\nSystem shutdown. Goodbye!\n";
                break;
            default:
                cout << "[ERROR] Invalid choice. Please enter 0-5.\n";
        }
    } while (choice != 0);

    return 0;
}