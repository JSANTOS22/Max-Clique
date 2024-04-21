#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <unordered_map>
#include <random>
#include <sstream>
#include <fstream>
#include <queue>

using namespace std;

/**
 * 
    IMPORTANT: This program finds a maximal clique by using a priority queue and vector of cliques.
               The steps of how I accomplished this is shown below:

    STEPS: 
        - we place each vertice in max priority queue based off of their adjacency size (# of neighbors)
        - in the find clique function, we add first item in queue to a clique that gets pushed into 'cliques' vector
            - for each ith clique in the 'cliques' vector:
                - check if the item has edges to every vertice in ith clique
                - if it does, we can add to clique 
                - else don't add to clique
            - if the item was never added into any clique in the 'cliques' vector, then push item as it's own clique in vector
            - move on to next item in queue
        - after find clique function, I return whichever clique in the 'cliques' vector has the biggest size
 * 
*/
class MySolution{

    private:

    // this will store a vector of cliques so that we can create multiple cliques and compare from the one graph
    vector<unordered_map<int, int> > cliques;

    // this is the structure of the condition to sort the pairs by its second elements
    struct myComp {
        constexpr bool operator()(
            pair<int, int> const& a, 
            pair<int, int> const& b)
            const noexcept {
                return a.second < b.second;
            }
    };

    // this will store all the vertices. Vertices with more neighbors will have priority in the queue
    typedef pair<int, int> vertice;
    priority_queue<vertice, vector<vertice>, myComp> queue;

    //------------- HELPER FUNCTIONS THAT ARE CALLED IN ORDER AS THEY APPEAR -------------//

    // 1st helper function called that stores all vertices into a priority queue that's ordered by it's neighbor count
    void organizeVertices(vector<vector<int> > &g){

        // initialize a counter 
        int count = 0;

        // loop through each vertice in graph and count their neighbors
        for (int i = 0; i < g.size(); i++){
            for (int j = 0; j < g[i].size(); j++){
                // if edge 'g[i][j] == 1' then that means there's an edge, so we can increment the counter
                if (g[i][j])
                    count++;
            }

            // push vertice and their neighbor found into priority queue
            queue.push(make_pair(i, count));
            
            // reset the counter to 0
            count = 0;
        }
    }

    // 2nd helper function called that iterates through all vertices to then potentially add them to clique
    void findClique(vector<vector<int> > &g){

        // initialize an unordered_map as a clique
        unordered_map<int, int> Clique;

        // add item at the top of queue into the clique since it has the most edges
        Clique[queue.top().first]++;
        queue.pop();

        // now push the Clique into the cliques vector
        cliques.push_back(Clique);

        // now iterate through the rest of the queue
        while (!queue.empty()){

            int v = queue.top().first;

            // this boolean operator keeps track to see if v was added to any clique at all
            // if not, then we must add it to it's own clique into the cliques vector
            bool newClique = true;

            // now we search to see if v fits in any cliques that exist so far in the cliques vector
            for (int i = 0; i < cliques.size(); i++){

                // make state of canAdd to true so we can possibly add to clique
                bool canAdd = true;

                // now we iterate through all vertices in Clique
                for (auto u : cliques[i]){

                    // checks the edge of v and u, 0 means there's no edge
                    if (g[v][u.first] == 0){
                        canAdd = false;
                        break;
                    }

                }

                // if canAdd is still true, then it has edges to all in Clique
                if (canAdd){
                    cliques[i][v]++; // add v into Clique at cliques[i] in cliques vector
                    newClique = false; // indicate that we don't have to allocate more storage for another clique in cliques vector
                }
            }

            // if new clique is still true, then that means v wasn't added into any existing clique
            // so we must make it the start of a new clique in the cliques vector
            if (newClique){
                unordered_map<int, int> newClique;
                newClique[v]++;
                cliques.push_back(newClique);
            }

            // pop the queue so we can get to the next value in the clique
            queue.pop();
        }
    }   

    // 3rd helper function called that goes through the cliques vector and find's which ever clique is the biggest one
    unordered_map<int, int> maxClique(){

        int max = 0;

        // iterate through the cliques that were found 
        for (int i = 0; i < cliques.size(); i++){
            
            // compare the previous max with the current clique size to see if we found a new max
            if (cliques[i].size() > cliques[max].size()){
                max = i;
            }
        }

        // now we return the maximum clique that was found
        return cliques[max];
    }

    public:

    // this is the main function that finds a clique in graph
    unordered_map<int,int> solution(vector<vector<int> > &g){

        // store all vertices into a priority queue which is ordered by their neighbor count
        organizeVertices(g);

        // call the function to find cliques and feed them into the cliques vector
        findClique(g);

        // now find the biggest clique that was found
        return maxClique();
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

/**
 * 
   0's repansent no edge, 1's repansent an edge
 * 
*/
int main(int argc, char** argv){

    // ensure correct usage of program
    if (argc != 2){
        cout << "Usage: ./a.out <samplefile.adjmat>" << endl;
        return 0;
    }

    vector<vector<int> > graph = openFile(argv[1]);

    // if graph is empty then that means the file couldn't be opened
    if (graph.size() == 0) return 0;

    // initialize class
    MySolution g; 
    clock_t start, end;

    start = clock();
    unordered_map<int,int> ans = g.solution(graph); // calls the function to initiate the algorithm
    end = clock();

    // calculate the time it took to run algorithm
    double time = (double) (end-start) / CLOCKS_PER_SEC * 1000.0;
    cout << "My Solution took: " << time << "ms" << endl;
    
    // print out the clique that was found
    cout << "Clique of size: "<<ans.size() << endl;
    for (auto x : ans) cout << x.first << " ";
    cout << endl;

    return 0;
}