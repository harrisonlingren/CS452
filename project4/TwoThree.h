#include "Node.h"
#include <iostream>
#include <queue>

using namespace std;

class TwoThree {
 private:
  // root node
  Node* root;

  // absorb function
  void absorb(Node* one, Node* two) {
    if (two == root) {
      Node* mega = new Node();
      mega->add_child(one);
      mega->add_child(two);
      root = mega;

    } else {
      Node* p = two->get_parent();
      p->add_child(one);

      int count_children = p->count_children;
      if (count_children > 3) {
        Node* p_l = new Node();
        int count_left = count_children / 2;
        int count_right = count_children - count_left;

        for (int x = 0; x < count_left; x++) {
          p_l->add_child(p->child[x]);
          p->child[x] = p->child[x + count_left];
        }

        if (count_left < count_right) {
          p->child[count_left] = p->child[count_children - 1];
        }

        for (int x = count_right; x < count_children; ++x) {
          p->child[x] = NULL;
        }

        p->count_children = count_right;
        p->calc_child_values();
        absorb(p_l, p);

      } else if (p->parent != NULL) {
        p->parent->propagate_children();
      }
    }
  }

  void print_one(Node* node) {
    if (node->is_leaf()) {
      node->print();
      return;

    } else {
      for (int x = 0; x < node->count_children; x++) {
        print_one(node->child[x]);
      }
    }
  }

  // discard function
  void discard(Node* node) {
    // let p be the parent of a
    Node* p = node->get_parent();

    // Remove the connection between p and a
    for (int x = 0; x < p->count_children; x++) {
      if (p->child[x] == node) {
        p->remove_child(x);
      }
    }

    delete (node);
    bool recurse_flag;  // so we know whether to keep going

    /*if (p has 2 children)
        return T; // might have to "re-adjust" p (and up) */
    // p has 1 child

    if (p->count_children >= 2) {
      return;
    } else if (p->get_parent() == NULL) {
      if (!p->child[0]->is_leaf()) {
        root = p->child[0];
        delete (p);
      }
      return;
    }

    // let totchildren = number of children among all siblings of p (including
    // p)     // totchildren  [3,7]

    Node* totchildren = p->get_parent();
    int num_totchildren = 0;
    int num_parent;

    for (int x = 0; x < totchildren->count_children; x++) {
      num_totchildren += totchildren->child[x]->count_children;
      if (totchildren->child[x]->count_children == 1) {
        num_parent = x;
      }
    }

    if (num_totchildren >= 4) {
      if (totchildren->count_children == 2) {
        if (totchildren->child[0]->count_children == 3) {
          totchildren->shift_children(0, 1);
        } else {
          totchildren->shift_children(1, 0);
        }

      } else if (num_totchildren == 5) {
        int y;
        if (num_parent == 2) {
          y = 1;
        } else {
          y = num_parent + 1;
        }

        totchildren->shift_children(num_parent, y);

      } else {
        if (num_parent == 0) {
          totchildren->shift_children(1, 0);
          if (totchildren->child[1]->count_children == 1) {
            totchildren->shift_children(2, 1);
          }

        } else if (num_parent == 2) {
          totchildren->shift_children(1, 2);
          if (totchildren->child[1]->count_children == 1) {
            totchildren->shift_children(0, 1);
          }

        } else {
          int y;
          if (totchildren->child[0]->count_children >
              totchildren->child[2]->count_children) {
            y = 0;
          } else {
            y = 1;
          }
          totchildren->shift_children(y, 1);
        }
      }

    } else {
      if (num_parent == 0) {
        totchildren->shift_children(0, 1);
      } else {
        totchildren->shift_children(1, 0);
      }

      discard(p);
      recurse_flag = true;
    }

    if (!recurse_flag) {
      totchildren->propagate_children();
    }
  }

  // private recursive Search
  Node* priv_search(Node* node, int q) {
    // base case
    if (node->is_leaf()) {
      return node;
    }

    // check against left, right, middle
    if (node->get_left()->get_value() <= q) {
      priv_search(node->get_left(), q);

    } else if (node->has_middle() && node->get_middle()->get_value() <= q) {
      priv_search(node->get_middle(), q);

    } else {
      priv_search(node->get_right(), q);
    }
  }

 public:
  // constructor
  TwoThree(Node* r) { root = r; }

  // search function
  Node* Search(int q) { return priv_search(root, q); }

  // insert function
  void Insert(int val) {
    Node* n = new Node(val);
    Node* s = Search(val);
    if (n == s) {
      return;
    }
    absorb(n, s);
  }

  // delete function
  void Delete(int val) {
    Node* node = Search(val);
    if (node->get_value() == val) {
      discard(node);
    }
  }

  void Print() {
    cout << endl;

    if (root == NULL)
      cout << "Empty is tree.";

    else {
      vector<Node*> v;  // list to print
      vector<Node*> c;  // list to hold children of v, for later use
      v.push_back(root);  // first level
      while (!v.empty()) {
        for (int x = 0; x < v.size(); x++) {  // print all nodes
          cout << "   ";
          v[x]->print();
          cout << ", ";
        }
        cout << endl;  // end this level
        for (int i = 0; i < v.size(); i++) {  // find children of nodes in v
          if (v[i]->left) c.push_back(v[i]->get_left());
          if (v[i]->middle) c.push_back(v[i]->get_middle());
          if (v[i]->right) c.push_back(v[i]->get_right());
        }
        v.clear();
        v.swap(c);
        c.clear();
      }
    }

    cout << endl;
  }
};
