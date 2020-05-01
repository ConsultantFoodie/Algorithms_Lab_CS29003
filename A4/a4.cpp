// Written by Hardik Tibrewal
// 18EC10020
#include <bits/stdc++.h>

using namespace std;

void greedy1(int n){ // First greedy implementation
	int i=0;
	printf("+++ Greedy 1\n");
	printf("\tStart     : %d\n", n);
	while(n!=1){
		if(n%2){
			n--;
			printf("\tDecrement : %d\n", n);
			i++;
		}
		else{
			n/=2;
			printf("\tDivide    : %d\n", n);
			i++;
		}
	}
	printf("--- Number of steps = %d\n", i);
}

int fact2(int x){ //Counting factors of 2 for a given int
	int cnt=0;
	while(!(x%2) && x!=0){
		x/=2;
		cnt++;
	}
	return cnt;
}

void greedy2(int n){ //Second greedy. Can increment by 1
	int steps=0;
	printf("+++ Greedy 2\n");
	printf("\tStart     : %d\n", n);
	while(n>3){
		if(n%2){
			if(fact2(n+1)>fact2(n-1)){
				n++;
				printf("\tIncrement : %d\n", n);
				steps++;
			}
			else{
				n--;
				printf("\tDecrement : %d\n", n);
				steps++;
			}
		}
		else{
			n/=2;
			printf("\tDivide    : %d\n", n);
			steps++;
		}
	}
	while(n!=1){
		n--;
		printf("\tDecrement : %d\n", n);
		steps++;
	}
	printf("--- Number of steps = %d\n", steps);
}

void greedy3(int n, vector<int> v){ //Third greedy
	int idx, max=INT_MIN, steps=0, i;
	set<int> s;
	s.clear();
	s.insert(n);
	printf("+++ Greedy 3\n");
	printf("\tStart     : %d\n", n);

	while(n!=1){
		if(n%2){
			max=INT_MIN;
			idx=0;
			for(i=0;i<v.size();i++){ //Checking which option gives most factors of 2
				if(fact2(n+v[i])>max && (n+v[i])>0 && s.find(n+v[i])==s.end()){
					max=fact2(n+v[i]);
					idx=i;
				}
			}

			n+=v[idx];
			s.insert(n);
			printf("\tAdd %-6d: %d\n", v[idx], n);
			steps++;

		}
		else{
			n/=2;
			s.insert(n);
			printf("\tDivide    : %d\n", n);
			steps++;
		}
	}
	printf("--- Number of steps = %d\n", steps);
}

void optimal(int n, vector<int> v){ //Optimal function
	int i, j, imprvd, size=n+11, temp, steps;
	int* arr=(int*)malloc(size*sizeof(int));
	int* brr=(int*)malloc(size*sizeof(int));

	for(i=0;i<size;i++){
		arr[i]=100000;
		brr[i]=-1;
	}
	arr[1]=0;

	while(1){
		imprvd=0;
		for(i=2;i<size;i++){ //Improving optimal in each step till no changes
			if(i%2==0){
				if(arr[i]>arr[i/2]+1){
					arr[i]=arr[i/2]+1;
					brr[i]=v.size();
					imprvd++;
				}
			}
			for(j=0;j<v.size();j++){
				if((i+v[j])>0 && (i+v[j])<size && (arr[i]>arr[i+v[j]]+1)){
					arr[i]=arr[i+v[j]]+1;
					imprvd++;
					brr[i]=j;
				}
			}
		}
		if(imprvd==0){
			break;
		}
	}
	steps=0;
	i=n;
	printf("+++ Optimal\n");
	printf("\tStart     : %d\n", n);
	while(i>1){
		steps++;
		if(brr[i]==v.size()){
			i/=2;
			printf("\tDivide    : %d\n", i);
		}
		else if(brr[i]>=0){
			temp=v[brr[i]];
			i+=temp;
			printf("\tAdd %-6d: %d\n", temp, i);
		}
	}
	printf("--- Number of steps = %d\n", steps);
}

int main(){
	int n, k, ip;
	cin>>n;

	//Calling all functions
	greedy1(n);
	cout<<endl<<endl;
	greedy2(n);
	cout<<endl<<endl;
	cin>>k;
	vector<int> v;
	for(int i=0;i<k;i++){
		cin>>ip;
		v.push_back(ip);
	}
	sort(v.begin(),v.end());
	greedy3(n, v);
	cout<<endl<<endl;
	optimal(n,v);
	return 0;
}