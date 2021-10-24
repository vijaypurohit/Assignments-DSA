#ifndef DSA_A03_TREAPCLASS_H
#define DSA_A03_TREAPCLASS_H

/********************************************************************************************
	TREAP Class
********************************************************************************************/
class Treap {
private:
    TNode *troot;                   // troot is the root of the treap.
    TNode *ptr, *newptr;            // other pointers for help in functions
    TNode *save, *loc, *par;
    unsigned int upper_pr, lower_pr;    // upper value and lower value of prority respectively.


    TNode * t_insert(TNode *, int, int);
    TNode * t_delete(TNode *, int);
    void t_print_graphviz(TNode *, string ); // print by graphviz, ptr is provided in order to print different rooted tree.
    void t_display(TNode *, int ); // display helper function for displaying in console.

    // function to delete all the nodes of the tree and make tree empty.
    TNode* makeEmpty(TNode *tnode) {
        ptr = tnode; TNode *temp;
        if (tnode == nullptr) { 	// if root of tree is NULL
            return nullptr;
        }
        queue<TNode *> qLo;		// queue level order
        qLo.push(ptr);

        while (!qLo.empty())
        {
            temp = qLo.front();
            cout << " "<< temp->key ;
            qLo.pop();

            if (temp->leftc != nullptr)
                qLo.push(temp->leftc);
            if (temp->rightc != nullptr)
                qLo.push(temp->rightc);

            delete temp;
        }
        return nullptr;
    } //end of makeEmpty()

public:

    unsigned long int height_final_treap, height_sum_nodes;   // height of final tree, sum of height of each node
    double height_avg_node_treap;
    unsigned long int comp_insert_treap, comp_delete_treap, comp_final_treap;       // node comparisons count
    unsigned long int rot_insert_treap, rot_delete_treap, rot_final_treap;          // rotations count
    unsigned long int total_nodes;                                                           // total count of nodes.

    Treap(){
        troot = nullptr;
        ptr = nullptr;  newptr = nullptr;
        save = nullptr; loc = nullptr; par = nullptr;
        upper_pr=1000; lower_pr=100;

        total_nodes=0;
        height_final_treap=0; height_avg_node_treap=0; height_sum_nodes=0;
        comp_insert_treap=0; comp_delete_treap=0; comp_final_treap=0;
        rot_insert_treap=0; rot_delete_treap=0; rot_final_treap=0;

    }

    Treap(const Treap &o_obj); //copy constructor
    void operator=(const Treap &); // assignment operator overloading for copying
    TNode* copyLeftRightNode(TNode*);	// helper function for copying.

    void Insert(int); //insert function
    void Insert(int, int); //insert function with priority.
    int Search(int); //Search function
    void Delete(int);
    TNode *Imbalance_LL(TNode *);       // LL Imbalance, rightc rotation
    TNode *Imbalance_RR(TNode *);       // RR Imbalance, leftc rotation

    void display() 	{	t_display(troot, 1);}       //Display function in console.
    void print_graphviz(string file_name) {  t_print_graphviz(troot, file_name) ; }//printing using graphviz, taking filename as input to save
    void show_parameters();
    void write_parameters(ofstream &, int);
    unsigned long calculate_sum_of_nodes_heights(TNode *); // to calculate sum of height of each node

    // helper function to return value of pointers.
    TNode* returnPar() {return par;}
    TNode* returnLoc() {return loc;}
    int returnParKey() {return par->key;}
    int returnLocKey() {return loc->key;}

    //destructor
    ~Treap() {
        //cout << "\n Free Memory (TREAP), deleting: ";
        //head->rightc = makeEmpty(head->rightc);				// deleting all nodes
        //if(troot)cout<<troot->key<<"C ";
        delete troot;
        troot = nullptr;
    }
}; //end of Class Treap

/*******************************************************************************************
	copy constructor: copying from one object to new object
	Input: object to be copied.
********************************************************************************************/
Treap::Treap(const Treap &o_obj) {

    if (o_obj.troot == nullptr) {
        cout << "\n Empty Tree Can't Clone";
        troot = nullptr;
        return;
    }

    if (troot != nullptr) {
        troot = makeEmpty(troot);
    }
    troot = copyLeftRightNode(o_obj.troot);
}

