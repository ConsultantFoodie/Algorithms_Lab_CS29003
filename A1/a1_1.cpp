// Written by Hardik Tibrewal
// 18EC10020

#include<bits/stdc++.h>

using namespace std;

bool brute(int *arr, int n){  //Brute force algorithms checking location of each sauce and comparing
	int i, j, k, m, pos_i, pos_j, pos_k;
	bool flag=true;
	for(i=1;i<=n;i++){
		for(j=i+1;j<=n;j++){
			for(k=j+1;k<=n;k++){
				for(m=0;m<n;m++){
					if(arr[m]==i) pos_i=m;
					else if(arr[m]==j) pos_j=m;
					else if(arr[m]==k) pos_k=m;
				}
				if((pos_k<pos_i && pos_i<pos_j)){
					flag=false;
					break;
				}
			}
			if(!flag) break;
		}
		if(!flag) break;
	}
	return flag;
}

bool bet_brute(int *arr, int n){ //Checking locations and verifying if sauces are placed correctly
	int i, j, k, a, b, c;
	bool flag=true;
	for(i=0;i<n;i++){
		c=arr[i];
		for(j=i+1;j<n;j++){
			a=arr[j];
			for(k=j+1;k<n;k++){
				b=arr[k];
				if((a<b && b<c)){
					flag=false;
					break;
				}
			}
			if(!flag) break;
		}
		if(!flag) break;
	}
	return flag;
}

bool ref_brute(int *arr, int n){ // Ensuring every number less than a number occuring after it follows a decreasing sequence
	int i, j, prev, curr; // This ensures that the order c a b does not appear
	bool flag=true;
	for(i=0;i<n;i++){
		j=i+1;
		curr=arr[i];
		while(j<n){
			if(arr[j]<arr[i]){
				prev=curr;
				curr=arr[j];
				if(curr>=prev){
					flag=0;
					break;
				}
			}
			j++;
		}
		if(!flag) break;
	}
	return flag;
}

bool bye_brute(int *arr, int n){ // Linear time algorithm
	stack<int> s;
	int a=1, i;
	for(i=0;i<n;i++){
		while(a<=arr[i]){  //Ensuring decreasing sequence of numbers occuring after a number at place 'i'
			s.push(a);
			a++;
		}
		if(s.size()==0 || s.top()!=arr[i]) return false;  //returning negatives if the top of the stack is different than expected or stack is empty
		s.pop();
	}
	return true; //If loop ends, no unwanted sequence was present
}

void print(bool flag){  //printing the outcome
	if(flag) cout<<"Algolicious"<<endl;
	else cout<<"Unalgolicious"<<endl;
	return;
}

int main(){
	int n, i;
	cin>>n;
	int* arr=(int*)malloc(n*sizeof(int));
	for(int i=0;i<n;i++){
		cin>>arr[i];
	} // Input taken

	//Displaying sequnce
	cout<<"n = "<<n<<endl<<endl;
	cout<<"+++ Sequence: ";
	for(i=0;i<n;i++){
		cout<<arr[i]<<" ";
	}
	cout<<endl;

	//Calling and printing all methods
	cout<<"\tMethod 0: ";
	print(brute(arr,n));
	cout<<"\tMethod 1: ";
	print(bet_brute(arr,n));
	cout<<"\tMethod 2: ";
	print(ref_brute(arr,n));
	cout<<"\tMethod 3: ";
	print(bye_brute(arr,n));

	return 0;
}