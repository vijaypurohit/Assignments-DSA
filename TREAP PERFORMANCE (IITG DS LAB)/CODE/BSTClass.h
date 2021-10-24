#ifndef DSA_A03_BSTCLASS_H
#define DSA_A03_BSTCLASS_H
/********************************************************************************************
	Threaded BST Class
********************************************************************************************/
class ThBST {
private:
    ThBSTNode *troot;
    ThBSTNode *ptr, *newptr;
    ThBSTNode *save, *loc, *par;

    void t_print_graphviz(ThBSTNode *ptr, string fname); // print by graphviz, ptr is provided in order to print different rooted tree.
    void t_display(ThBSTNode*, int ); // display helper function for displaying on console.



public:
    unsigned long int height_final_bst, height_sum_nodes;   // height of final tree, sum of height of each node
    double height_avg_node_bst;
    unsigned long int comp_insert_bst, comp_delete_bst, comp_final_bst;       // node comparisons count
    unsigned long int total_nodes;

    ThBST() {
        troot = nullptr;
        ptr = nullptr; newptr = nullptr;
        save = nullptr; loc = nullptr; par = nullptr;
        total_nodes=0;
        height_final_bst=0; height_avg_node_bst=0; height_sum_nodes=0;
        comp_insert_bst=0; comp_delete_bst=0; comp_final_bst=0;
    }

    void Insert(int); //insert function
    int Search(int); //Search function
                     // Delete functions
    void Delete(int);
    void case_a(ThBSTNode *, ThBSTNode *); // node having no child
    void case_b(ThBSTNode *, ThBSTNode *); // node having both child
    void case_c(ThBSTNode *, ThBSTNode *); // node having one child

    void display() {	t_display(troot, 1);	}
    void print_graphviz(string file_name) { t_print_graphviz(troot, file_name); }
    void show_parameters();     // to display all the parameters of the Tree
    void write_parameters(ofstream &foutt, int nop);
    unsigned long calculate_sum_of_nodes_heights(ThBSTNode *); // to calculate sum of height of each node


    ThBSTNode* inOrderSuccessor(ThBSTNode *pNode);
    ThBSTNode* inOrderPredecessor(ThBSTNode *pNode);

    ThBSTNode* returnPar() { return par;}
    ThBSTNode* returnLoc() {return loc; }
    int returnParKey() {return par->key;}
    int returnLocKey() {return loc->key; }

    ~ThBST() {
//        cout << "\n Free Memory (ThBST), deleting: ";
//        troot = makeEmpty(troot);
//        if(troot)cout<<troot->key<<"C ";
        delete troot;
        troot = nullptr;
    }

};

/********************************************************************************************
	Helper function for Displaying Tree in the console for better visualization and operations handling.
	Input: tnode as a root node for recursively printing, level as Tree Level initiallly 1.
********************************************************************************************/
void ThBST::t_display(ThBSTNode *tnode, int level)
{
    int i;
    if (tnode != nullptr)
    {
        if (!tnode->isRightTh)
            t_display(tnode->rightc, level + 1);

        cout << endl;
        if (tnode == troot)
            cout << "Root->:"<<troot->ht<<": ";
        else {
            for (i = 0; i < level; i++)
                cout << "       ";
        }
        cout << tnode->key<< " (h " << tnode->ht<<") " ;
//		cout << tnode->key << "(" << tnode->lcnt << "," << tnode->rcnt << ")";
        if (!tnode->isLeftTh)
            t_display(tnode->leftc, level + 1);
    }
}

/********************************************************************************************
	Search function for node key in BST TREE.
    Finds the location (loc) of item key and also location(par) of parent of item
    *   loc = nullptr and par = nullptr (troot=nullptr) => tree empty
    *	loc != nullptr (=ptr=troot) and par = nullptr  => item is troot
    *	loc = nullptr and par != nullptr  => item is not in tree
	Input: item to be searched
********************************************************************************************/
int ThBST::Search(int item)
{
    save = nullptr;  // to store parent location temporarary.
    ptr = troot;

    while (ptr != nullptr) {
        if ( item == ptr->key ) {
            loc = ptr;
            par = save;
            return 1;
        }
        save = ptr;
        if ( item < ptr->key ) {
            if (!ptr->isLeftTh)
                ptr = ptr->leftc;
            else
                break;
        }
        else {
            if (!ptr->isRightTh)
                ptr = ptr->rightc;
            else
                break;
        }
    }
    loc = nullptr;
    par = save;
    return -1;
}

