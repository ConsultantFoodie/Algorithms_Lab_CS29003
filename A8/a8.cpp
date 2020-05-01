// Written by Hardik Tibrewal
// 18EC10020
#include<bits/stdc++.h>

using namespace std;

typedef long long int llint; //using long long ints

typedef struct node{ //struct for nodes in heap
	llint num;
	llint *keys;
}NODE;

typedef struct heap{ //struct for heap
	llint node_cap;
	llint n_keys;
	llint n_nodes;
	NODE* node_arr;
}HEAP;

HEAP initheap(llint p, llint n_max){ //initialising heap and all the nodes in the heap. setting active nodes and number of current keys to 0
	HEAP h;
	h.node_cap=p;
	h.n_keys=0;
	h.n_nodes=0;

	llint i, num = (n_max+p-1)/p;
	h.node_arr = (NODE*)malloc(num*sizeof(NODE));

	for(i=0;i<num;i++){
		h.node_arr[i].keys=(llint*)malloc(p*sizeof(llint));
	}
	return h;
}

llint find_min_idx(llint *arr, llint size){ //finds the index of the minimum element in a given array
	llint idx=-1, min=INT_MAX;

	for(llint i=0;i<size;i++){
		if(arr[i]<min){
			min=arr[i];
			idx=i;
		}
	}
	return idx;
}

llint find_max_idx(llint *arr, llint size){ //finds the index of the maximum element in a given array
	llint idx=-1, max=-INT_MAX;

	for(llint i=0;i<size;i++){
		if(arr[i]>max){
			max=arr[i];
			idx=i;
		}
	}
	return idx;
}

void prntheap(HEAP h){ //Prints the heap
	llint p=h.node_cap;
	llint N = h.n_nodes;

	cout<<"\n+++ "<<h.n_keys<<" insertions made\n";
	llint i, j;

	for(i=0;i<N;i++){
		cout<<"\t[";
		for(j=0;j<h.node_arr[i].num;j++){
			printf("%5lld", h.node_arr[i].keys[j]);
		}
		cout<<"  ]\n";
	}

}

HEAP insert(HEAP h, llint x){ //Function to insert elements in the heap
	llint i, j;
	for(i=0;i<h.n_nodes;i++){
		if(h.node_arr[i].num<h.node_cap){ //Finding the node in which insert should operate.
			break;
		}
	}
	if(h.node_arr[i].num==0){ //if a new node is needed, increasing number of nodes of heap
		h.n_nodes++;
	}

	//inserting the key
	h.node_arr[i].keys[h.node_arr[i].num]=x;
	h.node_arr[i].num++;
	h.n_keys++;

	//
	llint min_child, max_prnt, min_prnt, max_child, *temp_arr, temp_size;
	min_prnt = h.node_arr[(i-1)/2].keys[find_min_idx(h.node_arr[(i-1)/2].keys, h.node_arr[(i-1)/2].num)];
	max_child= h.node_arr[i].keys[find_max_idx(h.node_arr[i].keys, h.node_arr[i].num)];

	while(i>0 && min_prnt<max_child){
		temp_size=h.node_arr[(i-1)/2].num + h.node_arr[i].num;
		temp_arr = (llint*)malloc(temp_size*sizeof(llint));
		for(j=0;j<temp_size;j++){
			if(j<h.node_cap){
				temp_arr[j]=h.node_arr[(i-1)/2].keys[j];
			}
			else{
				temp_arr[j]=h.node_arr[i].keys[j-h.node_cap];
			}
		}

		sort(temp_arr, temp_arr+temp_size, greater<llint>());

		for(j=0;j<temp_size;j++){
			if(j<h.node_cap){
				h.node_arr[(i-1)/2].keys[j]=temp_arr[j];
			}
			else{
				h.node_arr[i].keys[j-h.node_cap]=temp_arr[j];
			}
		}
		i=(i-1)/2;
		if(i>0){
			min_prnt = h.node_arr[(i-1)/2].keys[find_min_idx(h.node_arr[(i-1)/2].keys, h.node_arr[(i-1)/2].num)];
			max_child= h.node_arr[i].keys[find_max_idx(h.node_arr[i].keys, h.node_arr[i].num)];
		}

		free(temp_arr);

	}

	return h;

}

llint findmax(HEAP h){ //returns index of the maximum value in the root node.
	if(h.n_keys!=0){
		return find_max_idx(h.node_arr[0].keys,h.node_arr[0].num);
	}
	else return -1;
}