/********************************************************************************************
	operator= overloading: copying from one object to new object
	input: object to be copied.
********************************************************************************************/
void Treap::operator=(const Treap &o_obj)
{
    if (o_obj.troot == nullptr) {
        cout << "\n Empty Tree Can't Clone";
        troot = nullptr;
        return;
    }

    if (troot != nullptr) {
        troot = makeEmpty(troot);
    }
    troot = copyLeftRightNode(o_obj.troot);
}

/********************************************************************************************
	Helper function for copy constructor, taking input as root and recursively copying.
	This function creates clone by copying key and leftc and rightc pointers
********************************************************************************************/
TNode* Treap::copyLeftRightNode(TNode* tnode) {
    if (tnode == nullptr)
        return nullptr;

    TNode* cloneNode =  new TNode(tnode->key, tnode->pr);
//    cloneNode->pr = tnode->pr;
//    cloneNode->key = tnode->key;
    cloneNode->leftc  = copyLeftRightNode(tnode->leftc);
    cloneNode->rightc = copyLeftRightNode(tnode->rightc);
    return cloneNode;
}

/********************************************************************************************
	Helper function for Displaying Tree in the console for better visualization and operations handling.
	Input: tnode as a root node for recursively printing, level as Tree Level initially 1.
********************************************************************************************/
void Treap::t_display(TNode *tnode, int level)
{
    int i;
    if (tnode != nullptr)
    {
        t_display(tnode->rightc, level + 1);
        cout<<endl;
        if (tnode == troot)
            cout<<"Root->:"<<troot->ht<<":  ";
        else
        {
            for (i = 0;i < level;i++)
                cout<<"       ";
        }
//	        cout << tnode->key;
        cout << tnode->pr << "|k: " << tnode->key<< " (h " << tnode->ht<<") " ;
        t_display(tnode->leftc, level + 1);
    }
}

/********************************************************************************************
	Rotation Function where there is LL Imbalance (rightc rotation)
	Input: Root Node Where Imbalance occur.
********************************************************************************************/
TNode *Treap::Imbalance_LL(TNode *root)
{
    //Imbalance LL //rightc rotation
    TNode *left_of_root = root->leftc;
    root->leftc = left_of_root->rightc;
    left_of_root->rightc = root;
    root->adjust_height();
    return left_of_root;
    /*
                      r                               lor
                    /  \     Right Rotation          /  \
                  lor   x   - - - - - - - >        llc    r
                  /  \                                   / \
                 llc  lrc                               lrc  x
     */
}

/********************************************************************************************
	Rotation Function where there is RR Imbalance (leftc rotation)
	Input: Root Node Where Imbalance occur.
********************************************************************************************/
TNode *Treap::Imbalance_RR(TNode *root)
{
    //Imbalance RR //leftc rotation
    TNode *right_of_root = root->rightc;
    root->rightc = right_of_root->leftc;
    right_of_root->leftc = root;
    root->adjust_height();
    return right_of_root;
    /*
                     ror                               r
                    /  \     Left Rotation           /  \
                   r   rrc   < - - - - - - -        x   ror
                  / \                                   / \
                 x  rlc                               rlc  rrc
     */
}

/********************************************************************************************
	Search function for node key in Treap.
	Input: item to be searched
********************************************************************************************/
int Treap::Search(int item)
{
    save = nullptr;  // to store parent location temporary.
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
} // end of Treap::Search

/********************************************************************************************
	Insert function for inserting into Treap.
	Input: item 'e' to be inserted
********************************************************************************************/
void Treap::Insert(int item){
    int priority = (rand() %(upper_pr - lower_pr + 1)) + lower_pr;

    // First Node Being Inserted.
    if(troot == nullptr){
        troot = new TNode(item,priority);
        total_nodes++;
        return ;
    }

    troot = t_insert(troot, item, priority);
}// end of Treap::Insert(item)
void Treap::Insert(int item, int priority){
    // First Node Being Inserted.
    if(troot == nullptr){
        troot = new TNode(item,priority);
        total_nodes++;
        return ;
    }

    troot = t_insert(troot, item, priority);
}// end of Treap::Insert(item, priority)

