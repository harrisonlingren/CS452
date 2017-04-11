using namespace std;

class Leaf {
private:
    // create empty Leaf
    void init() {

    }

public:
    // constructor (no val)
    Leaf() {
        init();
    }

    // constructor (with val)
    Leaf(int val) {

    }


    // is this a leaf...?
    bool is_leaf() {

    }


    // init all attributes
    int value;
    static const int left = 0; static const int right = 0;
    static const int middle = 0; static const int overflow = 0;


    // init parent & children leaves
    Leaf* parent; Leaf* child[6];
    int children_values[6]; int count_children;


    // get_left, middle, right functions
    Leaf* get_left() {

    }

    Leaf* get_middle() {

    }

    Leaf* get_right() {

    }


    // check if left, middle, right exists
    bool has_left() {

    }

    bool has_middle() {

    }

    bool has_right() {

    }


    // get Leaf value
    int value() {

    }


    // add child
    void add_child(Leaf* leaf) {

    }


    // remove child
    Leaf* remove_child(int pos) {

    }
}
