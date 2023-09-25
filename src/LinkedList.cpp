//
// Created by Kaitlyn Archambault on 2023-09-22.
//

#include "../inc/LinkedList.h"

LinkedList::LinkedList() {
    size = 0;
    head = nullptr;
    tail = nullptr;
}

LinkedList::~LinkedList() = default;

void LinkedList::insertNode(int line_num, std::string data) {
    if (line_num <= 0 || line_num > size + 1) return;

    Node * temp = new Node();
    temp->data = std::move(data);

    //If this is the first node being inserted
    if (size == 0) {
        temp->position = line_num;
        temp->next = nullptr;
        head = temp;

    } else {
        //insert the new node at the position specified
        temp->position = line_num;
        Node * curr = head;
        Node * prev = nullptr;

        //special case for inserting on first node
        if (line_num == 1) {
            head = temp;
            temp->next = curr;
        } else {

            while ( curr != nullptr && curr->position != line_num) {
                prev = curr;
                curr = curr->next;
            }
            //new node gets inserted between the previous and current node
            prev->next = temp;
            temp->next = curr;
        }
    }
    //update list size and node positions to reflect the addition
    size++;
    updateNodePositions();
}

void LinkedList::editNode(int position, std::string data) {
    if (position <= 0 || position > size) return;

    Node * curr = head;
    while (curr != nullptr && curr->position != position) curr = curr->next;
    curr->data = std::move(data);
}

void LinkedList::deleteNode(int position) {
    if (position <= 0 || position > size) return;

    Node * curr = head;
    Node * prev = nullptr;
    Node * temp;

    //keep looping through until reaching the desired position
    while (curr != nullptr && curr->position != position) {
        prev = curr;
        curr = curr->next;
    }

    //special cases for first and last lines in document
    if (position == 1) {
        temp = head;
        head = head->next;
        delete temp;

    //any lines in the middle of the document
    } else {
        if (prev != nullptr && curr != nullptr) prev->next = curr->next;
        delete curr;
    }

    //update the size value of the document to reflect the deletion
    updateNodePositions();
    size--;
}

void LinkedList::updateNodePositions() {
    Node * line = head;
    Node * prev = nullptr;

    while (line != nullptr) {
        if (prev == nullptr)
            line->position = 1;
        else
            line->position = prev->position + 1;

        prev = line;
        line = line->next;
    }
}

std::string LinkedList::getNodeData(int position) {
    if (position <= 0 || position > size) return "";

    Node * curr = head;

    //get to the desired line
    while (curr != nullptr && curr->position != position) curr = curr->next;

    return curr->data;
}
