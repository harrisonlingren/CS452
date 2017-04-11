#include "Node.h"

using namespace std;

class TwoThree {
private:
    // root node
    Node* root;

    // absorb function
    void absorb(Node* one, Node* two) {

    }


    // discard function
    void discard(Node* node) {

    }


    // private recursive Search
    Node* priv_search(Node* node, int q) {

        // base case
        if (node.is_leaf()) { return node; }

        // check against left, right, middle
        if (node.get_left().get_value() <= q ) {
            priv_search(node.get_left(), q);

        } else if (node.get_middle().get_value() <= q) {
            priv_search(node.get_middle(), q);

        } else {
            priv_search(node.get_right(), q);

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
    void Insert(Node* node) {

    }

    // delete function
    void Delete(Node* node) {

    }
}
