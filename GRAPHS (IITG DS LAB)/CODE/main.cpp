/*********************************************************************************
	DS LAB CS 513: Assignment 04 (GRAPHS)
	Roll No - 214101058 | Vijay Purohit
***********************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <limits>
using namespace std;

#include <queue>
#include <stack>
#include <unordered_map>
#include <set>

// Define Infinity
# define INF 0x3f3f3f3f

// for Linux and Windows Pause and Clear Command
#if defined(_WIN32)
    #define PAUSE "pause"
    #define CLR "cls"
#elif defined(unix) || defined(__unix__) || defined(__unix)
    #define PAUSE "read -p 'Press Enter to continue...' var"
    #define CLR "clear"
#endif

/*********** Graph Structure ***********/

class DiGraph{
    int V; // Number of Vertices
    int E; // Number of Edges
    int S; // Source Vertex

    // Adjacency List
    vector< pair<int,int> > *adj; // adjacency list[u] = {v1,wt1}->{v2,wt2}
    vector< pair<int,int> > *clone_adj; // Minimal SCC Graph Adjacency List

    vector<bool> bfs_vis;  // book keeping variable to check visited node in BFS Traversal

    //for DFS
    int dfs_number;
    vector<int> dfs_st; // DFS node start time
    vector<int> dfs_et; // DFS node end time
    vector<int> lowlink; // DFS node low(v) number
    unordered_map<int, unordered_map<int,pair<char,int>>> dfs_edge;  //DFS Edge Classification [u][v]={'T', uv_wt}

    // for SCC
    int max_scc;            // max num of scc present in the graph.
    vector<int> vertex_component;  // variable to store vertex belong to which component
    vector<int> *scc; // strongly connected components

    // for Dijkstra
    vector<int> dij_distance;   // book keeping variable to store shortest distance of node in Dijkstra
    vector<int> dij_parents;    // book keeping variable to store parent of node in Dijkstra

public:
    DiGraph(int num_v, int num_e){
        this->V=num_v;
        this->E=num_e;
        this->S=0;
        adj = new vector<pair<int,int>>[this->V];
        clone_adj = new  vector<pair<int,int>>[this->V];
        scc = new vector<int>[this->V];
        max_scc=0;
        dfs_number=0;
    }

    void addDirectedEdge(int, int, int); // Add Directed Edge in Graph(u,v,wt)
    void showDiGraphAdjList();  // Showing the Adj List Of Graph
    void printDiGraph(const string &); // Print the Original Graph using Graphviz

    // for Dijkstra
    void DijkstraSSSPAlgo(int);      // Dijkstra SSSP Algorithm
    void showDiGraphDijkstraValues(int); // Show Shortest Distance of Each Node
    void printGraphVizDijkstra(const string &); // Print the Dijkstra Graph using Graphviz

    // for DFS
    void DFSDiGraph(int );          // DFS Function
    void DFSRec(int s, vector<bool> &, stack<int> &, vector<bool> &);  // DFS Helper Recursive Function
    void printGraphDFSTraversal(const string &); // Print the DFS Graph using Graphviz

    // For SCC
    void printGraphSCCTarjan(const string &); // Print the SCC Graph using Graphviz
    bool CheckSemiConnectedDiGraph(); // Check Semi-Connectedness of Graph
    void printCondensedDiGraph(int cgV, int cgE, vector<pair<int, int>> *cgadj, const vector<int> &top, bool ); // Print the Condensed Graph using Graphviz

    // for Minimal SCC
    void ReduceMainSCCEdges(); // Reduce the SCC Graph
    void printGraphMinimalSCC(const string &);  // Print the Minimal Graph using Graphviz
    void SCCMinimalDFSHelper(int, unordered_map<int, bool> &, unordered_map<int, unordered_map<int, int>> &); // DFS helper function to check reachability

    ~DiGraph()= default;


};//Class DiGraph

/**************** Variable Declaration ****************/
DiGraph *dg;
const string input_directory = "input_files/";          // input directory name
const string output_directory = "output_files/";        // output directory name
const string read_filename   = "_read_input.txt";       // default read file name
// graphviz colors list
const string graphviz_colors[12]={"brown", "indigo", "darkorange", "gold2", "forestgreen", "darkturquoise", "crimson",  "maroon", "plum", "hotpink", "tomato", "thistle"};

/********************************************************************************************
 Function to add Directed Edge in Graph in its adjacency list.
********************************************************************************************/
void DiGraph::addDirectedEdge(int u, int v, int wt){
    adj[u].emplace_back(make_pair(v,wt));
}

/********************************************************************************************
	Function to Show Adjacency List of the Graph
********************************************************************************************/
void DiGraph::showDiGraphAdjList() {
    cout << "\n\t Adjacency List of G(" << V<<", "<<E<<") :: \n";
    for (int v = 0; v < V; ++v) {
        cout << "\n V["<< v << "]:";
        for (auto w : adj[v])
            cout << " -> (" << w.first<<", "<<w.second<<")";
        printf("\n");
    }
}

/********************************************************************************************
    Graphviz function to print original DiGraph
********************************************************************************************/
void DiGraph::printDiGraph(const string& file_name){
    if (V == 0) {
        cout << "Graph is Empty."<<endl;
        return;
    }

    ofstream fout;
    string fileopen = file_name+".gv";
    string filepath = output_directory+fileopen;
    fout.open(filepath.c_str(), ios::out);
    if (!fout) {
        cout << "\n**File failed to open**\n\n";
        fout.clear();
    }
/**********************/
    queue<int> qLoGraph;		// queue level order Graph
    bfs_vis.assign(V,false);

    vector<int> level(V,0);
    string rank_same[V];
    int max_level=1;

    int u_val;
    int v_val, uv_wt;
    int c=0;
/**********************/
    fout << "digraph G_"<<V<<"_"<<E<<" {" << endl;
    fout << "  ranksep=\"equally\"; \n"
            "  rankdir=LR; " << endl << endl;

    fout << "X[label = \"VAL\"];"<<endl<<endl;

    for(int i=0; i<V; i++)
    {
        if(!bfs_vis[i]){
            qLoGraph.push(i);
            bfs_vis[i]=true;

            string curr_color = graphviz_colors[c]; c=(c+1)%12;

            level[i]=0;
            rank_same[level[i]].append(to_string(i)).append("; ");

            while (!qLoGraph.empty())		// Level order Traversal
            {
                u_val = qLoGraph.front();  qLoGraph.pop();

                fout << u_val << " [label = \""<<u_val<<"\", color="<<curr_color<<", shape=circle];"<< endl;

                for(auto w: adj[u_val]){
                    v_val = w.first;        //adj node value
                    uv_wt = w.second;  //edge u-v weight

                    if(!bfs_vis[v_val]){
                        bfs_vis[v_val]=true;
                        qLoGraph.push(v_val);

                        level[v_val]=level[u_val]+1;
                        if(level[v_val]+1>max_level)max_level++;
                        rank_same[level[v_val]].append(to_string(v_val)).append("; ");
                    }

                    fout <<"\t"<< u_val << " -> " << v_val << "[label=\""<<uv_wt<<"\"];" << endl;
                }//for adj
                fout << endl ;
            }//while qLoGraph
        }// bfs_vis
    }// for each vertex i

    //printing node with same level together
    for(int i=0; i<max_level; i++){
        if(!rank_same[i].empty())
            fout<<"{rank = same; "<<rank_same[i]<<"}; "<<endl;
    }//for i

    fout << "}" << endl;
/**********************/
    fout.close();

    string cmd = "dot -Tpng "+filepath+" -o "+output_directory+file_name+".png";
    system((const char*)cmd.c_str());
    cout << "\n\t 1. Original DiGraph:: Printed, FileName = "<<output_directory<<file_name<<".png \n";
    remove(filepath.c_str());
}//printDiGraph