/********************************************************************************************
	Helper Recursive function for inserting into Treap and performing necessary rotations.
	Input: item 'e' to be inserted and Priority 'p'
********************************************************************************************/
TNode * Treap::t_insert(TNode *root, int e, int p){


    if(root == nullptr)         // Given Node(Root) is null, Creating New Node then Return it
    {   newptr = nullptr;
        newptr = new TNode(e,p);
        total_nodes++;
        return newptr;
    }

    comp_insert_treap++;

    if (e < root->key)      // If element is smaller than current root key
    {
        root->leftc = t_insert(root->leftc, e, p);  // Go and Insert in Left Sub-Tree

        if (root->leftc->pr < root->pr)  // Fix Min-Heap Ordering Property with respect to priorities.
        {   //Imbalance LL //rightc rotation
            rot_insert_treap++;
            root = Imbalance_LL(root);
        }
    }
    else if (e > root->key)  // If element is greater than current root key
    {
        root->rightc = t_insert(root->rightc, e, p); // Go and Insert in Right Sub-Tree

        if (root->rightc->pr < root->pr)// Fix Min-Heap Ordering Property with respect to priorities.
        {   //Imbalance RR //leftc rotation
            rot_insert_treap++;
            root = Imbalance_RR(root);
        }
    }
    else if(e == root->key){
//        throw "Duplicate Key Detected!! => " + to_string(e);
        throw "Duplicate Key Detected!! ";

    }
    root->adjust_height();
    return root;
} // end of Treap::Insert(root,e,p)

/********************************************************************************************
	Delete function for deleting from Treap
	Input: item to be deleted.
********************************************************************************************/
void Treap::Delete(int item) {
    if(troot == nullptr){
//        cout<<"\n\t\t => Tree Empty"<<endl;
//        return;
        throw "Tree Empty";
    }

    troot = t_delete(troot, item);
}//end of Treap::Delete(item)

/********************************************************************************************
	Helper Recursive function for deleting from Treap and performing necessary rotations.
	Input: item to be deleted.
********************************************************************************************/
TNode * Treap::t_delete(TNode * root, int e) {

    if(root == nullptr){
        throw "Element not Present in the Treap.";
//        return nullptr;
    }

    comp_delete_treap++;

    if(e < root->key)
    {
        root->leftc = t_delete(root->leftc, e);
    }
    else if(e > root->key)
    {
        root->rightc = t_delete(root->rightc, e);
    }
    else // element = root->key
    {
        if (root->leftc == nullptr && root->rightc == nullptr){
            delete (root);
            total_nodes--;
            return nullptr;
        }
        else if (root->leftc == nullptr){
            rot_delete_treap++;
            root = Imbalance_RR(root); //leftc rotate

            root->leftc = t_delete(root->leftc, e);
        }
        else if (root->rightc == nullptr){
            rot_delete_treap++;
            root = Imbalance_LL(root); //rightc rotate

            root->rightc = t_delete(root->rightc, e);
        } // If both leftc and rightc child are not NULL
        else if (root->leftc->pr < root->rightc->pr)
        {
            rot_delete_treap++;
            root = Imbalance_LL(root);
            root->rightc = t_delete(root->rightc, e);
        }
        else
        {
            rot_delete_treap++;
            root = Imbalance_RR(root);
            root->leftc = t_delete(root->leftc, e);
        }
    }
    root->adjust_height();
    return root;

} //end of Teap::Delete(item)

/********************************************************************************************
	GraphViz Print Function
	Input: tnode is the root node from where we have to print.
           file_name is the name of the file to be saved.
********************************************************************************************/
void Treap::t_print_graphviz(TNode *tnode, string file_name) {
    if (tnode == nullptr) {
        cout << " Tree is Empty"<<endl;
        return;
    }

    ofstream fout;
    string fileopen = file_name+".gv";
    fout.open(fileopen.c_str(), ios::out);
    if (!fout) {
        cout << "\n**File failed to open**\n\n";
        fout.clear();
    }

    fout << "digraph Treap {" << endl;
    fout << "node [shape = record,height=.1];" << endl << endl;

    fout << "X[label = \"<L> LC| {<D> Pr|Key (Ht) }|<R> RC \"];" ;

    queue<TNode *> qLoTreap;		// queue level order Treap
    qLoTreap.push(tnode);

    while (!qLoTreap.empty())		// Level order Traversal
    {
        TNode *tmp = qLoTreap.front();
//        	cout << tmp->key << " ";
        if (tmp->leftc != nullptr  || tmp->rightc != nullptr)
            fout << tmp->key << "[label = \"<L>| {<D> " << tmp->pr << "|" <<tmp->key <<" ("<<tmp->ht<<")"<< "} |<R> \"];"<< endl;
        else
            fout << tmp->key << "[label = \"<L>*| {<D> " << tmp->pr << "|" <<tmp->key <<" ("<<tmp->ht<<")"<<  "} |<R>* \", fontcolor=red];" << endl;

        if (tmp->leftc != nullptr)
            fout << "\"" << tmp->key << "\":L ->" << tmp->leftc->key << ":D;" << endl;

        if (tmp->rightc != nullptr)
            fout << "\"" << tmp->key << "\":R ->" << tmp->rightc->key << ":D;" << endl;

        fout << endl;

        qLoTreap.pop();

        if (tmp->leftc != nullptr) /* Enqueue leftc child */
            qLoTreap.push(tmp->leftc);

        if (tmp->rightc != nullptr) /*Enqueue rightc child */
            qLoTreap.push(tmp->rightc);
    }

    fout << "}" << endl;

    cout << "\n\t Treap printed - file name = "<<file_name<<".png";

    fout.close();

    string cmd = "dot -Tpng "+file_name+".gv -o "+file_name+".png";
    system((const char*)cmd.c_str());
    remove(fileopen.c_str());
}

