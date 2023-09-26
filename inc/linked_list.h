//
// Created by Kaitlyn Archambault on 2023-09-22.
//

#ifndef ASSIGNMENT1_LINKED_LIST_H
#define ASSIGNMENT1_LINKED_LIST_H

#include <string>

class LinkedList {
public:
    LinkedList();
    virtual ~LinkedList();

protected:
    int m_size;
    std::string getNodeData(int line_num);
    void insertNode(int line_num, std::string data);
    void editNode(int line_num, std::string data);
    void deleteNode(int position);
    void updateNodePositions();

private:
    struct Node {
        int m_position;
        std::string m_data;
        Node * m_next;
    };
    Node * m_head;

};

#endif //ASSIGNMENT1_LINKED_LIST_H