/********************************************** --- Dijkstra Functions --- ****************************************************/
/********************************************************************************************
	Single Source Shortest Path, Dijkstra Algorithm
    Input: Source Vertex To Find SSSP to any other vertex.
********************************************************************************************/
void DiGraph::DijkstraSSSPAlgo(int src)
{
    int u_dist, u_val, v, uv_wt;
    priority_queue < pair<int,int>, vector<pair<int,int>>, greater<pair<int, int>> > pq;  // MinHeap pairs {distance, vertex}

    vector<bool> dij_relaxed(V, false);    // book keeping variable to check relaxed vertex in Dijkstra

    dij_distance.assign(V,INF);
    dij_parents.assign(V,-1);

    pq.push({0, src});

    while(!pq.empty()){
        auto u = pq.top(); pq.pop();    // node pair e{distance, vertex}
             u_dist = u.first; //node distance
             u_val = u.second;  //node value

        if(dij_relaxed[u_val]) continue;

        dij_relaxed[u_val] = true;
        dij_distance[u_val] = u_dist;

        for(auto w: adj[u_val]){
            v = w.first;        //adj node value
            uv_wt = w.second;  //edge u-v weight
            if((!dij_relaxed[v]) && ((u_dist + uv_wt) < dij_distance[v]) ){
                dij_parents[v] = u_val;
                dij_distance[v] = u_dist + uv_wt;
                pq.push({dij_distance[v],v});        // relaxing edge
            }//if relax
        }// for w
    }//while pq


} //dijkstra

/********************************************************************************************
	Function to Show Dijkstra Distance and Parent of Node in the Console.
********************************************************************************************/
void DiGraph::showDiGraphDijkstraValues(int src) {
    cout << "\n\t Dijkstra Distance For G(" << V<<", "<<E<<") ::\n\t Source :: "<<src<<"\t V[i]-->(DIS, PAR)\n";
    for (int v = 0; v < V; ++v) {
        cout << "\n V["<< v << "]:";
        if(dij_distance[v]!=INF)
            cout << " -> (" << dij_distance[v]<<", v["<<dij_parents[v]<<"])";
        else
            cout << " -> (INF, v["<<dij_parents[v]<<"])";
        printf("\n");
    }
}

/********************************************************************************************
    Graphviz Function to print Graph with Shortest Distance of Each node and
    the path
********************************************************************************************/
void DiGraph::printGraphVizDijkstra(const string& file_name){
    if (V == 0) {
        cout << "Dijkstra:: Graph is Empty."<<endl;
        return;
    }

    ofstream fout;
    string fileopen = file_name+".gv";
    string filepath = output_directory+fileopen;
    fout.open(filepath.c_str(), ios::out);
    if (!fout) {
        cout << "\nDijkstra:: **File failed to open**\n\n";
        fout.clear();
    }
 /**********************/
    queue<int> qLoGraph;		// queue level order Graph
    bfs_vis.assign(V,false);

    vector<int> level(V,0);
    string rank_same[V];
    int max_level=1;

    int u_val, u_dist, u_par;
    int v_val, v_par, uv_wt;

    fout << "digraph DijkstraSSSP {" << endl;
    fout << "  ranksep=\"equally\"; \n"
            "  rankdir=LR; " << endl << endl;

    fout << "X[label = \"DIS\\n(VAL, PAR)\"];"<<endl<<endl;

    for(int i=0; i<V; i++)
    {

        if(!bfs_vis[i]){
            qLoGraph.push(i);
            bfs_vis[i]=true;

            level[i]=0;
            rank_same[level[i]].append(to_string(i)).append("; ");

            while (!qLoGraph.empty())		// Level order Traversal
            {
                u_val = qLoGraph.front();  qLoGraph.pop();
                u_dist = dij_distance[u_val];
                u_par = dij_parents[u_val];

                //printing node
                if(u_dist!=INF)
                    fout << u_val << " [label = \"" <<u_dist<< " \\n(" <<u_val<< ","<<u_par<<")\", style=bold, color=mediumseagreen];"<< endl;
                else
                    fout << u_val << " [label = \" \u221E \\n(" <<u_val<< ","<<u_par<<")\", color=brown];"<< endl;

//                fout << u_val << " [label = \"" <<u_val<< " \\n(\u221E," <<u_par<< ")\", color=brown];"<< endl;

                for(auto w: adj[u_val]){
                    v_val = w.first;        //adj node value
                    uv_wt = w.second;  //edge u-v weight
                    v_par = dij_parents[v_val];

                    if(!bfs_vis[v_val]){
                        bfs_vis[v_val]=true;
                        qLoGraph.push(v_val);
                        level[v_val]=level[u_val]+1;
                        if(level[v_val]+1>max_level)max_level++;
                        rank_same[level[v_val]].append(to_string(v_val)).append("; ");
                    }

                    //printing edges
                    if(v_par!=u_val)
                        fout <<"\t"<< u_val << " -> " << v_val << "[style=dashed, color=slategray, label=\""<<uv_wt<<"\"];" << endl;
                    else
                        fout <<"\t"<< u_val << " -> " << v_val << "[style=bold, color=salmon, label=\""<<uv_wt<<"\"];" << endl;

                }//for adj
                fout << endl ;
            }//while qLoGraph
        }// bfs_vis
    }// for each vertex i

    //printing node with same level together
    for(int i=0; i<max_level; i++){
        if(!rank_same[i].empty())
        fout<<"{rank = same; "<<rank_same[i]<<"}; "<<endl;
    }//for i

    fout << "}" << endl;
/**********************/
    fout.close();

    string cmd = "dot -Tpng "+filepath+" -o "+output_directory+file_name+".png";
    system((const char*)cmd.c_str());

    cout << "\n\t 2. Dijkstra:: Printed, FileName = "<<output_directory<<file_name<<".png \n";
    remove(filepath.c_str());
}//printGraphVizDijkstra

/********************************************** --- DFS Functions --- ****************************************************/