/********************************************************************************************
	To Get Average Height of the Nodes of the Tree
********************************************************************************************/
unsigned long int Treap::calculate_sum_of_nodes_heights(TNode *tnode){
    unsigned long int left_ht = 0, right_ht = 0;

    if(tnode == nullptr)
        return 0;

    if(tnode->leftc == nullptr && tnode->rightc == nullptr)
        return tnode->ht;

    if(tnode->leftc != nullptr) left_ht = calculate_sum_of_nodes_heights(tnode->leftc);
    if(tnode->rightc != nullptr) right_ht = calculate_sum_of_nodes_heights(tnode->rightc);
    return (tnode->ht + left_ht + right_ht);
}
/********************************************************************************************
	To Display All the parameters of the Tree.
********************************************************************************************/
void Treap::show_parameters(){
    height_sum_nodes = calculate_sum_of_nodes_heights(troot);

    if (troot != nullptr)
        height_final_treap = troot->ht;
    else
        height_final_treap = 0;

    if (total_nodes != 0)
        height_avg_node_treap = (double) height_sum_nodes / total_nodes;
    else
        height_avg_node_treap = 0;

    comp_final_treap = comp_insert_treap + comp_delete_treap;
    rot_final_treap  = rot_insert_treap + rot_delete_treap;

    cout << "\n\n <------- Current Value of Parameters  ------->";

    cout << "\n Total Nodes: "<<total_nodes;
    cout << "\n Height - Height of Final Tree: "<<height_final_treap;
    cout << "\n Height - Avg Height of elements (SUM: "<<height_sum_nodes<<") in Final Tree: "<<height_avg_node_treap;

    cout << "\n Comparisons - Total Node Comparisons in Insert Delete: "<<comp_final_treap;
    cout << "\n Comparisons - Total Node Comparisons in Insert: "<<comp_insert_treap;
    cout << "\n Comparisons - Total Node Comparisons in Delete: "<<comp_delete_treap;

    cout << "\n Rotations - Total Rotations in Insert Delete: "<<rot_final_treap;
    cout << "\n Rotations - Total Rotations in Insert: "<<rot_insert_treap;
    cout << "\n Rotations - Total Rotations in Delete: "<<rot_delete_treap;
    cout << "\n <-------->";

}//end of show_parameters

void Treap::write_parameters(ofstream &foutt, int nop) {

    height_sum_nodes = calculate_sum_of_nodes_heights(troot);

    if (troot != nullptr)
        height_final_treap = troot->ht;
    else
        height_final_treap = 0;

    if (total_nodes != 0)
        height_avg_node_treap = (double) height_sum_nodes / total_nodes;
    else
        height_avg_node_treap = 0;

    comp_final_treap = comp_insert_treap + comp_delete_treap;
    rot_final_treap = rot_insert_treap + rot_delete_treap;
//    foutt<<nop<<","<<total_nodes<<","<<height_final_treap<<","<<height_avg_node_treap<<","<<comp_final_treap<<","<<comp_insert_treap<<","<<comp_delete_treap<<","<<rot_final_treap<<","<<rot_insert_treap<<","<<rot_delete_treap<<"\n";
}
#endif //DSA_A03_TREAPCLASS_H

