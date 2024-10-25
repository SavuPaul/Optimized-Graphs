/* SAVU PAUL CRISTIAN */

#include "heap.h"
#include <limits.h>

typedef struct node
{
    int depth;
    int c;
	int v;
	struct node *next;
} TNode, *ATNode;

typedef struct {
    int nn;
    ATNode *adl;
} TGraphL;

// allocates memory for the graph
void alloc_graph(TGraphL *G, int n)
{
    int i;
    G->nn = n;
	G->adl = (ATNode*) malloc((n + 1) * sizeof(ATNode));
	for (i = 0; i < n; i++)
		G->adl[i] = NULL;
}

// returns the row on which the location is in the matrix
int getRow(char (*mat)[50], char (*name)[50], int name_idx, int row) {
    int idx = 0;
    for (idx = 0; idx < name_idx; idx++) {
        if (strcmp(mat[idx], name[row]) == 0) {
            return idx;
        }
    }
}

// inserts the edge between 2 locations
void insert_edge_list(TGraphL *G, int c, char (*name1)[50], char (*name2)[50], char (*mat)[50], int name_idx, int row, char *argv[]) {
    int row1, row2;
    row1 = getRow(mat, name1, name_idx, row); //1
    row2 = getRow(mat, name2, name_idx, row); //2

    // for task 1 (graf neorientat) - edge is both ways
    if (strcmp(argv[1], "1") == 0) {
        TNode *node1 = malloc(sizeof(TNode));
        node1->c = c;
        node1->v = row2;
        node1->next = G->adl[row1];
        G->adl[row1] = node1;
        TNode *node2 = malloc(sizeof(TNode));
        node2->c = c;
        node2->v = row1;
        node2->next = G->adl[row2];
        G->adl[row2] = node2;
    }

    // for task 2 (graf orientat) - edge is one way
    if (strcmp(argv[1], "2") == 0) {
        TNode *node1 = malloc(sizeof(TNode));
        node1->c = c;
        node1->v = row2;
        node1->next = G->adl[row1];
        G->adl[row1] = node1;
    }
}

/* 
    depth search to determine if there's a path between 2 nodes:
    returns 1 if a path exists, 0 otherwise
*/
int exists_path_helper(TGraphL *G, int start, int destination, int *visited) {
    visited[start] = 1;
    int i = 0;
    if (start == destination) {
        return 1;
    }
    ATNode iter = G->adl[start];
    while (iter != NULL) {
        if (visited[iter->v] == 0) {
            if (exists_path_helper(G, iter->v, destination, visited) == 1) {
                return 1;
            }
        }
        iter = iter->next;
    }
    return 0;
}

int exists_path(TGraphL *G, int start, int destionation) {
    int *visited = malloc(G->nn * sizeof(int));
    for (int i = 0; i < G->nn; i++) {
        visited[i] = 0;
    }
    int path = exists_path_helper(G, start, destionation, visited);
    free(visited);
    return path;
}

// depth search
void dfs(TGraphL *G ,int node, int *visited) {
    visited[node] = 1;
    int i = 0;
    ATNode iter = G->adl[node];
    while (iter != NULL) {
        if (visited[iter->v] == 0) {
            dfs(G, iter->v, visited);
        }
        iter = iter->next;
    }
}

// returns the number of connected components
int sepparations(TGraphL *G) {
    int sepparations = 0;
    int *visited = malloc(G->nn * sizeof(int));
    int i = 0;
    for (i = 0; i < G->nn; i++) {
        visited[i] = 0;
    }

    for (i = 0; i < G->nn; i++) {
        if (visited[i] == 0) {
            dfs(G, i, visited);
            sepparations++;
        }
    }

    free(visited);
    return sepparations;
    return 0;
}