/********************************************************************************************
    DFS Main Function to find Start time, End time, Low Number, SCC Components.
********************************************************************************************/
void DiGraph::DFSDiGraph(int src=0){
    vector<bool> dfs_rec(V,false);       // book keeping variable to indicate node is currently in stack
    vector<bool> dfs_vis(V,false);       // book keeping variable to indicate node is currently in stack
    stack<int> st;

    dfs_st.assign(V,0);
    dfs_et.assign(V,0);
    lowlink.assign(V,0);
    vertex_component.assign(V,0);

    dfs_number=0;
    max_scc=0;
//    delete[] scc;
//    scc = new vector<int>[V];

    DFSRec(src, dfs_rec,st, dfs_vis);

    for(int v=0; v<V; v++){
        if(dfs_st[v]==0)
            DFSRec(v, dfs_rec,st, dfs_vis);
    }
}

/********************************************************************************************
    DFS Helper Recursive Function
********************************************************************************************/
void DiGraph::DFSRec(int s,  vector<bool> &dfs_rec,  stack<int> &st, vector<bool> &dfs_vis){

    ++dfs_number;
    dfs_st[s] = dfs_number;
    lowlink[s] = dfs_number;

//    cout<<"1 lowlink["<<s<<"]: "<< lowlink[s]<<endl;

    dfs_rec[s]=true;
    dfs_vis[s]=true;
    st.push(s);

    for(auto w: adj[s]){
//        cout<<" w first ["<<s<<"]: "<<w.first<<endl;
        if(dfs_st[w.first]==0)       //tree edge
        {
            DFSRec(w.first, dfs_rec, st, dfs_vis);
            dfs_edge[s][w.first].first='T';
            dfs_edge[s][w.first].second=w.second;
//            dag_edge[s][w.first]='T';

            lowlink[s] = min(lowlink[s], lowlink[w.first]);
//            cout<<"2 lowlink["<<s<<"]: "<< lowlink[s]<<endl;
        }
        else if(dfs_st[w.first]>dfs_st[s]) //forward edge
        {// if( dfs_et[s] > dfs_et[w.first] && dfs_st[w.first] > dfs_st[s] && dfs_et[w.first] > dfs_st[w.first] )
            dfs_edge[s][w.first].first='F';
            dfs_edge[s][w.first].second=w.second;
//            dag_edge[s][w.first]='F';
        }
        else if(!dfs_vis[w.first])  // cross edge
        {// if( dfs_et[s] > dfs_st[s] && dfs_st[s] > dfs_et[w.first] && dfs_et[w.first] > dfs_st[w.first]  )
            dfs_edge[s][w.first].first='C';
            dfs_edge[s][w.first].second=w.second;
//            dag_edge[s][w.first]='C';
            if(dfs_rec[w.first])lowlink[s] = min(lowlink[s], dfs_st[w.first]);
        }
        else // back edge
        { // if( dfs_et[w.first] >= dfs_et[s] && dfs_st[w.first] <= dfs_st[s] )
            dfs_edge[s][w.first].first='B';
            dfs_edge[s][w.first].second=w.second;
//            dag_edge[s][w.first]='B';
            if(dfs_rec[w.first])lowlink[s] = min(lowlink[s], dfs_st[w.first]);
//            cout<<"3 lowlink["<<s<<"]: "<< lowlink[s]<<endl;
        }

    }//for adj
    dfs_vis[s]=false;
    ++dfs_number;
    dfs_et[s] = dfs_number;

    int se; //stack element
    //base vertex
    if(lowlink[s] == dfs_st[s]){
        scc[max_scc].clear();
//        vector<int> component;
        while(st.top() != s){
            se = st.top(); st.pop();
            dfs_rec[se] = false;
            scc[max_scc].push_back(se);
            vertex_component[se]=max_scc;
//            cout << se << " ";
        }
        se = st.top(); st.pop();
        dfs_rec[se]=false;
//        cout << se << "\n";
        scc[max_scc].push_back(se);
        vertex_component[se]=max_scc;
//        scc[max_scc].push_back(component);
        max_scc++;
    }//lowlink s
}//DFSRec

/********************************************************************************************
    Print the DFS Traversal of Graph using Graphviz
********************************************************************************************/
void DiGraph::printGraphDFSTraversal(const string& file_name){
    if (V == 0) {
        cout << "DFS Traversal:: Graph is Empty."<<endl;
        return;
    }

    ofstream fout;
    string fileopen = file_name+".gv";
    string filepath = output_directory+fileopen;
    fout.open(filepath.c_str(), ios::out);
    if (!fout) {
        cout << "\nDFS Traversal:: **File failed to open**\n\n";
        fout.clear();
    }
/**********************/
    queue<int> qLoGraph;		// queue level order Graph
    bfs_vis.assign(V,false);

    vector<int> level(V,0);
    string rank_same[V];
    int max_level=1;

    int u_val, u_st, u_et;
    int v_val, uv_wt;
    char uv_e;
/**********************/
    fout << "digraph DFSTraversal {" << endl;
    fout << "  ranksep=\"equally\"; \n"
            "  rankdir=LR; " << endl << endl;

    fout << "X[label = \"VAL\\n(ST, ET)\"];"<<endl<<endl;

    for(int i=0; i<V; i++)
    {
        if(!bfs_vis[i]){
            qLoGraph.push(i);
            bfs_vis[i]=true;
            string curr_color = graphviz_colors[i%12];
            level[i]=0;
            rank_same[level[i]].append(to_string(i)).append("; ");

            while (!qLoGraph.empty())		// Level order Traversal
            {
                u_val = qLoGraph.front();  qLoGraph.pop();
                u_st = dfs_st[u_val];
                u_et = dfs_et[u_val];

                //printing node
                if(u_st!=1)
                    fout << u_val << " [label = \""<<u_val<<"\\n("<<u_st<<","<<u_et<<")\", style=bold, color="<<curr_color<<"];"<< endl;
                else
                    fout << u_val << " [label = \""<<u_val<<"\\n("<<u_st<<","<<u_et<<")\", style=bold];"<< endl;

                for(auto w: adj[u_val]){
                    v_val = w.first;        //adj node value
                    uv_wt = w.second;  //edge u-v weight
                    uv_e = dfs_edge[u_val][v_val].first;

                    if(!bfs_vis[v_val]){
                        bfs_vis[v_val]=true;
                        qLoGraph.push(v_val);
                        level[v_val]=level[u_val]+1;
                        if(level[v_val]+1>max_level)max_level++;
                        rank_same[level[v_val]].append(to_string(v_val)).append("; ");
                    }

                    //printing edges
                    if(uv_e=='T')
                        fout <<"\t"<< u_val << " -> " << v_val << "[style=bold, label=\""<<uv_wt<<" T\"];" << endl;
                    else if(uv_e=='F')
                        fout <<"\t"<< u_val << " -> " << v_val << "[style=solid, color=lightseagreen, label=\""<<uv_wt<<" F\"];" << endl;
                    else if(uv_e=='C')
                        fout <<"\t"<< u_val << " -> " << v_val << "[style=solid, color=slategray, label=\""<<uv_wt<<" C\"];" << endl;
                    else if(uv_e=='B')
                        fout <<"\t"<< u_val << " -> " << v_val << "[style=solid, color=salmon, label=\""<<uv_wt<<" B\"];" << endl;

                }//for adj
                fout << endl ;
            }//while qLoGraph
        }// bfs_vis
    }// for each vertex i

    //printing node with same level together
    for(int i=0; i<max_level; i++){
        if(!rank_same[i].empty())
            fout<<"{rank = same; "<<rank_same[i]<<"}; "<<endl;
    }//for i

    fout << "}" << endl;
/**********************/
    fout.close();

    string cmd = "dot -Tpng "+filepath+" -o "+output_directory+file_name+".png";
    system((const char*)cmd.c_str());
    cout << "\n\t 3. DFS Traversal:: Printed, FileName = "<<output_directory<<file_name<<".png \n";
    remove(filepath.c_str());
}//printGraphVizDijkstra

