/*********************************************************************************
	DS LAB CS 513: Assignment 03 (Performance Evaluation of AVL, BST, TREAP)
	Roll No - 214101058 | Vijay Purohit
***********************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <ctime>
#include <random>       //to generate random number
#include <chrono>       //to seed random number
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

/*********** Treap Node ***********/
#include "TNode.h"
#include "TreapClass.h"

/*********** AVL Node ***********/
#include "ANode.h"
#include "AVLClass.h"

/*********** BST Node ***********/
#include "BNode.h"
#include "BSTClass.h"

/*********** Variable Declarations ***********/
//files
const string TestCaseDirectory = "input_test_files/";
const string AnalysisDirectory = "output_analysis_files/";
const string ReadTestCasesFileName = "input_test_files/_read_user_test_files.txt";
ifstream readFileTest;
//operations
int nop_ins=0, nop_del=0, ele, delete_index;
float ins_ratio[]={0.5,0.6,0.7,0.8};
const int max_op = 15000;				// maximum number of operations to consider

unsigned seed = chrono::system_clock::now().time_since_epoch().count(); // (present time) and clock's epoch
mt19937 r_generator(seed); // mt19937 is a standard mersenne_twister_engine
uniform_int_distribution<int> my_random_op(0, 1);       // for selecting operation insert/delete
uniform_int_distribution<int> my_random_ratio(0, 3);       // for selecting insert ratio

/********************************************************************************************
    Function to Generate Default Test Case Files.
********************************************************************************************/
void GenerateDefaultTestCases(){
    string file_name, fileopen;

    ofstream fout;      //file stream object to write

    cout<<"\n Generating Default Test Case Files: \n ";
    for(int nop = 100; nop<= max_op; nop+=500) {  //num of operations
        if(nop==600)nop=500;
        //opening respective file
        file_name="nop_"+to_string(nop);
        fileopen = TestCaseDirectory + file_name + ".txt";
        fout.open(fileopen.c_str(), ios::out);
            if (!fout) {
                cout << "\n**File failed to open**: "<<fileopen<<"\n\n";
                fout.clear();
                return;
            }

        //reinitialzing for each loop
        nop_ins=0; nop_del=0;
        int i=1;
        float ins_ratio_value = nop*ins_ratio[my_random_ratio(r_generator)];
        float del_ratio_value = nop-ins_ratio_value;
        vector<int> inserted_elements;
        uniform_int_distribution<int> my_random_ins(1, nop); // create a distribution for value, define the range
        bool insert_op = true;

        //wrting in file
        fout<<nop<<"  "<<ins_ratio_value<<" : "<<del_ratio_value<<"\n"; //first line as number of operations.

        //first insert
            ele = my_random_ins(r_generator);
        fout << "Insert "<< ele<<"\n";
            inserted_elements.push_back(ele);
            nop_ins++;

        for(i=2; i<=nop; i++){
            if(nop_ins > ins_ratio_value) insert_op = false;
            else if(nop_del > del_ratio_value ) insert_op = true;
            else if(my_random_op(r_generator)) insert_op = true;
            else insert_op = false;

            if(insert_op){      //insert operation
                    ele = my_random_ins(r_generator);
                fout << "Insert "<< ele<<"\n";
                    inserted_elements.push_back(ele);
                    nop_ins++;
            }
            else{               //delete operation
                    uniform_int_distribution<int> my_random_del(0, nop_ins-1);
                    delete_index = my_random_del(r_generator);
                fout << "Delete "<< inserted_elements[delete_index]<<"\n";
                    nop_del++;
            }
        }//end of for loop i
        fout.close();
        cout<<"\n => Test File Generated: "<<fileopen<<"  "<<ins_ratio_value<<":"<<del_ratio_value<<"\n";
    }//end of for loop operations
    cout<<"\n\n =>  Use Second Option to Evaluate Parameters of these files.\n";
}//end of GenerateDefaultTestCases()