/********************************************************************************************
	Insert function for inserting into BSTree.
	Input: item to be inserted.
********************************************************************************************/
void ThBST::Insert(int k) {

    save = nullptr;  // to store parent location temporarary.
    ptr = troot;

    while (ptr != nullptr) {

        comp_insert_bst++;

        if ( k == ptr->key ) {
            loc = ptr;
            par = save;
            throw "Duplicate Values Detected!!";
        }
        save = ptr;
        if ( k < ptr->key ) {
            if (!ptr->isLeftTh)
                ptr = ptr->leftc;
            else
                break;
        }
        else {
            if (!ptr->isRightTh)
                ptr = ptr->rightc;
            else
                break;
        }
    }
    loc = nullptr;
    par = save; // parent location.

    ThBSTNode *np = new ThBSTNode(k);
    total_nodes++;

    if (troot == nullptr)
        troot = np;
    else if ( np->key < par->key)
    {
        np->leftc = par->leftc;
        np->rightc = par;

        par->isLeftTh = false;
        par->leftc = np;
    }
    else {
        np->rightc = par->rightc;
        np->leftc = par;

        par->isRightTh = false;
        par->rightc = np;
    }

}

/********************************************************************************************
	Traversal function for finding InOrder Successor
********************************************************************************************/
ThBSTNode* ThBST::inOrderSuccessor(ThBSTNode *pNode) {

    // if Right Link is Thread to inorder successor
    if (pNode->isRightTh) {
        return pNode->rightc;
    }
    //Else return leftmost child of rightc subtree
    pNode = pNode->rightc;
    while (!pNode->isLeftTh)
        pNode = pNode->leftc;
    return pNode;
}

/********************************************************************************************
	Traversal function for finding InOrder Predecessor
********************************************************************************************/
ThBSTNode* ThBST::inOrderPredecessor(ThBSTNode *pNode) {

    // if Left Link is Thread to inorder predecessor
    if (pNode->isLeftTh) {
        return pNode->leftc;
    }
    //Else return rightmost child of leftc subtree
    pNode = pNode->leftc;
    while (!pNode->isRightTh)
        pNode = pNode->rightc;
    return pNode;
}

/********************************************************************************************
	Delete function for deleting from Threaded BSTree
	Input: item to be deleted.
********************************************************************************************/
void ThBST::Delete(int k) {
    if (troot == nullptr) {
//        cout << "\n\t\t => Tree empty" << endl;
//        return;
        throw "=> Tree empty";
    }

    save = nullptr;  // to store parent location temporarary.
    ptr = troot;
    while (ptr != nullptr) {

        comp_delete_bst++;

        if ( k == ptr->key ) {
            loc = ptr;
            par = save;
            break;
        }
        save = ptr;
        if ( k < ptr->key ) {
            if (!ptr->isLeftTh)
                ptr = ptr->leftc;
            else{
                loc = nullptr;
                par = save;
                break;
            }
        }
        else {
            if (!ptr->isRightTh)
                ptr = ptr->rightc;
            else{
                loc = nullptr;
                par = save;
                break;
            }
        }
    }

    if (loc == nullptr){
//        cout << "\n\t\t =>  Item not present in tree: " <<k<< endl;
        throw "Item not present in tree";
        return;
    }

    if ((!loc->isLeftTh && !loc->isRightTh))	// node having both child
        case_b(par, loc);
    else if (!loc->isLeftTh || !loc->isRightTh) // node having one child
        case_c(par, loc);
    else 									   // node having no child
        case_a(par, loc);

    total_nodes--;

//    cout << "\n\t Element Deleted \n";
    loc = nullptr;
}

/********************************************************************************************
	Delete Helper function. For Node having no child.
	input: par is parent of node to be deleted. loc is location of node to be deleted.
********************************************************************************************/
void ThBST::case_a(ThBSTNode* par, ThBSTNode* loc){

    if (par == nullptr)
        troot = nullptr;				//   When root is the only node in tree having no child
    else if (loc == par->leftc) {		// if node is in leftc of parent
        par->isLeftTh = true;
        par->leftc = loc->leftc;
        loc->leftc=nullptr;
    }
    else {
        par->isRightTh = true;
        par->rightc = loc->rightc;
        loc->rightc=nullptr;
    }

    delete loc;
}

/********************************************************************************************
	Delete Helper function. For Node having one child
	input: par is parent of node to be deleted. loc is location of node to be deleted.
********************************************************************************************/
void ThBST::case_c(ThBSTNode* par, ThBSTNode* loc){

    ThBSTNode *child, *LocinSucc, *LocinPred;

    if (!loc->isLeftTh)
        child = loc->leftc;
    else
        child = loc->rightc;

    if (par == nullptr) {
        troot = child;				//   When root is the only node in tree having no child
    }
    else
    {
        if (loc == par->leftc)		// if node is in leftc of parent
        {
            par->leftc = child;
        }
        else {
            par->rightc = child;
        }

    }
    LocinSucc = inOrderSuccessor(loc);
    LocinPred = inOrderPredecessor(loc);

    //   has leftc subtree.
    if (!loc->isLeftTh)
        LocinPred->rightc = LocinSucc;
    else if (!loc->isRightTh) { // has rightc subtree.
        LocinSucc->leftc = LocinPred;
    }

    loc->leftc= nullptr;
    loc->rightc= nullptr;
    loc->isLeftTh= true;
    loc->isRightTh= true;
    delete loc;
}