/********************************************** --- SCC Functions --- ****************************************************/

/********************************************************************************************
    Tarjan Algorithm uses DFS function and after that this function is used to print the
    Graph using SCC with its components.
********************************************************************************************/
void DiGraph::printGraphSCCTarjan(const string& file_name){
    if (V == 0) {
        cout << "SCCTarjan:: Graph is Empty."<<endl;
        return;
    }

    ofstream fout;
    string fileopen = file_name+".gv";
    string filepath = output_directory+fileopen;
    fout.open(filepath.c_str(), ios::out);
    if (!fout) {
        cout << "\nSCCTarjan:: **File failed to open**\n\n";
        fout.clear();
    }
/**********************/
    queue<int> qLoGraph;		// queue level order Graph
    bfs_vis.assign(V,false);

    vector<int> level(V,0);
    string rank_same[V];


    int max_level=1;

    int u_val, u_st, u_et, u_low;
    int v_val, uv_wt;
    char uv_e;

    string node_color;
    string line_style;
/**********************/
    fout << "digraph SCCTarjan {" << endl;
    fout << "  ranksep=\"equally\"; \n"
            "  rankdir=LR; " << endl << endl;

    fout << "X[label = \"VAL\\n(ST, LOW)\"];"<<endl<<endl;

    for(int i=0; i<V; i++)
    {

        if(!bfs_vis[i]){

            qLoGraph.push(i);
            bfs_vis[i]=true;
            node_color = graphviz_colors[i % 12];
//            c=(c+1)%10;
            level[i]=0;
            rank_same[level[i]].clear();
            rank_same[level[i]].append(to_string(i)).append("; ");

            while (!qLoGraph.empty())		// Level order Traversal
            {
                u_val = qLoGraph.front();  qLoGraph.pop();
                u_st = dfs_st[u_val];
                u_et = dfs_et[u_val];
                u_low = lowlink[u_val];

                //printing node
                if(u_st!=u_low)
                    fout << u_val << " [label = \"" << u_val << "\\n(" << u_st << "/" << u_low << ")\", style=bold, color="<<node_color<<", shape=circle];" << endl; // , "<<u_low<<"
                else
                    fout << u_val << " [label = \""<<u_val<<"\\n("<<u_st<<"/"<<u_low<<")\", style=\"filled,bold\", color=lightgray, shape=doublecircle];"<< endl;

                for(auto w: adj[u_val]){
                    v_val = w.first;        //adj node value
                    uv_wt = w.second;  //edge u-v weight
                    uv_e = dfs_edge[u_val][v_val].first;

                    if(vertex_component[u_val] == vertex_component[v_val])
                        line_style="solid";
                    else
                        line_style = "dashed";

                    if(!bfs_vis[v_val]){
                        bfs_vis[v_val]=true;
                        qLoGraph.push(v_val);
                        level[v_val]=level[u_val]+1;
                        if(level[v_val]+1>max_level)max_level++;
                        rank_same[level[v_val]].append(to_string(v_val)).append("; ");
                    }

                    //printing edges
                    if(uv_e=='T')
                        fout <<"\t"<< u_val << " -> " << v_val << "[style=\""<<line_style<<",bold\", label=\""<<uv_wt<<"/T\"];" << endl;
                    else if(uv_e=='F')
                        fout <<"\t"<< u_val << " -> " << v_val << "[style="<<line_style<<", color=lightseagreen, label=\""<<uv_wt<<"/F\"];" << endl;
                    else if(uv_e=='C')
                        fout <<"\t"<< u_val << " -> " << v_val << "[style="<<line_style<<", color=slategray, label=\""<<uv_wt<<"/C\"];" << endl;
                    else if(uv_e=='B')
                        fout <<"\t"<< u_val << " -> " << v_val << "[style="<<line_style<<", color=salmon, label=\""<<uv_wt<<"/B\"];" << endl;

                }//for adj
                fout << endl ;
            }//while qLoGraph
        }// bfs_vis
    }// for each vertex i

    //printing node with same level together
    for(int i=0; i<max_scc; i++){
        node_color = graphviz_colors[i % 12];
//        int scc_i_size = scc[i].size();
        string comp_same[V];
        set<int> comp_level;            //unique level within a component
        fout << endl ;
        fout<<"subgraph cluster_"<<i+1<<"{ "<<endl;
        fout<<"\tlabel = \"C"<<i+1<<"\"; "<<endl;
        fout << "\tcolor = " << node_color << "; " << endl;

            cout << "\n\t C [" << i+1<<"] vertexes: ";
        for(auto cmp: scc[i]) {
            cout <<cmp<<"  ";
        }

        for(auto cmp: scc[i]){
//            cout <<"\n c:"<<cmp<<" l:"<<level[cmp]<<" s:";
            comp_same[level[cmp]].append(to_string(cmp)).append("; ");
            comp_level.emplace(level[cmp]);
//            cout <<comp_same[level[cmp]]<<"\t";
        }
        for(auto l: comp_level)
            fout<<"\t{rank = same;"<<comp_same[l]<<" }"<<endl;

        fout<<" }"<<endl;
    }//for i

    fout << "}" << endl;
/**********************/
    fout.close();

    string cmd = "dot -Tpng "+filepath+" -o "+output_directory+file_name+".png";
    system((const char*)cmd.c_str());
    cout << "\n\t 5. SCCTarjan:: Num of Components: "<<max_scc;
    cout << "\n\t 5. SCCTarjan:: Printed, FileName = "<<output_directory<<file_name<<".png \n";
    remove(filepath.c_str());
}//printGraphVizDijkstra

/********************************************** --- Semi-Connectedness Functions --- ****************************************************/

