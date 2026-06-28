#include "Task4_Search_TP078856.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

namespace task4_module
{
    bool validLocation(int x, int y)
    {
        if (x >= 0 && y >= 0)
        {
            return true;
        }
        else
        {
            cout << "Invalid location" << endl;
            return false;
        }
    }

    ItemBST::ItemBST()
    {
        //initialize the tree and get data from file
        rootByID = NULL;
        rootByName = NULL;
        readFile();
    }

    ItemBST::~ItemBST()
    {
        //delete the trees
        deleteTree(rootByID);
        deleteTree(rootByName);
    }

    void ItemBST::deleteTree(TreeNode* root)
    {
        //delete until reaching the end of the tree
        if (root == NULL)
            return;

        //delete left and right root as the new subtree root
        deleteTree(root -> left);
        deleteTree(root -> right);

        //delete the current root
        delete root;
    }

    void ItemBST::readFile()
    {
        ifstream file("Warehouse_G17.txt");
        if (!file.is_open())
        {
            cout << "Error in opening file: Warehouse_G17.txt" << endl;
            return;
        }

        //loop
        string line;
        getline(file, line); //skip header
        while (getline(file, line))
        {
            //initialize
            stringstream ss(line);
            string datum;

            //input the value into data
            Item tmp;
            getline(ss, datum, ',');
            tmp.id = stoi(datum);
            getline(ss, datum, ',');
            tmp.name = datum;
            getline(ss, datum, ',');
            tmp.x = stoi(datum);
            getline(ss, datum, ',');
            tmp.y = stoi(datum);

            //insert the data into tree
            insert(tmp);
        }
    }

    void ItemBST::updateFile()
    {
        ofstream file("Warehouse_G17.txt");
        if (!file.is_open())
        {
            cout << "Error in opening file: Warehouse_G17.txt" << endl;
            return;
        }

        //write header
        file << "ItemID,ItemName,X,Y\n";

        //write the data in pre-order traversal
        updateFilePreOrder(rootByID, file);
    }

    void ItemBST::updateFilePreOrder(TreeNode* node, ofstream& file)
    {
        //root - left - right
        if (node == NULL)
            return;

        //write current node data
        file << to_string(node -> data.id) << ","
             << node -> data.name << ","
             << to_string(node -> data.x) << ","
             << to_string(node -> data.y)
              << "\n";

        //write left and right subtree
        updateFilePreOrder(node -> left, file);
        updateFilePreOrder(node -> right, file);
    }

    bool ItemBST::insert(Item item)
    {
        //create new leaf node
        TreeNode* newNodeByID = new TreeNode;
        newNodeByID -> data = item;
        newNodeByID -> left = NULL;
        newNodeByID -> right = NULL;

        TreeNode* newNodeByName = new TreeNode;
        newNodeByName -> data = item;
        newNodeByName -> left = NULL;
        newNodeByName -> right = NULL;

        //insert if root is empty
        if (rootByID == NULL && rootByName == NULL)
        {
            rootByID = newNodeByID;
            rootByName = newNodeByName;
            return true;
        }

        //search the parent node by ID
        TreeNode* parentByID = searchParentNode(rootByID, item, true);
        
        //check for duplicate ID
        if (parentByID == NULL)
        {
            cout << "Duplicate ID\n";
            delete newNodeByID;
            delete newNodeByName;
            return false;
        }

        //by name
        TreeNode* parentByName = searchParentNode(rootByName, item, false);

        //check for duplicate Name
        if (parentByName == NULL)
        {
            cout << "Duplicate Name\n";
            delete newNodeByID;
            delete newNodeByName;
            return false;
        }

        //insert item by ID
        if (item.id < (parentByID -> data).id)
            parentByID -> left = newNodeByID;
        else
            parentByID -> right = newNodeByID;

        //by name
        if (item.name < (parentByName -> data).name)
            parentByName -> left = newNodeByName;
        else
            parentByName -> right = newNodeByName;

        //update size
        return true;
    }

    bool ItemBST::insertToID(Item item)
    {
        //create new leaf node
        TreeNode* newNodeByID = new TreeNode;
        newNodeByID -> data = item;
        newNodeByID -> left = NULL;
        newNodeByID -> right = NULL;

        //insert if root is empty
        if (rootByID == NULL && rootByName == NULL)
        {
            rootByID = newNodeByID;
            return true;
        }

        //search parent node
        TreeNode* parentByID = searchParentNode(rootByID, item, true);
        
        //check duplicate ID
        if (parentByID == NULL)
        {
            delete newNodeByID;
            return false;
        }

        //insert item
        if (item.id < (parentByID -> data).id)
            parentByID -> left = newNodeByID;
        else
            parentByID -> right = newNodeByID;

        return true;
    }

