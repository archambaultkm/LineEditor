//
// Created by Kaitlyn Archambault on 2023-09-22.
//

#ifndef ASSIGNMENT1_LINKED_LIST_H
#define ASSIGNMENT1_LINKED_LIST_H

/**
 * Simple Linked List class
 *
 * Includes protected functions to insert, edit, and delete nodes
 * at a provided position in the list. As this is a class template,
 * Node data is not confined to any type.
 *
 */
template <class T>
class LinkedList {
public:
    LinkedList();
    virtual ~LinkedList();

protected:
    int m_size;
    T getNodeData(int line_num);
    void insertNode(int line_num, T data);
    void editNode(int line_num, T data);
    void deleteNode(int position);

private:
    struct Node {
        int m_position;
        T m_data;
        Node * m_next;
    };
    Node * m_head;
    void updateNodePositions();

};

#endif //ASSIGNMENT1_LINKED_LIST_H