/********************************************************************************************
	Function to Generate User Input Test Case File.
********************************************************************************************/
void GenerateTestCases(int nop, float insert_ratio_value ){

    string file_name, fileopen;
    ofstream fout;      //file stream object to write
    cout<<"\n Generating Test Case File: \n ";
        //opening respective file
        file_name="top_"+to_string(nop);
        fileopen = TestCaseDirectory + file_name + ".txt";
        fout.open(fileopen.c_str(), ios::out);
        if (!fout) {
            cout << "\n**File failed to open**: "<<fileopen<<"\n\n";
            fout.clear();
            return;
        }
        //reinitialzing for each loop
        nop_ins=0; nop_del=0;
        int i=1;
        float ins_ratio_value = nop*insert_ratio_value;
        float del_ratio_value = nop-ins_ratio_value;
        vector<int> inserted_elements;
        uniform_int_distribution<int> my_random_ins(1, nop); // create a distribution, define the range
        bool insert_op = true;

        //wrting in file
        fout<<nop<<"  "<<ins_ratio_value<<" : "<<del_ratio_value<<"\n"; //first line as number of operations.

        //first insert
        ele = my_random_ins(r_generator);
        fout << "Insert "<< ele<<"\n";
        inserted_elements.push_back(ele);
        nop_ins++;

        for(i=2; i<=nop; i++){
            if(nop_ins > ins_ratio_value) insert_op = false;
            else if(nop_del > del_ratio_value ) insert_op = true;
            else if(my_random_op(r_generator)) insert_op = true;
            else insert_op = false;

            if(insert_op){      //insert operation
                ele = my_random_ins(r_generator);
                fout << "Insert "<< ele<<"\n";
                inserted_elements.push_back(ele);
                nop_ins++;
            }
            else{               //delete operation
                uniform_int_distribution<int> my_random_del(0, nop_ins-1);
                delete_index = my_random_del(r_generator);
                fout << "Delete "<< inserted_elements[delete_index]<<"\n";
                nop_del++;
            }
        }//end of for loop i
        fout.close();
        cout<<"\n=> Test File Generated: "<<fileopen<<"  "<<ins_ratio_value<<":"<<del_ratio_value<<"\n";

}//end of GenerateTestCases()

/********************************************************************************************
    Function to Read Default Test Case Files.
********************************************************************************************/
void ReadDefaultTestCases(){
    string file_name, fileopen;
    string output_fileopen;

    ifstream fin;      //file stream object to read
    ofstream fout;      //file stream object to write

    output_fileopen = AnalysisDirectory + "analysis_default_" + ".csv";
    fout.open(output_fileopen.c_str(), ios::out);
    if (!fout) {
        cout << "\n**File failed to open: "<<output_fileopen<<"\n\n";
        fout.clear();
        return;
    }

    fout<<"#Operations,"
          "#Nodes,"
          "Height (AVL),Height (BST),Height (TREAP),"
          "Avg Height Node (AVL),Avg Height Node (BST),Avg Height Node (TREAP),"
          "Node Comparisons (AVL),Node Comparisons (BST),Node Comparisons (TREAP),"
          "Rotations (AVL),Rotations (BST),Rotations (TREAP),"
          "#Insert, #Delete\n";

    cout<<"\n Reading Default Test Case Files: \n ";
    for(int nop = 100; nop<= max_op; nop+=500) {  //num of operations
        if(nop==600)nop=500;
        //opening respective file
        file_name="nop_"+to_string(nop);
        fileopen = TestCaseDirectory + file_name + ".txt";
        fin.open(fileopen.c_str(), ios::in);
        if (!fin) {
            cout << "\n**File failed to open: "<<fileopen<<"\n\n";
            fin.clear();
            continue;
        }

        cout<<"\n\n\t=> Reading File: "<<fileopen<<"\n";

        unsigned int read_nop, priority;
        float ins_ratio_value ;
        float del_ratio_value ;
        string op_read;

        fin>>read_nop>>ins_ratio_value>>op_read>>del_ratio_value;

        uniform_int_distribution<int> my_random_priority(1, read_nop);


        AVLTree *avl_r = new AVLTree();
        ThBST *bst_r = new ThBST();
        Treap *treap_r = new Treap();

        while(fin>>op_read>>ele){
            if(op_read == "Insert")
            {
                try{ avl_r->Insert(ele); }catch (const char *msg) {//                    cout << "\t\t =>" << msg << endl;
                }
                try{bst_r->Insert(ele); }catch (const char *msg) {//                    cout << "\t\t =>" << msg << endl;
                }
                priority = my_random_priority(r_generator);
                try{  treap_r->Insert(ele,priority);}catch (const char *msg) {//                    cout << "\t\t =>" << msg << endl;
                }
            }
            else if(op_read == "Delete")
            {
                try{ avl_r->Delete(ele); }catch (const char *msg) {//                    cout << "\t\t =>" << msg << endl;
                }
                try{ bst_r->Delete(ele); }catch (const char *msg) {//                    cout << "\t\t =>" << msg << endl;
                }
                try{  treap_r->Delete(ele); }catch (const char *msg) {//                    cout << "\t\t =>" << msg << endl;
                }
            }
        }
        avl_r->write_parameters(fout, read_nop);
        bst_r->write_parameters(fout, read_nop);
        treap_r->write_parameters(fout, read_nop);


        fout<<nop<<","\
            <<avl_r->total_nodes<<","\
            //<<bst_r->total_nodes<<","<<treap_r->total_nodes<<","
                    <<avl_r->height_final_avl<<","<<bst_r->height_final_bst<<","<<treap_r->height_final_treap<<","\
                    <<avl_r->height_avg_node_avl<<","<<bst_r->height_avg_node_bst<<","<<treap_r->height_avg_node_treap<<","\
                    <<avl_r->comp_final_avl<<","<<bst_r->comp_final_bst<<","<<treap_r->comp_final_treap<<","\
                    <<avl_r->rot_final_avl<<","<<0<<","<<treap_r->rot_final_treap<<","\
                    <<ins_ratio_value<<","<<del_ratio_value<<"\n";



        fin.close();
        delete avl_r;
        delete bst_r;
        delete treap_r;
    }//end of for loop operations
    fout.close();
    cout<<"\n\n=> Analysis File Generated: "<<output_fileopen<<"\n";

}//end of GenerateDefaultTestCases()

