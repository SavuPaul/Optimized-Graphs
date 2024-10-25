/* SAVU PAUL CRISTIAN */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "heap.h"

int main(int argc, char *argv[]) {
    FILE *fin, *fout;
    // opens the files for reading / writing
    fin = fopen("tema3.in", "r");
    fout = fopen("tema3.out", "w");

    // task 2
    if (strcmp(argv[1], "1") == 0) {

        // reads N and M;
        int N, M;
        fscanf(fin, "%d", &N);
        fscanf(fin, "%d", &M);

        TGraphL G;
        alloc_graph(&G, N);

        // name1 and name2 are used to read the 2 locations
        char name1[100][50], name2[100][50];
        // name_mat is a char matrix used to store all locations
        char name_mat[N][50];
        int cost = 0;
        int name_idx = 0, ok = 1, row = 0;

        // establishes the connections between nodes
        for (int i = 0; i < M; i++) {
            // reads one row at a time
            fscanf(fin, "%s", name1[row]);
            fscanf(fin, "%s", name2[row]);
            fscanf(fin, "%d", &cost);
            /*  
                checks if either of the names already exists
                and adds them if they do not exist (it uses
                a char matrix which memorises all locations)
            */
            ok = 1;
            
            // checks if name1 already exists
            for (int j = 0; j < name_idx; j++) {
                if (strcmp(name_mat[j], name1[row]) == 0) {
                    ok = 0;
                    break;
                }
            }

            // if not, it's added to name_mat matrix
            if (ok == 1) {
                strcpy(name_mat[name_idx], name1[row]);
                name_idx++;
            }

            ok = 1;

            // checks if name2 already exists
            for (int j = 0; j < name_idx; j++) {
                if (strcmp(name_mat[j], name2[row]) == 0) {
                    ok = 0;
                    break;
                }
            }

            // if not, it's added to name_mat matrix
            if (ok == 1) {
                strcpy(name_mat[name_idx], name2[row]);
                name_idx++;
            }

            // inserts the edge between the 2 locations
            insert_edge_list(&G, cost, name1, name2,
                            name_mat, name_idx, row, argv);
            row++;
        }

        // calculates the number of connected components
        int components = sepparations(&G);
        fprintf(fout, "%d\n", components);

        // applies Prim's algorithm on each connected component
        int *min_costs = malloc(components * sizeof(int));
        Prim(G, components, min_costs);

        // sorts the costs vector
        int aux;
        for (int i = 0; i < components - 1; i++) {
            for (int j = i + 1; j < components; j++) {
                if (min_costs[i] > min_costs[j]) {
                    aux = min_costs[i];
                    min_costs[i] = min_costs[j];
                    min_costs[j] = aux;
                }
            }
        }

        // prints the vector to the file
        for (int i = 0; i < components; i++) {
            fprintf(fout, "%d\n", min_costs[i]);
        }

        // deallocates memory
        free(min_costs);
        free_graph(&G);
        fclose(fin);
        fclose(fout);
        return 0;
    }

    // task 2
    if (strcmp(argv[1], "2") == 0) {
        // opens the files for reading / writing
        fin = fopen("tema3.in", "r");
        fout = fopen("tema3.out", "w");

        // reads N and M
        int N, M;
        fscanf(fin, "%d", &N);
        fscanf(fin, "%d", &M);
    
        // allocates memory for the Graph
        TGraphL G;
        alloc_graph(&G, N);

        int row = 0, ok = 1, name_idx = 0, cost;
        char name_mat[N][50];
        char name1[100][50], name2[100][50];

        // establishes the connections between nodes
        for (int i = 0; i < M; i++) {
            // reads one row at a time
            fscanf(fin, "%s", name1[row]);
            fscanf(fin, "%s", name2[row]);
            fscanf(fin, "%d", &cost);

            /*  
                checks if either of the names already exists
                and adds them if they do not exist (it uses
                a char matrix which memorises all locations)
            */
            ok = 1;
            
            for (int j = 0; j < name_idx; j++) {
                if (strcmp(name_mat[j], name1[row]) == 0) {
                    ok = 0;
                    break;
                }
            }

            if (ok == 1) {
                strcpy(name_mat[name_idx], name1[row]);
                name_idx++;
            }

            ok = 1;

            for (int j = 0; j < name_idx; j++) {
                if (strcmp(name_mat[j], name2[row]) == 0) {
                    ok = 0;
                    break;
                }
            }

            if (ok == 1) {
                strcpy(name_mat[name_idx], name2[row]);
                name_idx++;
            }

            insert_edge_list(&G, cost, name1, name2, name_mat, name_idx, row, argv);
            row++;
        }

        // setting the depths of each node
        char name[20];
        int depth = 0, idx = 0, idx_corabie = 0, idx_insula = 0;
        for (int i = 0; i < N; i++) {
            fscanf(fin, "%s", name);
            fscanf(fin, "%d", &depth);
            for (int j = 0; j < name_idx; j++) {
                // stores the idx of current node
                if (strcmp(name_mat[j], name) == 0) {
                    idx = j;
                }
                // stores the idx for destination (corabie)
                if (strcmp(name_mat[j], "Corabie") == 0) {
                    idx_corabie = j;
                }
                // stores the idx for starting point (insula)
                if (strcmp(name_mat[j], "Insula") == 0) {
                    idx_insula = j;
                }
            }

            /* 
                "name" is searched through all adj lists
                and its depth is updated 
            */
            for (int j = 0; j < N; j++) {
                ATNode iter = G.adl[j];
                while (iter != NULL) {
                    if (iter->v == idx) {
                        iter->depth = depth;
                    }
                    iter = iter->next;
                }
            }
        }

        int weight = 0, min_depth = 1;
        // reads the weight
        fscanf(fin, "%d", &weight);

        // checks if a path exists between the island and the boat
        if (exists_path(&G, idx_insula, idx_corabie) == 0) {
            fprintf(fout, "Echipajul nu poate transporta comoara inapoi la corabie");
            free_graph(&G);
            fclose(fin);
            fclose(fout);
            return 0;
        }
        
        // checks if a path exists between the boat and the island
        if (exists_path(&G, idx_corabie, idx_insula) == 1) {
            dijkstra(G, idx_insula, idx_corabie, name_mat, &min_depth, fout);
        } else {
            fprintf(fout, "Echipajul nu poate ajunge la insula\n");
            free_graph(&G);
            fclose(fin);
            fclose(fout);
            return 0;
        }
        fprintf(fout, "%d\n", min_depth);
        fprintf(fout, "%d\n", weight / min_depth);
        free_graph(&G);
        fclose(fin);
        fclose(fout);
        return 0;
    }
}