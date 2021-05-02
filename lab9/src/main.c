#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <limits.h>


enum WorkWithOstov { INT_MAXX = -3, MAKED = -2, INFINITY = -1, WORKS = 0, OK = 1, OVERFLOW = 2, NO_PATH = 3 };


typedef struct Edge {
	int from;
	unsigned int length;
	char type;
}Edge;


typedef struct Node {
	unsigned int value;
}Node;


typedef struct Graph {
	Node* matrix;
	int n;
	int m;
}Graph;


char Deikstra(int from, int in, const Graph* graph, Edge* edges_ans) {
	int count = 1;
	int count_int_max = 0;
	edges_ans[from].length = 0;
	edges_ans[from].from = from;
	edges_ans[from].type = MAKED;
	Edge min = {from, 0, INFINITY};

	while (count < graph->n) {
		char used = 0;
		for (int j = 0; j < graph->n; j++) {
			if (graph->matrix[min.from * graph->n + j].value > 0 && edges_ans[j].type > MAKED) {
				if (graph->matrix[min.from * graph->n + j].value + edges_ans[min.from].length <= edges_ans[j].length || edges_ans[j].type == INFINITY) {
					edges_ans[j].length = graph->matrix[min.from * graph->n + j].value + edges_ans[min.from].length;
					if (edges_ans[j].length > INT_MAX) {
						edges_ans[j].type = OVERFLOW;
					}
					else {
						edges_ans[j].type = WORKS;			
					}
					edges_ans[j].from = min.from;
				}
				if (graph->matrix[min.from * graph->n + j].value + edges_ans[min.from].length > INT_MAX) {
					count_int_max++;
				}
			}
		}

		min.length = 0;
		min.type = INFINITY;
		min.from = from;
		for (int i = 0; i < graph->n; i++) {
			if (edges_ans[i].length > 0 && (edges_ans[i].length < min.length || min.type == INFINITY) && edges_ans[i].type != MAKED) {
				if (edges_ans[i].length <= edges_ans[min.from].length || min.from == from) {
					min.from = i;
					min.type = WORKS;
					min.length = edges_ans[i].length;
				}
				used = 1;
			}
		}

		if (min.length > INT_MAX) {
			edges_ans[min.from].length = 2147483648; // INT_MAX + 1
			edges_ans[min.from].type = INT_MAXX;
		}
		else {
			edges_ans[min.from].length = min.length;
			edges_ans[min.from].type = MAKED;
		}

		if (used == 0) {
			if (from != in) {
				return NO_PATH;
			}
			else {
				return OK;
			}
		}
		count++;
	}

	if (count_int_max > 1 && (edges_ans[in].type == OVERFLOW || edges_ans[in].type == INT_MAXX) )
		return OVERFLOW;

	int point = in;
	while (edges_ans[point].from != from) {
		if (count < 0) {
			return NO_PATH;
		}
		point = edges_ans[point].from;
		count--;
	}

	return OK;
}


bool read_edges(FILE* test_file, Graph* graph) {
	int line;
	int column;
	unsigned int weight;
	for (int i = 0; i < graph->m; i++) {
		if (fscanf(test_file, "%i%i%u", &line, &column, &weight) == EOF) {
			printf("bad number of lines");
			return false;
		}

		if (line < 1 || column < 1 || line > graph->n || column > graph->n) {
			printf("bad vertex");
			return false;
		}

		if (weight > INT_MAX) {
			printf("bad length");
			return false;
		}

		line -= 1;
		column -= 1;

		graph->matrix[line * graph->n + column].value = weight;
		graph->matrix[column * graph->n + line].value = weight;
	}

	return true;
}


int main() {
	FILE* file;
	file = stdin;
	Graph graph;

	if (fscanf(file, "%i", &graph.n) == EOF) {
		printf("bad number of lines");
		fclose(file);
		return 0;
	}
	
	int from;
	int in;
	if (fscanf(file, "%i%i", &from, &in) == EOF) {
		printf("bad number of lines");
		fclose(file);
		return 0;
	}

	if (from < 1 || in < 1 || from > graph.n || in > graph.n) {
		printf("bad vertex");
		fclose(file);
		return 0;
	}

	in--;
	from--;

	if (fscanf(file, "%i", &graph.m) == EOF) {
		printf("bad number of lines");
		fclose(file);
		return 0;
	}

	if (graph.n <= 0 || graph.n > 5000) {
		if (graph.n == 0) {
			printf("oo");
		}
		else {
			printf("bad number of vertices");
		}
		fclose(file);
		return 0;
	}

	if (graph.m < 0 || graph.m > graph.n * (graph.n + 1) / 2) {
		printf("bad number of edges");
		fclose(file);
		return 0;
	}

	graph.matrix = (Node*)calloc(graph.n * graph.n, sizeof(Node));
	if (graph.matrix == NULL) {
		fclose(file);
		return 0;
	}

	Edge* edges_ans = (Edge*)malloc((graph.n) * sizeof(Edge));
	if (edges_ans == NULL) {
		free(graph.matrix);
		fclose(file);
		return 0;
	}
	for (int i = 0; i < graph.n; i++) {
		edges_ans[i].type = INFINITY;
		edges_ans[i].length = 0;
	}

	if ( read_edges(file, &graph) ) {
		int result_type = Deikstra(from, in, &graph, edges_ans);
		
		for (int i = 0; i < graph.n; i++) {
			if (edges_ans[i].type == OVERFLOW || edges_ans[i].type == INT_MAXX) {
				printf("INT_MAX+ ");
			}
			else if (edges_ans[i].type == INFINITY) {
				printf("oo ");
			}
			else {
				printf("%u ", edges_ans[i].length);
			}
		}
		printf("\n");

		if (result_type == NO_PATH) {
			printf("no path");

		}
		else if (result_type == OK) {
			int point = in;
			printf("%i ", in + 1);
			while (edges_ans[point].from != from) {
				printf("%i ", edges_ans[point].from + 1);
				point = edges_ans[point].from;
			}
			printf("%i", from + 1);
		}
		else {
			printf("overflow");
		}
	}
	
	free(graph.matrix);
	free(edges_ans);
	fclose(file);
	return 0;
}