    bool ItemBST::insertToName(Item item)
    {
        //create new leaf node
        TreeNode* newNodeByName = new TreeNode;
        newNodeByName -> data = item;
        newNodeByName -> left = NULL;
        newNodeByName -> right = NULL;

        //insert if root empty
        if (rootByName == NULL)
        {
            rootByName = newNodeByName;
            return true;
        }

        //sear parent node
        TreeNode* parentByName = searchParentNode(rootByName, item, false);

        //check duplicate name
        if (parentByName == NULL)
        {
            delete newNodeByName;
            return false;
        }

        //insert item
        if (item.name < (parentByName -> data).name)
            parentByName -> left = newNodeByName;
        else
            parentByName -> right = newNodeByName;

        return true;
    }

    TreeNode* ItemBST::searchParentNode(TreeNode* root, Item item, bool byID)
    {
        //initialize
        TreeNode* parent = NULL;
        TreeNode* node = root;

        //traverse to the intended leaf node as parent
        while (node != NULL)
        {
            parent = node;

            if (byID)
            {
                //check duplicate ID
                if (item.id == (node -> data).id)
                    return NULL;

                //traverse left or right based on current ID
                if (item.id < (node -> data).id)
                    node = node -> left;
                else
                    node = node -> right;
            }
            else
            {
                //check duplicate Name
                if (item.name == (node -> data).name)
                    return NULL;

                //traverse left or right based on current Name
                if (item.name < (node -> data).name)
                    node = node -> left;
                else
                    node = node -> right;
            }
        }

        //found parent node
        return parent;
    }

    void ItemBST::inOrderTraversalDisplay(TreeNode* root)
    {
        if (root == NULL)
            return;

        inOrderTraversalDisplay(root -> left);

        //display data
        cout << " | " << setw(7) << (root -> data).id 
             << " | " << setw(30) << (root -> data).name 
             << " | " << setw(5) << (root -> data).x
             << " | " << setw(5) << (root -> data).y
             << " |" << endl;
        
        inOrderTraversalDisplay(root -> right);
    }

    void ItemBST::remove(TreeNode* parent, TreeNode* node, bool byID)
    {
        //node is leaf
        if (node -> left == NULL && node -> right == NULL)
        {
            //node is root
            if (parent == NULL)
            {
                if (byID)
                    rootByID = NULL;
                else
                    rootByName = NULL;
            }

            //remove reference to the node from parent
            else if (parent -> left == node)
                parent -> left = NULL;
            else
                parent -> right = NULL;

            //delete node
            delete node;
            return;
        }

        //case 2 = Node has no left subtree
        if (node -> left == NULL)
        {
            //node is root
            if (parent == NULL)
            {
                if (byID)
                    rootByID = node -> right;
                else
                    rootByName = node -> right;
            }

            //change reference from node to right child
            else if (parent -> left == node)
                parent -> left = node -> right;
            else
                parent -> right = node -> right;

            //delete the node
            delete node;
            return;
        }

        //case 3 = node has no right subtree
        if (node -> right == NULL)
        {
            //node is root
            if (parent == NULL)
            {
                if (byID)
                    rootByID = node -> left;
                else
                    rootByName = node -> left;
            }

            //change reference from node to left child
            else if (parent -> left == node)
                parent -> left = node -> left;
            else
                parent -> right = node -> left;

            //delete node
            delete node;
            return;
        }

        //case 4 = node has left and right subtree
        //find successor node
        TreeNode* successorParent = node;
        TreeNode* successor = node -> right;
        //traverse to the leftmost node in the right subtree
        while (successor -> left != NULL)
        {
            successorParent = successor;
            successor = successor -> left;
        }

        //change the data of the node to successor's data
        node -> data = successor -> data;

        //change the reference from the successor's parent to successor's right child
        if (successorParent -> left == successor)
            successorParent -> left = successor -> right;
        else
            successorParent -> right = successor -> right;

        //delete the original data of node and successor node
        delete successor;
    }

    Item* ItemBST::searchByID(int id)
    {
        return searchByID(id, rootByID);
    }

    Item* ItemBST::searchByName(string name)
    {
        return searchByName(name, rootByName);
    }

    Item* ItemBST::searchByID(int id, TreeNode* node)
    {
        if (node == NULL)
            return NULL;

        if (id == (node -> data).id)
            return &(node -> data);

        if (id < (node -> data).id)
            return searchByID(id, node -> left);
        else
            return searchByID(id, node -> right);
    }

    Item* ItemBST::searchByName(string name, TreeNode* node)
    {
        if (node == NULL)
            return NULL;

        if (name == (node -> data).name)
            return &(node -> data);

        if (name < (node -> data).name)
            return searchByName(name, node -> left);
        else
            return searchByName(name, node -> right);
    }

    bool ItemBST::insertItem(int id, string name, int x, int y)
    {
        if (!validLocation(x, y))
        {
            return false;
        }

        if (name.empty())
        {
            cout << "Name cannot be empty\n";
            return false;
        }

        Item item;
        item.id = id;
        item.name = name;
        item.x = x;
        item.y = y;

        return insert(item);
    }