/********************************************************************************************
    Function to Check Semi-Connectedness of the Graph
********************************************************************************************/
bool DiGraph::CheckSemiConnectedDiGraph(){

    auto *condensed_adj = new  vector<pair<int,int>>[V];     // condensed Graph of SCC i.e. DAG adjancey list
    vector<int> baseVertexIndegree(V);       // Base Vertex Indegree
    int v, uv_wt;
    int num_of_edges_cg=0;

    // which vertex belong to which component and making condensed graph for it
    for(int u=0; u<V; u++){
       for(auto w: adj[u]){
            v = w.first;
            uv_wt = w.second;
            if(vertex_component[u] != vertex_component[v]) {
                condensed_adj[vertex_component[u]].emplace_back(make_pair(vertex_component[v], uv_wt));
                baseVertexIndegree[vertex_component[v]]++;
                num_of_edges_cg++;
            }
       } //for w
    }//for v

//    cout << "\n\t Adjacency List of CG(" << max_scc<<", "<<E<<") :: \n";
//    for (int v = 0; v < max_scc; ++v) {
//        cout << "\n V["<< v << "]:";
//        for (auto w : condensed_adj[v])
//            cout << " -> (" << w.first<<", "<<w.second<<")";
//        printf("\n");
//    }

    // performing topological order for a SCC Condensed Graph
    queue<int> qTopo;
    vector<int> topological_order;
    for (int i=0; i < max_scc; i++)
        if(baseVertexIndegree[i]==0)
            qTopo.push(i);

    while(!qTopo.empty()){
        int u = qTopo.front(); qTopo.pop();
        topological_order.push_back(u);

        for(auto w: condensed_adj[u]){
            if(--baseVertexIndegree[w.first]==0)
                qTopo.push(w.first);
        }
    }//while qTopo


//        for(auto w: topological_order)
//            cout<<" "<<w;



    // checking if topological order has a chain or not.
    bool areAdjNodeConnected = false;
    for(int n=1; n<max_scc; n++){
        areAdjNodeConnected = false;
        for(auto cw: condensed_adj[topological_order[n-1]]){
            if(cw.first == topological_order[n])
                areAdjNodeConnected = true;
        }//for cw
        if(!areAdjNodeConnected)  {
            printCondensedDiGraph(max_scc,num_of_edges_cg,condensed_adj,topological_order, false);
            return false;
        }
//        return false;
    }// for n
    printCondensedDiGraph(max_scc,num_of_edges_cg,condensed_adj,topological_order, true);
    return true;

}

/********************************************************************************************
    Graphviz Function to Print Condensed Graph of SCC
********************************************************************************************/
void DiGraph::printCondensedDiGraph(int cgV, int cgE, vector<pair<int,int>> cgadj[], const vector<int>& top, bool areAdjNodeConnected){
    if (cgV == 0) {
        cout << "Condensed Graph is Empty."<<endl;
        return;
    }

    string file_name = "Condensed_SCC_DiGRAPH_G_V_" + to_string(cgV) + "_E_" + to_string(cgE) + "__" + (areAdjNodeConnected ? "TRUE" : "FALSE");

    ofstream fout;
    string fileopen = file_name+".gv";
    string filepath = output_directory+fileopen;
    fout.open(filepath.c_str(), ios::out);
    if (!fout) {
        cout << "\nCondensed Graph:: **File failed to open**\n\n";
        fout.clear();
    }
/**********************/
    queue<int> qLoGraph;		// queue level order Graph
    bfs_vis.assign(cgV, false);

    vector<int> level(cgV, 0);
    string rank_same[cgV];
    int max_level=1;

    int u_val;
    int v_val, uv_wt;
/**********************/
    fout << "digraph condensedGraph_" << cgV << "_" << cgE << " {" << endl;
    fout << "  rankdir=LR; " << endl << endl;

    fout << "X[label = \"Component\"];"<<endl<<endl;

    for(int i=cgV-1; i >= 0; i--)
    {
        if(!bfs_vis[i]){
            qLoGraph.push(i);
            bfs_vis[i]=true;

            string curr_color = graphviz_colors[i%12];

            level[i]=0;
            rank_same[level[i]].append(to_string(i)).append("; ");

            while (!qLoGraph.empty())		// Level order Traversal
            {
                u_val = qLoGraph.front();  qLoGraph.pop();

                fout << u_val << " [label = \""<<u_val<<"\", color="<<curr_color<<", shape=circle];"<< endl;

                for(auto w: cgadj[u_val]){
                    v_val = w.first;        //cgadj node value
                    uv_wt = w.second;  //edge u-v weight

                    if(!bfs_vis[v_val]){
                        bfs_vis[v_val]=true;
                        qLoGraph.push(v_val);

                        level[v_val]=level[u_val]+1;
                        if(level[v_val]+1>max_level)max_level++;
                        rank_same[level[v_val]].append(to_string(v_val)).append("; ");
                    }

                    fout <<"\t"<< u_val << " -> " << v_val << "[label=\""<<uv_wt<<"\"];" << endl;
                }//for cgadj
                fout << endl ;
            }//while qLoGraph
        }// bfs_vis
    }// for each vertex i

    //printing node with same level together
    for(int i=0; i<max_level; i++){
        if(!rank_same[i].empty())
            fout<<"{rank = same; "<<rank_same[i]<<"}; "<<endl;
    }//for i

    fout << "}" << endl;
/**********************/
    fout.close();

    string cmd = "dot -Tpng "+filepath+" -o "+output_directory+file_name+".png";
    system((const char*)cmd.c_str());
    cout << "\n\t 4. Condensed Graph:: Printed, FileName = "<<output_directory<<file_name<<".png \n";
    remove(filepath.c_str());
}//printDiGraph