/********************************************************************************************
	Delete Helper function. For Node having both child
	input: par is parent of node to be deleted. loc is location of node to be deleted.
********************************************************************************************/
void ThBST::case_b(ThBSTNode* par, ThBSTNode* loc){
    ThBSTNode *suc, *parsuc;							// suc = inorder successor of loc node . parsuc = loc of the parent of the suc
    // findinf suc and parsuc
    save = loc;

    ptr = loc->rightc;
    while (!ptr->isLeftTh) {
        comp_delete_bst++;
        save = ptr;
        ptr = ptr->leftc;
    }

    parsuc = save;
    suc = ptr;

    loc->key = suc->key;

    if (suc->isLeftTh && suc->isRightTh)	 // delete inorder successor
        case_a(parsuc, suc);					//node having no child
    else
        case_c(parsuc, suc);					//node having one child
}

/********************************************************************************************
	GraphViz Print Function
	input: ptr is the root node from where we have to print. file_name is the name of the file to be saved.
********************************************************************************************/
void ThBST::t_print_graphviz(ThBSTNode *ptr, string file_name) {
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

    fout << "digraph ThBST {" << endl;
    fout << "node [shape = record,height=.1];" << endl << endl;

    fout << "X[label = \"<L> LC| <D> Key (Ht) |<R> RC \"];"<< endl ;

    queue<ThBSTNode *> qLoBST;		// queue level order BST
    qLoBST.push(ptr);

    while (!qLoBST.empty())
    {
        ThBSTNode *tmp = qLoBST.front();
//        	cout << tmp->key << " ";
        if (!tmp->isLeftTh || !tmp->isRightTh)
            fout << tmp->key << "[label = \"<L>| <D> " << tmp->key  <<"("<<tmp->ht<<")"<<  " |<R> \"]" << endl;
        else
            fout << tmp->key << "[label = \"<L>*| <D> " << tmp->key <<"("<<tmp->ht<<")"<< " | <R>* \", fontcolor=red]" << endl;

        if (!tmp->isLeftTh)
            fout << "\"" << tmp->key << "\":L ->" << tmp->leftc->key << ":D" << endl;
        else if (tmp->leftc != nullptr )
            fout << "\"" << tmp->key << "\":L ->" << tmp->leftc->key << ":D[style=dotted, color=blue]" << endl;

        if (!tmp->isRightTh)
            fout << "\"" << tmp->key << "\":R ->" << tmp->rightc->key << ":D" << endl;
        else if (tmp->rightc != nullptr )
            fout << "\"" << tmp->key << "\":R ->" << tmp->rightc->key << ":D[style=dotted, color=blue]" << endl;

        fout << endl;

        qLoBST.pop();
        /* Enqueue leftc child */
        if (!tmp->isLeftTh)
            qLoBST.push(tmp->leftc);
        /*Enqueue rightc child */
        if (!tmp->isRightTh)
            qLoBST.push(tmp->rightc);
    }

    fout << "}" << endl;
    cout << "\n\t Threaded BST printed - file name = "<<file_name<<".png\n";
    fout.close();
    string cmd = "dot -Tpng "+file_name+".gv -o "+file_name+".png";
    system((const char*)cmd.c_str());
    remove(fileopen.c_str());
}

/********************************************************************************************
	To Get Average Height of the Nodes of the Tree
********************************************************************************************/
unsigned long int ThBST::calculate_sum_of_nodes_heights(ThBSTNode *tnode) {
    unsigned long int left_ht_sum = 0, right_ht_sum = 0;
    int left_ht = 0, right_ht = 0;

    if (tnode == nullptr)
        return 0;

    if (tnode->isLeftTh && tnode->isRightTh){
        tnode->ht=0;
        return tnode->ht;
    }

    if (!tnode->isLeftTh)
    {
        left_ht_sum = calculate_sum_of_nodes_heights(tnode->leftc);

        left_ht = tnode->leftc->ht;
    }
    if (!tnode->isRightTh) {
        right_ht_sum = calculate_sum_of_nodes_heights(tnode->rightc);

        right_ht = tnode->rightc->ht;
    }
    tnode->ht = 1 + ((left_ht > right_ht) ? left_ht : right_ht);
//    cout << "\n tnode->key: "<<tnode->key<<" h "<< tnode->ht;
//    cout << "\t left_ht_sum "<<left_ht_sum;
//    cout << "\t right_ht_sum "<<right_ht_sum;
    return (tnode->ht + left_ht_sum + right_ht_sum);
}

