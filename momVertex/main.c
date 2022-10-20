#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 50010

#define DEBUG

int i, size, idx;

/* 
	Description: graph structure
	head[N] = an array that stores all vertices
	edge_[N] = an array that stores an edge of current vertex, it stores the index of next_vertex
	next_vertex_[N] = an array that stores
	income_edge[N] = an array that stores the nunmber of income edge of an vertex, to check if mother vertex existance
*/
typedef struct {
	int head[N], edge_[N], next_vertex_[N];
	int income_edge[N];	
} Graph;


typedef struct {
	int value;
	Node* next_node;
} Node;

typedef struct {
	Node* curr_node;
	Node* next_node;
} edge;

/* 
	Description: graph construction
	Input: Graph* g = the pointer to the graph
		   int curr_v = the location of current vertex in the array
		   int next_v = the location of next vertex in the array that connect with
		   				current array with an edge
	Output: None
*/
void add(Graph* g, int curr_v, int next_v) {
	g->edge_[idx] = next_v;
	g->next_vertex_[idx] = g->head[curr_v];
	g->head[curr_v] = idx ++;
}

/* 
	Description: search through the vertex array looking for the vertex			 
	Input: char **vertex_array_dptr = the double pointer of vertex array
		   char* curr_v = the vertex that we are looking for
	Output: if the vertex exist, return the location of that vertex in the array
		    else return -1
*/
int search (char **vertex_array_dptr, char *curr_v) {
	for (i = 0; i < size; i ++) {
		if (strcmp(vertex_array_dptr[i], curr_v) == 0) {
			return i;
		}
	}
	return -1;
}
/*
	Description: run dfs on the given graph
	inputs: Graph* g = the pointer to the graph
			int idx_v = the index of current vertex
			int* visited = the array that stores whether the graph is visited or not
			int* mom_count = the array that judge if this vertex is an mother vertex
			int u = a helper integer for mom_count
*/
void DFSUtil (Graph* g, int idx_v, int *visited, int *mom_count, int u) {
	visited[idx_v] = 1;
	mom_count[u] = idx_v;

	int j;
	for (j = g->head[idx_v]; j != -1; j = g->next_vertex_[j]) {
		int k = g->edge_[j];
		if (!visited[k]) {
			DFSUtil(g, k, visited, mom_count, u + 1);
		}
	}
}

/* 
	Description: a helper function for SCC to generate a transpose of a graph
	Input: Graph* g = the pointer to the graph
	Output: tans_g = the transposed graph

*/
Graph* get_transpose(Graph *g) {
	Graph *trans_g = malloc(sizeof(Graph));
	memset(trans_g->head, -1, sizeof(trans_g->head));
	
	for (i = 0; i < size; i ++) {
		int j;
		for (j = g->head[i]; j != -1; j = g->next_vertex_[j]) {
			int k = g->edge_[j];
			add(trans_g, k, i);
		}
	}
	return trans_g;
}

/* 
	Description: a helper function for SCC to find the order of vertex traverse
	Input: Graph* g = the pointer to the graph
		   int t = index of current vertex in the vertex array
		   int *visited = the pointer of visited array
		   int *stack = the pointer of a stack that stores the order of traverse
	Output: tans_g = the transposed graph

*/
int tt = 0;										// a stack counter
void fill_order (Graph* g, int t, int *visited, int *stack) {
	visited[t] = 1;								// mark this vertex as visited
	
	for (i = g->head[t]; i != -1; i = g->next_vertex_[i]) {
		int j = g->edge_[i];
		if (!visited[j]) {
			fill_order(g, j, visited, stack);
		}	
	}
	stack[tt ++] = t;
}

/* 
	Description: The main logic for finding mother vertex, use the idea of strong
				 connect components
	Input: Graph* g = the pointer to the graph
	Output: int ans = number of mother vertices

*/
int find_SCCs(Graph* g) {
	int stack[N];
	int visited[N];
	
	memset(visited, 0, sizeof (visited));           // initiate the visited array to 0
	for (i = 0; i < size; i ++) {
		if (!visited[i]) {
			fill_order(g, i, visited, stack);
		}
	}
	
	Graph *trans_g = get_transpose(g);
	memset(visited, 0, sizeof (visited));
	
	int count[N];									// count array stores the number of mother verices
	memset(count, -1, sizeof (count));
	while (tt != 0) {
		int t = stack[-- tt];
		if (!visited[t]) {	
			DFSUtil(trans_g, t, visited, count, 0);
#ifdef DEBUG
			printf("\n");
#endif
			break;
		}
	}
	
	int ans = 0;
	for (i = 0; count[i] != -1; i ++) ans += 1;
	free(trans_g);
	return ans;
}

int main() {
	
#ifdef DEBUG
	freopen("node.txt", "r", stdin);							// open the vertex file
#else
	freopen("118379821279745746467.followers", "r", stdin);
#endif

	char *temp_str;												// use temp string to store each node
	char *Vertex_array[N];										// add all temp strings to the vertex array
	temp_str = malloc(30 * sizeof (char));
	for (size = 0; scanf("%s", temp_str) != EOF; size++ ) {
		Vertex_array[size] = malloc(30 * sizeof (char));
		strcpy(Vertex_array[size], temp_str);
	}
	free(temp_str);

#ifdef DEBUG	
	printf("============== vertex =================\n");
	for (i = 0; i < size; i ++) printf("%s ", Vertex_array[i]);
	printf("\n============== edges ==================\n");
#endif

#ifdef DEBUG	
	freopen("edges.txt", "r", stdin);							// open the edge file
#else
	freopen("118379821279745746467.edges", "r", stdin);
#endif
	Graph* g = malloc(sizeof (Graph));
	memset(g->head, -1, sizeof (g->head));
	char *temp_out = malloc(30 * sizeof (char));					// define temp_out and temp_in for an edge
	char *temp_in = malloc(30 * sizeof (char));
	for (i = 0; scanf("%s %s", temp_out, temp_in) != EOF; i ++) {
		int idx_curr = search(Vertex_array, temp_out);				
		int idx_next = search(Vertex_array, temp_in);
		if (idx_curr == -1 || idx_next == -1) {
			exit(1);	
		}
#ifdef DEBUG
		printf("%d %d\n", idx_curr, idx_next);
#endif
		add(g, idx_curr, idx_next);								// create the graph
		g->income_edge[idx_next] ++;							// add a income edge count to Next_vertex
	}
	free(temp_out);
	free(temp_in);
	
	int res = 0;
	for (i = 0; i < size; i ++) {
		if (g->income_edge[i] == 0) {
			res += 1;
		}
	}
	if (res > 1) printf("-1\n");								// if any vertex doesn't have income edge, we cannot reach it
	else {														// thus return -1
		int ans = all_DFS(g);
		printf("\n=============== ans from smart ===================\n");
		printf("%d\n", ans);

	}
	free(g);
}
