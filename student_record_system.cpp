#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;

struct Student {
    int enrolment_id;
    string name;
    string department;
    Student* left;
    Student* right;

    Student(int id, string n, string dept) {
        enrolment_id = id;
        name = n;
        department = dept;
        left = NULL;
        right = NULL;
    }
};

class StudentBST {
private:
    Student* root;
    int comparisons;

    Student* insertHelper(Student* node, int id, string name, string dept, bool &success) {
        if (node == NULL) {
            success = true;
            return new Student(id, name, dept);
        }
        if (id == node->enrolment_id) {
            cout << "Error: Duplicate Enrolment ID (" << id << ") found! Record rejected.\n";
            success = false;
            return node;
        }
        if (id < node->enrolment_id) {
            node->left = insertHelper(node->left, id, name, dept, success);
        } else {
            node->right = insertHelper(node->right, id, name, dept, success);
        }
        return node;
    }

    Student* findMin(Student* node) {
        while (node->left != NULL) {
            node = node->left;
        }
        return node;
    }

    Student* deleteHelper(Student* node, int id, bool &deleted) {
        if (node == NULL) {
            deleted = false;
            return NULL;
        }
        if (id < node->enrolment_id) {
            node->left = deleteHelper(node->left, id, deleted);
        } else if (id > node->enrolment_id) {
            node->right = deleteHelper(node->right, id, deleted);
        } else {
            deleted = true;
            if (node->left == NULL && node->right == NULL) {
                delete node;
                return NULL;
            } else if (node->left == NULL) {
                Student* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == NULL) {
                Student* temp = node->left;
                delete node;
                return temp;
            } else {
                Student* temp = findMin(node->right);
                node->enrolment_id = temp->enrolment_id;
                node->name = temp->name;
                node->department = temp->department;
                node->right = deleteHelper(node->right, temp->enrolment_id, deleted);
            }
        }
        return node;
    }

    void inorderHelper(Student* node) {
        if (node != NULL) {
            inorderHelper(node->left);
            cout << left << setw(15) << node->enrolment_id 
                 << setw(20) << node->name 
                 << setw(15) << node->department << "\n";
            inorderHelper(node->right);
        }
    }

    void preorderHelper(Student* node) {
        if (node != NULL) {
            cout << node->enrolment_id << " (" << node->name << ")  ";
            preorderHelper(node->left);
            preorderHelper(node->right);
        }
    }

    void postorderHelper(Student* node) {
        if (node != NULL) {
            postorderHelper(node->left);
            postorderHelper(node->right);
            cout << node->enrolment_id << " (" << node->name << ")  ";
        }
    }

public:
    StudentBST() {
        root = NULL;
        comparisons = 0;
    }

    void insertRecord(int id, string name, string dept) {
        bool success = false;
        root = insertHelper(root, id, name, dept, success);
        if (success) {
            cout << "Record inserted successfully for ID: " << id << "\n";
        }
    }

