// btree.cpp

#include <iostream>
#include "btreelib.h"
#include <ctime>
#include <string>
using namespace std;

int main()
{
    BTree tree = BTree();
	srand(time(NULL));
    while (true) {
		system("cls");
		tree.view();
        cout << " \n 1 - Fill up the tree, \n 2 - find the key in the tree, \n 3 - delete the key from the tree, \n 4 - exit";
        int option;
        cin >> option;
        switch (option) {
        case 1: {
			system("cls");
			int option;
			cout << "1 - Fill up the tree with random numbers, \n 2 - Enter numbers: ";
			cin >> option;
			if (option == 1) {
				int n;
				cout << "\n Enter the number of keys: ";
				cin >> n;
				for (int i = 0; i < n; i++)
					tree.insert(rand() % 150 + 1);
			}
			else {
				int n;
				cout << " \n How many keys do you want to add? ";
				cin >> n;
				for (int i = 0; i < n; i++) {
					int num;
					system("cls");
					cout << "Enter the key value: ";
					cin >> num;
					tree.insert(num);
				}
			}
            break;
        }
        case 2: {
			system("cls");
			tree.view();
			cout << "Enter the key you want to find: ";
			int key;
			cin >> key;
			bool status = tree.search(key);
			if (status)
				cout << "There is the key (" + to_string(key) + ") in B-Tree!" << endl;
			else
				cout << "There isn't such key in the B-Tree!" << endl;
			system("pause");
            break;
        }
        case 3: {
			system("cls");
			tree.view();
			cout << "Enter the key you want to delete: ";
			int key;
			cin >> key;
			tree.remove(key);
			tree.view();
			cout << "The key was successfully deleted!" << endl;
			system("pause");
            break;
        }
        case 4: {
            return 0;
            break;
        }
        }
    }
}