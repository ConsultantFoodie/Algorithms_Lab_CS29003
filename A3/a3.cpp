//Written by Hardik Tibrewal
// 18EC10020
#include<bits/stdc++.h>

using namespace std;

typedef struct{ //Struct to store a line
	double m;
	double c;
	int n;
} LINE;

typedef struct{ //struct to store a point
	double x;
	double y;
} PNT;

PNT intrsct(LINE A, LINE B){ //Finds point of intersection of 2 lines
	PNT ans;
	ans.x=((B.c-A.c)/(A.m-B.m));
	ans.y=B.m*ans.x+B.c;
	return ans;
}

void print(LINE* line, int n){ //Prints all lines in a neat format
	int i;
	for(i=0;i<n;i++){
		printf("Line %5d:%16lf x %c %16lf\n",line[i].n, line[i].m, (line[i].c>0)?'+':'-', abs(line[i].c));
	}
}

void first(LINE* line, int n){ // Finds region of visibility in O(n^2)
	int i, j, min_idx=-1;
	double min_m=DBL_MAX; 
	for(i=0;i<n;i++){ //Finds line with minimum slope, as that will be the left most edge
		if(line[i].m<min_m){
			min_m=line[i].m;
			min_idx=i;
		}
	}
	LINE L, K=line[min_idx];

	int *arr=(int*)malloc(n*sizeof(int)); //Array of visibility
	for(i=0;i<n;i++) arr[i]=0;

	arr[min_idx]=1;
	int idx=-1;

	PNT Q,P;
	Q.x=-DBL_MAX; Q.y=DBL_MAX;
	double curr_x, curr_y, prev_x=-DBL_MAX, prev_y;

	cout<<"\n+++ Method 1\n";

	while(Q.x!=DBL_MAX){ //Finds leftmost intersection point for next vertex of region
		L=K;
		curr_x=DBL_MAX;
		idx=-1;
		for(i=0;i<n;i++){
			if(arr[i]!=1){
				P=intrsct(L,line[i]);
				if(P.x>Q.x && P.x<curr_x){
					curr_x=P.x;
					curr_y=P.y;
					idx=i;
				}
			}
		}
		arr[idx]=1; 
		//Printing the latest edge
		if(Q.x==-DBL_MAX){
			cout<<"Line "<<min_idx<<": From MINUS_INFINITY to ("<<curr_x<<","<<curr_y<<")"<<endl;
		}
		else if(curr_x==DBL_MAX){
			cout<<"Line "<<min_idx<<": From ("<<Q.x<<","<<Q.y<<") to PLUS_INFINITY"<<endl;

		}
		else{
			cout<<"Line "<<min_idx<<": From ("<<Q.x<<","<<Q.y<<") to ("<<curr_x<<","<<curr_y<<")"<<endl;
		}
		K=line[idx]; //moving to next line and next point
		Q.x=curr_x;
		Q.y=curr_y;
		min_idx=idx;
	}
}

bool comp(LINE A, LINE B){ //comparator function for custom std::sort
	return (A.m<B.m);
}

void second(LINE* line, int n){ //Function to find region of visibility in O(n lgn)
	int i, j;
	PNT Q;
	Q.x=-DBL_MAX;
	stack<LINE> s; //Initialising stacks
	stack<PNT> spnt;
	s.push(line[0]); //Line with least slope (sorted array) 
	spnt.push(Q);
	i=1;
	for(i=1;i<n;i++){ //Moving through lines one by one and popping if new intersection point 
		while(intrsct(line[i], s.top()).x<spnt.top().x){ //occurs before previous intersection point
			spnt.pop();
			s.pop();
		}
		spnt.push(intrsct(line[i],s.top())); //Pushing latest line and point in stack
		s.push(line[i]);
	}

	LINE* ans=(LINE*)malloc(s.size()*sizeof(LINE)); //Making array from stack for ease of printing
	i=s.size()-1; j=i+1;
	while(s.size()!=0){
		ans[i]=s.top();
		s.pop();
		i--;
	}

	//Printing the region
	printf("\n+++ Method 2\n");

	for(i=0;i<j;i++){
		printf("Line %5d: ", ans[i].n);
		if(i==0){
			printf("From MINUS_INFINITY to ");
		}
		else{
			printf("From (%lf,%lf) to ", intrsct(ans[i],ans[i-1]).x, intrsct(ans[i],ans[i-1]).y);			
		}

		if(i==(j-1)){
			printf("PLUS_INFINITY\n");
		}
		else{
			printf("(%lf,%lf)\n", intrsct(ans[i],ans[i+1]).x, intrsct(ans[i],ans[i+1]).y);
		}
	}
	return;
}

int main(){
	int i, n;
	cin>>n;
	LINE* line=(LINE*)malloc(n*sizeof(LINE));
	//Taking input
	for(i=0;i<n;i++){
		cin>>line[i].m>>line[i].c;
		line[i].n=i;
	}

	//calling functions and printing misc info
	printf("\n+++ Lines before sorting:\n");
	print(line, n);

	first(line, n);

	sort(line, line+n, comp);

	printf("\n+++ Lines after sorting:\n");
	print(line, n);

	second(line, n);
	return 0;
}