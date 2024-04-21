#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <unordered_map>
#include <random>
#include <fstream>
#include <sstream>

using namespace std;

/**
 * 
   IMPORTANT: this program finds a clique given a graph by finding a minimal vertex cover of the graphs compliment.
              The steps are listed below:
 
   1. Utilize the O(E) algorithm to find vertex cover of compliment graph
   (for all edges, iff the edge isn't already covered add both vertices to cover)

   2. Get the independent set, remember that IS(G) = G.v / VC(G.v)
   
   3. Now get Max Clique by reverting graph back to original, remember that MC(G) = IS(G')

   NOTE: the output of this program will be very bad as the vertex cover found is only an estimate cover
 * 
*/

// A hash function used to hash a pair of any kind, in our case we will just be using integers
struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
 
        if (hash1 != hash2) {
            return hash1 ^ hash2;              
        }
         
        // If hash1 == hash2, their XOR is zero.
          return hash1;
    }
};

// this class is for storing all the edges of a given graph, this will be called in the vertex cover class to get edges
// of the compliment graph
class Edges {

    public:

    // this stores all the edges in graph
    unordered_map<pair<int,int>, bool, hash_pair> edges;

    // function to store all edges 
    void getEdges(vector<vector<int> > &g){

        // iterate through each vertice
        for (int i = 0; i < g.size(); i++){
            // now iterate through each vertice's neighbors, we start at j+1 to avoid adding already covered edges
            for (int j = i+1; j < g.size(); j++){
                if (g[i][j]){
                    // if there exists an edge from i to j, then we can add it to our edges map
                    pair<int,int> edge(i,j);
                    edges[edge];
                }
            }
        }

    }
};

// this class helps find a vertex cover in a given graph, it also helps determine a compliment graph
class VertexCover {

    private:

        // this stores the compliment graph of G 
        vector<vector<int> > gComplement;

    public: 

        // function that creates the compliment graph of G
        void createComplement(vector<vector<int> > &g){
            int n = g.size();

            // iterate through all of G
            for (int i = 0; i < n; i++){
                vector<int>row; // temporary row vector
                for (int j = 0; j < n; j++){

                    // make sure we don't change the 0 edge from ith vertice to itself
                    if (i == j){
                        row.push_back(0);
                    } else {
                        row.push_back(g[i][j] == 0 ? 1 : 0); // otherwise we swap the values
                    }

                }
                gComplement.push_back(row);
            }
        }

        // function to return the compliment graph that will be fed into the independent set class
        vector<vector<int> > getComplement(){
            return gComplement;
        }

        // for all edges, iff the edge isn't already covered add both vertices to cover
        unordered_map<int,bool> getVC(){

            // call the Edges class to get all edges from the compliment graph
            Edges ed; ed.getEdges(gComplement);

            // this is where the vertices of the vertex cover will be stored
            // I use unordered_map for O(1) lookup time in best case
            unordered_map<int,bool>set;

            for (auto x : ed.edges){
                // set u and v to corresponding vertices
                int u = x.first.first;
                int v = x.first.second;

                // if the edge isn't already covered
                if (set.find(u) == set.end() && set.find(v) == set.end()){

                    // add the vertices to the set
                    set[u];
                    set[v];

                }
            }
            return set;
        }
};

// remember that IS(G) = G.v / VC(G.v)
class IndependentSet {

    public: 
        
        vector<int> getIS(unordered_map<int,bool> &VC, int size){
            // initialize Independent Set vector
            vector<int> IS;
            
            // iterate through all vertices within the graph
            for (int i = 0; i < size; i++){

                // if current vertice isn't in the Vertex Cover, then it is in independent set
                if (VC.find(i) == VC.end()){
                    IS.push_back(i);
                } 
            }
            
            // now we just return the IS
            return IS;
        }

};

// remember that MC(G) = IS(G')
class MaxClique {

    public:

        // all this function does is just print out the vertices in the Independet Set of the compliment graph
        // this is because of the fact that MC(G) = IS(G')
        void printClique(vector<int> &IS){
            for (auto x : IS){
                cout << x << " ";
            } cout << endl;
        }
};

// this is the function that reads in a diagonal input graph to make it a whole nxn matrix
vector<vector<int> > openFile(string filename){
    vector<vector<int> > graph;

    ifstream inputFile(filename);

    if (!inputFile){ // checking to see if file can be opened
        cout << "Error: unable to open file" << endl;
        return {};
    }

    string line;

    // read in diagonal graph
    while (getline(inputFile, line)){
        vector <int> row;
        istringstream inputstream(line);
        int val = 0;
        bool first = true;
        while(inputstream >> val ){
            if (first){ // this makes it so that it doesn't read in the first column
                first = false;
            } else {
                row.push_back(val);
            }  
        }

        row.push_back(0); // adds the 0 in the back 

        graph.push_back(row);
               
    }

    // close the file
    inputFile.close();

    // Make the diagonal graph symmetric
    for (int i = 0; i < graph.size(); ++i) {
        for (int j = i + 1; j < graph.size(); ++j) {
            graph[i].push_back(graph[j][i]);
        }
    }

    return graph;
}

int main(int argc, char** argv){

    // ensure correct usage of program
    if (argc != 2){
        cout << "Usage: ./a.out <samplefile.adjmat>" << endl;
        return 0;
    }

    // initialzing clocks
    clock_t start, end;

    // initialize the graph to be tested
    vector<vector<int> > graph = openFile(argv[1]);

    // if graph is empty then that means the file couldn't be opened
    if (graph.size() == 0) return 0;

    // initialize classes 
    VertexCover vc; IndependentSet is; MaxClique mc;

    // get vertex cover of compliment graph
    start = clock(); // start the clock
    vc.createComplement(graph);
    unordered_map<int,bool> VC = vc.getVC();

    // get the independet set of the compliment graph
    vector<int> IS = is.getIS(VC, graph.size());

    // print out the clique found 
    end = clock(); // end the clock
    double time = (double) (end-start) / CLOCKS_PER_SEC * 1000.0;
    cout << "Heuristic took: " << time << endl;
    mc.printClique(IS);

    return 0;
}