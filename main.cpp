//g++ -Wall -Werror -std=c++11  main.cpp 

#include<iostream>
#include<fstream>
#include<string>
#include <math.h>       

using namespace std;

struct Node{ // Node struct that is the node of the red black tree 

    Node* left; //left child
    Node* right; //right child
    Node* parent; //parent
    string name; //name
    bool color; //color
    int point;
    int rebound;
    int assist;
};

static Node* max_point = NULL; //will store max pointed user
static Node* max_assists = NULL; //will store max assisted user
static Node* max_rebs = NULL; // will store max rebounded user
static int first_season = 1;

class BasketballDB{ // Basketball Database 
    public:
    Node* root;
    Node* max_point;
    Node* max_assists;
    Node* max_rebs;
    BasketballDB(){
        root = NULL;
        max_point = NULL;
        max_assists = NULL;
        max_rebs = NULL;
    }
};

void deleteTree(Node* node)  //delete nodes recursively
{  
    if (node == NULL) return;  
  
    deleteTree(node->left);  //deletes from left subtree
    deleteTree(node->right);  //deletes from right subtree
    delete node; //delete selected node
}  

void printPreorder(Node* node, int depth=0) 
{  //this is the function that prints in preorder with indentation of first season

    if (node == NULL) 
        return; 
    if(first_season == 1){ //only print first season
        string color;
        for(int i=0; i<depth; i++)
            cout <<"-"; //print depth
        depth++;
        if (node->color == 0){ // if color is black
            color = "(BLACK)";
        } else { // if color is red
            color = "(RED)"; 
        }
        cout<< color << "  " << node->name << endl; //print color and name
    }
    if (max_point == NULL){ //first player of the season
        max_point = node;
        max_assists = node;
        max_rebs = node;
    }

    if(node->assist > max_assists->assist){ //for finding max assisted
        max_assists = node;
    }

    if(node->point > max_point->point){ //for finding max pointed
        max_point = node;
    }

    if(node->rebound > max_rebs->rebound){ //for finding max rebounded
        max_rebs = node;
    }

    printPreorder(node->left, depth);  //recurrence on the left child since this is preorder
  
    printPreorder(node->right, depth); //then recurrence on the right child since this is preorder
}

Node* GetParent(Node* &n){ //it gets the parent of node
    if (n->parent){
        return n->parent;
    } else {
        return NULL; //there is no parent
    }
}

Node* GetGrandParent(Node* &n){ //gets the grandparent of node
    Node* parent = GetParent(n); //parent
    return GetParent(parent); //parent of parent=grandparent
}

Node* GetSibling(Node* &n){ //gets sibling of the node
    Node* parent = GetParent(n);
    if (parent != NULL){
        if (n == parent->left){
            return parent->right;
        } else {
            return parent->left;
        }
    } else {
        return NULL;
    }
}

Node* GetUncle(Node* &n){ //gets uncle
    Node* parent = GetParent(n);
    return GetSibling(parent);
}

void RotateLeft(Node* n) { //rotates left
    Node* new_node = n->right; //this will become the new node 
    Node* parent_node = GetParent(n); //new node will be child of parents parent
    
    if (new_node == NULL){ // Since the leaves of a red-black tree are empty, they cannot become internal nodes.
        exit(1);
    }

    n->right = new_node->left; //right child of n will be left child of nnew
    new_node->left = n; //n will be left child of n
    n->parent = new_node; //nnew will be parent of n

   
    if (n->right != NULL) {  //Handle other child/parent pointers.
        n->right->parent = n;
    }

    if (parent_node != NULL) { //if new_node is not root now that means n was not root
        if (n == parent_node->left) {
            parent_node->left = new_node;
        } else if (n == parent_node->right) {
            parent_node->right = new_node;
        }
    }
    new_node->parent = parent_node;
}

void RotateRight(Node* n) {
    Node* new_node = n->left;
    Node* parent = GetParent(n);

    if (new_node == NULL){ // Since the leaves of a red-black tree are empty, they cannot become internal nodes.
        exit(1);
    }

    n->left = new_node->right; //left child of parent will be right child of new node
    new_node->right = n; //parent will be right child of new node
    n->parent = new_node; //parent of parent will be new node.

    // Handle other child/parent pointers.
    if (n->left != NULL) { //if parent has left child
        n->left->parent = n;
    }

    // Initially n could be the root.
    if (parent != NULL) { //if parent was not root
        if (n == parent->left) {  //if parent was left child
            parent->left = new_node;
        } else if (n == parent->right) { //if parent was right child
            parent->right = new_node; 
        }
    }
    new_node->parent = parent; //make parent of new node as grandparent
}

void InsertCase4Step2(Node* n) {
  Node* p = GetParent(n);
  Node* g = GetGrandParent(n);

  if (n == p->left) {
    RotateRight(g);
  } else {
    RotateLeft(g);
  }
  p->color = false;
  g->color = true;
}

