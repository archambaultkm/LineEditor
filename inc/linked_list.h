/**
 * @author Kaitlyn Archambault
 * @date Sept. 26, 2023
 */

#ifndef ASSIGNMENT1_LINKED_LIST_H
#define ASSIGNMENT1_LINKED_LIST_H

#include <string>

/**
 * @brief Simple Linked List class template
 *
 * Includes protected functions to insert, edit, and delete nodes
 * at a provided position in the list. As this is a class template,
 * Node data is not confined to any type.
 *
 */
template <class T>
class LinkedList {
public:
    LinkedList() {
        m_size = 0;
        m_head = nullptr;
    }
    virtual ~LinkedList() = default;

protected:
    int m_size;

    /**
     * Use to access the data stored in a specific Node
     *
     * @param line_num position in list of node to delete
     * @returns Node m_data property
     */
    T getNodeData(int line_num) {
        if (line_num <= 0 || line_num > m_size) return "";

        Node * curr = m_head;

        // get to the desired line
        while (curr != nullptr && curr->m_position != line_num)
            curr = curr->m_next;

        return curr->m_data;
    }

    /**
     * Use to access the data stored in a consecutive range of nodes
     *
     * @param range_start position in list of node to delete
     * @param range_end end of range of nodes
     * @returns array
     */
    std::vector<T> getNodeData(int range_start, int range_end) {
        std::vector<T> contents;

        Node * curr = m_head;

        // get to the desired line
        while (curr != nullptr && curr->m_position != range_start)
            curr = curr->m_next;

        for (int i=range_start; i <= range_end; i++) {
            contents.push_back(curr->m_data);
            curr = curr->m_next;
        }

        return contents;
    }

    /**
     * Inserts a new Node in Linked List
     *
     * @param line_num line to insert new node at
     * @param data data for the new node to hold
     * @note also increases List m_size property and calls function to update other node positions
     */
    void insertNode(int line_num, T data) {
        if (line_num <= 0 || line_num > m_size + 1) return;

        Node * temp = new Node();
        temp->m_data = std::move(data);

        // If this is the first node being inserted
        if (m_size == 0) {
            temp->m_position = line_num;
            temp->m_next = nullptr;
            m_head = temp;

        } else {
            temp->m_position = line_num;
            Node * curr = m_head;
            Node * prev = nullptr;

            // update "m_head" if inserting above first node
            if (line_num == 1) {
                m_head = temp;
                temp->m_next = curr;

            } else {

                while ( curr != nullptr && curr->m_position != line_num) {
                    prev = curr;
                    curr = curr->m_next;
                }
                // new node gets inserted between the previous and current node
                prev->m_next = temp;
                temp->m_next = curr;
            }
        }

        // update list m_size and node positions to reflect the addition
        m_size++;
        updateNodePositions();
    }

    /**
     * Edits an existing Node in Linked List
     *
     * @param line_num position of node to edit
     * @param data text to replace the existing Node m_data
     */
    void editNode(int line_num, T data) {
        if (line_num <= 0 || line_num > m_size) return;

        Node * curr = m_head;
        while (curr != nullptr && curr->m_position != line_num)
            curr = curr->m_next;

        curr->m_data = std::move(data);
    }

    /**
     * Deletes an existing Node in Linked List
     *
     * @param line_num position of node to delete
     * @note also decreases List m_size property and calls function to update other node positions
     */
    void deleteNode(int position) {
        if (position <= 0 || position > m_size) return;

        Node * curr = m_head;
        Node * prev = nullptr;
        Node * temp;

        while (curr != nullptr && curr->m_position != position) {
            prev = curr;
            curr = curr->m_next;
        }

        // special case for first line in document ("m_head" must be accurate)
        if (position == 1) {
            temp = m_head;
            m_head = m_head->m_next;
            delete temp;

        } else {
            if (prev != nullptr && curr != nullptr) prev->m_next = curr->m_next;
            delete curr;
        }

        // update the m_size value + node positions of the list to reflect the deletion
        updateNodePositions();
        m_size--;
    }

private:
    struct Node {
        int m_position;
        T m_data;
        Node * m_next;
    };
    Node * m_head;

    /**
     * Iterates through all nodes in Linked List and updates their m_position
     *
     * @note called after inserting or deleting a node
     */
    void updateNodePositions() {
        Node *line = m_head;
        Node *prev = nullptr;

        while (line != nullptr) {
            if (prev == nullptr)
                line->m_position = 1;
            else
                line->m_position = prev->m_position + 1;

            prev = line;
            line = line->m_next;
        }
    }
};

#endif //ASSIGNMENT1_LINKED_LIST_H
