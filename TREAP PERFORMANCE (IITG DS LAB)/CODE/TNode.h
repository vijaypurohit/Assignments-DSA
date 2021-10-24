#ifndef DSA_A03_TNODE_H
#define DSA_A03_TNODE_H

/*********** Treap Node Structure ***********/
class TNode {
    int key; 			// key stored in treap node
    int pr;  			// priority of the treap node
    int ht;  			// height of the treap node
    TNode* leftc; 		// pointer to leftc child
    TNode* rightc; 		// pointer to rightc child


    TNode(){
        key = 0;
        pr = 0;
        ht = 0;
        leftc = nullptr;
        rightc = nullptr;

    }

    TNode(int x, int p){
        key = x;
        pr = p;
        ht = 0;
        leftc = nullptr;
        rightc = nullptr;

    }

    // To Adjust Height of Node, during insertion and deletion.
    void adjust_height(){
        int left_ht = 0, right_ht = 0;
        if(leftc == nullptr && rightc == nullptr)
        {
            ht = 0;
        }
        else {

            if (leftc != nullptr) left_ht = leftc->ht;
            if (rightc != nullptr) right_ht = rightc->ht;
            ht = 1 + ((left_ht > right_ht) ? left_ht : right_ht);
        }
    }

    ~TNode(){
        if(leftc){
           // cout<<leftc->key<<"L ";
            delete leftc;
        }
        if(rightc){
           // cout<<rightc->key<<"R ";
            delete rightc;
        }
    }
    friend class Treap;     // Now class  TTree can access private members of TNode
}; //end of Class TNode


#endif //DSA_A03_TNODE_H
