/*********************************************************************************
	DS LAB CS 513: Assignment 03 (TREAPS)
	Roll No - 214101058 | Vijay Purohit
    TREAP IMPLEMENTATION
***********************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <limits>
#include <random>       //to generate random number
#include <chrono>       //to seed random number
using namespace std;

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

/********************************************************************************************
	Main Function
********************************************************************************************/
int main(){
    srand((unsigned)time(0));
    
    string fn ; 		// filename for png file to be saved.
    string suffix ;    	// filename suffix to distinguish different files.

    char choice;		// choice exercised.
    int flag;			// flag for element search result.

    Treap treap;        // Treap object

    do {
        int inf;
        char ch;
        // for multiple inputs
        stringstream input_stream;
        string temp_string;
        string multi_input ; //= "a b c 4 e6 8";

        cout << endl << endl << endl;
        system(PAUSE);
        system(CLR);

        //treap.display();

        cout << "\n\n ------- TREAP MENU  -------";

        cout << "\n 1. Display - Console 			";
        cout << "\n 2. Default Treap - Insertion (sir pdf)			";

        cout << "\n\n 3. INSERT an element k ";
        cout << "\n 4. SEARCH an element k";
        cout << "\n 5. DELETE an element k";
        cout << "\n 6. INSERT Multiple elements in a Line";

        cout << "\n\n p. Print Tree - GraphViz";
        cout << "\n s. Show Parameters";
        cout << "\n n. Exit - Bye		\n\n  --Choice : ";
        cin >> ch;
        cout << "\n <-------->";

        switch (ch) {
            case '1' :
                treap.display();
                break;
                //Default Insert.
            case '2' :

                try {
                    // 107, 101, 98, 111, 102, 104, 119, 109, 99, 97, 116
                    // 1 : k, e, b, o, f, h, w, m, c, a, s , , t
                    // Insert(item, Priority);
                    treap.Insert(107, 3);  //k
                    treap.Insert(101, 13); //e
                    treap.Insert(98, 37); //b
                    treap.Insert(111, 45); //o
                    treap.Insert(102, 51); //f
                    treap.Insert(104, 57); //h
                    treap.Insert(119, 67); //w
                    treap.Insert(109, 78); //m
                    treap.Insert(99, 84); //c
                    treap.Insert(97, 89); //a
                  //  treap.Insert(115, 90); //s
//                    treap.Insert(116, 14); //t

                    cout << "\n Default Treap is inserted. ";
                }
                catch (const char *msg) {
                    cout << "\t" << msg << endl;
                }
                break;

                //Insert
            case '3' :
                cout << "\n Enter Information (int) for new Treap Node .. : ";
                cin >> inf;        // Saving Info for the new node
                if (cin.fail()) { //Not an int.
                    cout << "\t" << "Not an Integer: "<< inf << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                try {
                    treap.Insert(inf);
                    cout << "\tNODE  INSERTED : "<<inf;
                }
                catch (const char *msg) {
                    cout << "\t\t => " << msg << endl;
                }
                break;

                //Search
            case '4' :
                cout << "\n Enter a Node value to SEARCH : ";
                cin >> inf;
                if (cin.fail()) {//Not an int.
                    cout << "\t" << "Not an Integer: "<< inf << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                flag = treap.Search(inf);

                if (flag == 1) {
                    cout << "\n Search Successful for Element (" << inf << ").";
                    if (treap.returnPar() != nullptr)
                        cout << "\n 	Its parent key and address: " << treap.returnParKey() << " , "
                             << treap.returnPar();
                    cout << "\n	Its location: " << treap.returnLocKey() << " , " << treap.returnLoc();
                } else if (flag == -1) {
                    cout << "\n Element (" << inf << ") Not Found.";
                } else
                    cout << "\n Search Operation Unsuccessful.";
                break;

                //Delete
            case '5' :
                cout << "\n Enter a Node value to DELETE : ";
                cin >> inf;
                if (cin.fail()) {//Not an int.
                    cout << "\t" << "Not an Integer: " << inf<<  endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                try {
                    treap.Delete(inf);
                    cout << "\tNODE  DELETED : "<<inf;
                }
                catch (const char *msg) {
                  cout << "\t\t => " << msg << endl;
                }


                break;

                //multiple Insert
            case '6' :
                    cout << "\n Enter Keys (int) for inserting in Treap Node (separated with space) (in the end press Enter) \n\t :: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, multi_input);
                    input_stream << multi_input;

                    while(getline(input_stream, temp_string, ' ')) {
                        if(stringstream(temp_string)>>inf)
                        {
//                            cout<<inf<<endl;
                            try {
                                treap.Insert(inf);
                            }
                            catch (const char *msg) {
                                cout << "\t\t =>" << msg << endl;
                            }
                        }
                    }
                break;
                //Print
            case 'p':
                cout << "\n Enter File Name suffix to distinguish your operation (w/o space): ";
                    cin>>suffix;

                fn = "TREAP_"+suffix;
                cout << "\n Printing Tree: ";

                treap.print_graphviz(fn);
                break;
            //show parameters
            case 's':
               treap.show_parameters();
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
} //end of main()
