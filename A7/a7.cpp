// Written by Hardik Tibrewal
// 18EC10020

#include <bits/stdc++.h>

using namespace std;
typedef long long int llint;

typedef struct node{ //Node struct for BST
	int key;
	node* left;
	node* right;
}NODE;

NODE* init(int val){ //Init a new node with given val
	NODE* temp;
	temp = new NODE;
	temp->key=val;
	temp->left=NULL;
	temp->right=NULL;
	return temp;
}

llint fact(int n){ //factorial function
	llint i=1, ans=1;
	for(i=2;i<=n;i++){
		ans=ans*i;
	}
	return ans;
}

llint combin(int n, int r){ // nCr function

	return (fact(n)/(fact(r)*fact(n-r)));
}

int cnt1(int n){ //ounts the number of set bits in the binary representation of an int
	int cnt=0;
	while(n){
		cnt+=n&1;
		n>>=1;
	}
	return cnt;
}

llint countseq(int* A, int n){ //counts the number of isogenic sequences for a given sequence
	if(n<=2) return 1;
	int *Al, l, *Ar, r, i;
	Al=(int*)malloc(n*sizeof(int));
	Ar=(int*)malloc(n*sizeof(int));
	l=0; r=0;
	for(i=1;i<n;i++){
		if(A[i]<A[0]){
			Al[l]=A[i];
			l++;
		}
		else{
			Ar[r]=A[i];
			r++;
		}
	}

	return countseq(Al,l)*countseq(Ar,r)*combin(n-1,l);
}

int** findallseq(int* A, int n){ //Finds all isogenic sequences for the given sequence
	int** Ans, **Al_ans, **Ar_ans, i, j, k, *Al, l, *Ar, r, number;
	if(n<=2){ //Base case
		Ans=(int**)malloc(1*sizeof(int*));
		Ans[0]=(int*)malloc(n*sizeof(int));
		for(i=0;i<n;i++){
			Ans[0][i]=A[i];
		}
		return Ans;
	}
	
	Al=(int*)malloc(n*sizeof(int)); //All sequences for left subtree of BST
	Ar=(int*)malloc(n*sizeof(int)); //All sequences for right subtree of BST
	l=0; r=0;
	for(i=1;i<n;i++){
		if(A[i]<A[0]){
			Al[l]=A[i];
			l++;
		}
		else{
			Ar[r]=A[i];
			r++;
		}
	}

	//recursive calls
	Al_ans=findallseq(Al, l);
	Ar_ans=findallseq(Ar, r);

	int rows_l=countseq(Al,l);
	int rows_r=countseq(Ar, r);
	int rows = countseq(A,n);

	//init a blank answer array
	Ans = (int**)malloc(rows*sizeof(int*));
	for(i=0;i<rows;i++){
		Ans[i] = (int*)malloc(n*sizeof(int));
	}

	int row_cntr=0, ii, jj, kk;
	llint LIM = 1<<(n-1);

	//using binary representation of a number to find out the positions of numbers to be placed. We use an n-bit number with "r" set bits
	// each 1 represents a number greater than the root
	for(i=0;i<LIM;i++){
		if(cnt1(i)==r){
			for(j=0;j<rows_l;j++){
				for(k=0;k<rows_r;k++){
					number=i; ii=1; jj=0; kk=0;
					while(ii<n){
						if(number&1){
							Ans[row_cntr][ii]=Ar_ans[k][kk];
							ii++;
							kk++;
						}
						else{
							Ans[row_cntr][ii]=Al_ans[j][jj];
							ii++;
							jj++;
						}
						number>>=1;
					}
					Ans[row_cntr][0]=A[0];
					row_cntr++;
				}
			}
		}
	}

	return Ans;
}

NODE* BSTins(NODE* T, int x){ //inserts a node into a tree
	if(T==NULL){
		T=init(x);
		return T;
	}
	if(x<T->key){
		T->left=BSTins(T->left, x);
	}
	else{
		T->right=BSTins(T->right, x);
	}

}

NODE* BSTcons(int* A, int n){ //constructs a tree from an array
	int i;
	NODE* T=NULL;
	for(i=0;i<n;i++){
		T=BSTins(T, A[i]);
	}
	return T;
}

void preorder(NODE* T){ //preorder printing
	if(T==NULL) return;

	cout<<T->key<<"   ";
	preorder(T->left);
	preorder(T->right);
	return;
}

void inorder(NODE* T){ //inorder printing
	if(T==NULL) return;

	inorder(T->left);
	cout<<T->key<<"   ";
	inorder(T->right);
	return;
}

void prntree(NODE* T){ //prints a tree in preorder and inorder
	cout<<"\tPreorder\t\t:";
	preorder(T);
	cout<<"\n\tInorder \t\t:";
	inorder(T);
	cout<<endl;
	return;
}

bool BSTsame(NODE* T1, NODE* T2){ //checks if two BSTs are identical
	if(T1==NULL && T2==NULL) return true;
	else if(T1==NULL || T2==NULL) return false;
	return ((T1->key==T2->key) && BSTsame(T1->left, T2->left) && BSTsame(T1->right, T2->right));
}

void BSTfree(NODE* T){ //frees memory allocated to a BST
	if(T==NULL) return;
	BSTfree(T->left);
	BSTfree(T->right);
	free(T);
	return;
}

void checkall(NODE* T, int** A, int rows, int cols){ //checks if all generated sequences correspond to the same tree
	int i, cnt=0;
	NODE* CHECK;
	for(i=0;i<rows;i++){
		CHECK=BSTcons(A[i], cols);
		if(!(BSTsame(T, CHECK))) cnt++;
		BSTfree(CHECK);
	}
	printf("+++ Checking all sequences\n");
	if(cnt==0){
		printf("\tAll trees match\n");
	}
	else{
		printf("\t%d trees do not match\n", cnt);
	}
	return;
}
int main(){
	int* A, n, i, j, cnt;
	cin>>n;
	A = (int*)malloc(n*sizeof(int));
	for(i=0;i<n;i++){
		cin>>A[i];
	}

	//counting and finding all sequences
	cnt=countseq(A,n);
	cout<<"\n+++ Sequence count\n";
	cout<<"\tTotal number of sequences = "<<cnt<<endl;
	int** ans=findallseq(A, n);

	cout<<"\n+++ All sequences\n";
	for(i=0;i<cnt;i++){
		printf("\tSequence %-6d: ", i+1);
		for(j=0;j<n;j++){
			cout<<ans[i][j]<<" ";
		}
		cout<<endl;
	}

	//making a tree and checking with all sequence trees
	NODE* T;
	T = BSTcons(A,n);
	cout<<"+++ BST constructed from input array\n";
	prntree(T);

	checkall(T, ans, cnt, n);
	return 0;
}