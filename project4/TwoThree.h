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


public:
    // constructor
    TwoThree(Node* r) {
        root = r;
    }

    // search function
    Node* Search(Node* node, int q) {

        if (root /* has no children */) {
            root.get_left().Search()
        }

    }

    // insert function
    void Insert(Node* node) {

    }

    // delete function
    void Delete(Node* node) {

    }
}
