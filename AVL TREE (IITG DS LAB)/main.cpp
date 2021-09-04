/*
	Assignmet 02 - DS LAB CS 513
	Roll No - 214101058
				Vijay Purohit
*/

#include <iostream>
#include <fstream>
#include <string>
//#include <conio.h>
//#include <curses.h>
#include<limits>
using namespace std;

#include <stack>
#include <queue>

#if defined(_WIN32)
  #define PAUSE "pause"
  #define CLR "cls"
#elif defined(unix) || defined(__unix__) || defined(__unix)
  #define PAUSE "read -p 'Press Enter to continue...' var"
  #define CLR "clear"
 #endif

//AVL Tree Node Structure 
#include "avl_nodes.h"

/********************************************************************************************
	AVL TREE Class
********************************************************************************************/
class ATree{
	private: 
		ANode *troot, *head;			// troot is dummy variable for root of the tree (head->rightc). 
		ANode *ptr, *newptr;			// other pointers for help in functions
		ANode *save, *loc, *par;
		
		stack<ANode*> pathToP;			//stack is used in deletion process for maintaing the path from root to deleted node, in order to balance tree.
	
		void t_print_graphviz(ANode *ptr, string file_name); // print by graphviz, ptr is provided in order to print different rooted tree.
		void t_display(ANode *ptr, int level); // display helper function for displaying on console.
		
		// function to delete all the nodes of the tree and make tree empty.
		ANode* makeEmpty(ANode *tnode) {
			ANode* ptr = tnode, *temp;
			if (tnode == NULL) { 	// if root of tree is NULL
				return NULL;
			}

			queue<ANode *> qLoAVL;		// queue level order AVL
			qLoAVL.push(ptr);
		
			while (qLoAVL.empty() == false)
			{
				temp = qLoAVL.front();
		        	cout << " "<< temp->key ;
				qLoAVL.pop();
				
				if (temp->leftc != nullptr)
					qLoAVL.push(temp->leftc);
				if (temp->rightc != nullptr)
					qLoAVL.push(temp->rightc);
					
				delete temp;
			}
	
			return NULL;
		}
		
	public:
		ATree(){
			head = new ANode(); // special header node appears at the top of the tree
			head->leftc = new ANode(0); // left link of head is used to keep track of the overall level.
			head->rightc = nullptr; // right link is a pointer to the root of the tree.
			troot = nullptr;
			ptr = nullptr; newptr = nullptr; 
			save = nullptr; loc = nullptr; par = nullptr;
		}
		
		//copy constructor
		ATree(const ATree &a);
		// assignment operator overloading for copying
		void operator=(const ATree &a_obj);
		ANode* copyLeftRightNode(ANode*);	// helper function for copying.
		
		//display on console. helper function
		void display() 		{	t_display(head->rightc, 1);	}
		
		//insert function
		void t_insert(int);
		
		//search function
		int search(int);
		
		//printing using graphviz, taking filename as input to save
		void print_graphviz(string file_name) {  t_print_graphviz(head->rightc, file_name) ; } 
		
		// Delete function
		void Delete(int);
		void case_a(ANode* , ANode* );		// node having zero or one child
		void case_b(ANode* , ANode* );		// node having both children.
		
		//Rotation Function
		ANode* Imbalance_LL(ANode*, ANode *, int , int );
		ANode* Imbalance_RR(ANode*, ANode *, int , int );
		ANode* Imbalance_LR(ANode*, ANode *, int , int );
		ANode* Imbalance_RL(ANode*, ANode *, int , int );
		
		// helper function to return value of pointers.
		ANode* returnPar() {
			return par;
		}
		ANode* returnLoc() {
			return loc;
		}
		int returnParKey() {
			return par->key;
		}
		int returnLocKey() {
			return loc->key;
		}
		
		//destructor
		~ATree() {
			cout << "\n Free Memory (tree), deleting: ";
			//head->rightc = makeEmpty(head->rightc);				// deleting all nodes
			delete head;
			troot = head = nullptr;
			
		}
};

//AVL CLASS Functions
#include "avl_functions.h"

