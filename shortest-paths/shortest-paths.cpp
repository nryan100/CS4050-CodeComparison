#include <iostream>
#include <fstream>
#include <limits>
#include <time.h>
#include <string>
#include <set>
#include <queue>
#include <iomanip>
// Uncomment below to display matrices
// #define PRINT_MATRICES

const int INF = std::numeric_limits<int>::max() / 2;
int num_verts = 0; // Global variable for the total number of vertexes
clock_t clock_start, clock_end; // Global variables for calculating runtime

int** readAdjMatrix(std::string filename);
void printAdjMatrix(int **mtx);
float getDuration(std::string input);
int** floyd(int** W);
int* dijkstra(int** W, int sv);


int main() {
    int** W = readAdjMatrix("graph_100verts.txt");
    std::cout << "n_verts = " << num_verts << "\n";
    
    // Run floyds algorithm
    clock_start = clock();
    int** res_floyds = floyd(W);
    
    float floyds_duration = (float)(clock() - clock_start) / CLOCKS_PER_SEC;

    // getDuration("Done! Floyds algorithm took ");  

#ifdef PRINT_MATRICES
    printAdjMatrix(res_floyds);
#endif
    
    // Run Dijkstras algorithm with a single starting vertex
    int start_vert = 2;
    int* cost = dijkstra(W, start_vert);
    std::cout << "Dijkstras for starting vert " << start_vert << " = [ ";
    for (int i = 0; i < num_verts; i++) {
        std::cout << cost[i] << " ";
    }
    std::cout << "] \n\n";

    clock_start = clock();
    // Run Dijkstras algorithm for all starting points
    int* res_dijkstra[num_verts];
    for (int sv = 0; sv < num_verts; sv++) {
        res_dijkstra[sv] = dijkstra(W, sv);
    }
    
    float dijkstras_duration = (float)(clock() - clock_start) / CLOCKS_PER_SEC;

    // getDuration("Done! Dijkstras algorithm for all starting points took ");
#ifdef PRINT_MATRICES
    printAdjMatrix(res_dijkstra);
#endif
    
    // Verify that both results are equal
    bool equal = true;
    for (int i = 0; i < num_verts; i++) {
        for (int j = 0; j < num_verts; j++) {
            if (res_dijkstra[i][j] != res_floyds[i][j]) {
                equal = false;
                break;
            }
        }
    }
    std::string res = (equal) ? "equal" : "different";
    std::cout << "Floyds and Dijkstras algorithms give " << res << " results!\n";

    return 0;
}

/**
 * Reads the adjacency matrix from a formatted file and stores its values in a weight matrix W.
 * The weight matrix is used in both floyds and dijkstras algorithms. 
 * 
 * @param filename
 * @return Weight matrix W
*/
int** readAdjMatrix(std::string filename) { 
    int** W = 0;
    std::fstream file;
    file.open(filename, std::ios::in);
    if (file.is_open()) {
        std::string line;
        std::getline(file, line);
        num_verts = std::stoi(line);
        W = new int* [num_verts];
        for (int i = 0; i < num_verts; i++) {
            W[i] = new int[num_verts];
            for (int j = 0; j < num_verts; j++) {
                if (i == j) {
                    W[i][j] = 0;
                } else {
                    W[i][j] = INF;
                }
            }
        }
        while (std::getline(file, line)) {
            int current = std::stoi(line.substr(0, line.find(" ")));
            line.erase(0, line.find(" ") + 1);
            int neighbor, weight = 0;
            while (line.find(" ") != std::string::npos) {
                neighbor = std::stoi(line.substr(0, line.find(" ")));
                line.erase(0, line.find(" ") + 1);
                weight = std::stoi(line.substr(0, line.find(" ")));
                line.erase(0, line.find(" ") + 1);
                W[current][neighbor] = weight;
            }
        }
        file.close();
    }
    return W;
}

/**
 * Prints the adjacency matrix
 * @param mtx: matrix to print
*/
void printAdjMatrix(int **mtx) {

    for (int i = 0; i < num_verts; i++) {
        for (int j = 0; j < num_verts; j++) {
            printf("%s%2i", " ", mtx[i][j]);
        }
        printf("%s", "\n");
    }
    std::cout << "\n";
}

/**
 * Calculates the duration between clock_start & clock_end
 * @param input: Tells the user what algorithm the duration belongs to
 */
float getDuration(std::string input) {
    float duration = (float)(clock_end - clock_start) / CLOCKS_PER_SEC;
    std::cout << std::fixed;
    std::cout.precision(5);
    std::cout << input << duration << " seconds.\n" ;
    return duration;
}

/**
 * Performs floyds algorithm.
 * @param W: weight matrix
 * @return Computed weight matrix
 */

int** floyd(int** W) {
    int** w = new int* [num_verts];
    for (int i = 0; i < num_verts; i++) {
        w[i] = new int[num_verts];
        for (int j = 0; j < num_verts; j++) {
            w[i][j] = W[i][j];
        }
    }
    for (int i = 0; i < num_verts; i++) {
        for (int j = 0; j < num_verts; j++) {
            for (int k = 0; k < num_verts; k++) {
                w[j][k] = std::min(w[j][k], w[j][i] + w[i][k]);
            }
        }
    }
    return w;
}

/**
 * Vertex structure to mimic python tuples for the priority queue 
 * implementation in Dijkstra's algorithm
 */
struct Vertex {
    int weight, vertex;

    Vertex(int w, int v) : weight(w), vertex(v) {}

    bool operator < (const struct Vertex& other) const {
        return weight > other.weight;
    }
};

/**
 * Performs Dijkstras algorithm by implementing a priority queue
 * @param W: Weight matrix
 * @param sv: Starting vertex
 * @return Cost matrix for the given starting vertex (sv)
 */
int* dijkstra(int** W, int sv) {
    int* cost = new int[num_verts];
    for (int i = 0; i < num_verts; i++) {
        cost[i] = INF;
    }
    cost[sv] = 0;
    std::set<int> visited;
    std::priority_queue<Vertex> pq;
    pq.push(Vertex(0, sv));
    while (!pq.empty()) {
        int weight = pq.top().weight;
        int current = pq.top().vertex;
        pq.pop();
        visited.insert(current);
        for (int adjacent = 0; adjacent < num_verts; adjacent++) {
            if (W[current][adjacent] != 0 && W[current][adjacent] != INF) {
                int distance = W[current][adjacent];
                if (visited.find(adjacent) == visited.end()) {
                    int oldCost = cost[adjacent];
                    int newCost = cost[current] + distance;
                    if (newCost < oldCost) {
                        pq.push(Vertex(newCost, adjacent));
                        cost[adjacent] = newCost;
                    }
                } 
            } 
        }
    }
    return cost;
}