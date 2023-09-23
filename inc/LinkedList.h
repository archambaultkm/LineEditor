//
// Created by Kaitlyn Archambault on 2023-09-22.
//

#ifndef ASSIGNMENT1_LINKEDLIST_H
#define ASSIGNMENT1_LINKEDLIST_H

#include <string>

class LinkedList {
public:
    LinkedList();
    virtual ~LinkedList();
    std::string getNodeData(int position);
    void insertNode(int line_num, std::string data);
    void editNode(int position, std::string data);
    void deleteNode(int position);
    void updateNodePositions();
    friend std::ostream& operator<<(std::ostream& output, const LinkedList& document);
private:
    struct Node {
        int position;
        std::string data;
        Node * next;
    };
    Node * head;
    Node * tail;
protected:
    int size;
};


#endif //ASSIGNMENT1_LINKEDLIST_H
