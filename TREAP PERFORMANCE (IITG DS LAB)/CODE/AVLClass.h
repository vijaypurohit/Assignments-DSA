#ifndef DSA_A03_AVLCLASS_H
#define DSA_A03_AVLCLASS_H

/********************************************************************************************
	AVL TREE Class
********************************************************************************************/
class AVLTree{
private:
    ANode *troot, *head;			// troot is dummy variable for root of the tree (head->rightc).
    ANode *ptr, *newptr;			// other pointers for help in functions
    ANode *save, *loc, *par;

    stack<ANode*> pathToP;			//stack is used in deletion process for maintaing the path from root to deleted node, in order to balance tree.

    void t_print_graphviz(ANode *, string); // print by graphviz, ptr is provided in order to print different rooted tree.
    void t_display(ANode *, int); // display helper function for displaying on console.

    // function to delete all the nodes of the tree and make tree empty.
    ANode* makeEmpty(ANode *tnode) {
        ANode *ptr_e = tnode;
        ANode *temp;
        if (tnode == nullptr) { 	// if root of tree is NULL
            return nullptr;
        }

        queue<ANode *> qLoAVL;		// queue level order AVL
        qLoAVL.push(ptr_e);

        while (!qLoAVL.empty())
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

        return nullptr;
    }

public:
    unsigned long int height_final_avl, height_sum_nodes;   // height of final tree, sum of height of each node
    double height_avg_node_avl;
    unsigned long int comp_insert_avl, comp_delete_avl, comp_final_avl;       // node comparisons count
    unsigned long int rot_insert_avl, rot_delete_avl, rot_final_avl;          // rotations count
    unsigned long int total_nodes;                                                           // total count of nodes.

    AVLTree(){
        head = new ANode(); // special header node appears at the top of the tree
        head->leftc = new ANode(0); // leftc link of head is used to keep track of the overall level.
        head->rightc = nullptr; // rightc link is a pointer to the root of the tree.
        troot = nullptr;
        ptr = nullptr; newptr = nullptr;
        save = nullptr; loc = nullptr; par = nullptr;

        total_nodes=0;
        height_final_avl=0; height_avg_node_avl=0; height_sum_nodes=0;
        comp_insert_avl=0; comp_delete_avl=0; comp_final_avl=0;
        rot_insert_avl=0; rot_delete_avl=0; rot_final_avl=0;
    }

    AVLTree(const AVLTree &a); //copy constructor
    void operator=(const AVLTree &a_obj); // assignment operator overloading for copying
    ANode* copyLeftRightNode(ANode*);	// helper function for copying.

    void Insert(int); //insert function
    int Search(int);  //Search function
                        // Delete functions
    void Delete(int);
    void case_a(ANode* , ANode* );		// node having zero or one child
    void case_b(ANode* , ANode* );		// node having both children.
                        //Rotation Function
    ANode* Imbalance_LL(ANode*, ANode *, int , int );
    ANode* Imbalance_RR(ANode*, ANode *, int , int );
    ANode* Imbalance_LR(ANode*, ANode *, int , int );
    ANode* Imbalance_RL(ANode*, ANode *, int , int );

    void display() {t_display(head->rightc, 1);}  //display on console. helper function
    void print_graphviz(string file_name) {  t_print_graphviz(head->rightc, file_name) ; }  //printing using graphviz, taking filename as input to save
    void show_parameters();     // to display all the parameters of the Tree
    void write_parameters(ofstream &foutt, int nop);
    unsigned long calculate_sum_of_nodes_heights(ANode *); // to calculate sum of height of each node

    // helper function to return value of pointers.
    ANode* returnPar() { return par; }
    ANode* returnLoc() {return loc;}
    int returnParKey() {return par->key;}
    int returnLocKey() {return loc->key; }

    //destructor
    ~AVLTree() {
//        cout << "\n Free Memory (AVL), deleting: ";
        //head->rightc = makeEmpty(head->rightc);				// deleting all nodes
//        if(troot)cout<<troot->key<<"C ";
        delete head;
        troot = head = nullptr;
    }


}; //end of Class AVLTree

