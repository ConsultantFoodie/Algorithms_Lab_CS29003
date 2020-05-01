//Written by Hardik Tibrewal
//18EC10020
#include <bits/stdc++.h>

using namespace std;

typedef struct graph{ //struct for graphs
	int num_v;
	char* color; //red or blue
	int* v;
	vector<int>* adj; //adjacency list
	map<int, int> mf; //mappinng global index to local index
	map<int, int> mr; //mapping local index to global index
}GRAPH;

GRAPH readgraph(){
	GRAPH G;
	int n, i, j, v1, v2;
	char temp;

	cin>>n; //Takes in number of vertices
	G.num_v = n;
	G.color = (char*)malloc(n*sizeof(char));
	G.v = (int*)malloc(n*sizeof(int));
	G.adj = (vector<int>*)malloc(n*sizeof(vector<int>));

	for(i=0;i<n;i++){
		cin>>G.color[i];
	}

	for(i=0;i<n;i++){ //filling maps
		G.v[i] = i;
		G.mf.insert(pair<int, int>(i, i));
		G.mr.insert(pair<int, int>(i, i));
	}
	
	while(1){
		cin>>v1; //taking edges and making adjacency list
		if(v1==-1){
			break;
		}
		cin>>v2;

		G.adj[v1].push_back(v2);
		G.adj[v2].push_back(v1);
	}

	return G;
}

void prngraph(GRAPH G){ //prints a graph according to global index
	int i;

	for(i=0;i<G.num_v;i++){

		printf("\t[%c] %-3d ->   ", G.color[i], G.mr[G.v[i]]);
		for(auto it = (G.adj)[i].begin(); it!=(G.adj)[i].end();it++){
			printf("%d, ", G.mr[*it]);
		}
		cout<<endl;

	}
	return;
}

GRAPH getcolgraph(GRAPH G, char col){ //gives a subgraph of a given color
	GRAPH ans;
	int i, j, k, col_size=0;

	for(i=0;i<G.num_v;i++){
		if(G.color[i]==col){
			ans.mf.insert(pair<int, int>(i, col_size)); //mapping local and global addresses
			ans.mr.insert(pair<int, int>(col_size, i));
			col_size++;
		}
	}

	ans.num_v = col_size;
	ans.color = (char*)malloc(col_size*sizeof(char));
	ans.v = (int*)malloc(col_size*sizeof(int));
	ans.adj = (vector<int>*)malloc(col_size*sizeof(vector<int>));


	j=0;
	for(i=0;i<G.num_v;i++){
		if(G.color[i]==col){
			ans.color[j] = col;
			ans.v[j] = j;
			for(k=0;k<G.adj[i].size();k++){
				if(G.color[G.adj[i][k]]==col){ //only pushing vertices of same color
					ans.adj[j].push_back(ans.mf[G.adj[i][k]]);
				}
			}
			j++;
		}
	}
	return ans;
}

void DFS(GRAPH G, int v, int prnt, int* visit, int* parent, int* level){ //DFS for graph
	visit[v] = 1;
	parent[v] = prnt;
	if(prnt!=-1){
		level[v] = level[prnt]+1;
	}
	else{
		level[v] = 0;
	}

	auto it = G.adj[v].begin();
	for(;it!=G.adj[v].end();it++){

		if(visit[*it]==0){
			DFS(G, *it, v, visit, parent, level);
		}

		else{
			if(*it!=prnt && level[*it]<level[v]){ //Found cycle. printing cycle in global addresses
				int node=v;
				printf("(%d, ", G.mr[node]);
				while(parent[node]!=*it){
					printf("%d, ", G.mr[parent[node]]);
					node = parent[node];
				}
				printf("%d)", G.mr[parent[node]]);

				node = v;
				printf(", Color: (%c, ", G.color[G.mr[node]]);
				while(parent[node]!=*it){
					printf("%c, ", G.color[G.mr[parent[node]]]);
					node = parent[node];
				}
				printf("%c)\n", G.color[G.mr[parent[node]]]);
			}
		}
	}
	return;

}

int* multiDFS(GRAPH G){ //multi DFS for disconnected graphs
	int *visit, *parent, *level, i;
	visit = (int*)malloc(G.num_v*sizeof(int));
	parent = (int*)malloc(G.num_v*sizeof(int));
	level = (int*)malloc(G.num_v*sizeof(int));
	for(i=0;i<G.num_v;i++){
		visit[i]=0;
	}

	for(i=0;i<G.num_v;i++){
		if(visit[i]==0){
			DFS(G, i, -1, visit, parent, level);
		}
	}

	return parent; //returns array containing parent of each local node
}

