//Written by Hardik Tibrewal
//18EC10020
#include <bits/stdc++.h>
#define LIM 1000000 //gives upper limit of cost

using namespace std;

typedef struct graph{ //struct to store the graph using matrices
	int n;
	char **type;
	int **cost;
}GRAPH;

GRAPH readgraph(){ //reads a graph from stdin
	GRAPH ans;
	int i, j, a, b, n, cst;
	char flight;

	cin>>n;

	ans.n=n;

	ans.type=(char**)malloc(n*sizeof(char*));
	ans.cost=(int**)malloc(n*sizeof(int*));

	for(i=0;i<n;i++){

		ans.type[i]=(char*)malloc(n*sizeof(char));
		ans.cost[i]=(int*)malloc(n*sizeof(int));

		for(j=0;j<n;j++){ //initializing the matrix
			if(i==j){
				ans.type[i][j]='s';
				ans.cost[i][j]=0;
			}
			else{
				ans.type[i][j]='-';
				ans.cost[i][j]=LIM;
			}
		}
	}

	while(1){ //reading input and storing in matrix
		cin>>a;
		if(a==-1){
			break;
		}
		scanf("%d %d %c", &b, &cst, &flight);
		ans.type[a][b]=flight;
		ans.cost[a][b]=cst;
	}
	return ans;
}

GRAPH getAIgraph(GRAPH G){ //gets graph of only AI flights
	GRAPH ai;
	int i, j;

	ai.n=G.n;

	ai.type=(char**)malloc(ai.n*sizeof(char*));
	ai.cost=(int**)malloc(ai.n*sizeof(int*));

	for(i=0;i<ai.n;i++){ //copying suitable values
		ai.type[i]=(char*)malloc(ai.n*sizeof(char));
		ai.cost[i]=(int*)malloc(ai.n*sizeof(int));
		for(j=0;j<ai.n;j++){
			if(G.type[i][j]=='a'){
				ai.type[i][j]='a';
				ai.cost[i][j]=G.cost[i][j];
			}
			else if(i==j){
				ai.type[i][j]='s';
				ai.cost[i][j]=0;
			}
			else{
				ai.type[i][j]='-';
				ai.cost[i][j]=LIM;
			}
		}
	}
	return ai;
}

void prngraph(GRAPH G){ //prints graph in a neat format
	int i, j;
	for(i=0;i<G.n;i++){
		printf("\t%-3d ->", i);
		for(j=0;j<G.n;j++){
			if(G.type[i][j]!='s' && G.type[i][j]!='-'){
				printf("%4d (%d, %c)", j, G.cost[i][j], G.type[i][j]);
			}
		}
		cout<<endl;
	}
	return;
}

void prnarr(int** ans, int n){ //prints a matrix in a neat format
	int i, j;
	cout<<"\t\t  ";
	for(i=0;i<n;i++){
		printf("%6d", i);
	}
	cout<<endl<<endl;
	for(i=0;i<n;i++){
		printf("\t%-3d ->", i);
		for(j=0;j<n;j++){
			if(ans[i][j]!=LIM){
				printf("%6d", ans[i][j]);
			}
			else{
				printf("     -");
			}
		}
		cout<<endl;
	}
	return;
}

int** APSP(GRAPH G){ //finds all pairs shortest parts for a grap using Floyd-Warshall Algorithm
	int **ans, i, j, k, n=G.n;
	ans=(int**)malloc(n*sizeof(int*));
	for(i=0;i<n;i++){
		ans[i]=(int*)malloc(n*sizeof(int));
		for(j=0;j<n;j++){
			ans[i][j]=G.cost[i][j];
		}
	}

	for(k=0;k<n;k++){
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				if(ans[i][j]>(ans[i][k]+ans[k][j])){
					ans[i][j]=ans[i][k]+ans[k][j];
				}
			}
		}
	}

	return ans;
}

int** APSPone(GRAPH G, int** only_ai){ //considers at most one non-AI flight
	int** ans, i, j, k, l, n=G.n;
	ans=(int**)malloc(n*sizeof(int*));
	for(i=0;i<n;i++){
		ans[i]=(int*)malloc(n*sizeof(int));
		for(j=0;j<n;j++){
			ans[i][j]=only_ai[i][j];
		}
	}

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(ans[i][j]==LIM){ //If only AI path exists, then we will always choose that, otherwise check for non-AI
				for(k=0;k<n;k++){
					for(l=0;l<n;l++){
						if(G.type[k][l]=='n'){
							if((only_ai[i][k]+G.cost[k][l]+only_ai[l][j])<ans[i][j]){ //checking if using a non-AI flight creates a new route
								ans[i][j]=only_ai[i][k]+G.cost[k][l]+only_ai[l][j];
							}
						}
					}
				}
			}
		}
	}

	return ans;
}

int** APSPany(GRAPH G, int** only_ai){ //Finds the cheapest non-AI paths using any number of AI flights if only-AI paths are not possible
	int** ans, i, j, k, n=G.n;
	ans=APSP(G); //Floyd-Warshall on the whole graph 

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(only_ai[i][j]!=LIM){ //changing iff only-AI path not possible
				ans[i][j]=only_ai[i][j];
			}
		}
	}
	return ans;
}

int main(){
	GRAPH G, G_ai;
	int** only_ai, **one_nai, **any_nai;
	G=readgraph(); //input

	cout<<"+++ Original graph"<<endl; //printing the original graph
	prngraph(G);
	cout<<endl;

	G_ai=getAIgraph(G); //getting AI subgraph and printing
	cout<<"+++ AI subgraph"<<endl;
	prngraph(G_ai);
	cout<<endl;

	only_ai=APSP(G_ai); //only AI shortest paths
	cout<<"+++ Cheapest AI prices"<<endl;
	prnarr(only_ai, G.n);
	cout<<endl;

	one_nai=APSPone(G, only_ai); //with at most one non AI
	cout<<"+++ Cheapest prices with at most one non-AI leg"<<endl;
	prnarr(one_nai, G.n);
	cout<<endl;

	any_nai=APSPany(G, only_ai); //with any number of non-AI
	cout<<"+++ Cheapest prices with any number of non-AI legs"<<endl;
	prnarr(any_nai, G.n);
	cout<<endl;
	return 0;
}