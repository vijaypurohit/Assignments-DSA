#ifndef DSA_A03_ANODE_H
#define DSA_A03_ANODE_H

/*********** AVL Node Structure ***********/
class ANode {
    int key; 			//key stored in avl node
    ANode* leftc; 		//pointer to leftc child
    ANode* rightc; 		// pointer to rightc child
    int bf;  			// balance factor bf = height(leftc subtree) – height(rightc subtree)
    int ht;             // height of the node

    ANode(){
        key = 0;
        bf = 0;
        leftc = nullptr;
        rightc = nullptr;
        ht = 0;
    }

    ANode(int x){
        key = x;
        leftc = nullptr;
        rightc = nullptr;
        bf = 0;
        ht = 0;
    }

    ~ANode(){
        if(leftc){
//            cout<<leftc->key<<"L ";
            delete leftc;
        }
        if(rightc){
//            cout<<rightc->key<<"R ";
            delete rightc;
        }
    }
    friend class AVLTree; // Now class  AVLTree can access private members of ANode
};

#endif //DSA_A03_ANODE_H