/********************************************************************************************
	Function to Read User Input Test Case File.
********************************************************************************************/
void ReadTestCases(){
    string fileopen, file_name;
    string output_fileopen;

    ifstream fin;      //file stream object to read
    ofstream fout;      //file stream object to write

    int nofiles=0;
    readFileTest>>nofiles;

    output_fileopen = AnalysisDirectory + "t_"+to_string(nofiles)+ "_user_test_file_analysis.csv";
    fout.open(output_fileopen.c_str(), ios::out);
    if (!fout) {
        cout << "\n**File failed to open: "<<output_fileopen<<"\n\n";
        fout.clear();
        return;
    }

    fout<<"#Operations,"
          "#Nodes,"
          "Height (AVL),Height (BST),Height (TREAP),"
          "Avg Height Node (AVL),Avg Height Node (BST),Avg Height Node (TREAP),"
          "Node Comparisons (AVL),Node Comparisons (BST),Node Comparisons (TREAP),"
          "Rotations (AVL),Rotations (BST),Rotations (TREAP),"
          "#Insert, #Delete\n";

    cout<<"\n Reading "<<nofiles<<" User Test Case Files: \n ";

    for(int nop = 1; nop<= nofiles; nop++) {  //num of operations

        //opening respective file
        readFileTest>>file_name;
        fileopen = TestCaseDirectory + file_name;
        fin.open(fileopen.c_str(), ios::in);
        if (!fin) {
            cout << "\n**File failed to open: "<<fileopen<<"\n\n";
            fin.clear();
            continue;
        }

        cout<<"\n\n\t=> Reading File: "<<fileopen<<"\n";

        nop_ins=0; nop_del=0;
        unsigned int read_nop=0, priority=0;
        float ins_ratio_value=0 ;
        float del_ratio_value=0 ;
        string op_read;

        fin>>read_nop>>ins_ratio_value>>op_read>>del_ratio_value;
//        cout<<" "<<read_nop<<" "<<ins_ratio_value<<" "<<op_read<<" "<<del_ratio_value<<endl;
        if(ins_ratio_value==0 || del_ratio_value==0){
            fin.clear();
            fin.seekg(0, std::ios_base::beg);
            fin>>read_nop;
        }

        uniform_int_distribution<int> my_random_priority(1, read_nop);

        AVLTree *avl_r = new AVLTree();
        ThBST *bst_r = new ThBST();
        Treap *treap_r = new Treap();

        while(fin>>op_read>>ele){

            if(op_read == "Insert")
            {
                nop_ins++;
                try{ avl_r->Insert(ele); }catch (const char *msg) {//                    cout << "\t\t =>" << msg << endl;
                }
                try{bst_r->Insert(ele); }catch (const char *msg) {//                    cout << "\t\t =>" << msg << endl;
                }
                priority = my_random_priority(r_generator);
                try{  treap_r->Insert(ele,priority);}catch (const char *msg) {//                    cout << "\t\t =>" << msg << endl;
                }
            }
            else if(op_read == "Delete")
            {
                nop_del++;
                try{ avl_r->Delete(ele); }catch (const char *msg) {//                    cout << "\t\t =>" << msg << endl;
                }
                try{ bst_r->Delete(ele); }catch (const char *msg) {//                    cout << "\t\t =>" << msg << endl;
                }
                try{  treap_r->Delete(ele); }catch (const char *msg) {//                    cout << "\t\t =>" << msg << endl;
                }
            }
        }
        avl_r->write_parameters(fout, read_nop);
        bst_r->write_parameters(fout, read_nop);
        treap_r->write_parameters(fout, read_nop);

        ins_ratio_value=nop_ins;
        del_ratio_value=nop_del;

        fout<<read_nop<<","\
            <<avl_r->total_nodes<<","\
            //<<bst_r->total_nodes<<","<<treap_r->total_nodes<<","
            <<avl_r->height_final_avl<<","<<bst_r->height_final_bst<<","<<treap_r->height_final_treap<<","\
                    <<avl_r->height_avg_node_avl<<","<<bst_r->height_avg_node_bst<<","<<treap_r->height_avg_node_treap<<","\
                    <<avl_r->comp_final_avl<<","<<bst_r->comp_final_bst<<","<<treap_r->comp_final_treap<<","\
                    <<avl_r->rot_final_avl<<","<<0<<","<<treap_r->rot_final_treap<<","\
                    <<ins_ratio_value<<","<<del_ratio_value<<"\n";



        fin.close();
        delete avl_r;
        delete bst_r;
        delete treap_r;
    }//end of for loop operations
    fout.close();
    cout<<"\n\n=> Analysis File Generated: "<<output_fileopen<<"\n";
}//end of GenerateTestCases()