    bool ItemBST::updateItemNameByID(int id, string newName)
    {
        if (newName.empty())
        {
            cout << "Name cannot be empty\n";
            return false;
        }

        //search for item that matches the id
        Item* itemByID = searchByID(id);
        if (itemByID == NULL)
        {
            cout << "ID is not found" << endl;
            return false;
        }

        //name same as new name
        if (itemByID -> name == newName)
        {
            cout << "The new name is the same as the old name" << endl;
            return true;
        }

        //try to insert new name
        Item item;
        item.id = itemByID -> id;
        item.name = newName;
        item.x = itemByID -> x;
        item.y = itemByID -> y;
        
        //name must be unique
        if (!insertToName(item))
        {
            cout << "Name must be unique" << endl;
            return false;
        }

        //check in case error (debugging purpose)
        if (!removeItem(itemByID -> name, rootByName))
        {
            cout << "Error: Name is not found" << endl;
            return false;
        }

        //update the name
        itemByID -> name = newName;
        return true;
    }

    bool ItemBST::updateItemNameByName(string name, string newName)
    {
        if (newName.empty())
        {
            cout << "Name cannot be empty\n";
            return false;
        }

        //name same as new name
        if (name == newName)
        {
            cout << "The new name is the same as the old name" << endl;
            return true;
        }

        //search for item that matches the name
        Item* itemByName = searchByName(name);
        if (itemByName == NULL)
        {
            cout << "Name is not found" << endl;
            return false;
        }

        //search for item that matches the id
        Item* itemByID = searchByID(itemByName -> id);
        if (itemByID == NULL)
        {
            cout << "ID is not found" << endl;
            return false;
        }

        //try to insert new name
        Item item;
        item.id = itemByName -> id;
        item.name = newName;
        item.x = itemByName -> x;
        item.y = itemByName -> y;

        //update name, must unique
        if (!insertToName(item))
        {
            cout << "Name must be unique" << endl;
            return false;
        }

        // Check in case error (debugging purpose)
        if (!removeItem(name, rootByName))
        {
            cout << "Error: Name is not found" << endl;
            return false;
        }
        itemByID -> name = newName;
        return true;
    }

    bool ItemBST::updateItemByID(int id, int newX, int newY)
    {
        if (!validLocation(newX, newY))
        {
            return false;
        }

        Item* item = searchByID(id);
        if (item == NULL)
        {
            cout << "ID is not found" << endl;
            return false;
        }

        Item* item2 = searchByName(item -> name);

        item -> x = newX;
        item -> y = newY;
        item2 -> x = newX;
        item2 -> y = newY;
        return true;
    }

    bool ItemBST::updateItemByName(string name, int newX, int newY)
    {
        if (!validLocation(newX, newY))
        {
            return false;
        }

        Item* item = searchByName(name);
        if (item == NULL)
        {
            cout << "Name is not found" << endl;
            return false;
        }

        Item* item2 = searchByID(item -> id);

        item -> x = newX;
        item -> y = newY;
        item2 -> x = newX;
        item2 -> y = newY;
        return true;
    }
    
    bool ItemBST::removeItemByID(int id)
    {
        Item* item = searchByID(id);

        if (item == NULL)
        {
            cout << "ID is not found" << endl;
            return false;
        }

        removeItem(item -> name, rootByName);
        removeItem(id, rootByID);
        return true;
    }

    bool ItemBST::removeItemByName(std::string name)
    {
        Item* item = searchByName(name);

        if (item == NULL)
        {
            cout << "Name is not found" << endl;
            return false;
        }

        removeItem(item -> id, rootByID);
        removeItem(name, rootByName);
        return true;
    }

    bool ItemBST::removeItem(int id, TreeNode* root)
    {
        TreeNode* parent = NULL;
        TreeNode* node = root;
        
        while (node != NULL)
        {
            if ((node -> data).id == id)
            {
                remove(parent, node, true);
                return true;
            }

            parent = node;
            if (id < (node -> data).id)
                node = node -> left;
            else
                node = node -> right;
        }

        return false;
    }

    bool ItemBST::removeItem(string name, TreeNode* root)
    {
        TreeNode* parent = NULL;
        TreeNode* node = root;
        
        while (node != NULL)
        {
            if ((node -> data).name == name)
            {
                remove(parent, node, false);
                return true;
            }

            parent = node;
            if (name < (node -> data).name)
                node = node -> left;
            else
                node = node -> right;
        }

        return false;
    }

    void ItemBST::displaySortedByID()
    {
        displayBorder();
        cout << " | " << setw(7) << "ID"
             << " | " << setw(30) << "Name"
             << " | " << setw(5) << "X"
             << " | " << setw(5) << "Y"
             << " | " << endl;
        displayBorder();
        inOrderTraversalDisplay(rootByID);
        displayBorder();
    }

    void ItemBST::displaySortedByName()
    {
        displayBorder();
        cout << " | " << setw(7) << "ID"
             << " | " << setw(30) << "Name"
             << " | " << setw(5) << "X"
             << " | " << setw(5) << "Y"
             << " | " << endl;
        displayBorder();
        inOrderTraversalDisplay(rootByName);
        displayBorder();
    }

    void ItemBST::displayBorder()
    {
        cout << "-----------------------------------------------------------------" << endl;
    }

}