/********************************************************************************************
	Main Function
********************************************************************************************/
int main(){
	
	string fn ; 		// filename for png file to be saved.
	string suffix ;    	// filename suffix to distinguish different files.
	
	char choice;			// choice exercised.
	int flag;			// flag for element search result.

	ATree avl_obj ;		// our AVL Tree object
		
	do {
		int inf;
		char ch;
		cout << endl << endl << endl;
		system(PAUSE);
		//cout << "Press \'Return\' to continue." << endl;
		//cin.flush();
		//cin.get();
		//string cmd = PAUSE;
    		//system((const char*)cmd.c_str());
    		
		system(CLR);
		
    	
		avl_obj.display();

		cout << "\n\n ---- AVL Tree Menu  ----";

		cout << "\n 1. Display - Console 			";
		cout << "\n 2. Default Tree - Insertion 			";

		cout << "\n\n i. INSERT an element k ";
		cout << "\n s. SEARCH an element k";
		cout << "\n d. DELETE an element k";
		cout << "\n\n p. Print Tree - GraphViz";

		cout << "\n n. Exit - Bye		\n\n  --Choice : ";
			cin >> ch;
		cout << "\n --------";
		
			switch (ch)
			{
				case '1' :    avl_obj.display();
						break;
				//Default Insert.
				case '2' : 
//								avl_obj.t_insert(50);
//								avl_obj.t_insert(30);
//								avl_obj.t_insert(80);
//								avl_obj.t_insert(20);
//								avl_obj.t_insert(40);
//								avl_obj.t_insert(70);
//								avl_obj.t_insert(100);
//								avl_obj.t_insert(10);
//								avl_obj.t_insert(25);
//								avl_obj.t_insert(35);
//								avl_obj.t_insert(45);
//								avl_obj.t_insert(60);
//								avl_obj.t_insert(32);
		//1						
//								avl_obj.t_insert(21);
//								avl_obj.t_insert(26);
//								avl_obj.t_insert(30);
//								avl_obj.t_insert(9);
//								avl_obj.t_insert(4);
//								avl_obj.t_insert(14);
//								avl_obj.t_insert(28);
//								avl_obj.t_insert(18);
//								avl_obj.t_insert(15);
//								avl_obj.t_insert(10);
//								avl_obj.t_insert(2);
//								avl_obj.t_insert(3);
//								avl_obj.t_insert(7);
		//2
//								avl_obj.t_insert(60);
//								avl_obj.t_insert(50);
//								avl_obj.t_insert(80);
//								avl_obj.t_insert(20);
//								avl_obj.t_insert(55);
//								avl_obj.t_insert(75);
//								avl_obj.t_insert(82);
//								
//								avl_obj.t_insert(10);
//								avl_obj.t_insert(32);
//								avl_obj.t_insert(53);
//								avl_obj.t_insert(56);
//								avl_obj.t_insert(73);
//								avl_obj.t_insert(76);
//								avl_obj.t_insert(81);
//								avl_obj.t_insert(83);
//								
//								avl_obj.t_insert(7);
//								avl_obj.t_insert(12);
//								avl_obj.t_insert(30);
//								avl_obj.t_insert(40);
//								avl_obj.t_insert(52);
//								avl_obj.t_insert(54);
//								avl_obj.t_insert(57);
//								avl_obj.t_insert(72);
//								avl_obj.t_insert(74);
//								avl_obj.t_insert(78);
//								avl_obj.t_insert(84);
//								
//								avl_obj.t_insert(6);
//								avl_obj.t_insert(8);
//								avl_obj.t_insert(14);
//								avl_obj.t_insert(45);
//								avl_obj.t_insert(51);
//								avl_obj.t_insert(71);
//								
//								avl_obj.t_insert(5);
	//3
								avl_obj.t_insert(20);
								avl_obj.t_insert(10);
								avl_obj.t_insert(30);
								avl_obj.t_insert(3);
								avl_obj.t_insert(15);
								avl_obj.t_insert(25);
								avl_obj.t_insert(40);
								avl_obj.t_insert(2);
								avl_obj.t_insert(9);
								avl_obj.t_insert(35);
//								
								
								cout <<"\n Default Tree is inserted. ";
						break;
						
				//Insert
				case 'i' :  cout <<"\n Enter Information (int) for new AVL Node .. : ";
					  			cin >> inf;		// Saving Info for the new node
					  			if (cin.fail()) {
    									//Not an int.
    									cout <<"\t"<<"Not a Integer:"<< endl;
    									cin.clear();
    									cin.ignore(numeric_limits<streamsize>::max(), '\n');
    									continue;
								}
								try {
										avl_obj.t_insert(inf);
								}
								catch (const char* msg) {
									cout <<"\t"<<msg<< endl;
								}
						break;
						
				//Search
				case 's' :	cout << "\n Enter a Node value to SEARCH : ";
								cin >> inf;
								if (cin.fail()) {
    									//Not an int.
    									cout <<"\t"<<"Not a Integer:"<< endl;
    									cin.clear();
    									cin.ignore(numeric_limits<streamsize>::max(), '\n');
    									continue;
								}
								flag = avl_obj.search(inf);
								
								if (flag == 1) {
									cout << "\n Search Successful for Element (" << inf << ").";
									if (avl_obj.returnPar() != NULL)
										cout << "\n 	Its parent value and address: " << avl_obj.returnParKey() << " , " << avl_obj.returnPar();
									cout << "\n	Its location: " << avl_obj.returnLocKey() << " , " << avl_obj.returnLoc();
								}
								else if (flag == -1) {
									cout << "\n Element (" << inf << ") Not Found.";
								}
								else
								cout << "\n Search Operation Unsuccessful.";
						break;
						
				//Delete
				case 'd' :	cout << "\n Enter a Node value to DELETE : ";
								cin >> inf;
								if (cin.fail()) {
    									//Not an int.
    									cout <<"\t"<<"Not a Integer:"<< endl;
    									cin.clear();
    									cin.ignore(numeric_limits<streamsize>::max(), '\n');
    									continue;
								}
							avl_obj.Delete(inf);
							
				break;
				
				//Print
				case 'p' : 
						cout << "\n Enter File Name suffix to distinguish your operation (w/o space): ";
							cin>>suffix;
							
						fn = "a_avl_"+suffix;;
						cout << "\n Printing Tree: ";
						
						avl_obj.print_graphviz(fn);
				break;

				case 'n' : cout << "\n Bye \n"; break;
				default  :   cout << "\n--Invalid Choice. Enter Again \n";
			}
			choice=ch;
		} while (choice != 'n');

	//getch();
	cin.get();
	cin.get();
	//system(PAUSE);
	return 0;
}


