/********************************************************************************************
	To Display All the parameters of the Tree.
********************************************************************************************/
void ThBST::show_parameters(){

    height_sum_nodes = calculate_sum_of_nodes_heights(troot);

    if(troot!= nullptr)
        height_final_bst=troot->ht;
    else
        height_final_bst = 0;

    if(total_nodes!=0)
        height_avg_node_bst= (double)height_sum_nodes / total_nodes;
    else
        height_avg_node_bst=0;

    comp_final_bst = comp_insert_bst + comp_delete_bst;

    cout << "\n\n <------- Current Value of Parameters  ------->";

    cout << "\n Total Nodes: "<<total_nodes;
    cout << "\n Height - Height of Final Tree: " << height_final_bst;
    cout << "\n Height - Avg Height of elements (SUM: " << height_sum_nodes << ") in Final Tree: " << height_avg_node_bst;

    cout << "\n Comparisons - Total Node Comparisons in Insert Delete: " << comp_final_bst;
    cout << "\n Comparisons - Total Node Comparisons in Insert: " << comp_insert_bst;
    cout << "\n Comparisons - Total Node Comparisons in Delete: " << comp_delete_bst;

    cout << "\n <-------->";

}//end of show_parameters

void ThBST::write_parameters(ofstream &foutt, int nop) {
    height_sum_nodes = calculate_sum_of_nodes_heights(troot);

    if(troot!= nullptr)
        height_final_bst=troot->ht;
    else
        height_final_bst = 0;

    if(total_nodes!=0)
        height_avg_node_bst= (double)height_sum_nodes / total_nodes;
    else
        height_avg_node_bst=0;

    comp_final_bst = comp_insert_bst + comp_delete_bst;
//    foutt<<nop<<","<<total_nodes<<","<<height_final_bst<<","<<height_avg_node_bst<<","<<comp_final_bst<<","<<comp_insert_bst<<","<<comp_delete_bst<<","<<0<<","<<0<<","<<0<<"\n";
}

#endif //DSA_A03_BSTCLASS_H

 /********************************************************************************************
	Main Function
********************************************************************************************/
/*
int main() {
    string fn ; 		// filename for png file to be saved.
    string suffix ;    	// filename suffix to distinguish different files.

    char choice;			// choice exercised.
    int flag;			// flag for element Search result.

    ThBST bst_obj;		//  Threaded BSTree object

    do {
        int inf;
        char ch;
        cout << endl << endl << endl;
        system(PAUSE);
        system(CLR);

        bst_obj.display();

        cout << "\n\n ----  BINARY SEARCH TREE MENU  ----";

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

            case '1' :    bst_obj.display();
                break;
            case '2' :
                try {
                    bst_obj.Insert(30);
                    bst_obj.Insert(20);
                    bst_obj.Insert(40);
                    bst_obj.Insert(70);
                    bst_obj.Insert(60);
                    bst_obj.Insert(80);
                    bst_obj.Insert(10);
                    bst_obj.Insert(15);
                }
                catch (const char* msg) {
                    cout <<"\t"<<msg<< endl;
                }
                break;
                //Insert
            case '3' :  cout << "\n Enter Infomartion (int) for new BST Node .. : ";
                cin >> inf;		// Saving Info for the new node
                if (cin.fail()) { //Not an int.
                    cout <<"\t"<<"Not an Integer:"<< endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                try {
                    bst_obj.Insert(inf);
                    cout << "\tNODE  INSERTED : "<<inf;
                }
                catch (const char* msg) {
                    cout <<"\t"<<msg<< endl;
                }
                break;
            case '4' :	cout << "\n Enter a Node value to SEARCH : ";
                cin >> inf;
                if (cin.fail()) {//Not an int.
                    cout <<"\t"<<"Not an Integer:"<< endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                flag = bst_obj.Search(inf);
                if (flag == 1) {
                    cout << "\n Search Successful for Element (" << inf << ").";
                    if (bst_obj.returnPar() != nullptr)
                        cout << "\n 	Its parent value and address: " << bst_obj.returnParKey()  << " , " << bst_obj.returnPar();
                    cout << "\n	Its location: " << bst_obj.returnLocKey() << " , " << bst_obj.returnLoc();
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
                bst_obj.Delete(inf);
                break;

                //Print
            case 'p' : cout << "\n Enter File Name suffix to distinguish your operation (w/o space): ";
                cin>>suffix;

                fn = "ThBST_"+suffix;;
                cout << "\n Printing Tree: ";

                bst_obj.print_graphviz(fn);
                break;

                //show parameters
            case 's':
                bst_obj.show_parameters();
                break;

            case 'n' : cout << "\n Bye"; break;
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