/*******************************************************************************************
	copy constructor: copying from one object to other
	Input: object to be copied.
********************************************************************************************/
AVLTree::AVLTree(const AVLTree &avl_o) {
    troot = head->rightc;
    if (avl_o.head->rightc ==  nullptr) {
        cout << "\n Empty Tree Can't Clone";
        head->rightc = troot = nullptr;
        return;
    }

    if (troot != nullptr) {
        head->rightc = makeEmpty(troot);
    }

    head->rightc = copyLeftRightNode(avl_o.troot);
}

/********************************************************************************************
	operator= overloading: copying from one object to other
	Input: object to be copied.
********************************************************************************************/
void AVLTree::operator=(const AVLTree &avl_o)
{
    troot = head->rightc;

    if (avl_o.head->rightc ==  nullptr) {
        cout << "\n Empty Tree Can't Clone";
        head->rightc = troot = nullptr;
        return;
    }
    if (troot != nullptr) {
        head->rightc = makeEmpty(troot);
    }

    head->rightc = copyLeftRightNode(avl_o.troot);
}

/********************************************************************************************
	Helper function for copy constructor, taking input as root and recursively copying.
	This function creates clone by copying key and leftc and rightc pointers
********************************************************************************************/
ANode* AVLTree::copyLeftRightNode(ANode* tnode) {
    if (tnode == nullptr)
        return nullptr;

    ANode* cloneNode =  new ANode(tnode->key);

    cloneNode->bf = tnode->bf;
    cloneNode->key = tnode->key;

    cloneNode->leftc  = copyLeftRightNode(tnode->leftc);
    cloneNode->rightc = copyLeftRightNode(tnode->rightc);

    return cloneNode;

}

/********************************************************************************************
	Helper function for Displaying Tree in the console for better visualization and operations handling.
	Input: ptr as a root node for recursively printing, level as Tree Level initiallly 1.
********************************************************************************************/
void AVLTree::t_display(ANode *tnode, int level)
{
    int i;
    if (tnode != nullptr)
    {
        t_display(tnode->rightc, level+1);
        cout<<endl;
        if (tnode == head->rightc)
            cout<<"Root->:"<<head->leftc->key<<"  ";
        else
        {
            for (i = 0;i < level;i++)
                cout<<"       ";
        }
//	        cout<<ptr->key;
        cout << tnode->key << "|" << tnode->bf<< " (h " << tnode->ht<<") " ;
        t_display(tnode->leftc, level+1);
    }
}

/********************************************************************************************
	Search function for node key in AVL TREE.
	Input: item to be searched
********************************************************************************************/
int AVLTree::Search(int item)
{
    troot =  head->rightc;
    save = nullptr;  // to store parent location temporarary.
    ptr = troot;

    while(ptr != nullptr)
    {
        if( item == ptr->key )
        {
            loc = ptr; par = save; return 1;
        }
        save = ptr;
        if( item < ptr->key )
            ptr = ptr->leftc;
        else
            ptr=ptr->rightc;
    }
    loc = nullptr;
    par = save;
    return -1;
}