// Prim's algorithm for each connected component
void Prim(TGraphL G, int components, int *min_costs)
{
    MinHeap* heap = newQueue(G.nn);
	int visited[G.nn];
	int d[G.nn];
	int P[G.nn];

    // index for the vector of total minimum costs
    int idx = -1;

    // initiliases the min_costs array with 0
    for (int i = 0; i < components; i++) {
        min_costs[i] = 0;
    }

	for (int i = 0; i < G.nn; i++) {
		d[i] = INT_MAX;
		visited[i] = 0;
		P[i] = -1;
    }

    // start position
	int s = 0;
    // number is the counter for how many components have been searched
    int number = 0;
    // how many components have been searched so far
    int done = 0;

    do {
        number = 0;
        d[s] = 0;
        insertInHeap(heap, s, d[s]);

        while (!isEmpty(heap)) {
            MinHeapNode* u = removeMin(heap);
            int index = u->v;
            visited[index] = 1;

            ATNode iter = G.adl[index];

            while (iter != NULL) {
                int v = iter->v;
                int w = iter->c;

                if (visited[v] == 0 && w < d[v]) {
                    d[v] = w;
                    P[v] = index;
                    insertInHeap(heap, v, d[v]);
                }

                iter = iter->next;
            }
        }
        // one more component is done
        done++;
        int copy_done = done;

        // moves to the next component
        for (int i = 1; i < G.nn; i++) {
            if (P[i] == -1) {
                number++;
                copy_done--;
            }
            if (copy_done == 0) {
                s = i;
            }
        }
    } while (number > components);

    // calculates the total cost of each spanning tree
    ATNode iter;
    for (int i = 0; i < G.nn; i++) {
        if (P[i] == -1) {
            idx++;
        }
        for (iter = G.adl[i]; iter != NULL; iter = iter->next) {
            if (iter->v == P[i]) {
                min_costs[idx] = min_costs[idx] + iter->c;
            }
        }
    }
    freeQueue(heap);
}

// also finds the minimum depth in the graph
void dijkstra(TGraphL G, int s, int idx_corabie, char (*name)[50], int *min_depth, FILE* fout)
{
	MinHeap* h = newQueue(G.nn);
	int visited[G.nn];
    int d[G.nn];
	float score[G.nn];
	int P[G.nn];

    for (int i = 0; i < G.nn; i++) {
        d[i] = INT_MAX;
		score[i] = INT_MAX;
		visited[i] = 0;
		P[i] = -1;
    }

    d[s] = 0;
	score[s] = 0;
	visited[s] = 1;
	insertInHeap(h, s, 0);

	while (!isEmpty(h)) {
		MinHeapNode* u =  removeMin(h);
		visited[u->v] = 1;
		ATNode iter = G.adl[u->v];

		while (iter != NULL) {
			if (visited[iter->v] == 0 && score[iter->v] > score[u->v] + (float)iter->c / iter->depth) {
                d[iter->v] = d[u->v] + iter->c;
				score[iter->v] = score[u->v] + (float)iter->c / iter->depth;
				P[iter->v] = u->v;
                insertInHeap(h, iter->v, score[iter->v]);
			}
			iter = iter->next;
		}
	}
    // shortest path from destionation to start
    int crt = idx_corabie, size = 0;
    int path[G.nn];

    /* 
        initialises min_depth with the depth
        before the destionation 
    */
    for (int i = 0; i < G.nn; i++) {
        ATNode iter = G.adl[i];
        while (iter != NULL) {
            if (iter->v == P[idx_corabie]) {
                *min_depth = iter->depth;
                iter = NULL;
                i = G.nn;
                break;
            }
            iter = iter->next;
        }
    }

    // finds the minimum depth and the path from the boat to the island
    while (crt != -1) {
        for (int i = 0; i < G.nn; i++) {
            ATNode iter = G.adl[i];
            while (iter != NULL) {
                if (iter->v == crt) {
                    if (iter->depth < *min_depth && iter->depth != 1) {
                        *min_depth = iter->depth;
                        iter = NULL;
                        i = G.nn;
                        break;
                    }
                }
                iter = iter->next;
            }
        }
        path[size] = crt;
        size++;
        crt = P[crt];
    }

    // prints the path backwards (from island to the boat)
    for (int i = size - 1; i >= 0; i--) {
        fprintf(fout, "%s ", name[path[i]]);
    }
    fprintf(fout, "\n");
    fprintf(fout, "%d\n", d[idx_corabie]);
    freeQueue(h);
}

// deallocates the memory of the graph
void free_graph(TGraphL *G)
{
    int i;
    ATNode iter;
    for (i = 1; i < G->nn; i++){
		iter = G->adl[i];
		while (iter != NULL){
			ATNode aux = iter;
			iter = iter->next;
			free(aux);
		}
		G->adl[i] = NULL;
	}
	G->nn = 0;
}