/********************************************** --- Minimal SCC Functions --- ****************************************************/
/********************************************************************************************
    Function to Reduce the Edges in the SCC
********************************************************************************************/
void DiGraph::ReduceMainSCCEdges(){

    int v, uv_wt;
    int vcu, vcv;
    int max_edges_overall_scc=0;
    unordered_map<int, unordered_map<int,int>> dag_edge;
//    vector<bool> cmp_node_has_edge(V,false);
//    vector<int> baseVertexIndegree(V);       // Base Vertex Indegree

/************* Adding Necessary Edges Across Different SCC *************/
     for(int u=0; u<V; u++){
        clone_adj[u].clear();
        for(auto w: adj[u]){
            v = w.first;
            uv_wt = w.second;
            vcu=vertex_component[u];
            vcv=vertex_component[v];
            if(vcu != vcv)  // two different component graph
            {

                if(dag_edge[vcu].find(vcv) == dag_edge[vcu].end() ) // edge not exist between them
                {
//                    char edge_t = dfs_edge[u][v].first;
//                    if(edge_t!='F')
//                    {
                        clone_adj[u].emplace_back(make_pair(v,uv_wt));
                        dag_edge[vcu][vcv]=1;                   // add the edge
                        max_edges_overall_scc++;
//                        cmp_node_has_edge[v]=true;
//                    }
                }
            }
        } //for w
    }//for v


/************* Adding Necessary Edges Within the SCC *************/
    int s, d;
    int sd_wt, ds_wt;
    char edge_type;
    int curr_s2;
    unordered_map<int, unordered_map<int,int>> edgeAdded;   // to store which edges are added.

    for(int i=0; i<max_scc; i++){
    // if component graph size is 1 then do nothing
        if(scc[i].size() == 1)
            continue;
    // if component graph size is 2 then just add cycle between them
        if(scc[i].size() == 2)
        {
             s = scc[i][0];
             d = scc[i][1];
             sd_wt = dfs_edge[s][d].second;
            ds_wt = dfs_edge[d][s].second;
            max_edges_overall_scc+=2;
            edgeAdded[s][d]=1;
            edgeAdded[d][s]=1;
            clone_adj[s].emplace_back(make_pair(d,sd_wt));
            clone_adj[d].emplace_back(make_pair(s,ds_wt));
            continue;
        }//if scc size =2

        unordered_map<int, int> sccCrossEdges;  // store Cross Edges
//        unordered_map<int, int> sccBackEdges;   // store Back Edges
        vector<int> sccBackEdgesVec[V];   // store Back Edges
        vector<int> sccTreeEdgesVec[V];   // store Tree Edges

    // if component graph size is >=3
        for(auto s1: scc[i]){
            for(auto s2: scc[i]){
//                edge_type = dfs_edge[s1][s2].first;
                    // two different node and edge exist between them
                    if(s1 != s2 && dfs_edge[s1].find(s2) != dfs_edge[s1].end())
                    {
                        edge_type = dfs_edge[s1][s2].first;
                        sd_wt = dfs_edge[s1][s2].second;
//                        cout<<"\ns1 --> s2: "<<s1<<"-->"<<s2<<"\t";
//                        cout<<" ("<<edge_type<<") / "<<sd_wt<<"\n";

                        switch(edge_type){
                            case 'T':// if tree edge is not present
                                    if( edgeAdded[s1].find(s2) == edgeAdded[s1].end()){
//                                        clone_adj[s1].emplace_back(make_pair(s2,sd_wt));
                                        edgeAdded[s1][s2]=1;
                                        sccTreeEdgesVec[s1].emplace_back(s2);
//                                        max_edges_overall_scc++;
                                    }
                                break;
                            case 'C': // if cross edge is not present
                                    if( sccCrossEdges.find(s1) == sccCrossEdges.end()){
                                        sccCrossEdges.insert({s1,s2});
                                    }
                                    else{
                                         curr_s2 = sccCrossEdges.at(s1); //previous destination
                                        if(dfs_st[s2]<dfs_st[curr_s2] && dfs_et[s2] > dfs_et[curr_s2])
                                            sccCrossEdges.insert({s1,s2});  //update old src-dest to new superset cross edge
                                    }
                                break;
                            case 'B':
//                                if(s1!=s2)      // no self loops
//                                    sccBackEdges.insert({s1,s2});
                                sccBackEdgesVec[s1].emplace_back(s2);
                                    edgeAdded[s1][s2]=1;
                                break;
                            default: break;//do nothing;
                        }//switch edge type

                    }//if s1 != s2
            } //for s2
        }// for s1

        // Add cross edges from map to graph
        for(auto &umce: sccCrossEdges){
            s = umce.first;
            d = umce.second;
            sd_wt = dfs_edge[s][d].second;
            clone_adj[s].emplace_back(make_pair(d,sd_wt));
            edgeAdded[s][d]=1;
            max_edges_overall_scc++;
        }// for um

        // Add back edges from map to graph
//        for(int bi=0; bi< V; bi++) {
//            for (auto x: sccBackEdgesVec[bi]){
//                    s = bi;
//                d = x;
//                sd_wt = dfs_edge[s][d].second;
//                cout << "\n\tbx --> by: " << s << "-->" << d << "\t";
//                cout << " (" << "B" << ") / " << sd_wt << "\n";
//            }
//        }
        for(int bi=0; bi<V; bi++) {
            for (auto x: sccBackEdgesVec[bi]) {
                s = bi;
                d = x;
                sd_wt = dfs_edge[s][d].second;

                unordered_map<int, bool> v_visited; //vertex visited in component;

//                cout << "\n\ts --> d: " << s << "-->" << d << "\t";
//                cout << " (" << "B" << ") / " << sd_wt << "\n";

                edgeAdded[s].erase(d);
//                for (auto vv: v_visited) {
//                    cout << "\n\t  1vis[" << vv.first << "]-->" << vv.second << "\t";
//                }
                //call dfs to see if d can still be reachable by removing back edge;
                SCCMinimalDFSHelper(s, v_visited, edgeAdded);
                //if cannot reachable then add the back edge otherwise it is already deleted.
//                for (auto vv: v_visited) {
//                    cout << "\n\t  2vis[" << vv.first << "]-->" << vv.second << "\t";
//                }
                if (v_visited.find(d) == v_visited.end()) {
                    clone_adj[s].emplace_back(make_pair(d, sd_wt));
                    edgeAdded[s][d] = 1;
                    max_edges_overall_scc++;
                }
            }// for x
        }//for bi to V

        // Add tree edges from map to graph
        for(int ti=0; ti<V; ti++) {
            for (auto tx: sccTreeEdgesVec[ti]) {
                s = ti;
                d = tx;
                sd_wt = dfs_edge[s][d].second;

                unordered_map<int, bool> v_visited; //vertex visited in component;

//                cout << "\n\ts --> d: " << s << "-->" << d << "\t";
//                cout << " (" << "B" << ") / " << sd_wt << "\n";

                edgeAdded[s].erase(d);
//                for (auto vv: v_visited) {
//                    cout << "\n\t  1vis[" << vv.first << "]-->" << vv.second << "\t";
//                }
                //call dfs to see if d can still be reachable by removing back edge;
                SCCMinimalDFSHelper(s, v_visited, edgeAdded);
                //if cannot reachable then add the back edge otherwise it is already deleted.
//                for (auto vv: v_visited) {
//                    cout << "\n\t  2vis[" << vv.first << "]-->" << vv.second << "\t";
//                }
                if (v_visited.find(d) == v_visited.end()) {
                    clone_adj[s].emplace_back(make_pair(d, sd_wt));
                    edgeAdded[s][d] = 1;
                    max_edges_overall_scc++;
                }
            }// for x
        }//for bi to V


    }// for i to max_scc


//    cout << "\n\t Adjacency List of Minimal Graph G(" << max_scc<<", "<<max_edges_overall_scc<<") :: \n";
//    for (int u = 0; u < V; ++u) {
//        cout << "\n V["<< u << "]:";
//        for (auto w : clone_adj[u])
//            cout << " -> (" << w.first<<", "<<w.second<<")";
//        printf("\n");
//    }
    cout << "\n\t 6. MinimalSCC:: Graph G("<<V<<", "<<max_edges_overall_scc<<") \n";
}

