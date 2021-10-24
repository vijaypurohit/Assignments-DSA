#ifndef DSA_A03_BNODE_H
#define DSA_A03_BNODE_H

//BST Node
struct BNode {
    int key;
    BNode* leftc;
    BNode* rightc;
    bool isLeftTh, isRightTh;
    int ht;
};

/*********** Threaded BST Structure ***********/
class ThBSTNode {
    int key; 			//key stored in node
    int ht;             // height of the node
    ThBSTNode* leftc; 		//pointer to leftc child
    ThBSTNode* rightc; 		// pointer to rightc child
    bool isLeftTh, isRightTh;

    ThBSTNode(){
        key = 0;
        ht = 0;
        leftc = nullptr;
        rightc = nullptr;
        isLeftTh = true;
        isRightTh = true;
    }

    ThBSTNode(int x){
        key = x;
        ht = 0;
        leftc = nullptr;
        rightc = nullptr;
        isLeftTh = true; // TRUE means Left or Right pointer are Threads
        isRightTh = true;
    }

    ~ThBSTNode(){
        if(!isLeftTh){
//            cout<<leftc->key<<"L ";
            delete leftc;
        }
        if(!isRightTh){
//            cout<<rightc->key<<"R ";
            delete rightc;
        }
    }
    friend class ThBST; // Now class ThBST can access private members of ThBSTNode
};
#endif //DSA_A03_BNODE_H