/********************************************************************************************
	Insert function for inserting into AVL Tree and performing necessary rotations.
	Input: item to be inserted.
********************************************************************************************/
void AVLTree::Insert(int k){
    //locating the position to insert
    troot = head->rightc;
    ANode *t = head;       // t always points to the parent of s
    ANode *s = troot;	// s will point to the place where rebalancing may be necessary
    ANode *p = troot;	// p will move down the tree.
    ANode *q = nullptr;	//q will point to the new node which is being inserted.

    ANode *newptr = new ANode(k);		// create new node now
    //if the rightc side of head (root) is null, ie tree is empty, inserting it there
    if(troot == nullptr){
        head->rightc =  troot = newptr;
        total_nodes++;
        return;
    }

    while(p != nullptr){

        comp_insert_avl++;

        if(k < p->key){ 	//Move Left
            q = p->leftc;
            if(!q) {		// q is null
                p->leftc = newptr; //INSERT
                q = p->leftc;
                break;
            }
            else{
                if(q->bf != 0){
                    t = p;
                    s = q;
                }
                p = q;
            }
        }
        else if(k > p->key){	//Move Right
            q = p->rightc;
            if(!q){
                p->rightc = newptr; //INSERT
                q = p->rightc;
                break;
            }
            else{
                if(q->bf != 0){
                    t = p;
                    s = q;
                }
                p = q;
            }
        }
        else{
            delete newptr;
            throw "Duplicate Values Detected!!";
        }
    }

    total_nodes++;

    ANode *r=nullptr;		// it will point to the node where firts potential rotation may happen.
    //Adjust Balance Factors
    int a = k < s->key ? 1 : -1;

    //	r = LINK(a, s);
    r = (a==1) ? s->leftc : s->rightc;
    //	p = LINK(a, s);
    p = (a==1) ? s->leftc : s->rightc;

    while(p != q){
        if(k < p->key){
            p->bf = 1;
            p = p->leftc;
        }
        else if(k > p->key){
            p->bf = -1;
            p = p->rightc;
        }
    }

    //Balancing ACT
    if(s->bf == 0){				// tree has grown higher
        s->bf = a;
        head->leftc->key++;
        return;
    }
    else if(s->bf == -1*a){		// tree has gotten more balanced
        s->bf = 0;
        return;
    }
    else{	// s->bf == a tree has gotten out of balance
        if(r->bf == a){ //single rotation
            if(a == 1){			//LL Imbalance
                p = Imbalance_LL(s, r, 0, 0);
            }
            else if(a == -1){  // RR Imbalance
                p = Imbalance_RR(s, r, 0, 0);
            }
            rot_insert_avl++;
        }
        else if(r->bf == -1*a){		//double rotation
            if(a == 1){ 	// LR Imbalance
                p = Imbalance_LR(s, r, a, 1);
            }
            else if(a == -1){ 	 // RL Imbalance
                p = Imbalance_RL(s, r, a, 1);
            }
            rot_insert_avl = rot_insert_avl + 2;
        }
    }
/* p pointing to the new subtree root. t pointing to the parent of the old subtree root s.
 	adjusting root after rotation. */
    if(s == t->rightc) t->rightc = p;
    else t->leftc = p;

}

/********************************************************************************************
	Delete function for deleting from AVL Tree and performing necessary rotations.
	Input: item to be deleted.
********************************************************************************************/
void AVLTree::Delete(int k)
{
    troot =  head->rightc;
    // Tree is Empty
    if (troot == nullptr) {
//        cout << "\n\t\t => Tree empty" << endl;
//        return;
        throw "=> Tree empty";
    }

    while (!pathToP.empty())		//empty stack, if element already present.
        pathToP.pop();

    ANode *t = head;       // t always points to the parent of s
    ANode *s = troot;	// s will point to the place where rebalancing may be necessary
    ANode *p = troot;	// p will move down the tree.
    ANode *r = nullptr;	// r point to the node where firts potential rotation may happen. leftc/rightc of s
    bool exist = false;
    int a;

    pathToP.push(t);

    while(p != nullptr)
    {
        comp_delete_avl++;

        if( k == p->key ){
            exist=true;				// element already exist
            break;
        }
        if( k < p->key ){
            pathToP.push(p);
            p = p->leftc;
        }
        else{
            pathToP.push(p);
            p=p->rightc;
        }
    }

    // element does not exist
    if (!exist){
//        cout << "\n\t\t =>  Item not present in tree" << endl;
        while (!pathToP.empty())
            pathToP.pop();
        throw "Item not present in tree";
        return;
    }

    // element exist and we have path from root to it. deleting that element like normal BST
    if (p->leftc != nullptr && p->rightc != nullptr)	// Node having both child
    {
        case_b(pathToP.top(), p);
        k = p->key;  				// now k is successor key. and we will balance from that point
    }
    else
        case_a(pathToP.top(), p);				// node having one or no child

    total_nodes--;

    // Adjusting balance factor
    while(pathToP.top() != head){

        s = pathToP.top();
        pathToP.pop();
        t = pathToP.top();
        a = k < s->key ? 1 : -1;

        if(s->bf == a){
            s->bf = 0;
            continue;
        }
        else if(s->bf == 0){
            s->bf = -1*a;
            return;
        }
        else{	// s->bf == -a Rebalancing is required!

            r = (s->bf==1) ? s->leftc : s->rightc; //	r = LINK(s->bf, s);

            if(r->bf == -1*a){ //single rotation
                if(a == -1){	//LL Imbalance
                    p = Imbalance_LL(s, r, 0, 0);
                }
                else if(a == 1){  // RR Imbalance
                    p = Imbalance_RR(s, r, 0, 0);
                }
                rot_delete_avl++;
            } else if(r->bf == 0){ //single rotation
                if(a == -1){	//LL Imbalance
                    p = Imbalance_LL(s, r, -2, a);
                }
                else if(a == 1){  // RR Imbalance
                    p = Imbalance_RR(s, r, -2, a);
                }
                rot_delete_avl++;
            }
            else if(r->bf == a){		//double rotation
                if(a == -1){ 	// LR Imbalance
                    p = Imbalance_LR(s, r, a, 0);
                }
                else if(a == 1){ 	 // RL Imbalance
                    p = Imbalance_RL(s, r, a, 0);
                }
                rot_delete_avl = rot_delete_avl + 2;
            }
        }
        // p pointing to the new subtree root
        // t pointing to the parent of the old subtree root s.
        if(s == t->rightc) t->rightc = p;
        else t->leftc = p;

        if(s->bf !=0 && r->bf!=0){
            //height is not reduced. so we can return from there itself.
            return;
        }

    }//end of while

    if(pathToP.top()==head && head->leftc->key >0) {
        head->leftc->key--;
    }

}