    void searchRecord(int id) {
        comparisons = 0;
        Student* curr = root;
        while (curr != NULL) {
            comparisons++;
            if (curr->enrolment_id == id) {
                cout << "\n--- Student Record Found ---\n";
                cout << "Enrolment ID : " << curr->enrolment_id << "\n";
                cout << "Name         : " << curr->name << "\n";
                cout << "Department   : " << curr->department << "\n";
                cout << "Total Comparisons Made: " << comparisons << "\n";
                return;
            } else if (id < curr->enrolment_id) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
        cout << "\nStudent with Enrolment ID " << id << " not found.\n";
        cout << "Total Comparisons Made: " << comparisons << "\n";
    }

    void deleteRecord(int id) {
        bool deleted = false;
        root = deleteHelper(root, id, deleted);
        if (deleted) {
            cout << "Record with ID " << id << " deleted successfully.\n";
        } else {
            cout << "Record with ID " << id << " not found!\n";
        }
    }

    void displayInorder() {
        if (root == NULL) {
            cout << "BST is empty.\n";
            return;
        }
        cout << "\n----------------------------------------------------\n";
        cout << left << setw(15) << "Enrolment ID" << setw(20) << "Name" << setw(15) << "Department" << "\n";
        cout << "----------------------------------------------------\n";
        inorderHelper(root);
        cout << "----------------------------------------------------\n";
    }

    void displayTraversals() {
        if (root == NULL) {
            cout << "BST is empty.\n";
            return;
        }
        cout << "\n--- Preorder Traversal ---\n";
        preorderHelper(root);
        cout << "\n\n--- Postorder Traversal ---\n";
        postorderHelper(root);
        cout << "\n";
    }

    void runTestCases() {
        cout << "\n=== EXECUTING MANDATORY TEST CASES ===\n";
        
        cout << "\n[Test Case 1: Search Existing vs Non-Existing Record]\n";
        searchRecord(105);
        searchRecord(999);

        cout << "\n[Test Case 2: Duplicate Key Insertion]\n";
        insertRecord(105, "Duplicate Test", "CSE");

        cout << "\n[Test Case 3: Leaf Node Deletion (ID 101)]\n";
        deleteRecord(101);

        cout << "\n[Test Case 4: Single Child Node Deletion (ID 110)]\n";
        deleteRecord(110);

        cout << "\n[Test Case 5: Two Children Node Deletion (ID 105)]\n";
        deleteRecord(105);

        cout << "\nUpdated Inorder Traversal after Deletions:\n";
        displayInorder();
    }
};

void runEfficiencyComparison() {
    cout << "\n======================================================\n";
    cout << "   COMPARING BALANCED VS SKEWED INSERTION ORDER      \n";
    cout << "======================================================\n";

    StudentBST balancedTree;
    int balancedIDs[] = {105, 102, 108, 101, 103, 107, 110};
    for(int i = 0; i < 7; i++) {
        int id = balancedIDs[i];
        stringstream ss;
        ss << "Student_" << id;
        balancedTree.insertRecord(id, ss.str(), "CSE");
    }

    StudentBST skewedTree;
    int skewedIDs[] = {101, 102, 103, 105, 107, 108, 110};
    for(int i = 0; i < 7; i++) {
        int id = skewedIDs[i];
        stringstream ss;
        ss << "Student_" << id;
        skewedTree.insertRecord(id, ss.str(), "CSE");
    }

    cout << "\nSearching for Enrolment ID 110:";
    cout << "\n1. In Balanced-looking BST:";
    balancedTree.searchRecord(110);

    cout << "\n2. In Sorted/Skewed BST:";
    skewedTree.searchRecord(110);
}

int main() {
    StudentBST bst;

    bst.insertRecord(105, "Aarav Sharma", "CSE");
    bst.insertRecord(102, "Diya Patel", "ECE");
    bst.insertRecord(108, "Rohan Verma", "ME");
    bst.insertRecord(101, "Ananya Gupta", "CSE");
    bst.insertRecord(103, "Kabir Singh", "CE");
    bst.insertRecord(107, "Isha Nair", "ECE");
    bst.insertRecord(110, "Siddharth Rao", "CSE");
    bst.insertRecord(104, "Meera Joshi", "IT");
    bst.insertRecord(106, "Aditya Kumar", "ME");
    bst.insertRecord(109, "Priya Reddy", "IT");

    int choice;
    while (true) {
        cout << "\n============================================\n";
        cout << "  STUDENT RECORD SEARCH SYSTEM (BST MENU)   \n";
        cout << "============================================\n";
        cout << "1. Insert New Student Record\n";
        cout << "2. Search Student Record by ID\n";
        cout << "3. Delete Student Record\n";
        cout << "4. Display Records (Inorder - Sorted)\n";
        cout << "5. Display Preorder & Postorder Traversals\n";
        cout << "6. Compare Balanced vs Skewed BST Efficiency\n";
        cout << "7. Run Mandatory Test Cases\n";
        cout << "8. Exit\n";
        cout << "Enter choice (1-8): ";
        if (!(cin >> choice)) break;

        if (choice == 1) {
            int id;
            string name, dept;
            cout << "Enter Enrolment ID: ";
            cin >> id;
            cin.ignore();
            cout << "Enter Student Name: ";
            getline(cin, name);
            cout << "Enter Department: ";
            getline(cin, dept);
            bst.insertRecord(id, name, dept);
        } else if (choice == 2) {
            int id;
            cout << "Enter Enrolment ID to search: ";
            cin >> id;
            bst.searchRecord(id);
        } else if (choice == 3) {
            int id;
            cout << "Enter Enrolment ID to delete: ";
            cin >> id;
            bst.deleteRecord(id);
        } else if (choice == 4) {
            bst.displayInorder();
        } else if (choice == 5) {
            bst.displayTraversals();
        } else if (choice == 6) {
            runEfficiencyComparison();
        } else if (choice == 7) {
            bst.runTestCases();
        } else if (choice == 8) {
            cout << "Exiting program.\n";
            break;
        } else {
            cout << "Invalid choice! Please try again.\n";
        }
    }
    return 0;
}
