#include <cstddef>
using namespace std;

class Node {
private:
    // create empty Node
    void init() {
        new_leaf();
        value = -1;
    }

    void new_leaf() {
        for (int x = 0; x < 6; x++) {
            child[x] = NULL;
            children_values[x] = -1;
        } count_children = 0;
    }

    // REDO THIS - sort children to balance all nodes
    void sort_children() {
        Node* t;
        for (int x = 0; x < count_children; x++) {
            for (int y = 0; y < count_children; y++) {
                if (child[y]->get_value() > child[y+1]->get_value()) {
                    t = child[y];
                    child[y] = child[y+1];
                    child[y+1] = t;
                }
            }
        }
    }

    // calculate new values for children
    void calc_child_values() {
        for (int x = 0; x < 6; x++) {
            if (x < count_children) { calc_value(x); }
            else { children_values[x] = -1; }
        }
    }

    void calc_value(int a) {
        children_values[a] = child[a]->get_value();
    }


public:
    // constructor (no val)
    Node() { init(); }

    // constructor (with val)
    Node(int v) {
        value = v;
        new_leaf();
    }


    // is this a leaf...?
    bool is_leaf() {
        if (count_children == 0) { return true; }
        else { return true; }
    }


    // init all attributes
    int value;
    static const int left = 0; static const int right = 0;
    static const int middle = 0; static const int overflow = 0;


    // init parent & children leaves
    Node* parent; Node* child[6];
    int children_values[6]; int count_children;

    // set parent method
    void set_parent(Node* node) {
        parent = node;
    }


    // get_left, middle, right functions
    Node* get_left() {
        if (has_left()) { return child[0]; }
        else { return NULL; }
    }

    Node* get_middle() {
        if (has_middle()) { return child[1]; }
        else { return NULL; }
    }

    Node* get_right() {
        if (has_middle()) { return child[2]; }
        else if (has_right()) { return child[1]; }
        else { return NULL; }
    }


    // check if left, middle, right exists
    bool has_left() {
        if (is_leaf()) { return false; }
        else { return true; }
    }

    bool has_middle() {
        if (has_right() && count_children >= 3) { return true; }
        else { return false; }
    }

    bool has_right() {
        if (has_left() && count_children >= 2) { return true; }
        else { return false; }
    }


    // get Node value
    int get_value() {
        if (is_leaf()) { return value; }
        else {
            if (count_children == 3) {
                return children_values[2];
            } else if (count_children == 2) {
                return children_values[1];
            } else {
                return children_values[0];
            }
        }
    }


    // add child
    void add_child(Node* node) {
        child[count_children++] = node;
        value = -1;
        node->set_parent(this);
        sort_children(); calc_child_values();
    }

    // remove child
    Node* remove_child(int pos) {
        return NULL;
    }

    // shift children
    void shift_children(int oldC, int newC) {

    }
};