GRAPH getrbgraph(GRAPH G, GRAPH Gr, GRAPH Gb, int* prnt_r, int* prnt_b){ //gets graph of both colors with tree of single colors and cross-edges
	GRAPH GRB;
	GRB.num_v = G.num_v;
	GRB.color = G.color;
	GRB.v = G.v;
	GRB.mf = G.mf;
	GRB.mr = G.mr;
	GRB.adj = (vector<int>*)malloc(G.num_v*sizeof(vector<int>));

	int i, j, flag;
	for(i=0;i<Gr.num_v;i++){
		if(prnt_r[i]!=-1){ //pushing global adresses after checking for doubles fro red subgraph
			flag=0;
			for(j=0;j<GRB.adj[Gr.mr[i]].size();j++){
				if(GRB.adj[Gr.mr[i]][j]==Gr.mr[prnt_r[i]]){
					flag=1;
					break;
				}
			}
			if(flag!=1) GRB.adj[Gr.mr[i]].push_back(Gr.mr[prnt_r[i]]);

			flag = 0;
			for(j=0;j<GRB.adj[Gr.mr[prnt_r[i]]].size();j++){
				if(GRB.adj[Gr.mr[prnt_r[i]]][j]==Gr.mr[i]){
					flag=1;
					break;
				}
			}
			if(flag!=1) GRB.adj[Gr.mr[prnt_r[i]]].push_back(Gr.mr[i]);
		}
	}

	for(i=0;i<Gb.num_v;i++){
		if(prnt_b[i]!=-1){ //pushing global adresses after checking for doubles fro blue subgraph
			flag=0;
			for(j=0;j<GRB.adj[Gb.mr[i]].size();j++){
				if(GRB.adj[Gb.mr[i]][j]==Gb.mr[prnt_b[i]]){
					flag=1;
					break;
				}
			}
			if(flag!=1) GRB.adj[Gb.mr[i]].push_back(Gb.mr[prnt_b[i]]);

			flag = 0;
			for(j=0;j<GRB.adj[Gb.mr[prnt_b[i]]].size();j++){
				if(GRB.adj[Gb.mr[prnt_b[i]]][j]==Gb.mr[i]){
					flag=1;
					break;
				}
			}
			if(flag!=1) GRB.adj[Gb.mr[prnt_b[i]]].push_back(Gb.mr[i]);
		}
	}

	for(i=0;i<G.num_v;i++){
		for(j=0;j<G.adj[i].size();j++){
			if(G.color[G.adj[i][j]]!=G.color[i]){ //pushing cross-edges
				flag = 0;
				for(int k=0;k<GRB.adj[G.mr[G.adj[i][j]]].size();k++){
					if(GRB.adj[G.mr[G.adj[i][j]]][k] == G.mr[i]){
						flag=1;
						break;
					}
				}
				if(flag!=1) GRB.adj[G.mr[G.adj[i][j]]].push_back(G.mr[i]);

				flag=0;
				for(int k=0;k<GRB.adj[G.mr[i]].size();k++){
					if(GRB.adj[G.mr[i]][k] == G.mr[G.adj[i][j]]){
						flag=1;
						break;
					}
				}
				if(flag!=1) GRB.adj[G.mr[i]].push_back(G.mr[G.adj[i][j]]);
			}
		}
	}

	return GRB;
}

int main(){
	GRAPH G, Gr, Gb, GRB;
	G = readgraph(); //reading graph
	printf("+++ Original graph (G)\n");
	prngraph(G);
	cout<<endl;

	//making colored subgraphs
	Gr = getcolgraph(G, 'r');
	Gb = getcolgraph(G, 'b');

	printf("+++ Red subgraph (GR)\n");
	prngraph(Gr);
	cout<<endl;

	printf("+++ Blue subgraph (GB)\n");
	prngraph(Gb);
	cout<<endl;
	
	int *prnt_r, *prnt_b, *final;

	//getting cycles in colored graphs and parent pointers
	printf("+++ Red cycles\n");
	prnt_r = multiDFS(Gr);
	cout<<endl;

	printf("+++ Blue cycles\n");
	prnt_b = multiDFS(Gb);
	cout<<endl;

	//getting nonmonochrome graph
	GRB = getrbgraph(G, Gr, Gb, prnt_r, prnt_b);

	printf("+++ Nonmonochromatic graph (GRB)\n");
	prngraph(GRB);
	cout<<endl;

	//getting multicolor cycles
	printf("+++ Multi-color cycles\n");
	final = multiDFS(GRB);
	cout<<endl;
	return 0;
}