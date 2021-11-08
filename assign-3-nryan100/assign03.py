"""
Assign 03 - Nicholas Ryan

Directions:
    * Complete the graph algorithm functions given below. Note that it may be
      helpful to define auxiliary/helper functions that are called from the
      functions below.  Refer to the README.md file for additional info.

    * NOTE: As with other assignments, please feel free to share ideas with
      others and to reference sources from textbooks or online. However, do your
      best to attain a reasonable grasp of the algorithm that you are
      implementing as there will very likely be questions related to it on
      quizzes/exams.

    * NOTE: Remember to add a docstring for each function, and that a reasonable
      coding style is followed (e.g. blank lines between functions).
      Your program will not pass the tests if this is not done!
"""
# could be useful for Dijkstra
from queue import PriorityQueue
# for timing checks
import time

# use a very large number as placeholder for infinity
import sys
INF = sys.maxsize


def adjMatFromFile(filename):
    f = open(filename, "r")
    n_verts = int(f.readline())
    adjmat = [[INF] * n_verts for i in range(n_verts)]
    for i in range(n_verts):
        adjmat[i][i] = 0
    for line in f:
        int_list = [int(i) for i in line.split()]
        vert = int_list.pop(0)
        assert len(int_list) % 2 == 0
        n_neighbors = len(int_list) // 2
        neighbors = [int_list[n] for n in range(0, len(int_list), 2)]
        distances = [int_list[d] for d in range(1, len(int_list), 2)]
        for i in range(n_neighbors):
            adjmat[vert][neighbors[i]] = distances[i]
    f.close()
    return adjmat


def printAdjMat(mat, width=3):
    """ Print an adj/weight matrix padded with spaces and with vertex names. """
    res_str = '     ' + ' '.join([str(v).rjust(width, ' ') for v in range(len(mat))]) + '\n'
    res_str += '    ' + '-' * ((width + 1) * len(mat)) + '\n'
    for i, row in enumerate(mat):
        row_str = [str(elem).rjust(width, ' ') for elem in row]
        res_str += ' ' + str(i).rjust(2, ' ') + ' |' + ' '.join(row_str) + "\n"
    print(res_str)


def floyd(W):
    length = len(W)
    w = W.copy()
    for i in range(length):
        for j in range(length):
            for k in range(length):
                w[j][k] = min(w[j][k], w[j][i] + w[i][k])
    return w


def dijkstra(W, sv):
    length = len(W)
    visited = set()
    pq = PriorityQueue()
    cost = [INF] * length
    cost[sv] = 0
    pq.put((0, sv))

    while not pq.empty():
        (weight, current) = pq.get()
        visited.add(current)
        for adjacent in range(length):

            if W[current][adjacent] != 0 and W[current][adjacent] != INF:
                dist = W[current][adjacent]

                if adjacent not in visited:
                    old = cost[adjacent]
                    new = cost[current] + dist

                    if new < old:
                        pq.put((new, adjacent))
                        cost[adjacent] = new
    return cost


def assign03_main():
    """ Demonstrate the functions, starting with creating the graph. """
    g = adjMatFromFile("graph_100verts.txt")

    # Run Floyd's algorithm
    start_time = time.time()
    res_floyd = floyd(g)
    elapsed_time_floyd = time.time() - start_time
    # printAdjMat(res_floyd, width=4)

    # Run Dijkstra's for a single starting vertex, 2
    start_vert = 2
    res_dijkstra_single = dijkstra(g, start_vert)
    # print(f" dijkstra (for starting vert {start_vert})= : {res_dijkstra_single}")

    # Check that the two produce same results by comparing result from
    # dijkstra with the corresponding row from floyd's output matrix
    assert res_floyd[start_vert] == res_dijkstra_single

    # Run Dijkstra's overall starting points (note this is not the intended way
    # to utilize this algorithm, however we are using it as point of comparion).
    res_dijkstra = [[None] * len(g) for i in range(len(g))]
    start_time = time.time()
    for sv in range(len(g)):
        res_dijkstra[sv] = dijkstra(g, sv)
    elapsed_time_dijkstra = time.time() - start_time
    
    print(str(elapsed_time_floyd) + "," + str(elapsed_time_dijkstra))

    # Compare times, Dijkstra's should be slower (for non-trivial sized graphs)
    # print(f"  Dijkstra's elapsed time (for all starts): {elapsed_time_dijkstra:.2f}")
    # print(f"  Floyd's elapsed time: {elapsed_time_floyd:.2f}")

    # # Double check again that the results are the same
    assert res_floyd == res_dijkstra


# Check if the program is being run directly (i.e. not being imported)
if __name__ == '__main__':
    assign03_main()
