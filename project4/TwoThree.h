#include "Node.h"
#include <iostream>

using namespace std;

class TwoThree {
private:
    // root node
    Node* root;

    // absorb function
    Node* absorb(Node* one, Node* two) {
        if (root == two) {
            Node* mega;
            mega->add_child(one);
            mega->add_child(two);
            return mega;
        }

        if (two->parent->count_children < 3) {
            // "easy" insertion
            if (two->parent->get_middle() == NULL) {
                two->parent->add_child(one);
            } else {

            }

            print_tree();
            return NULL;

        } else {
            /* Overfill p by inserting a. Create a p-left and move the two smallest children of p to p->left */
            absorb(two->parent->get_left(), two->parent);
        }
    }

    void print_tree() {
        cout << "we're printing a tree here! nice!";
    }


    // discard function
    void discard(Node* node) {

    }


    // private recursive Search
    Node* priv_search(Node* node, int q) {

        // base case
        if (node->is_leaf()) { return node; }

        // check against left, right, middle
        if (node->get_left()->get_value() <= q ) {
            priv_search(node->get_left(), q);

        } else if (node->get_middle()->get_value() <= q) {
            priv_search(node->get_middle(), q);

        } else {
            priv_search(node->get_right(), q);

        }
    }


public:
    // constructor
    TwoThree(Node* r) {
        root = r;
    }

    // search function
    Node* Search(int q) {
        return priv_search(root, q);
    }

    // insert function
    void Insert(int val) {
        Node* n = new Node(val);
        Node* s = Search(val);
        if (n == s) {
            return;
        } absorb(n, s);
    }

    // delete function
    void Delete(int val) {

    }
};