void AVLTree::case_a(ANode* par, ANode* loc)
{
    ANode *child;

    if (loc->leftc == nullptr && loc->rightc == nullptr) // Node having no leftc or rightc child so intialising child = NULL i.e. no child
        child = nullptr;
    else if (loc->leftc != nullptr)
    {
        child = loc->leftc;
        loc->leftc=nullptr;
    }
    else
    {
        child = loc->rightc;
        loc->rightc=nullptr;
    }

    if (par == nullptr){
        head->rightc = troot = child;	//   When root is the only node in tree having no child
    }
    else
    {
        if (loc == par->leftc)	// if node is in leftc of parent
            par->leftc = child;
        else
            par->rightc = child;
    }

    delete loc;
}

/********************************************************************************************
	Delete Helper function. For Node having both children
	input: par is parent of node to be deleted. loc is location of node to be deleted.
********************************************************************************************/
void AVLTree::case_b(ANode* par, ANode* loc)
{
    ANode *suc, *parsuc;	// suc = inorder successor of loc node . parsuc = loc of the parent of the suc
    // findinf suc and parsuc

    pathToP.push(loc);

    save = loc;
    ptr = loc->rightc;

    while (ptr->leftc != nullptr)
    {
        comp_delete_avl++;
        pathToP.push(ptr);
        save = ptr;
        ptr = ptr->leftc;
    }
    parsuc = save;
    suc = ptr;

    loc->key = suc->key;  //replacing loc key with successor key
    // delete inorder successor
    case_a(parsuc, suc);

}

/********************************************************************************************
	Rotation Function whene there is LL Imbalance (rightc rotation)
	input: s will point to the place where rebalancing may be necessary
		   r point to the node where firsts potential rotation may happen.
		   s_bf is new bf of s
		   r_bf is new bf of r
********************************************************************************************/
ANode* AVLTree::Imbalance_LL(ANode* s, ANode *r, int s_bf, int r_bf){
    ANode *p = r;
    s->leftc = r->rightc; //rightc rotation
    r->rightc = s;

    p = r;
    if(r->bf!=0)s->bf = s_bf;
    r->bf = r_bf;
    return p;
    /* insertion
    (a=1)	  s	(1)			    r (0)
            /   			  /   \
       (1) r,p	 		 	 i     s (0)
         /
        i

    */
    /* deletion
    (a=-1)	  s	(1)			    r (0)
            /  \			  /   \
       (1) r,p  d	 		 x     s (0)
         /
        x

    */
    /* deletion r bf == 0 height of tree is not reduced
    (a=-1)	  s	(1)			    r (a) r bf = a
            /  \			  /   \
       (0) r,p  d	 		 x     s (1) s bf = same
         / 	\     		     	  /
        x    y		  	         y

    */
}