/********************************************************************************************
	Main Function
********************************************************************************************/
int main() {
    srand((unsigned)time(0));


    string fn ; 		// filename for png file to be saved.
    string suffix ;    	// filename suffix to distinguish different files.

    char choice;			// choice exercised.

   

    do {
        int num_op;
        float insert_ratio_value;
         stringstream input_stream_op;
         string temp_string_op;
        string multi_input_op ; //= "a b c 4 e6 8";


        char ch;
        cout << endl << endl << endl;
        system(PAUSE);
        system(CLR);

        cout << "\n\n ---- TREE PERFORMANCE EVALUATION MENU  ----";

        cout << "\n 1. Default Test Files Generation. 			";
        cout << "\n 2. Read Default Test Files & Evaluate Parameters.		";
        cout << "\n\n 3. User Input Test Files Generation. 			";
        cout << "\n 4. Read User Input Test Files & Evaluate Parameters.		";

        cout << "\n\n n. Exit - Bye	\n\n  --Choice : ";

        cin >> ch;
        cout << "\n --------";
        switch (ch)
        {

            case '1' : GenerateDefaultTestCases();
                break;
            case '2' : ReadDefaultTestCases();
                break;
            case '3' :
// 100 0.6, 500 0.7, 1000 0.5, 1500 0.7, 2000 0.6, 2500 0.6, 3000 0.7, 3500 0.8, 4000 0.9, 4500 0.8, 5000 0.7, 5500 0.6, 6000 0.7, 6500 0.8, 7000 0.7, 7500 0.8, 8000 0.7, 8500 0.6, 9000 0.7, 9500 0.8, 10000 0.8
                cout << "\n Enter #of Operations & Insert Frequency \n (eg 2000 0.6, 2500 0.7, 3000 0.8) (each (Op freq) pair separated with comma) (in the end press Enter) \n\t :: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, multi_input_op);

                input_stream_op << multi_input_op;
                
                while(getline(input_stream_op, temp_string_op, ',') ) {
                    if(stringstream(temp_string_op)>>num_op>>insert_ratio_value )
                    {
//                    	cout<<num_op<<" "<<insert_ratio_value<<endl;
                        GenerateTestCases(num_op, insert_ratio_value);
                    }
                }
                break;

            case '4' : cout << "\n To Read: Enter File Names in the text file present in ("<<ReadTestCasesFileName<<" ) "
                                "\n First Line is the Number of Files "
                                "\n Each Next line is the Full Name of the file. \n";
                        cout<<"\n Files should be present in that same directory. \n Then Press Enter to Start Evaluation\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cin.get();

                        readFileTest.open(ReadTestCasesFileName.c_str(), ios::in);
                        if (!readFileTest) {
                            cout << "\n**File failed to open: "<<ReadTestCasesFileName<<"\n\n";
                            readFileTest.clear();
                            continue;
                        }
                        ReadTestCases();
                readFileTest.close();
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

//random number	
//random_device rd;             // obtain a random number from hardware. Not working non-deterministically on Dev CPP
//mt19937 r_generator(rd());    // seed the generator
//objects
//ThBST bst_obj_d;		//  Threaded BSTree object
//AVLTree avl_obj_d ;		// AVL Tree object
//Treap treap_d;        // Treap object
//                    cout << "\n Enter Number of Operations: ";
//                        cin>>num_op;
//                        cout << "\n Enter Ratio of Insert Frequency (eg 0.6 of 0.6:0.4): ";
//                        cin>>insert_ratio_value;
//                GenerateTestCases(num_op, insert_ratio_value); 
//        avl_r->show_parameters();
//        bst_r->show_parameters();
//        treap_r->show_parameters();
//        avl_r->print_graphviz(AnalysisDirectory+file_name+"_AVL");
//        bst_r->print_graphviz(AnalysisDirectory+file_name+"_ThBST");
//        treap_r->print_graphviz(AnalysisDirectory+file_name+"_TREAP");

