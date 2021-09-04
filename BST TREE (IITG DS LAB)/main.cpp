/*
	Assignmet 01 - DS LAB CS 513
	Roll No - 214101058

*/

#include <bits/stdc++.h>
#include <fstream>
#include<unordered_map>
#include <conio.h>
using namespace std;


//BST Node
struct BNode {
	int key;
	BNode* left;
	BNode* right;
	bool isLeftTh, isRightTh;
	int lcnt, rcnt;
};

//LinkedList Node
struct LNode {
	int info;
	LNode* next;
};

class LnkLst {
private:
	LNode  *start, *ptr, *newptr, *save, *rear;
	void l_Display(LNode *np)
	{
		while (np != NULL)
		{
			cout << np->info << " -> ";
			np = np->next;
		}
	}

public:
	LnkLst() {
		start = rear = NULL;
		ptr = newptr = NULL;
		save = NULL;
	}

	LNode *Create_New_Node(int n)
	{
		ptr = new LNode;
		if (ptr == NULL) {
			throw "Heap Memory Full";
		}
		ptr->info = n;
		ptr->next = NULL;
		return ptr;
	}

	void Insert_End(LNode *np)
	{
		if (start == NULL)
			start = rear = np  ;
		else
		{
			rear->next = np;
			rear = np;
		}
	}

	void Display()
	{
		l_Display(start);
	}

	void DelNode_Beg()
	{
		if (start == NULL )
		{
			cout << "\n UnderFlow !!! \n ";
			return;;
		}

		ptr = start;
		if (start->next == NULL)
			start = rear = NULL;
		else
			start = start->next;
		delete ptr;
	}

	void cleanList() {
		while (start != NULL)
			DelNode_Beg();
	}

	~LnkLst() {
		cout << "\n  Deleting LinkList Objects: ";
		while (start != NULL)
			DelNode_Beg();
	}
};


class ThBST {
private:
	BNode *troot;
	BNode *ptr, *newptr;
	BNode *save, *loc, *par;
	
	BNode *ctroot1, *ctroot2;

	BNode *Create_New_Node(int i) {
		ptr = new BNode;
		if (ptr == NULL) {
			throw "Heap Memory Full";
		}
		ptr->key = i;
		ptr->left = ptr->right = NULL;
		// TRUE means Left or Right pointer are Threads
		ptr->isLeftTh = ptr->isRightTh = true;
		ptr->lcnt = ptr->rcnt = 0;
		return ptr;
	}

	BNode* makeEmpty(BNode *tnode) {

		BNode* ptr = tnode, *temp;
		// if root of tree is NULL
		if (tnode == NULL) {
			return NULL;
		}
		while (ptr->isLeftTh == false)
			ptr = ptr->left;
		while (ptr != NULL) {
			cout << " " << ptr->key;
			temp = ptr;
			ptr = inOrderSuccessor(ptr);
			delete temp;
		}
		return NULL;
	}

	void t_inOrder(BNode* tnode);
	void t_preOrder(BNode* tnode);
	void t_postOrder(BNode* tnode);
	void t_inOrderReversal(BNode* ptr);

	void t_display(BNode* tnode, int level);
	void t_print_graphviz(BNode *ptr, string fname);

	void t_CountNodes(BNode *tnode);
	void t_CountLeaves(BNode *tnode);
	int t_getCountAtLevel(BNode *tnode, int curr, int desired);
	int t_maxDepth(BNode *tnode);

	void t_delNode(int f);
	void case_a(BNode *tnode1, BNode *tnode2);
	void case_b(BNode *tnode1, BNode *tnode2);
	void case_c(BNode *tnode1, BNode *tnode2);

	BNode* t_kthLargest(BNode *tnode, int k);
	void t_splitIntoTwo(int item, BNode *tnode);


public:
	int n_nodes, n_leaves, level_nodes;
	LnkLst eleBw;
	LnkLst revIoList;

	ThBST() {
		troot = NULL;
		ptr = NULL; newptr = NULL; save = NULL; loc = NULL; par = NULL;
		ctroot1= NULL; ctroot2= NULL;
		n_nodes = 1; n_leaves = 0; level_nodes = 0;
	}

	ThBST(const ThBST &b);
	void operator=(const ThBST &bst_o);