/********************************************************************************************
	Rotation Function whene there is RR Imbalance (leftc rotation)
	input: s will point to the place where rebalancing may be necessary
		   r point to the node where firsts potential rotation may happen.
		   s_bf is new bf of s
		   r_bf is new bf of r
********************************************************************************************/
ANode* AVLTree::Imbalance_RR(ANode* s, ANode *r, int s_bf, int r_bf){
    ANode *p = r;
    s->rightc = r->leftc;	//leftc rotation
    r->leftc = s;

    p = r;
    if(r->bf!=0)s->bf = s_bf;
    r->bf = r_bf;
    return p;
    /* insertion
    (a=-1)	  s	(-1)			   	r (0)
              \ 			  		/   \
                  r,p (-1) 	   (0) s     i
                   \
                  i

    */
    /* deletion
    (a=1)	  s	(-1)			   	 r (0)
            /  \ 			  		/   \
             d	r,p (-1) 	   (0) s     x
                   \
                  x
    */
    /* deletion r bf == 0 height of tree is not reduced
    (a=1)	  s	(-1)		   		r (a)	r bf = a =1
            /  \ 			  		/   \
             d	r,p (0) 	  (-1) s     x  s bf = same
                 / \     	  			\
              y   x 				 y

    */
}

/********************************************************************************************
	Rotation Function whene there is LR Imbalance
	input: s will point to the place where rebalancing may be necessary
		   r point to the node where firsts potential rotation may happen.
		   a is new bf at parent according to position of node in question
		   op is operation whether it is for insertion or deletion
********************************************************************************************/
ANode* AVLTree::Imbalance_LR(ANode* s, ANode *r, int a, int op){
    ANode *p = r->rightc;
    r->rightc = p->leftc; //leftc rotation
    p->leftc = r;
    s->leftc = p->rightc;  //rightc rotation
    p->rightc = s;

    // adjusting balance factor
    if(op==1) //insertion operation
    {
        s->bf = (p->bf == 0) ? 0 : (p->bf == a) ? -1*a : 0;
        r->bf = (p->bf == 0) ? 0 : (p->bf == a) ? 0 : a;

        /*  insertion
        (a=1)	  s	(2)			    p
                /  \			  /   \
           (-1) r	x 		 	 r      s
             /  \   		   /  \	  /   \
            y    p			  y	 plc  prc  x
             plc/ \prc
        */
    }
    else if(op==0)// deletion operation
    {
        s->bf = (p->bf == 0) ? 0 : (p->bf == a) ? 0 : a;
        r->bf = (p->bf == 0) ? 0 : (p->bf == a) ? -1*a : 0;
        /* deletion operation
        (a=-1)    s	(1)			    p
                /  \			  /   \
           (1) r	d 		 	 r      s
             /  \    \		   /  \	  /   \
            y    p	  x		  y	 plc  prc  x
            plc/  \prc
        */
    }

    p->bf = 0;
    return p;
}

/********************************************************************************************
	Rotation Function whene there is RL Imbalance
	input: s will point to the place where rebalancing may be necessary
		   r point to the node where firsts potential rotation may happen.
		   a is new bf at parent according to position of node in question
		   op is operation whether it is for insertion or deletion
********************************************************************************************/
ANode* AVLTree::Imbalance_RL(ANode* s, ANode *r, int a, int op){
    ANode *p = p = r->leftc;
    r->leftc = p->rightc;  //rightc rotation
    p->rightc = r;
    s->rightc = p->leftc;  //leftc rotation
    p->leftc = s;

    // adjusting balance factor
    if(op==1) //insertion operation
    {
        s->bf = (p->bf == 0) ? 0 : (p->bf == a) ? -1*a : 0; //	s->bf = (p->bf == 0) ? 0 : (p->bf == 1) ? -1 : 0;
        r->bf = (p->bf == 0) ? 0 : (p->bf == a) ? 0 : a;  //	r->bf = (p->bf == 0) ? 0 : (p->bf == 1) ? 0 : 1;
        /* insertion
        (a=-1)	  s			    	p
                /  \			  /   \
               x	r (1)		 s     r
                   / \		   /  \	  /   \
                  p	  y		  x	 plc  prc  y
             plc/  \prc
        */
    }
    else if(op==0)// deletion operation
    {
        s->bf = (p->bf == 0) ? 0 : (p->bf == a) ? 0 : a;
        r->bf = (p->bf == 0) ? 0 : (p->bf == a) ? -1*a : 0;
        /* deletion
        (a=+1)    s	(-1)			p
                /  \			  /   \
               d	r (+1)		 s     r
              /    / \		   /  \	  /   \
             x    p	  y		  x	 plc  prc  y
             plc/  \prc
        */
    }
    p->bf = 0;
    return p;
}