/********************************************************************************************
    DFS Helper function to check reachability of vertex by removing back edge
********************************************************************************************/
void DiGraph::SCCMinimalDFSHelper(int s, unordered_map<int, bool> &vis, unordered_map<int, unordered_map<int,int>> &SCCedgeAdded){

        vis.insert(make_pair(s, true));
        int d;
        for(auto w: adj[s]){
            d=w.first;
//            cout << "\n\tdfs --> : " << s << "-->" << d << "\t";
            if(vis.find(d) == vis.end() && vertex_component[s]==vertex_component[d]){
                if(SCCedgeAdded[s].find(d) != SCCedgeAdded[s].end()){
//                    cout << "\n\t called";
                    SCCMinimalDFSHelper(d,vis, SCCedgeAdded);
                } // if edge exist
            }//if vis
//            cout << "\n\t not called";
        }// for auto w
}

/********************************************************************************************
    Graphviz function to print minimal graph of SCC
********************************************************************************************/
void DiGraph::printGraphMinimalSCC(const string& file_name){
    if (V == 0) {
        cout << "MinimalSCC:: Graph is Empty."<<endl;
        return;
    }

    ofstream fout;
    string fileopen = file_name+".gv";
    string filepath = output_directory+fileopen;
    fout.open(filepath.c_str(), ios::out);
    if (!fout) {
        cout << "\nMinimalSCC:: **File failed to open**\n\n";
        fout.clear();
    }
/**********************/
    queue<int> qLoGraph;		// queue level order Graph
    bfs_vis.assign(V,false);

    vector<int> level(V,0);
    string rank_same[V];


    int max_level=1;

    int u_val, u_st, u_et, u_low;
    int v_val, uv_wt;
    char uv_e;

    string node_color;
    string line_style;
/**********************/
    fout << "digraph MinimalSCC {" << endl;
    fout << "  ranksep=\"equally\"; \n"
            "  rankdir=LR; " << endl << endl;

    fout << "X[label = \"VAL\\n(ST, LOW)\"];"<<endl<<endl;

    for(int i=0; i<V; i++)
    {

        if(!bfs_vis[i]){

            qLoGraph.push(i);
            bfs_vis[i]=true;
            node_color = graphviz_colors[i % 12];
//            c=(c+1)%10;
            level[i]=0;
            rank_same[level[i]]="";
            rank_same[level[i]].append(to_string(i)).append("; ");

            while (!qLoGraph.empty())		// Level order Traversal
            {
                u_val = qLoGraph.front();  qLoGraph.pop();
                u_st = dfs_st[u_val];
                u_et = dfs_et[u_val];
                u_low = lowlink[u_val];

                //printing node
                if(u_st!=u_low)
                    fout << u_val << " [label = \"" << u_val << "\\n(" << u_st << "/" << u_low << ")\", style=bold, color="<<node_color<<", shape=circle];" << endl; // , "<<u_low<<"
                else
                    fout << u_val << " [label = \""<<u_val<<"\\n("<<u_st<<"/"<<u_low<<")\", style=\"filled,bold\", color=lightgray, shape=doublecircle];"<< endl;

                for(auto w: clone_adj[u_val]){
                    v_val = w.first;        //adj node value
                    uv_wt = w.second;  //edge u-v weight
                    uv_e = dfs_edge[u_val][v_val].first;

                    if(vertex_component[u_val] == vertex_component[v_val])
                        line_style="solid";
                    else
                        line_style = "dashed";

                    if(!bfs_vis[v_val]){
                        bfs_vis[v_val]=true;
                        qLoGraph.push(v_val);
                        level[v_val]=level[u_val]+1;
                        if(level[v_val]+1>max_level)max_level++;
                        rank_same[level[v_val]].append(to_string(v_val)).append("; ");
                    }

                    //printing edges
                    if(uv_e=='T')
                        fout <<"\t"<< u_val << " -> " << v_val << "[style="<<line_style<<", label=\""<<uv_wt<<"/T\"];" << endl;
                    else if(uv_e=='F')
                        fout <<"\t"<< u_val << " -> " << v_val << "[style="<<line_style<<", color=lightseagreen, label=\""<<uv_wt<<"/F\"];" << endl;
                    else if(uv_e=='C')
                        fout <<"\t"<< u_val << " -> " << v_val << "[style="<<line_style<<", color=slategray, label=\""<<uv_wt<<"/C\"];" << endl;
                    else if(uv_e=='B')
                        fout <<"\t"<< u_val << " -> " << v_val << "[style="<<line_style<<", color=salmon, label=\""<<uv_wt<<"/B\"];" << endl;

                }//for adj
                fout << endl ;
            }//while qLoGraph
        }// bfs_vis
    }// for each vertex i

    //printing node with same level together
    for(int i=0; i<max_scc; i++){
        node_color = graphviz_colors[i % 12];
//        string comp_rank;
        string comp_same[V];
        set<int> comp_level;
        fout << endl ;
        fout<<"subgraph cluster_"<<i+1<<"{ "<<endl;
        fout<<"\tlabel = \"C"<<i+1<<"\"; "<<endl;
        fout << "\tcolor = " << node_color << "; " << endl;

        for(auto cmp: scc[i]){
            comp_level.emplace(level[cmp]);
            comp_same[level[cmp]].append(to_string(cmp)).append("; ");
//            comp_rank.append(to_string(cmp)).append("; ");
        }
        for(auto l: comp_level)
            fout<<"\t{rank = same;"<<comp_same[l]<<" }"<<endl;

        fout<<" }"<<endl;
    }//for i

    fout << "}" << endl;
/**********************/
    fout.close();

    string cmd = "dot -Tpng "+filepath+" -o "+output_directory+file_name+".png";
    system((const char*)cmd.c_str());

    cout << "\n\t 6. MinimalSCC:: Printed, FileName = "<<output_directory<<file_name<<".png \n";
    remove(filepath.c_str());
}//printGraphMinimalSCC

/********************************************************************************************
	Function to Read Graph Inputs from the file.
    Input: File Name and Reading Time at beginning of program or in middle of program.
********************************************************************************************/
void ReadGraphFromFile(const string& ip_file_name=read_filename, int firstTime=1){
    int num_of_vertexes=0, num_of_edges=0, edge_u=0, edge_v=0, edge_wt=0;
    int ne=0;
    ifstream fin_read;       //file stream object to read
    string input_file = input_directory + ip_file_name;
        if(firstTime==1){
            cout << "\n\t To Read Graph:"
                        "\n * Enter values in the file ( "<<input_file<<" ) "
                        "\n * First Line is (int): Num_Of_Vertex Num_of_Edges "
                        "\n * Each Next line is Edge (int): u v wt "
                        "\n\t ** Make sure VERTEX VALUE start from 0 to Num_Of_Vertex-1\n";
            cout<<"\n Then Press Enter to Start\n";
            //    cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
//            cin.get();
        }

    fin_read.open(input_file.c_str(),ios::in);
    if (!fin_read) {
        cout << "\nRead Input File:: **File failed to open: "<<input_file<<"\n\n";
        fin_read.clear();
        throw "Read Input File:: **File failed to open: ";
        cin.get();
        return;
    }

    if(fin_read>>num_of_vertexes>>num_of_edges && num_of_vertexes){
        dg = new DiGraph(num_of_vertexes,num_of_edges);        // Graph object
        while(++ne<=num_of_edges){              // reading edges
            if(fin_read>>edge_u>>edge_v>>edge_wt) {
                dg->addDirectedEdge(edge_u, edge_v, edge_wt);
            }else {
                cerr << "\t ==>Failed to Read Values of Edge["<<ne<<"] From The File! : " << input_file << "\n";
                throw "Read Input File:: Invalid Input File Values u, v and wt.";
            }
        }
    }
    else {
        cerr << "\t ==>Failed to Read Values V and E From The File! : " << input_file << "\n";
        throw "Read Input File:: Invalid Input File Values V and E.";
    }

    fin_read.close();
}//ReadGraphFromFile