	void insert(int i);

	BNode* inOrderSuccessor(BNode *ptr);
	BNode* inOrderPredecessor(BNode *ptr);
	void Srch_Loc_UnSort(int item);
	int search(int item);

	void inorder() 		{	t_inOrder(troot);  	}
	void preorder() 	{	t_preOrder(troot); 	}
	void postorder() 	{	t_postOrder(troot);	}
	void inorderReverse() { t_inOrderReversal(troot); }

	void display() 		{	t_display(troot, 1);	}
	void displayBwK1K2(BNode *ptr1, BNode *ptr2, int k1, int k2);
	void printTreeGv(string fname) { t_print_graphviz(troot, fname); }

	void CountNodes()  {	t_CountNodes(troot);	}
	void CountLeaves() {	t_CountLeaves(troot);	}
	int getCountAtLevel(int curr, int desired) {	return t_getCountAtLevel(troot, curr, desired);	}
	void increaseCountValue(BNode *i_par, int item);
	void decreaseCountValue(BNode *i_par, int item, bool left_side);
	int maxDepth() 		{	return t_maxDepth(troot);	}

	void DelNode(int flag) {	t_delNode(flag);	}

	BNode* kthLargest(int k) { return	t_kthLargest(troot, k);	}
	void splitIntoTwo(int item) {	t_splitIntoTwo(item, troot);}
	BNode* copyLeftRightNode(BNode* tNode, unordered_map<BNode *, BNode *> &clone_map);

	BNode* returnPar() {
		return par;
	}
	BNode* returnLoc() {
		return loc;
	}


	~ThBST() {
		cout << "\n Free Memory (tree), deleting: ";
		troot = makeEmpty(troot);
		if(ctroot2!=NULL)
			ctroot2 = makeEmpty(ctroot2);
	}
};

// This function creates clone by copying key and left and right pointers
// This function also stores mapping from given tree node to clone.
BNode* ThBST::copyLeftRightNode(BNode* tnode, unordered_map<BNode*, BNode*>&clone_map) {
	if (tnode == NULL)
		return NULL;
	
	BNode* cloneNode = Create_New_Node(tnode->key);
	cloneNode->isLeftTh = tnode->isLeftTh;
	cloneNode->isRightTh = tnode->isRightTh;
	cloneNode->lcnt = tnode->lcnt;
	cloneNode->rcnt = tnode->rcnt;
	
	clone_map[tnode] = cloneNode;
	
	if (tnode->isLeftTh == false) {
		cloneNode->left  = copyLeftRightNode(tnode->left, clone_map);
	}
	else if (tnode->left != NULL) {
		cloneNode->left = clone_map[tnode->left];
	}

	if (tnode->isRightTh == false) {
		cloneNode->right = copyLeftRightNode(tnode->right, clone_map);
	}
	else if (tnode->right != NULL) {
		cloneNode->right = clone_map[tnode->right];
	}

	return cloneNode;

}


ThBST::ThBST(const ThBST &bst_o) {
	if (bst_o.troot ==  NULL) {
		cout << "\n Empty Tree Can't Clone";
		troot = NULL;
		return;
	}
	unordered_map<BNode*, BNode*>clone_map;

	if (troot != NULL) {
		troot = makeEmpty(troot);
	}
	
	troot = copyLeftRightNode(bst_o.troot, clone_map);

//	t_inOrder(troot);
//	t_display(troot, 1);
}

void ThBST::operator=(const ThBST &bst_o)
{
	if (bst_o.troot ==  NULL) {
		cout << "\n Empty Tree Can't Clone";
		troot = NULL;
		return;
	}
	
	if (troot != NULL) {
		troot = makeEmpty(troot);
	}
	unordered_map<BNode *, BNode *> clone_map;
	troot = copyLeftRightNode(bst_o.troot, clone_map);
	
//	t_inOrder(troot);
//	t_display(troot, 1);
}

