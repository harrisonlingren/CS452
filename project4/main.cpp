#include <iostream>
#include "TwoThree.h"
using namespace std;

int main() {
	cout << "the main is working\n";


	Node* root = new Node(50);
	TwoThree tree(root);
	cout << "created root node 50" << endl;

	tree.Print();

	cout << "inserting 25...";
	tree.Insert(25);
	cout << " done!\n\n";
	tree.Print();

	cout << "inserting 30...";
	tree.Insert(30);
	cout << " done!\n\n";
	//tree.Print();

	cout << "inserting 35...";
	tree.Insert(35);
	cout << " done!\n\n";
	//tree.Print();

	cout << "inserting a bunch of shit...";
	tree.Insert(5);
	tree.Insert(10);
	tree.Insert(2);
	tree.Insert(7);
	cout << " done!\n\n";

	tree.Print(); cout << endl;

	cout << "deleting 10...";
	tree.Delete(10);
	cout << " done!\n\n";
	//tree.Print();

	cout << "deleting 35...";
	tree.Delete(35);
	cout << " done!\n\n";
	//tree.Print();

	cout << "deleting 30...";
	tree.Delete(30);
	cout << " done!\n\n";

	tree.Print();

	return 0;
}
