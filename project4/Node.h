using namespace std;

class Node {
private:
    // create empty Node
    void init() {
        new_leaf();
        value = -1;
    }

    void new_leaf() {
        for (size_t x = 0; x < 6; x++) {
            children[x] = NULL;
            children_values[x] = -1;
        } count_children = 0;
    }

    // REDO THIS - sort children to balance all nodes
    void sort_children() {
        Node* t;
        for (size_t x = 0; x < count_children; x++) {
            for (size_t y = 0; y < count_children; y++) {
                if (children[j].get_value() > children[j+1].get_value()) {
                    t = children[j];
                    children[j] = children[j+1];
                    children[j+1] = temp;
                }
            }
        }
    }

    // calculate new values for children
    void calc_child_values() {
        for (size_t x = 0; x < 6; x++) {
            if (i < count_children) { calc_value(i); }
            else { children_values[i] = -1; }
        }
    }

    void calc_value(int a) {
        children_values[a] = children[a].get_value();
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
        if (value == -1) { return false; }
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
        if (has_left()) {
            return children[0];

        } else { return NULL; }
    }

    Node* get_middle() {
        if (has_middle()) {
            return children[1];

        } else { return NULL; }
    }

    Node* get_right() {
        if (has_right() && has_middle()) {
            return children[2];

        } else if (has_right()) {
            return children[1];

        } else { return NULL; }
    }


    // check if left, middle, right exists
    bool has_left() {
        if (is_leaf()) { return false; }
        else { return true; }
    }

    bool has_middle() {
        if (has_right() && children >= 3) { return true; }
        else { return false; }
    }

    bool has_right() {
        if (has_left() && children >= 2) { return true; }
        else { return false; }
    }


    // get Node value
    int get_value() {
        if (is_leaf()) { return value; }
        else {
            if (count_children == 3) {
                return children_values[2];
            } else if (count_children == 2) {
                return children_values[1]
            } else {
                return children_values[0];
            }
        }
    }


    // add child
    void add_child(Node* node) {
        children[count_children++] = node;
        value = -1;
        node.set_parent(this);
        sort_children(); calc_child_values();
    }

    // remove child
    Node* remove_child(int pos) {

    }
}