/*
*   loc = NULL and par = NULL (troot=NULL) => tree empty
*	loc != NULL (=ptr=troot) and par = NULL  => item is troot
*	loc = NULL and par != NULL  => item is not in tree
*/
// Finds the location (loc) of item key and also location(par) of parent of item
void ThBST::Srch_Loc_UnSort(int item)
{
	save = NULL;  // to store parent location temporarary.
	ptr = troot;

	while (ptr != NULL) {
		if ( item == ptr->key ) {
			loc = ptr;
			par = save;
			throw "Duplicate Values Detected";
			return;
		}
		save = ptr;
		if ( item < ptr->key ) {
			if (ptr->isLeftTh == false)
				ptr = ptr->left;
			else
				break;
		}
		else {
			if (ptr->isRightTh == false)
				ptr = ptr->right;
			else
				break;
		}
	}
	loc = NULL;
	par = save;
}

// Same Search Function as above with slight modification for operation other than input
int ThBST::search(int item)
{
	save = NULL;  // to store parent location temporarary.
	ptr = troot;

	while (ptr != NULL) {
		if ( item == ptr->key ) {
			loc = ptr;
			par = save;
			return 1;
		}
		save = ptr;
		if ( item < ptr->key ) {
			if (ptr->isLeftTh == false)
				ptr = ptr->left;
			else
				break;
		}
		else {
			if (ptr->isRightTh == false)
				ptr = ptr->right;
			else
				break;
		}
	}
	loc = NULL;
	par = save;
	return -1;
}

// Insert function
void ThBST::insert(int i) {

	Srch_Loc_UnSort(i);
	BNode *np =	Create_New_Node(i);
	increaseCountValue(par, i);

	if (troot == NULL)
		troot = np;
	else if ( np->key < par->key)
	{
		np->left = par->left;
		np->right = par;

		par->isLeftTh = false;
		par->left = np;
	}
	else {
		np->right = par->right;
		np->left = par;

		par->isRightTh = false;
		par->right = np;
	}

}

// Traversal Functions
BNode* ThBST::inOrderSuccessor(BNode *ptr) {

	// if Right Link is Thread to inorder successor
	if (ptr->isRightTh == true) {
		return ptr->right;
	}
	//Else return leftmost child of right subtree
	ptr = ptr->right;
	while (ptr->isLeftTh == false)
		ptr = ptr->left;
	return ptr;
}

BNode* ThBST::inOrderPredecessor(BNode *ptr) {

	// if Left Link is Thread to inorder predecessor
	if (ptr->isLeftTh == true) {
		return ptr->left;
	}
	//Else return rightmost child of left subtree
	ptr = ptr->left;
	while (ptr->isRightTh == false)
		ptr = ptr->right;
	return ptr;
}

void ThBST::t_inOrder(BNode* ptr)
{
	// if root of tree is NULL
	if (troot == NULL) {
		cout << "\tTree is empty" << endl;
		return;
	}
	while (ptr->isLeftTh == false)
		ptr = ptr->left;
	while (ptr != NULL)
	{
		cout << ptr->key << " ";
		ptr = inOrderSuccessor(ptr);
	}
}

void ThBST::t_preOrder(BNode* ptr)
{
	if (troot == NULL) {
		cout << "\tTree is empty" << endl;
		return;
	}
//	if (ptr != NULL) {
//		cout << ptr->key << "  ";
//		t_preOrder(ptr->left);
//		t_preOrder(ptr->right);
//	}
	cout << ptr->key << "  ";
	if (ptr->isLeftTh == false)
		t_preOrder(ptr->left);
	if (ptr->isRightTh == false)
		t_preOrder(ptr->right);

}

void ThBST::t_postOrder(BNode* ptr)
{
	if (troot == NULL) {
		cout << "\tTree is empty" << endl;
		return;
	}
//	if (ptr != NULL) {
//		t_postOrder(ptr->left);
//		t_postOrder(ptr->right);
//		cout << ptr->key << "  ";
//	}

	if (ptr->isLeftTh == false)
		t_postOrder(ptr->left);
	if (ptr->isRightTh == false)
		t_postOrder(ptr->right);
	cout << ptr->key << "  ";
}

void ThBST::t_inOrderReversal(BNode* ptr)
{
	LNode *lp;
	revIoList.cleanList();
	// if root of tree is NULL
	if (troot == NULL) {
		cout << "\tTree is empty" << endl;
		return;
	}
	while (ptr->isRightTh == false)
		ptr = ptr->right;
	while (ptr != NULL)
	{
		cout << ptr->key << " ";
		try {
			lp = revIoList.Create_New_Node(ptr->key);
		}
		catch (const char* msg) {
			cout << msg << endl;
		}
		revIoList.Insert_End(lp);

		ptr = inOrderPredecessor(ptr);
	}
}