HEAP heapify(HEAP h, llint i){
	if(2*i+2>=h.n_nodes) return h; //Checking if at least 1 child exists

	llint root, left, right;
	root = find_min_idx(h.node_arr[i].keys, h.node_arr[i].num);
	left = find_max_idx(h.node_arr[2*i+1].keys, h.node_arr[2*i+1].num);
	if((2*i+2)<h.n_nodes) right = find_max_idx(h.node_arr[2*i+2].keys, h.node_arr[2*i+2].num); //checking if right child exists
	llint temp;
	if((2*i+2)<h.n_nodes){ //checking both nodes if right node exists, assuming left skewed balanced BST type structure
		if(h.node_arr[2*i+1].keys[left] > h.node_arr[i].keys[root] && h.node_arr[2*i+1].keys[left] >= h.node_arr[2*i+2].keys[right]){ //checking if left node has the biggest element
			temp=h.node_arr[i].keys[root];
			h.node_arr[i].keys[root] = h.node_arr[2*i+1].keys[left];
			h.node_arr[2*i+1].keys[left] = temp;
			h = heapify(h, 2*i+1);
		}
		else if(h.node_arr[2*i+2].keys[right] > h.node_arr[i].keys[root] && h.node_arr[2*i+2].keys[right] >= h.node_arr[2*i+1].keys[left]){ //checking if right node has the biggest element
			temp = h.node_arr[i].keys[root];
			h.node_arr[i].keys[root] = h.node_arr[2*i+2].keys[right];
			h.node_arr[2*i+2].keys[right] = temp;
			h = heapify(h, 2*i+2);
		}
	}
	else{ //just checking with left child since right child does not exist
		if(h.node_arr[2*i+1].keys[left] > h.node_arr[i].keys[root]){
			temp=h.node_arr[i].keys[root];
			h.node_arr[i].keys[root] = h.node_arr[2*i+1].keys[left];
			h.node_arr[2*i+1].keys[left] = temp;
			h = heapify(h, 2*i+1);
		}
	}
	return h;
}

HEAP delmax(HEAP h){ //function to delete max of heap
	int temp;
	temp=h.node_arr[0].keys[findmax(h)]; //finding max element

	//pushing it to the end of the node and decreasing number of keys in the node and heap by 1 
	h.node_arr[0].keys[findmax(h)]=h.node_arr[0].keys[h.node_arr[0].num - 1]; 
	h.node_arr[0].keys[h.node_arr[0].num - 1]=temp;
	h.node_arr[0].num--;
	h.n_keys--;

	//if only the root node exists, returning.
	if(h.n_nodes==1){
		return h;
	}
	else{ //else swapping with the last element of the heap and heapifying
		h.node_arr[0].keys[h.node_arr[0].num] = h.node_arr[h.n_nodes-1].keys[h.node_arr[h.n_nodes - 1].num - 1];
		h.node_arr[h.n_nodes-1].num--;
		h.node_arr[0].num++;
		if(h.node_arr[h.n_nodes-1].num==0){
			h.n_nodes--;
		}
		h=heapify(h, 0);
		return h;
	}

}



int main(){
	llint i, p, n_max, *arr;
	cin>>p>>n_max;
	arr=(llint*)malloc(n_max*sizeof(llint));
	for(i=0;i<n_max;i++){
		cin>>arr[i];
	}

	// making empty heap and filling it
	HEAP h = initheap(p, n_max);
	for(i=0;i<n_max;i++){
		h=insert(h, arr[i]);
	}
	
	prntheap(h);


	//freeing array to ensure sort works properly
	llint dels = 0;
	free(arr);
	arr=(llint*)malloc(n_max*sizeof(llint));
	i=n_max-1;

	while(h.n_keys && i>=0){
		arr[i]=h.node_arr[0].keys[findmax(h)];
		i--;
		h = delmax(h);
		dels++;
	}

	//flag to check if sorting is proper
	int flag=0;
	cout<<"\n+++ "<<dels<<" deletions made\n";
	cout<<"\n+++ Input array sorted\n";
	for(i=0;i<n_max;i++){
		printf("%lld ", arr[i]);
		if(i>0){
			if(arr[i]<arr[i-1]) flag=1;
		}
	}
	cout<<endl;
	//cout<<endl<<flag<<endl;
	return 0;
}