#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int INT_MAX = 500000;

struct Edge
{
    int airport1, airport2, distance;
};


struct Graph
{
    int V, E;
	// V is number of vertices and E is number of edges

    struct Edge* edge;
};

struct Graph* createGraph(int V, int E)
{
    struct Graph* graph = (struct Graph*) malloc( sizeof(struct Graph));
    graph->V = V;
    graph->E = E;
    graph->edge = (struct Edge*) malloc( graph->E * sizeof( struct Edge ) );
    return graph;
}

void airports(char *airports_file){
    FILE *fptr;


    fptr = fopen(airports_file, "r");
    if (fptr == NULL)
    {
        printf("Error accessing airport file\n");
        exit(0);
    }

    char c = fgetc(fptr);
    while (c != EOF)
    {
        printf ("%c", c);
        c = fgetc(fptr);
    }

    fclose(fptr);
}

int get_index(char *character, char **vertices_list, int length){
  for(int j = 0; j < length; j++){
    if(strcmp(vertices_list[j], character) == 0){
      return j;
    }
  }
  return -1;
}

void bellman_ford(struct Graph* graph, int src, int dst, char ** vertices_list){
    int V = graph->V;
    int E = graph->E;
    int distances[V];
    int path_dist[V];
    int path_airports[V];
    int i,j;

    for (i = 0; i < V; i++)
        distances[i] = INT_MAX;

    distances[src] = 0;
    for (i = 1; i <= V-1; i++){
        for (j = 0; j < E; j++){
            int u = graph->edge[j].airport1;
            int v = graph->edge[j].airport2;
            int weight = graph->edge[j].distance;
            if (distances[u] != INT_MAX && distances[u] + weight < distances[v]){
                distances[v] = distances[u] + weight;
                path_airports[v] = u;
                path_dist[v] = weight;
            }
            if (distances[v] != INT_MAX && distances[v] + weight < distances[u]){
                distances[u] = distances[v] + weight;
                path_airports[u] = v;
                path_dist[u] = weight;
            }
        }
    }
    printf("The Total Distance is %d\n", distances[dst]);
    /*for(int count = 0; count < V; count ++){
      printf("%d node and %d distance\n", path_airports[count], path_dist[count]);
    }*/

    int p = 0;
    int node = dst;
    int travel_route[V][2];
    while(node != src){
      travel_route[p][0] = node;
      travel_route[p][1] = path_dist[node];
      node = path_airports[node];
      p++;
    }
    /*printf("P is %d\n", p);
    for(int count = 0; count < p; count ++){
      printf("%d node and %d distance\n", travel_route[count][0], travel_route[count][1]);
    }*/

    printf("%s -> %s %d\n", vertices_list[src], vertices_list[travel_route[p-1][0]], travel_route[p-1][1]);
    for(int count = p-1; count >0; count --){
      printf("%s -> %s %d\n", vertices_list[travel_route[count][0]], vertices_list[travel_route[count-1][0]], travel_route[count-1][1]);
    }
}

void find_distance(char *dep, char *dest, char *dists_file, char *airports_file){
  int num_verts = 0;
  int num_edges = 0;
  FILE *fptr;
  // First Count all the Vertices to make a list of vertices
  fptr = fopen(airports_file, "r");
  if (fptr == NULL)
  {
      printf("Error accessing airport file\n");
      exit(0);
  }
  char buffer[200];
  while(fgets(buffer, 100, fptr) != NULL){
    num_verts++;
  }
  fclose(fptr);

  //Vertices List
  fptr = fopen(airports_file, "r");
  char ** vertices_list = malloc(sizeof(char*) * num_verts);
  int i = 0;
  while(fgets(buffer, 100, fptr) != NULL){
    vertices_list[i] = malloc(4);
    sscanf(buffer, "%s", vertices_list[i]);
    i++;
  }
  fclose(fptr);

  //Count edges
  fptr = fopen(dists_file, "r");
  if (fptr == NULL)
  {
      printf("Error accessing distance file\n");
      exit(0);
  }
  while(fgets(buffer, 100, fptr) != NULL){
    num_edges++;
  }
  fclose(fptr);

  struct Graph *graph = createGraph(num_verts, num_edges*2);

  fptr = fopen(dists_file, "r");
  i = 0;
  char start[4];
  char end[4];
  int weight;
  while (fgets(buffer, 100, fptr) != NULL)
  {
      sscanf(buffer, "%s %s %d", start, end, &weight);
      /*printf("%s ", start);
      printf("%s ", end);
      printf("%d\n", weight);*/
      int one = get_index(start, vertices_list, num_verts); //Need to change to hash table
      int two = get_index(end, vertices_list, num_verts); //Need to change to hash table
      graph->edge[i].airport1 = one;
      graph->edge[i].airport2 = two;
      graph->edge[i].distance = weight;
      i++;
  }
  fclose(fptr);
  int airport_dep = get_index(dep, vertices_list, num_verts);
  int airport_dest = get_index(dest, vertices_list, num_verts);
  if(airport_dep == -1 || airport_dest == -1){
    printf("Please enter valid airports, run command 'airports' to view a list of airports we operate from\n");
    return;
  }
  /*printf("%d vertices and %d edges\n", graph->V, graph->E );
  for(int loop = 0; loop < graph -> V; loop++){
      printf("%s ", vertices_list[loop]);
  }
  printf("Here are the edges\n");
  for(int loop = 0; loop < graph -> E; loop++){
      printf("edge #%d ",loop);
      printf("%d ", graph->edge[loop].airport1);
      printf("%d ", graph->edge[loop].airport2);
      printf("%d\n", graph->edge[loop].distance);
  }*/
  bellman_ford(graph, airport_dep, airport_dest, vertices_list);
}

int main(){
  char airports_file[] = "large-airports.txt";
  char dists_file[] = "large-dists.txt";
  printf("Welcome to the FTA Reservation System, run command 'help' for help\n");
  while(1){
    char command[50];
    printf(">> ");
    scanf("%s", command);

    if(strcmp(command, "help") == 0){
      printf("This is the FTA Reservation system\n");
      printf("Here are other commands you can run\n");
      printf("Run 'airports' to print a list of all airports we operate at\n");
      printf("Run 'distance airport1 airport2' to find the shortest distance between two airports we operate from\n");
      printf("Airport names should be the three letter code. Example: distance ALB SEA\n");
      printf("Run 'quit' to exit this program\n");
    }
    else if(strcmp(command, "quit") == 0){
      printf("You've exited the FTA Reservation System\n");
      break;
    }
    else if(strcmp(command, "airports") == 0){
      //printf("Request airports\n");
      airports(airports_file);
    }
    else if(strcmp(command, "distance") == 0){
      char dep[4];
      char dest[4];
      scanf("%s", dep);
      scanf("%s", dest);
      find_distance(dep, dest, dists_file, airports_file);
      //printf("%s %s\n",dep,dest);
    }
    else{
      printf("Command does not exist, run 'help' to view a list of availabe commands\n");
    }

  }
}