// Display Functions
void ThBST::t_display(BNode *ptr, int level)
{
	int i;
	if (ptr != NULL)
	{
		if (ptr->isRightTh == false)
			t_display(ptr->right, level + 1);

		cout << endl;
		if (ptr == troot)
			cout << "Root->:  ";
		else {
			for (i = 0; i < level; i++)
				cout << "       ";
		}
		cout << ptr->key ;
//		cout << ptr->key << "(" << ptr->lcnt << "," << ptr->rcnt << ")";
		if (ptr->isLeftTh == false)
			t_display(ptr->left, level + 1);
	}
}

void ThBST::t_print_graphviz(BNode *ptr, string file_name) {
	if (ptr == NULL) {
		cout << " Tree is Empty";
		return;
	}
	
	ofstream fout;
	fout.open(file_name.c_str(), ios::out);
	if (!fout) {
		cout << "\n**File failed to open**\n\n";
		fout.clear();
	}

	fout << "digraph thBST {" << endl;
	fout << "node [shape = record,height=.1];" << endl << endl;

	queue<BNode *> qLoBST;		// queue level order BST
	qLoBST.push(ptr);

	while (qLoBST.empty() == false)
	{
		BNode *tmp = qLoBST.front();
//        	cout << tmp->key << " ";
		if (tmp->isLeftTh != true  || tmp->isRightTh != true)
			fout << tmp->key << "[label = \"<L>|<D> " << tmp->key << "|<R> \"]" << endl;
		else
			fout << tmp->key << "[label = \"<L>|<D> " << tmp->key << "|<R> \", fontcolor=red]" << endl;

		if (tmp->isLeftTh == false)
			fout << "\"" << tmp->key << "\":L ->" << tmp->left->key << ":D" << endl;
		else if (tmp->left != NULL )
			fout << "\"" << tmp->key << "\":L ->" << tmp->left->key << ":D[style=dotted, color=blue]" << endl;

		if (tmp->isRightTh == false)
			fout << "\"" << tmp->key << "\":R ->" << tmp->right->key << ":D" << endl;
		else if (tmp->right != NULL )
			fout << "\"" << tmp->key << "\":R ->" << tmp->right->key << ":D[style=dotted, color=blue]" << endl;

		fout << endl;

		qLoBST.pop();
		/* Enqueue left child */
		if (tmp->isLeftTh != true)
			qLoBST.push(tmp->left);
		/*Enqueue right child */
		if (tmp->isRightTh != true)
			qLoBST.push(tmp->right);
	}

	fout << "}" << endl;

	cout << "\n\t printed.";

	fout.close();
}

void ThBST::displayBwK1K2(BNode *ptr1, BNode *ptr2, int k1, int k2) {
	LNode *lp;
	eleBw.cleanList();

	if (troot == NULL) {
		cout << "\tTree is empty" << endl;
		return;
	}

	while (ptr1 != ptr2)
	{
		cout << ptr1->key << " ";
		try {
			lp = eleBw.Create_New_Node(ptr1->key);
		}
		catch (const char* msg) {
			cout << msg << endl;
		}
		eleBw.Insert_End(lp);
		ptr1 = inOrderSuccessor(ptr1);
	}
	cout << ptr1->key << " ";
	try {
		lp = eleBw.Create_New_Node(ptr1->key);
	}
	catch (const char* msg) {
		cout << msg << endl;
	}
	eleBw.Insert_End(lp);


}

// Count Functions
void ThBST::t_CountNodes(BNode *ptr)
{
	if (ptr != NULL)
	{

		if (ptr->isLeftTh == false)
		{
			n_nodes++;
			t_CountNodes(ptr->left);
		}
		if (ptr->isRightTh == false)
		{
			n_nodes++;
			t_CountNodes(ptr->right);
		}
	}
}

void ThBST::t_CountLeaves(BNode *ptr)
{
	if (ptr != NULL)
	{
		if ( (ptr->isLeftTh == true) && (ptr->isRightTh == true) )
			n_leaves++;
		else
		{
			if (ptr->isLeftTh == false)
				t_CountLeaves(ptr->left);
			if (ptr->isRightTh == false)
				t_CountLeaves(ptr->right);
		}
	}
}

