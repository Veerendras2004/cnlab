#include <stdio.h>
#include <limits.h>

#define V 5  // Number of vertices in the graph
#define E 8  // Number of edges in the graph

void BellmanFord(int graph[E][3], int src) {
    int dis[V];
    
    for (int i = 0; i < V; i++) 
        dis[i] = INT_MAX;
    
    dis[src] = 0;

    for (int i = 0; i < V - 1; i++) 
        for (int j = 0; j < E; j++) 
            if (dis[graph[j][0]] != INT_MAX && dis[graph[j][0]] + graph[j][2] < dis[graph[j][1]]) 
                dis[graph[j][1]] = dis[graph[j][0]] + graph[j][2];

    for (int i = 0; i < E; i++) 
        if (dis[graph[i][0]] != INT_MAX && dis[graph[i][0]] + graph[i][2] < dis[graph[i][1]]) {
            printf("Graph contains negative weight cycle\n");
            return;
        }

    printf("Vertex Distance from Source\n");
    for (int i = 0; i < V; i++) 
        printf("%d\t\t%d\n", i, dis[i]);
}

int main() {
    int graph[E][3] = {
        { 0, 1, -1 }, { 0, 2, 4 }, { 1, 2, 3 },
        { 1, 3, 2 }, { 1, 4, 2 }, { 3, 2, 5 },
        { 3, 1, 1 }, { 4, 3, -3 }
    };

    BellmanFord(graph, 0); 

    return 0;
}
