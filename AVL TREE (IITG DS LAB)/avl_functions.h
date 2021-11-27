/*******************************************************************************************
	copy constructor: copying from one object to other
	input: object to be copied.
********************************************************************************************/
ATree::ATree(const ATree &avl_o) {
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
	input: object to be copied.
********************************************************************************************/
void ATree::operator=(const ATree &avl_o)
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
	This function creates clone by copying key and left and right pointers
********************************************************************************************/
ANode* ATree::copyLeftRightNode(ANode* tnode) {
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
	input: ptr as a root node for recursively printing, level as Tree Level initiallly 1.
********************************************************************************************/
void ATree::t_display(ANode *ptr, int level)
{
	 int i;
	  if (ptr != NULL)
	  {
	    t_display(ptr->rightc, level+1);
	    	cout<<endl;
	    if (ptr == head->rightc)
	        cout<<"Root->:"<<head->leftc->key<<"  ";
	    else
	    {
		  for (i = 0;i < level;i++)
	        cout<<"       ";
		}
//	        cout<<ptr->key;
	        cout << ptr->key << "|" << ptr->bf ;
	        t_display(ptr->leftc, level+1);
	  }
}

/********************************************************************************************
	Search function for node key in AVL TREE.
	input: item to be searched
********************************************************************************************/
int ATree::search(int item)
{
	troot =  head->rightc;
	save = nullptr;  // to store parent location temporarary.
	ptr = troot;

	while(ptr != NULL)
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
	loc = NULL;
	par = save;
	return -1;
}

/********************************************************************************************
	Insert function for inserting into AVL Tree and performing necessary rotations.
	input: item to be inserted.
********************************************************************************************/
void ATree::t_insert(int k){
	//locating the position to insert
	troot = head->rightc;
	ANode *t = head;       // t always points to the parent of s
	ANode *s = troot;	// s will point to the place where rebalancing may be necessary
	ANode *p = troot;	// p will move down the tree.
	ANode *q = nullptr;	//q will point to the new node which is being inserted.
	
	ANode *newptr = new ANode(k);		// create new node now
	//if the right side of head (root) is null, ie tree is empty, inserting it there
	if(troot == nullptr){
		head->rightc = newptr;
		troot = newptr;
		return;
	}
	
	while(p != nullptr){
 
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
		}
		else if(r->bf == -1*a){		//double rotation
			if(a == 1){ 	// LR Imbalance
				p = Imbalance_LR(s, r, a, 1);
			}
			else if(a == -1){ 	 // RL Imbalance	
				p = Imbalance_RL(s, r, a, 1);
			}
				
		}
	}
/* p pointing to the new subtree root. t pointing to the parent of the old subtree root s.
 	adjusting root after rotation. */
	if(s == t->rightc) t->rightc = p;
	else t->leftc = p;	
	
}

/********************************************************************************************
	Delete function for deleting from AVL Tree and performing necessary rotations.
	input: item to be deleted.
********************************************************************************************/
void ATree::Delete(int k)
{
	troot =  head->rightc;
	// Tree is Empty
	if (troot == nullptr) {
		cout << "\n\t\t => Tree empty" << endl;
		return;
	}
	
	while (!pathToP.empty())		//empty stack, if element already present.
	     	pathToP.pop();

	ANode *t = head;       // t always points to the parent of s
	ANode *s = troot;	// s will point to the place where rebalancing may be necessary
	ANode *p = troot;	// p will move down the tree.
	ANode *r = nullptr;	// r point to the node where firts potential rotation may happen. left/right of s
	bool exist = false;
	int a;
	
	pathToP.push(t);
	
	while(p != nullptr)
		{	
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
	if (exist == false){
		cout << "\n\t\t =>  Item not present in tree" << endl;
		while (!pathToP.empty())
		     pathToP.pop();
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
			} else if(r->bf == 0){ //single rotation
					if(a == -1){	//LL Imbalance
						p = Imbalance_LL(s, r, -2, a);
					}
					else if(a == 1){  // RR Imbalance
						p = Imbalance_RR(s, r, -2, a);
					}
			}
			else if(r->bf == a){		//double rotation
				if(a == -1){ 	// LR Imbalance
					p = Imbalance_LR(s, r, a, 0);
				}
				else if(a == 1){ 	 // RL Imbalance
					p = Imbalance_RL(s, r, a, 0);
				}
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

/********************************************************************************************
	Delete Helper function. For Node having one or no child
	input: par is parent of node to be deleted. loc is location of node to be deleted.
********************************************************************************************/
void ATree::case_a(ANode* par, ANode* loc)
{
    ANode *child;
    
	if (loc->leftc == nullptr && loc->rightc == nullptr) // Node having no left or right child so intialising child = NULL i.e. no child
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
        if (loc == par->leftc)	// if node is in left of parent 
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
void ATree::case_b(ANode* par, ANode* loc) 
{
    ANode *suc, *parsuc;	// suc = inorder successor of loc node . parsuc = loc of the parent of the suc
    // findinf suc and parsuc
    
	pathToP.push(loc);
	
	save = loc;
    ptr = loc->rightc;
    
    while (ptr->leftc != nullptr)
    {
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
	Rotation Function whene there is LL Imbalance (right rotation)
	input: s will point to the place where rebalancing may be necessary
		   r point to the node where firsts potential rotation may happen.
		   s_bf is new bf of s
		   r_bf is new bf of r
********************************************************************************************/
ANode* ATree::Imbalance_LL(ANode* s, ANode *r, int s_bf, int r_bf){
	ANode *p = r;
		s->leftc = r->rightc; //right rotation
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
	Rotation Function whene there is RR Imbalance (left rotation)
	input: s will point to the place where rebalancing may be necessary
		   r point to the node where firsts potential rotation may happen.
		   s_bf is new bf of s
		   r_bf is new bf of r
********************************************************************************************/
ANode* ATree::Imbalance_RR(ANode* s, ANode *r, int s_bf, int r_bf){
	ANode *p = r;
		s->rightc = r->leftc;	//left rotation
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
ANode* ATree::Imbalance_LR(ANode* s, ANode *r, int a, int op){
	ANode *p = r->rightc;
		r->rightc = p->leftc; //left rotation
		p->leftc = r;
		s->leftc = p->rightc;  //right rotation
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
ANode* ATree::Imbalance_RL(ANode* s, ANode *r,  int a, int op){
	ANode *p = p = r->leftc; 
		r->leftc = p->rightc;  //right rotation
		p->rightc = r;
		s->rightc = p->leftc;  //left rotation
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
void ATree::t_print_graphviz(ANode *ptr, string file_name) {
	if (ptr == NULL) {
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

	queue<ANode *> qLoAVL;		// queue level order AVL
	qLoAVL.push(ptr);

	while (qLoAVL.empty() == false)		// Level order Traversal
	{
		ANode *tmp = qLoAVL.front();
//        	cout << tmp->key << " ";
		if (tmp->leftc != nullptr  || tmp->rightc != nullptr)
			fout << tmp->key << "[label = \"<L>|<D> " << tmp->key << "(" <<tmp->bf <<")"<< "|<R> \"]"<< endl;
		else
			fout << tmp->key << "[label = \"<L>|<D> " << tmp->key << "(" <<tmp->bf <<")"<< "|<R> \", fontcolor=red]" << endl;

		if (tmp->leftc != nullptr)
			fout << "\"" << tmp->key << "\":L ->" << tmp->leftc->key << ":D" << endl;

		if (tmp->rightc != nullptr)
			fout << "\"" << tmp->key << "\":R ->" << tmp->rightc->key << ":D" << endl;

		fout << endl;

		qLoAVL.pop();
		
		if (tmp->leftc != nullptr) /* Enqueue left child */
			qLoAVL.push(tmp->leftc);
		
		if (tmp->rightc != nullptr) /*Enqueue right child */
			qLoAVL.push(tmp->rightc);
	}

	fout << "}" << endl;

	cout << "\n\t AVL Tree printed - file name = "<<file_name<<".png";

	fout.close();
	
	string cmd = "dot -Tpng "+file_name+".gv -o "+file_name+".png";
    	system((const char*)cmd.c_str());
}
