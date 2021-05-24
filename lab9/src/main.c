#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <limits.h>


enum WorkWithOstov { OK = 0, NO_PATH = 1, INT_MAXX = 2, OVERFLOW = 3, DONE = 4, WORKS = 5, INF = 6, DEL = 7, EXIST = 8, OVER = 9, DO = 10 };
enum WorkWithOver { WHITE = 0, BLACK = 1, MAIN = 2 };
enum TypeError { PASS, LINES, VERTEX, LENGTH };


typedef struct Edge {
	int from;
	unsigned int length;
	char type;
}Edge;


typedef struct Node {
	unsigned int value;
	char type;
}Node;


typedef struct Graph {
	Node* matrix;
	int vertices;
	int edges;
}Graph;


unsigned int pop_matrix(const Graph* graph, int line, int column) {
	return graph->matrix[line * graph->vertices + column].value;
}


void put_matrix(Graph* graph, int line, int column, unsigned int weight) {
	graph->matrix[line * graph->vertices + column].value = weight;
	graph->matrix[line * graph->vertices + column].type = EXIST;
	graph->matrix[column * graph->vertices + line].value = weight;
	graph->matrix[column * graph->vertices + line].type = EXIST;
}


void put_status_del(Graph* graph, int column, int string) {
	graph->matrix[string * graph->vertices + column].type = DEL;
	graph->matrix[column * graph->vertices + string].type = DEL;
}


bool is_cicle(Graph *graph, Edge* full, Edge* edges_min, int vertex, int start) {
	if (full[vertex].type == MAIN && vertex != start) {
		return true;
	}

	for (int j = 0; j < graph->vertices; j++) {
		if (graph->matrix[vertex * graph->vertices + j].type == EXIST /*&& graph->matrix[vertex/*edges_min[vertex].from// * graph->vertices + j].value > 0*/) {
			/*if (full) {
				if (j == start) {
					continue;
				}
				else {
					return false;
				}
			}*/

			put_status_del(graph, vertex, j);
			if ( is_cicle(graph, full, edges_min, j, start) ) {
				return true;
			}
		}
	}
	return false;
}


char Deikstra(int from, int in, Graph* graph, Edge* edges_ans) {
	int count = 1;
	edges_ans[from].length = 0;
	edges_ans[from].from = from;
	edges_ans[from].type = DONE;
	Edge min = {from, 0, INF};

	int before = from;
	while (count < graph->vertices) {
		char used = 0;
		for (int j = 0; j < graph->vertices; j++) {
			if (graph->matrix[min.from * graph->vertices + j].value > 0 && edges_ans[j].type > DONE) {
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

	if (edges_ans[in].type == OVERFLOW || edges_ans[in].type == INT_MAXX) {
		int pointer = in;
		int ways_min = 2;
		while (edges_ans[pointer].from != from) {
			ways_min++;
			pointer = edges_ans[pointer].from;
		}

		Edge* edges_min = (Edge*)malloc(ways_min * sizeof(Edge));
		Edge *full = (Edge*)calloc(graph->vertices, sizeof(Edge));

		pointer = in;
		for (int i = ways_min - 2; i >= 0; i--) {
			put_status_del(graph, edges_ans[pointer].from, pointer);
			full[pointer].type = MAIN;
			edges_min[i].from = edges_ans[pointer].from;
			pointer = edges_ans[pointer].from;
		}

		bool is_one_ways = true;
		for (int i = 0; is_one_ways && i < ways_min - 1; i++) {
			for (int j = 0; j < graph->vertices; j++) {
				if (graph->matrix[edges_min[i].from * graph->vertices + j].type == EXIST && graph->matrix[edges_min[i].from * graph->vertices + j].value > 0) {
					put_status_del(graph, edges_min[i].from, j);
					if (is_cicle(graph, full, edges_min, j, edges_min[i].from)) {
						is_one_ways = false;
						break;
					}
				}
			}
		}

		free(edges_min);
		free(full);
		if(!is_one_ways)
			return OVERFLOW;
	}

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

	graph.matrix = (Node*)calloc(graph.vertices * graph.vertices, sizeof(Node));
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
