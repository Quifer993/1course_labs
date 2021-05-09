#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <limits.h>


enum WorkWithOstov { OK = 0, NO_PATH = 1, INT_MAXX = 2, OVERFLOW = 3, DONE = 4, WORKS = 5, INF = 6 };
enum TypeError { PASS, LINES, VERTEX, LENGTH };


typedef struct Edge {
	int from;
	unsigned int length;
	char type;
}Edge;


typedef struct Graph {
	unsigned int* matrix;
	int vertices;
	int edges;
}Graph;


unsigned int pop_matrix(const Graph* graph, int line, int column) {
	return graph->matrix[line * graph->vertices + column];
}


void put_matrix(Graph* graph, int line, int column, unsigned int weight) {
	graph->matrix[line * graph->vertices + column] = weight;
	graph->matrix[column * graph->vertices + line] = weight;
}


char Deikstra(int from, int in, const Graph* graph, Edge* edges_ans) {
	int count = 1;
	bool is_one_ways = true;
	edges_ans[from].length = 0;
	edges_ans[from].from = from;
	edges_ans[from].type = DONE;
	Edge min = {from, 0, INF};

	int before = from;
	while (count < graph->vertices) {
		char used = 0;
		for (int j = 0; j < graph->vertices; j++) {
			if (edges_ans[j].type <= DONE && before != j && j != min.from && count < graph->edges) {
				is_one_ways = false;
			}
			else if (graph->matrix[min.from * graph->vertices + j] > 0 && edges_ans[j].type > DONE) {
				bool condition = pop_matrix(graph, min.from, j) + edges_ans[min.from].length <= edges_ans[j].length;
				if (condition || edges_ans[j].type == INF) {
					edges_ans[j].length = pop_matrix(graph, min.from, j) + edges_ans[min.from].length;
					if (edges_ans[j].length > INT_MAX) {
						edges_ans[j].type = OVERFLOW;
					}
					else {
						edges_ans[j].type = WORKS;			
					}
					edges_ans[j].from = min.from;
				}
			}
		}

		before = min.from;
		min.length = 0;
		min.type = INF;
		min.from = from;
		for (int i = 0; i < graph->vertices; i++) {
			unsigned int length_now = edges_ans[i].length;
			if (length_now > 0 && (length_now < min.length || min.type == INF) && edges_ans[i].type != DONE) {
				if (length_now <= edges_ans[min.from].length || min.from == from) {
					min.from = i;
					min.type = WORKS;
					min.length = length_now;
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
			edges_ans[min.from].type = DONE;
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

	if (!is_one_ways && (edges_ans[in].type == OVERFLOW || edges_ans[in].type == INT_MAXX) )
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


enum TypeError read_edges(FILE* test_file, Graph* graph) {
	int line;
	int column;
	unsigned int weight;
	for (int i = 0; i < graph->edges; i++) {
		if (fscanf(test_file, "%i%i%u", &line, &column, &weight) == EOF) {
			return LINES;
		}
		if (line < 1 || column < 1 || line > graph->vertices || column > graph->vertices) {
			return VERTEX;
		}
		if (weight > INT_MAX) {
			return LENGTH;
		}

		line -= 1;
		column -= 1;
		put_matrix(graph, line, column, weight);
	}
	return PASS;
}


int main() {
	FILE* file;
	file = stdin;
	Graph graph;

	if (fscanf(file, "%i", &graph.vertices) == EOF) {
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

	if (from < 1 || in < 1 || from > graph.vertices || in > graph.vertices) {
		printf("bad vertex");
		fclose(file);
		return 0;
	}

	in--;
	from--;

	if (fscanf(file, "%i", &graph.edges) == EOF) {
		printf("bad number of lines");
		fclose(file);
		return 0;
	}

	if (graph.vertices <= 0 || graph.vertices > 5000) {
		if (graph.vertices == 0) {
			printf("oo");
		}
		else {
			printf("bad number of vertices");
		}
		fclose(file);
		return 0;
	}

	if (graph.edges < 0 || graph.edges > graph.vertices * (graph.vertices + 1) / 2) {
		printf("bad number of edges");
		fclose(file);
		return 0;
	}

	graph.matrix = (unsigned int*)calloc(graph.vertices * graph.vertices, sizeof(unsigned int));
	if (graph.matrix == NULL) {
		fclose(file);
		return 0;
	}

	Edge* edges_ans = (Edge*)malloc((graph.vertices) * sizeof(Edge));
	if (edges_ans == NULL) {
		free(graph.matrix);
		fclose(file);
		return 0;
	}
	for (int i = 0; i < graph.vertices; i++) {
		edges_ans[i].type = INF;
		edges_ans[i].length = 0;
	}

	enum TypeError is_error = read_edges(file, &graph);
	if (is_error == PASS) {
		char result_type = Deikstra(from, in, &graph, edges_ans);
		
		for (int i = 0; i < graph.vertices; i++) {
			if (edges_ans[i].type == OVERFLOW || edges_ans[i].type == INT_MAXX) {
				printf("INT_MAX+ ");
			}
			else if (edges_ans[i].type == INF) {
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
	else if (is_error == LINES) {
		printf("bad number of lines");
	}
	else if (is_error == VERTEX) {
		printf("bad vertex");
	}
	else if (is_error == LENGTH) {
		printf("bad length");
	}
	
	free(graph.matrix);
	free(edges_ans);
	fclose(file);
	return 0;
}