void InsertCase4(Node* n) {
  Node* p = GetParent(n);
  Node* g = GetGrandParent(n);

  if (n == p->right && p == g->left) { //
    RotateLeft(p);
    n = n->left;
  } else if (n == p->left && p == g->right) {
    RotateRight(p);
    n = n->right;
  }
  InsertCase4Step2(n);
}

void UpdateTree(Node* n) {
  if (GetParent(n) == NULL) { //if no root
        n->color = false; //make root black
    } else if (GetParent(n)->color == false) { //if parent is black 
        //DO NOTHING but i did not deleted this condition if I delete i need to add parent red condition other else ifs
    } else if (GetUncle(n) != NULL && GetUncle(n)->color == true) { //If uncle is red and parent is red
        GetParent(n)->color = false; //make parent color black
        GetUncle(n)->color = false; //make uncle color black
        GetGrandParent(n)->color = true; //make grandparent red
        UpdateTree(GetGrandParent(n)); //if in the new mode, grandparent causes 2 red blacks conseq. tree must be uploaded.
    } else { //if parent is red and uncle is black
        InsertCase4(n); //there is 2 cases...
    }
}


void addPlayer(Node* &root, Node* newNode){
    
    if (root == NULL){
        root = newNode;
        UpdateTree(newNode);
    } else {
        if (newNode->name != root->name) { //if user name is not same
            if (newNode->name < root->name){
                if(root->left != NULL){ // go left child
                    addPlayer(root->left, newNode);
                    return;
                } else { //leftmost child is found
                    root->left = newNode;
                    newNode->parent = root;
                    UpdateTree(newNode);
                }
            } else{ //go right child
                if(root->right != NULL){
                    addPlayer(root->right, newNode);
                    return;
                } else { //if it is the point where righmost is found
                    root->right = newNode;
                    newNode->parent = root;
                    UpdateTree(newNode);
                }
            }
        } else { //if there is a user with this name update its informations
            root->assist += newNode->assist;
            root->point += newNode->point;
            root->rebound += newNode->rebound;
            delete newNode;
        }
    } 
}

int main(int argc, char** argv){
    ifstream file;
	file.open(argv[1]);

    if (!file){
		cerr << "File cannot be opened!";
		exit(1);
	}

    BasketballDB myDB; //basketball database
    string line;
    getline(file, line);

    string season;
    string ex_season;
    string name;
    string team;

    int rebound;
    int assist;
    int point;

    /****PURPOSE OF THESE LINES ARE TAKING THE FIRST LINE FROM CSV*/

    getline(file, season,','); 
    ex_season = season;

    getline(file, name,',');

    getline(file, team,',');

    getline(file, line,',');
    rebound = stoi(line);

    getline(file, line,',');
    assist = stoi(line);

    getline(file, line,'\n');
    point = stoi(line);
    /***********************************************/

    //new node is created
    Node* newNode = new Node;
    newNode->name = name;
    newNode->rebound = rebound;
    newNode->assist = assist;
    newNode->point = point;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    newNode->color = true; //node is colored as red
    

    addPlayer(myDB.root,newNode); //first node is added to the root

    while(!file.eof()){

        getline(file, season,','); //season is read

        getline(file, name,','); //player name is read

        getline(file, team,','); //team name is read

        getline(file, line,','); //rebound amount is read
        rebound = stoi(line);

        getline(file, line,','); //assist amount is read
        assist = stoi(line);

        getline(file, line,'\n'); //point amount is read
        point = stoi(line);

        if (ex_season != season){ // if newcoming data is at the next season, print the current max users.
            printPreorder(myDB.root);
            cout <<"End of the " << ex_season <<" Season" << endl;
            cout <<"Max Points: " << max_point->point <<" - Player Name: " << max_point->name << endl;
            cout <<"Max Assists: " << max_assists->assist <<" - Player Name: " << max_assists->name << endl;
            cout <<"Max Rebs: " << max_rebs->rebound <<" - Player Name: " << max_rebs->name << endl;
            max_point = NULL;
            max_assists = NULL;
            max_rebs = NULL;
            ex_season = season;
            first_season = 0;
        }

        //new node is created
        Node* newNode = new Node;
        newNode->name = name;
        newNode->rebound = rebound;
        newNode->assist = assist;
        newNode->point = point;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->parent = NULL;
        newNode->color = true; //node is colored as red
        
        addPlayer(myDB.root,newNode);

        while(myDB.root -> parent != NULL){ //after red black tree insertion, if root is changed!
            myDB.root = myDB.root->parent;
        }
    }

    printPreorder(myDB.root); // this is for the last season
    cout <<"End of the " << ex_season <<" Season" << endl;
    cout <<"Max Points: " << max_point->point <<" - Player Name: " << max_point->name << endl;
    cout <<"Max Assists: " << max_assists->assist <<" - Player Name: " << max_assists->name << endl;
    cout <<"Max Rebs: " << max_rebs->rebound <<" - Player Name: " << max_rebs->name << endl;
    max_point = NULL;
    max_assists = NULL;
    max_rebs = NULL;

    deleteTree(myDB.root); //clear dynamic tree
    file.close(); //close file

    return 0;
}