/********************************************************************************************
	GraphViz Print Function
	input: ptr is the root node from where we have to print. file_name is the name of the file to be saved.
********************************************************************************************/
void AVLTree::t_print_graphviz(ANode *ptr, string file_name) {
    if (ptr == nullptr) {
        cout << " Tree is Empty";
        return;
    }

    ofstream fout;
    string fileopen = file_name+".gv";
    fout.open(fileopen.c_str(), ios::out);
    if (!fout) {
        cout << "\n**File failed to open**\n\n";
        fout.clear();
    }

    fout << "digraph AVL {" << endl;
    fout << "node [shape = record,height=.1];" << endl << endl;

    fout << "X[label = \"<L> LC| {<D> Key (BF)|Ht }|<R> RC \"];"<< endl ;

    queue<ANode *> qLoAVL;		// queue level order AVL
    qLoAVL.push(ptr);

    while (!qLoAVL.empty())		// Level order Traversal
    {
        ANode *tmp = qLoAVL.front();
//        	cout << tmp->key << " ";
        if (tmp->leftc != nullptr  || tmp->rightc != nullptr)
            fout << tmp->key << "[label = \"<L>| {<D> " << tmp->key << "(" <<tmp->bf <<") |"<<tmp->ht<< "} |<R> \"];"<< endl;
        else
            fout << tmp->key << "[label = \"<L>* | {<D> " << tmp->key << "(" <<tmp->bf <<") |"<<tmp->ht<< "} |<R>* \", fontcolor=red];" << endl;

        if (tmp->leftc != nullptr)
            fout << "\"" << tmp->key << "\":L ->" << tmp->leftc->key << ":D" << endl;

        if (tmp->rightc != nullptr)
            fout << "\"" << tmp->key << "\":R ->" << tmp->rightc->key << ":D" << endl;

        fout << endl;

        qLoAVL.pop();

        if (tmp->leftc != nullptr) /* Enqueue leftc child */
            qLoAVL.push(tmp->leftc);

        if (tmp->rightc != nullptr) /*Enqueue rightc child */
            qLoAVL.push(tmp->rightc);
    }

    fout << "}" << endl;
    fout.close();

    cout << "\n\t AVL Tree printed - file name = "<<file_name<<".png";
    string cmd = "dot -Tpng "+file_name+".gv -o "+file_name+".png";
    system((const char*)cmd.c_str());

    remove(fileopen.c_str());
}

/********************************************************************************************
	To Get Average Height of the Nodes of the Tree
********************************************************************************************/
unsigned long int AVLTree::calculate_sum_of_nodes_heights(ANode *tnode) {
    unsigned long int left_ht_sum = 0, right_ht_sum = 0;
    int left_ht = 0, right_ht = 0;

    if (tnode == nullptr)
        return 0;

    if (tnode->leftc == nullptr && tnode->rightc == nullptr){
        tnode->ht=0;
        return tnode->ht;
    }

    if (tnode->leftc != nullptr)
    {
        left_ht_sum = calculate_sum_of_nodes_heights(tnode->leftc);

        left_ht = tnode->leftc->ht;
    }
    if (tnode->rightc != nullptr) {
        right_ht_sum = calculate_sum_of_nodes_heights(tnode->rightc);

        right_ht = tnode->rightc->ht;
    }
    tnode->ht = 1 + ((left_ht > right_ht) ? left_ht : right_ht);
//    cout << "\n tnode->key: "<<tnode->key<<" h "<< tnode->ht;
//    cout << "\n left_ht_sum "<<left_ht_sum;
//    cout << "\n right_ht_sum "<<right_ht_sum;
    return (tnode->ht + left_ht_sum + right_ht_sum);
}

