#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <fstream>
#include <sstream>

using namespace std;

/*
 *
   IMPORTANT: this file solves the maximum clique problem in the brute force way. *DONT RUN WITH GRAPHS BIGGER THAN 25 VERTICES*
              The steps are listed below:

   1. Enumerate all possible subsets of vertices.

   2. For each subset, check if it forms a clique by verifying if every pair of vertices within the subset is connected by an edge.
   
   3.Keep track of the largest clique found.
 * 
*/

class Brute{

   private:

   vector<vector<int>> subsets;

   // Enumerating subsets via backtracking recursively
   // this basically creates a tree of decisions of whether or not to add an item to the set 
   void enumerate(vector<int> &g, vector<int> &set, int index, int end) {

      // base case
      if (index == end){
         subsets.push_back(set);
         return;
      }

      // option 1 which is choosing to add item
      set.push_back(index);
      enumerate(g, set, index+1, end);

      // option 2 which is to exclude the item
      set.pop_back(); // backtrack
      enumerate(g, set, index+1, end);
      
   }

   // this is a helper function called in the findMax function to check if a subset is a Clique
   bool isClique(vector<int> &set, vector<vector<int> > &g){

      for (int i = 0; i < set.size(); i++){
         int vertice = set[i];
         for (int j = 0; j < set.size(); j++){

            // exclude checking edge to itself and check edge to every vertice in set
            if (i != j && g[vertice][set[j]] == 0){
               return false; // return false if edge is 0, because 0 edges mean non existing edge
            }

         }
      }

      return true;

   }

   // this is the function called to find the max Clique given all the possible subsets from the graph
   vector<int> findMax(vector<vector<int> > &g){
      vector<int> maxClique;
      
      // iterate through each subset
      for (int i = 0; i < subsets.size(); i++){

         // now check if it is a clique using isClique and compare size with current maxClique size
         if (isClique(subsets[i], g) && subsets[i].size() > maxClique.size())
            maxClique = subsets[i]; // set new maxClique if one is found
      }

      return maxClique;
   }

   public:

   vector<int> brute(vector<vector<int> > & g) {

      // enumerate all subsets in the graph, only need first row
      vector<int>set;
      enumerate(g[0], set, 0, g.size());

      /*// checking enumerate function, will delete later
      cout << "{ ";
      for (int i = 0; i < subsets.size(); i++){
         cout << "{";
         for (int j = 0; j < subsets[i].size(); j++){
            cout << subsets[i][j] << ",";
         }
         cout << "},";
      }
      cout << "}" << endl;*/

      return findMax(g);
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
   0's represent no edge, 1's represent an edge
 * 
*/
int main(int argc, char** argv){

   // ensure correct usage of program
   if (argc != 2){
      cout << "Usage: ./a.out <samplefile.adjmat>" << endl;
      return 0;
   }

   // initialize the graph to be tested
   vector<vector<int> > graph = openFile(argv[1]);

   // if graph is empty then that means the file couldn't be opened
   if (graph.size() == 0) return 0;

   // initalize Brute class and clock variables
   Brute b;
   clock_t start, end; 

   start = clock(); // start the time
   vector<int> ans = b.brute(graph); // calls the function to initiate the algorithm
   end = clock(); // end the time

   // calculate the time it took to run algorithm
   double time = (double) (end-start) / CLOCKS_PER_SEC * 1000.0;
   cout << "Brute Force took: " << time << endl;

   // print out the clique that brute force found
   cout << ans.size() << endl;
   for (auto x : ans) cout << x << " ";
   cout << endl;

}