int ThBST::t_getCountAtLevel(BNode *ptr, int curr, int desired)
{
	int leftcount = 0, rightcount = 0;
	// If this node doesn't exist, must be zero.
	if (ptr == NULL) return 0;
	// If this node is at desired level, must be one.
	if (curr == desired) return 1;
	// Otherwise sum of nodes at that level in left and right sub-trees.
	if (ptr->isLeftTh == false)
		leftcount = t_getCountAtLevel(ptr->left,  curr + 1, desired);
	if (ptr->isRightTh == false)
		rightcount = t_getCountAtLevel(ptr->right, curr + 1, desired);
	return leftcount + rightcount ;
}

int ThBST::t_maxDepth(BNode *ptr) {
	int lDepth = 0, rDepth = 0;
	if (ptr == NULL || (ptr->isLeftTh == true && ptr->isRightTh == true) )
		return 0;
	else
	{
		/* compute the depth of each subtree */
		if (ptr->isLeftTh == false)
			lDepth = t_maxDepth(ptr->left);
		if (ptr->isRightTh == false)
			rDepth = t_maxDepth(ptr->right);
		/* use the larger one */
		if (lDepth > rDepth)
			return (lDepth + 1);
		else
			return (rDepth + 1);
	}
}

void ThBST::increaseCountValue(BNode *i_par, int item) {
	BNode *rt = troot;

	while (i_par != rt) {
		if ( item < rt->key ) {
			rt->lcnt++;
			rt = rt->left;
		}
		else {
			rt->rcnt++;
			rt = rt->right;
		}
	}

	if (rt != NULL)
		if ( item < rt->key ) {
			rt->lcnt++;
		}
		else {
			rt->rcnt++;
		}
}

void ThBST::decreaseCountValue(BNode *i_par, int item, bool left_side) {
	BNode *rt = troot;

	while (i_par != rt) {
		if ( item < rt->key ) {
			rt->lcnt--;
			rt = rt->left;
		}
		else {
			rt->rcnt--;
			rt = rt->right;
		}
	}

	if (rt != NULL && rt->key != item) {
		if ( item < rt->key ) {
			rt->lcnt--;
		}
		else {
			rt->rcnt--;
		}
	} else if (rt->key == item) {
		if ( left_side ) {
			rt->lcnt--;
		}
		else {
			rt->rcnt--;
		}
	}

}

// Deleting Functions
void ThBST::t_delNode(int flag) {
	if (troot == NULL) {
		cout << "\n\t\t => Tree empty" << endl;
		return;
	}

	if (loc == NULL)
	{
		cout << "\n\t\t =>  Item not present in tree" << endl;
		return;
	}


	if ((loc->isLeftTh != true && loc->isRightTh != true))	// node having both child
		case_b(par, loc);
	else if (loc->isLeftTh != true || loc->isRightTh != true) // node having one child
		case_c(par, loc);
	else 											// node having no child
		case_a(par, loc);


	cout << "\n\t Element Deleted \n";
	t_display(troot, 1);
	loc = NULL;
}

void ThBST::case_a(BNode* par, BNode* loc) // node having no child
{
	bool fLeft = false;
	if (par == NULL)
		troot = NULL;								//   When root is the only node in tree having no child
	else if (loc == par->left) {										// if node is in left of parent
		par->isLeftTh = true;
		par->left = loc->left;
		fLeft = true;
	}
	else {
		par->isRightTh = true;
		par->right = loc->right;
		fLeft = false;
	}

	decreaseCountValue(par, loc->key, fLeft);
	delete loc;
}

void ThBST::case_c(BNode* par, BNode* loc) // node having one child
{

	BNode *child, *LocinSucc, *LocinPred;
	bool fLeft = false;

	if (loc->isLeftTh != true)
		child = loc->left;
	else
		child = loc->right;

	if (par == NULL) {
		troot = child;												//   When root is the only node in tree having no child
	}
	else
	{
		if (loc == par->left)							// if node is in left of parent
		{
			par->left = child;
			fLeft = true;
		}
		else {
			par->right = child;
			fLeft = false;
		}

	}
	LocinSucc = inOrderSuccessor(loc);
	LocinPred = inOrderPredecessor(loc);

	//   has left subtree.
	if (loc->isLeftTh != true)
		LocinPred->right = LocinSucc;
	else if (loc->isRightTh != true) { // has right subtree.
		LocinSucc->left = LocinPred;
	}

	decreaseCountValue(par, loc->key, fLeft);
	delete loc;

}