/********************************************************************************************
	To Display All the parameters of the Tree.
********************************************************************************************/
void AVLTree::show_parameters(){
    troot =  head->rightc;
    height_sum_nodes = calculate_sum_of_nodes_heights(troot);

    if(troot!= nullptr)
        height_final_avl=head->leftc->key;
    else
        height_final_avl = 0;

    if(total_nodes!=0)
        height_avg_node_avl= (double)height_sum_nodes / total_nodes;
    else
        height_avg_node_avl=0;

    comp_final_avl = comp_insert_avl + comp_delete_avl;
    rot_final_avl  = rot_insert_avl + rot_delete_avl;

    cout << "\n\n <------- Current Value of Parameters  ------->";

    cout << "\n Total Nodes: "<<total_nodes;
    cout << "\n Height - Height of Final Tree: " << height_final_avl;
    cout << "\n Height - Avg Height of elements (SUM: "<<height_sum_nodes<<") in Final Tree: " << height_avg_node_avl;

    cout << "\n Comparisons - Total Node Comparisons in Insert Delete: " << comp_final_avl;
    cout << "\n Comparisons - Total Node Comparisons in Insert: " << comp_insert_avl;
    cout << "\n Comparisons - Total Node Comparisons in Delete: " << comp_delete_avl;

    cout << "\n Rotations - Total Rotations in Insert Delete: " << rot_final_avl;
    cout << "\n Rotations - Total Rotations in Insert: " << rot_insert_avl;
    cout << "\n Rotations - Total Rotations in Delete: " << rot_delete_avl;
    cout << "\n <-------->";

}//end of show_parameters

void AVLTree::write_parameters(ofstream &foutt, int nop) {
    troot =  head->rightc;
    height_sum_nodes = calculate_sum_of_nodes_heights(troot);

    if(troot!= nullptr)
        height_final_avl=head->leftc->key;
    else
        height_final_avl = 0;

    if(total_nodes!=0)
        height_avg_node_avl= (double)height_sum_nodes / total_nodes;
    else
        height_avg_node_avl=0;

    comp_final_avl = comp_insert_avl + comp_delete_avl;
    rot_final_avl  =   + rot_delete_avl;

//    foutt<<nop<<","<<total_nodes<<","<<height_final_avl<<","<<height_avg_node_avl<<","<<comp_final_avl<<","<<comp_insert_avl<<","<<comp_delete_avl<<","<<rot_final_avl<<","<<rot_insert_avl<<","<<rot_delete_avl<<"\n";
}


