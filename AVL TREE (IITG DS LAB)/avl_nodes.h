//AVL Tree Node Structure 
class ANode {
	int key; 			//key stored in avl node
	ANode* leftc; 		//pointer to left child
	ANode* rightc; 		// pointer to right child
	int bf;  			// balance factor bf = height(left subtree) – height(right subtree) 
	
	ANode(){
		
	}
	
	ANode(int x){
	 	key = x;
	 	leftc = nullptr;
	 	rightc = nullptr;
	 	bf = 0;
	 }
	 
	~ANode(){
		if(leftc){
			cout<<leftc->key<<"L ";
			delete leftc;
		}
		if(rightc){
			cout<<rightc->key<<"R ";
			delete rightc;
		}
	}
	friend class ATree; // Now class  ATree can access private members of ANode
};