void ThBST::case_b(BNode* par, BNode* loc)
{

	BNode *suc, *parsuc;							// suc = inorder successor of loc node . parsuc = loc of the parent of the suc
	// findinf suc and parsuc
	save = loc;

	ptr = loc->right;
	while (ptr->isLeftTh != true) {
		save = ptr;
		ptr = ptr->left;
	}

	parsuc = save;
	suc = ptr;

	loc->key = suc->key;

	if (suc->isLeftTh == true && suc->isRightTh == true)	 // delete inorder successor
		case_a(parsuc, suc);					//doesnt have two children
	else
		case_c(parsuc, suc);					// have single child

}

BNode* ThBST::t_kthLargest(BNode *tnode, int k) {
	if (tnode == NULL) {
		return NULL;
	}
	
	int tcount, lcount, rcount;
	lcount = tnode->lcnt;
	rcount = tnode->rcnt;
	tcount = lcount + rcount + 1;
	if (k > tcount) {
		return NULL;
	}

	if (rcount + 1 == k)
		return tnode;

	if (k > rcount + 1)
		return t_kthLargest(tnode->left, k - rcount - 1);
	return t_kthLargest(tnode->right, k ); // else search in right subtree
}

void ThBST::t_splitIntoTwo(int k, BNode *ctroot) {
	
	if(ctroot == NULL){
		cout<<"\n Clone Tree is Empty;";
		return;
	}
	
	string cfn1 = "a_print_thbst_split1.gv";
	string cfn2 = "a_print_thbst_split2.gv";
	ctroot1= ctroot; 
	ctroot2= NULL;
	
	BNode *cutptr=NULL;
	BNode *fptr  =NULL;
	BNode *secptr=ctroot;
	bool toLeft = k < ctroot->key;
	
	while(secptr != NULL){

		while (secptr != NULL && (k < secptr->key) == toLeft) {
            fptr = secptr;
            if(toLeft){
            	if(secptr->isLeftTh == false)
            		secptr = secptr->left;
            	else{
//            		fptr = secptr;
            		secptr = NULL;
				}
			}
			else if(secptr->isRightTh == false)
            		secptr = secptr->right;
            	else{
//            		fptr = secptr;
            		secptr = NULL;
				}
        }
        

	    if (toLeft) {
	        fptr->left = NULL;
	        fptr->isLeftTh = true;
	    } else {
//	    	if(fptr->isRightTh != NULL)
//            		cutptr->isLeftTh = false;
	        fptr->right = NULL;
	        fptr->isRightTh = true;
	    }
	    

	    toLeft = !toLeft;
	    
        if (ctroot2 == NULL) {
            ctroot2 = secptr; // This is the root of the other tree.
        } else if (cutptr != NULL) {
             // re-attach the detached subtree
            if (toLeft) {
		        cutptr->left = secptr;
		        if(secptr != NULL)
            		cutptr->isLeftTh = false;
		    } else {
				cutptr->right = secptr;
		        if(secptr != NULL)
            		cutptr->isRightTh = false;
		    }
        }
        cutptr = fptr;
        fptr = NULL;
	}
	
	cout<<"\n Tree 1 inOrder : ";
		if (ctroot1 == NULL) {
			cout << "\t Splited Tree 1 is empty" << endl;
			return;
		}
		else {
			t_inOrder(ctroot1);
			t_print_graphviz(ctroot1, cfn1);
		}
	cout<<"\n Tree 2 inOrder : ";
		if (ctroot2 == NULL) {
				cout << "\t Splited Tree 2 is empty" << endl;
				return;
			}
		else{
			 t_inOrder(ctroot2);
			 t_print_graphviz(ctroot2, cfn2);
		}
		
//	troot=NULL;
	// for Normal BST
//	while(secptr != null){
//
//		while (secptr != null && (k < secptr->key) == toLeft) {
//            fptr = secptr;
//            secptr = toLeft? secptr->left : secptr->right;	
//        }
// 
//	    if (toLeft)  fptr->left = NULL;
//	    else fptr->right = NULL;
//	    
//		toLeft = !toLeft;
//
//        if (ctroot2 == null) {
//            ctroot2 = secptr; // This is the root of the other tree.
//        } else if (cutptr != null) {
//             // re-attach the detached subtree
//            if (toLeft)cutptr->left = secptr;
//			else cutptr->right = secptr;
//        }
//        cutptr = fptr;
//        fptr = NULL;
//	}
}