#endif //DSA_A03_AVLCLASS_H
/********************************************************************************************
	Main Function
********************************************************************************************/
/*
int main(){

    string fn ; 		// filename for png file to be saved.
    string suffix ;    	// filename suffix to distinguish different files.

    char choice;			// choice exercised.
    int flag;			// flag for element Search result.

    AVLTree avl_obj ;		// AVL Tree object

    do {
        int inf;
        char ch;
        cout << endl << endl << endl;
        system(PAUSE);
        system(CLR);

        avl_obj.display();

        cout << "\n\n ---- AVL TREE MENU  ----";

        cout << "\n 1. Display - Console 			";
        cout << "\n 2. Default Tree - Insertion 			";

        cout << "\n\n 3. INSERT an element k ";
        cout << "\n 4. SEARCH an element k";
        cout << "\n 5. DELETE an element k";

        cout << "\n\n p. Print Tree - GraphViz";
        cout << "\n s. Show Parameters";
        cout << "\n n. Exit - Bye		\n\n  --Choice : ";
        cin >> ch;
        cout << "\n --------";

        switch (ch)
        {
            case '1' :    avl_obj.display();
                break;
                //Default Insert.
            case '2' :
                try {


                    avl_obj.Insert(6);
                    avl_obj.Insert(2);
                    avl_obj.Insert(9);
                    avl_obj.Insert(1);
                    avl_obj.Insert(4);
                    avl_obj.Insert(8);
                    avl_obj.Insert(11);
                    avl_obj.Insert(3);
                    avl_obj.Insert(5);
                    avl_obj.Insert(7);
                    avl_obj.Insert(10);
                    avl_obj.Insert(12);
                    avl_obj.Insert(13);
                    //1
//								avl_obj.Insert(21);
//								avl_obj.Insert(26);
//								avl_obj.Insert(30);
//								avl_obj.Insert(9);
//								avl_obj.Insert(4);
//								avl_obj.Insert(14);
//								avl_obj.Insert(28);
//								avl_obj.Insert(18);
//								avl_obj.Insert(15);
//								avl_obj.Insert(10);
//								avl_obj.Insert(2);
//								avl_obj.Insert(3);
//								avl_obj.Insert(7);
                    //2
//								avl_obj.Insert(60);
//								avl_obj.Insert(50);
//								avl_obj.Insert(80);
//								avl_obj.Insert(20);
//								avl_obj.Insert(55);
//								avl_obj.Insert(75);
//								avl_obj.Insert(82);
//
//								avl_obj.Insert(10);
//								avl_obj.Insert(32);
//								avl_obj.Insert(53);
//								avl_obj.Insert(56);
//								avl_obj.Insert(73);
//								avl_obj.Insert(76);
//								avl_obj.Insert(81);
//								avl_obj.Insert(83);
//
//								avl_obj.Insert(7);
//								avl_obj.Insert(12);
//								avl_obj.Insert(30);
//								avl_obj.Insert(40);
//								avl_obj.Insert(52);
//								avl_obj.Insert(54);
//								avl_obj.Insert(57);
//								avl_obj.Insert(72);
//								avl_obj.Insert(74);
//								avl_obj.Insert(78);
//								avl_obj.Insert(84);
//
//								avl_obj.Insert(6);
//								avl_obj.Insert(8);
//								avl_obj.Insert(14);
//								avl_obj.Insert(45);
//								avl_obj.Insert(51);
//								avl_obj.Insert(71);
//
//								avl_obj.Insert(5);
                    //3
//								avl_obj.Insert(20);
//								avl_obj.Insert(10);
//								avl_obj.Insert(30);
//								avl_obj.Insert(3);
//								avl_obj.Insert(15);
//								avl_obj.Insert(25);
//								avl_obj.Insert(40);
//								avl_obj.Insert(2);
//								avl_obj.Insert(9);
//								avl_obj.Insert(35);
//

                    cout << "\n Default Tree is inserted. ";
                }
                catch (const char* msg) {
                    cout <<"\t"<<msg<< endl;
                }
                break;

                //Insert
            case '3' :  cout <<"\n Enter Information (int) for new AVL Node .. : ";
                cin >> inf;		// Saving Info for the new node
                if (cin.fail()) {//Not an int.
                    cout <<"\t"<<"Not an Integer:"<< endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                try {
                    avl_obj.Insert(inf);
                    cout << "\tNODE  INSERTED : "<<inf;
                }
                catch (const char* msg) {
                    cout <<"\t"<<msg<< endl;
                }
                break;

                //Search
            case '4' :	cout << "\n Enter a Node value to SEARCH : ";
                cin >> inf;
                if (cin.fail()) {//Not an int.
                    cout <<"\t"<<"Not an Integer:"<< endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                flag = avl_obj.Search(inf);

                if (flag == 1) {
                    cout << "\n Search Successful for Element (" << inf << ").";
                    if (avl_obj.returnPar() != nullptr)
                        cout << "\n 	Its parent value and address: " << avl_obj.returnParKey() << " , " << avl_obj.returnPar();
                    cout << "\n	Its location: " << avl_obj.returnLocKey << " , " << avl_obj.returnLoc();
                }
                else if (flag == -1) {
                    cout << "\n Element (" << inf << ") Not Found.";
                }
                else
                    cout << "\n Search Operation Unsuccessful.";
                break;

                //Delete
            case '5' :	cout << "\n Enter a Node value to DELETE : ";
                cin >> inf;
                if (cin.fail()) {//Not an int.
                    cout <<"\t"<<"Not an Integer:"<< endl;
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

                fn = "AVLTree_"+suffix;;
                cout << "\n Printing Tree: ";

                avl_obj.print_graphviz(fn);
                break;

                //show parameters
            case 's':
                avl_obj.show_parameters();
                break;

            case 'n' : cout << "\n Bye \n"; break;
            default  :   cout << "\n--Invalid Choice. Enter Again \n";
        }
        choice=ch;
    } while (choice != 'n');

    cout << "\n-->ENTER TO EXIT. \n";
    cin.get();
    cin.get();
    return 0;
}//end of main()
 */


