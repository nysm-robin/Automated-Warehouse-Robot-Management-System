#ifndef TASK1_ORDER_HPP
#define TASK1_ORDER_HPP

#include <string>
#include "shared_data.hpp"

struct Order {
    std::string orderID;   //auto-generated (example:ORD-001)
    std::string itemID;
    std::string itemName;
    int targetX;
    int targetY;
};

//custom singly-linked queue
struct QueueNode {
    Order data;
    QueueNode* next;
};

//custom FIFO queue
class OrderQueue {
private:
    QueueNode* front;
    QueueNode* rear;
    int orderCounter; //auto-increment for order ID

public:
    OrderQueue();
    ~OrderQueue(); //frees all heap nodes

    bool isEmpty();
    void enqueue(Order newOrder);
    bool getFrontOrder(Order& out);
    void dequeue();
    void dequeueSilent();
    void displayQueue();                
};

#endif //task1 order hpp