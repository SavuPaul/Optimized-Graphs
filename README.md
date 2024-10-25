# Graph Processing Program

This program processes a graph of locations and calculates connected components, minimum spanning trees, and optimal paths based on command-line arguments for two tasks.

## Table of Contents
1. [Task 1: Undirected Graph - Connected Components and Minimum Spanning Tree](#task-1-undirected-graph---connected-components-and-minimum-spanning-tree)
2. [Task 2: Directed Graph - Pathfinding and Depth Calculation](#task-2-directed-graph---pathfinding-and-depth-calculation)
3. [Memory Management](#memory-management)

---

### Task 1: Undirected Graph - Connected Components and Minimum Spanning Tree

After reading `N` (number of locations) and `M` (number of edges), memory is allocated for the graph, and the `M` edges are read into the following variables:
- **`name1`**: name of the first location,
- **`name2`**: name of the second location,
- **`cost`**: distance between the locations.

The `name_mat` matrix stores all unique locations, each assigned a unique index. This allows connections between locations to be established by associating them with their respective indices in `name_mat`.

1. **Inserting Edges**: The `insert_edge_list` function updates the adjacency lists for both locations in each edge, as the graph is undirected. The `get_row` function retrieves the row index of a given location in `name_mat`.

2. **Finding Connected Components**: After establishing connections, a depth-first search (DFS) traversal is used to determine the number of connected components in the graph, stored in the variable `components`.

3. **Minimum Spanning Tree (MST)**: Prim’s algorithm is applied to find the MST for each connected component, calculating the minimum cost of each spanning tree. This vector of costs is sorted and displayed once the function completes.

### Task 2: Directed Graph - Pathfinding and Depth Calculation

After reading `N` (number of locations) and `M` (number of edges), memory is allocated for the graph, and the `M` edges are read into the following variables:
- **`name1`**: name of the first location,
- **`name2`**: name of the second location,
- **`cost`**: distance between the locations.

Similarly to Task 1, each location appears only once in the `name_mat` matrix.

1. **Inserting Edges**: In Task 2, the graph is directed, so `insert_edge_list` only modifies the adjacency list of the starting node for each directed edge.

2. **Setting Node Depths**: Depths are set for each node by traversing all adjacency lists and updating each node's `depth` field. The indices of the island and ship are recorded to check for paths between them.

3. **Path Existence Check**: The `exists_path` function, along with `exists_path_helper`, traverses the graph to return `1` if a path exists or `0` otherwise. If no path exists from the island to the ship, an appropriate message is displayed, and memory is freed. 

4. **Bidirectional Path Check**: If a path exists from the island to the ship, the program then checks for a path from the ship back to the island. If no return path exists, a message is displayed, and memory is freed.

5. **Modified Dijkstra’s Algorithm**: If bidirectional paths are found, a modified Dijkstra’s algorithm calculates the optimal path based on the arc’s score, updating distances. The algorithm also finds the minimum depth (greater than 1) along the optimal path. 

6. **Displaying the Path**: The final path is displayed by associating each node’s key with its index in `name_mat`. The program outputs the optimal path distance, minimum depth, and the number of journeys required by the crew, followed by freeing allocated memory.

