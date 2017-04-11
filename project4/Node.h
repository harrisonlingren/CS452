using namespace std;

class Node {
private:
    // create empty Node
    void init() {

    }

public:
    // constructor (no val)
    Node() {
        init();
    }

    // constructor (with val)
    Node(int val) {

    }


    // is this a leaf...?
    bool is_leaf() {

    }


    // init all attributes
    int value;
    static const int left = 0; static const int right = 0;
    static const int middle = 0; static const int overflow = 0;


    // init parent & children leaves
    Node* parent; Node* child[6];
    int children_values[6]; int count_children;


    // get_left, middle, right functions
    Node* get_left() {

    }

    Node* get_middle() {

    }

    Node* get_right() {

    }


    // check if left, middle, right exists
    bool has_left() {

    }

    bool has_middle() {

    }

    bool has_right() {

    }


    // get Node value
    int value() {
        return value;
    }


    // add child
    void add_child(Node* node) {

    }


    // remove child
    Node* remove_child(int pos) {

    }
}