int main() {

	ThBST bst_obj;
	ThBST cloned_bst;
	char ch;
	int flag;
	int k1 = 0, k2 = 0;
	BNode *ptr1 = NULL, *ptr2 = NULL, *kptr = NULL;
	string fn = "a_print_thbst.gv";

	do {
		int inf, flag = -2, k1 = 0, k2 = 0;
		cout << endl << endl << endl;
		system("pause");
		system("cls");
		bst_obj.display();

		cout << "\n\n ----  Binary Search Tree Menu  ----";

		cout << "\n 1. Display - Console 			";
		cout << "\n 2. Traversal - PreOrder  ";
		cout << "\n 3.           - InOrder   	";
		cout << "\n 4.           - PostOrder   ";
		cout << "\n 5. Count - Nodes & Leaves   ";
		cout << "\n 6.       - Num of Nodes at Level i";
		cout << "\n 7.       - Height		";

		cout << "\n i. INSERT an element x ";
		cout << "\n s. SEARCH an element x";
		cout << "\n d. DELETE an element x";
		cout << "\n r. Traversal - Reverse InOrder ";
		cout << "\n e. Element - InOrder Successor ";
		cout << "\n b.         - All Elements Between k1 and k2 ";
		cout << "\n k.	    - kth Largest Element ";
		cout << "\n x. Split BST into Two";
		cout << "\n p. Print Tree - GraphViz";

		cout << "\n n. Exit - Bye		\n\n  --Choice : ";
//		  	 ch = getchar();
//		  	 scanf ("%c", &ch);
		cin >> ch;
		cout << "\n --------";
		switch (ch)
		{

		case '1' :    bst_obj.display();
				break;
		case '2' :	cout << "\n PreOrder Traversal is : "; 		bst_obj.preorder();	    	break;
		case '3' :	cout << "\n InOrder Traversal is : "; 		bst_obj.inorder();	    	break;
		case '4' :	cout << "\n PosteOrder Traversal is : "; 	bst_obj.postorder();	    break;
		case '5' :	bst_obj.CountNodes();	  bst_obj.CountLeaves();
					cout << "\n No of Nodes are  : " << bst_obj.n_nodes;
					cout << "\n No of Leaves are  : " << bst_obj.n_leaves;
				break;
		/*
		the depth of a node X in a tree T is defined as the length of the simple path (number of edges) from the root node of T to X.
		The height of a node Y is the number of edges on the longest downward simple path from Y to a leaf.
		The height of a tree is defined as the height of its root node.
		*/
		case '6' :	cout << "\n Enter Levels (from 0) to find total Nodes  : ";
						cin >> bst_obj.level_nodes;
						cout << "\n Num of Nodes at Levels " << bst_obj.level_nodes << " is " << bst_obj.getCountAtLevel(0, bst_obj.level_nodes); // level start with zero
				break;
		case '7' :	cout << "\n Height of the Tree(root) is  : " << bst_obj.maxDepth();
				break;
				
		case 'i' :  cout << "\n Enter Infomartion (int) for new BST Node .. : ";
					  cin >> inf;		// Saving Info for the new node
					try {
						bst_obj.insert(30);
						bst_obj.insert(20);
						bst_obj.insert(40);
						bst_obj.insert(70);
						bst_obj.insert(60);
						bst_obj.insert(80);
						bst_obj.insert(10);
						bst_obj.insert(15);
									bst_obj.insert(inf);
					}
					catch (const char* msg) {
						cout <<"\t"<<msg<< endl;
					}
				break;
		case 's' :	cout << "\n Enter a Node value to SEARCH : ";
						cin >> inf;
						flag = bst_obj.search(inf);
					if (flag == 1) {
						cout << "\n Search Successful for Element (" << inf << ").";
						if (bst_obj.returnPar() != NULL)
							cout << "\n 	Its parent value and address: " << bst_obj.returnPar()->key << " , " << bst_obj.returnPar();
						cout << "\n	Its location: " << bst_obj.returnLoc()->key << " , " << bst_obj.returnLoc();
					}
					else if (flag == -1) {
						cout << "\n Element (" << inf << ") Not Found.";
					}
					else
						cout << "\n Search Operation Unsuccessful.";
				break;
		case 'd' :	cout << "\n Enter a Node value to DELETE : ";
						cin >> inf;
					flag = bst_obj.search(inf);
					bst_obj.DelNode(flag);
				break;
		case 'r' :	cout << "\n Reverse InOrder Traversal is : "; bst_obj.inorderReverse();
					cout << "\n Reverse InOrder Traversal using List :"; bst_obj.revIoList.Display();
				break;	
		case 'e' :	cout << "\n Enter a Node value to find its inorder successor : ";
						cin >> inf;
					flag = bst_obj.search(inf);
					if (flag == 1) {
						BNode* ptr	= bst_obj.inOrderSuccessor(bst_obj.returnLoc());
						if (ptr != NULL)
							cout << "\n	Its inorder successor is: " << ptr->key;
						else cout << "\n	Its inorder successor does not exit: ";
					}
					else if (flag == -1) {
						cout << "\n Element (" << inf << ") does not exist.";
					}
				break;
		case 'b' :
					cout << "\n Enter k1 : ";
						cin >> k1;
//					flag = bst_obj.search(k1);
//					if (flag == 1) {
//						ptr1	= bst_obj.returnLoc();
//						cout << "\n Enter k2 : ";
//						cin >> k2;
//						flag = bst_obj.search(k2);
//						if (flag == 1) {
//							ptr2	= bst_obj.returnLoc();
//							cout << "\n Elements Between (" << k1 << ") and (" << k2 << ") are : ";
//							bst_obj.displayBwK1K2(ptr1, ptr2,  k1,  k2);
//						}
//						else if (flag == -1) {
//							cout << "\n Element (" << k2 << ") does not exist.";
//							break;
//						}
//					}
//					else if (flag == -1) {
//						cout << "\n Element (" << k1 << ") does not exist.";
//						break;
//					}
					flag = bst_obj.search(k1);
					ptr1	= bst_obj.returnLoc();
		
					if (ptr1 == NULL) {
						ptr1	= bst_obj.returnPar();
					}
					cout << "\n Enter k2 : ";
						cin >> k2;
					flag = bst_obj.search(k2);
					ptr2	= bst_obj.returnLoc();
					if (ptr2 == NULL) {
						ptr2	= bst_obj.returnPar();
					
					}
					if (ptr1 == NULL && ptr2 == NULL) {
						cout << "\n Tree Empty";
					}
					else{
						cout << "\n Elements Between (" << k1 << ") and (" << k2 << ") are : ";
							bst_obj.displayBwK1K2(ptr1, ptr2,  k1,  k2);
						cout << "\n Elements Between (" << k1 << ") and (" << k2 << ") using List :"; bst_obj.eleBw.Display();
					}
					break;
		case 'k' : cout << "\n Enter value of k to find kth largest : ";
						cin >> inf;

				kptr =  bst_obj.kthLargest(inf);
				if (kptr != NULL) {
					cout << "\n\t " << inf << "th Largest Element is : " << kptr->key;
				}
				else
					cout << "\n\t Value of k is greater than total nodes";
				break;
		case 'x' : cout << "\n Split BST into Two";
			cout << "\n Enter a value of k to make a split : ";
			cin >> inf;
				try {
					cloned_bst = bst_obj;
				}
				catch (const char* msg) {
					cout << msg << endl;
				}
				cloned_bst.display();
						 cloned_bst.splitIntoTwo(inf);
			break;
		case 'p' : cout << "\n Printing Tree: ";
					bst_obj.printTreeGv(fn);
			break;

		case 'n' : cout << "\n Bye"; break;
		default  :   cout << "\n--Invalid Choice. Enter Again \n";
		}
	} while (ch != 'n');


	getch();
	return 0;

	//
}