/********************************************************************************************
	Main Function
********************************************************************************************/
int main(){

    try{ ReadGraphFromFile();}
    catch (const char *msg) { cout << "\t\t =>" << msg << endl; return 0; }
    cout << "\n ==> File has been Successfully read.";

    string suffix ;    	// filename suffix to distinguish different files.
    string output_filename;
    char choice;		// choice exercised.

    do {
        int n_val;
        char ch;
        // for multiple inputs
        string graph_file_input ; //= "a b c 4 e6 8";

        cout << endl << endl << endl;
        system(PAUSE);
        system(CLR);

        cout << "\n\n ------- DiGRAPH MENU  -------";

        cout << "\n 1. INPUT: Read Different Graph Inputs From Another File and Replace Current One.			";

        cout << "\n\n 2. TRAVERSAL: DFS Traversal.";
        cout << "\n 3. SCC: Tarjan Algo.";
        cout << "\n 4. SHORTEST PATH: Dijkstra Algo.";

        cout << "\n\n 5. MINIMAL SCC: Same SCC of Graph G with E' as small as possible.";
        cout << "\n 6. SEMI-CONNECTED: Check Semi-connectedness of Graph.";

        cout << "\n\n a. All The Steps (p-2-3-4-5-6) in One Go.";

        cout << "\n\n s. SHOW: Show Adjacency List of Graph.";
        cout << "\n p. PRINT: Original Graph - GraphViz";
        cout << "\n n. Exit - Bye		\n\n  --Choice : ";
        cin >> ch;
        cout << "\n <-------->";

        switch (ch) {
                //Read Graph Input From File.
            case '1' :

                cout << "\n File Name to Read (present in "<<input_directory<<") (Make sure VERTEX VALUE start from 0 to Num_Of_Vertex-1)\n\t : ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, graph_file_input);
                try{
                    delete dg;
                    ReadGraphFromFile(graph_file_input,2);
                }
                catch (const char *msg) {
                    cout << "\t ==>" << msg << endl;
                    return 0;
                }
                cout << "\n ==> File has been Successfully read.";
                break;
                //DFS Traversal of Graph
            case '2' :
                cout << "\n Enter Source Vertex Value (int): ";
                    cin>>n_val;
                dg->DFSDiGraph(n_val);
                    output_filename = "3_DFSTraversal_S_"+to_string(n_val)+"_"+to_string(time(nullptr));
                    dg->printGraphDFSTraversal(output_filename);

                break;

                //SCC using Tarjan Algo.
            case '3' :
                cout << "\n Enter Source Vertex Value (int): ";
                    cin>>n_val;
                dg->DFSDiGraph(n_val);
                    output_filename = "5_TarjanSCC_S_"+to_string(n_val)+"_"+to_string(time(nullptr));
                    dg->printGraphSCCTarjan(output_filename);
                break;

                //Shortest Path Dijkstra Algo.
            case '4' :
                cout << "\n Enter Source Vertex Value (int): ";
                    cin>>n_val;
                dg->DijkstraSSSPAlgo(n_val);
                    output_filename = "2_DIJKSTRA_S_"+to_string(n_val)+"_"+to_string(time(nullptr));
                    dg->showDiGraphDijkstraValues(n_val);
                    dg->printGraphVizDijkstra(output_filename);

                break;

                //Same SCC of Graph G with E' as small as possible.
            case '5' :
                dg->DFSDiGraph();
                dg->ReduceMainSCCEdges();
                    output_filename = "6_MinimalSCC_S_"+to_string(0)+"_"+to_string(time(nullptr));
                    dg->printGraphMinimalSCC(output_filename);
                break;

                // Check Semi-connectedness of Graph.
            case '6' :
                    dg->DFSDiGraph();
                    cout<<" \n\t 4. Semi-Connectedness of Graph (T/F) : "<< std::boolalpha<<dg->CheckSemiConnectedDiGraph()<<endl;
                break;

                //All The Steps Above in One Go.
            case 'a':
                suffix = to_string(time(nullptr));
                cout << "\n Enter Source Vertex Value For All Operations (int): ";
                    cin>>n_val;
                 // 1. printing original graph
                    output_filename = suffix+"_1_Original_Digraph__";
                    dg->printDiGraph(output_filename);
                // 2. Dijkstra SSSP
                dg->DijkstraSSSPAlgo(n_val);
                    output_filename = suffix+"_2_DIJKSTRA_S_"+to_string(n_val)+"_";
                    dg->printGraphVizDijkstra(output_filename);

                // 3. DFS Traversal
                dg->DFSDiGraph(n_val);
                    output_filename = suffix+"_3_DFSTraversal_S_"+to_string(n_val)+"_";
                    dg->printGraphDFSTraversal(output_filename);

                // 4. Semi-Connectedness of Graph
                cout<<" \n\t 4. Semi-Connectedness of Graph (T/F) : "<< std::boolalpha<<dg->CheckSemiConnectedDiGraph()<<endl;

                // 5. Tarjan's SCC
                output_filename = suffix+"_5_TarjanSCC_S_"+to_string(n_val)+"_";
                    dg->printGraphSCCTarjan(output_filename);

                // 6. Minimized SCC Graph
                dg->ReduceMainSCCEdges();
                    output_filename = suffix+"_6_MinimalSCC_S_"+to_string(0)+"_";
                    dg->printGraphMinimalSCC(output_filename);

                // 2. Dijkstra Distance
                dg->showDiGraphDijkstraValues(n_val);
                break;
                //Show Adjacency List of Graph
            case 's':
                cout << "\n Graph is: \n";
                dg->showDiGraphAdjList();
                break;
                //Print Graph - GraphViz
            case 'p':
                output_filename = "1_Original_Digraph__"+to_string(time(nullptr));
                dg->printDiGraph(output_filename);
                break;

            case 'n' : cout << "\n Bye \n"; break;
            default  :   cout << "\n--Invalid Choice. Enter Again \n";
        }
        choice=ch;
    } while (choice != 'n');


    delete dg;
    cout << "\n-->ENTER TO EXIT. \n";
    cin.get();
    cin.get();
    return 0;
} //end of main()
