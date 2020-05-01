//Written by Hardik Tibrewal
// 18EC10020

#include <bits/stdc++.h>

using namespace std;

typedef struct node{ //struct for the node of the tree
	int key;
	node* left;
	node* right;
	node* prnt;
} NODE;

NODE* init(NODE* parent, int val){ //initialising a new node
	NODE* temp;
	temp = new NODE;
	temp->key=val;
	temp->left=NULL;
	temp->right=NULL;
	temp->prnt=parent;
	return temp;
}

NODE* maketree(NODE* root, int* keys, int* lefts, int* rights, int n, int i){ //making a new node from an array given the number of nodes in its left and right subtree
	NODE* temp=init(root, keys[i]);
	if(lefts[i]!=0){
		temp->left=maketree(temp, keys, lefts, rights, n, i+1);
	}
	if(rights[i]!=0){
		temp->right=maketree(temp, keys, lefts, rights, n, i+lefts[i]+1);
	}
	return temp;
}

NODE* readtree(){ //reading an array and arrays for left and right nodes, and making a dummy node to help with skew and unskew
	int i, n, *keys, *lefts, *rights;
	cin>>n;
	keys=(int*)malloc(n*sizeof(int));
	lefts=(int*)malloc(n*sizeof(int));
	rights=(int*)malloc(n*sizeof(int));

	for(i=0;i<n;i++){
		cin>>keys[i]>>lefts[i]>>rights[i];
	}
	NODE* dummy;
	dummy=new NODE;
	dummy->right=maketree(dummy, keys, lefts, rights, n, 0);
	return dummy;
}

void preorder(NODE* T){ //function to print in preorder
	if(T==NULL) return;

	cout<<T->key<<"   ";
	preorder(T->left);
	preorder(T->right);
	return;
}

void inorder(NODE* T){ //function to print in inorder
	if(T==NULL) return;

	inorder(T->left);
	cout<<T->key<<"   ";
	inorder(T->right);
	return;
}

void prntree(NODE* T){ //printing the tree in preorder and inorder
	cout<<"--- Preorder listing\n\t";
	preorder(T);
	cout<<"\n--- Inorder listing\n\t";
	inorder(T);
	cout<<endl;
	return;
}

void swapchild(NODE* root){ //function to swap children at a node
	NODE* temp;
	temp=root->left;
	root->left=root->right;
	root->right=temp;
	return;
}

NODE* lrotate(NODE* root){ //left rotate at a node
	NODE* new_root=root->right;
	root->prnt=new_root;
	root->right=new_root->left;
	if(new_root->left!=NULL) new_root->left->prnt=root;
	new_root->left=root;
	return new_root;
}

NODE* rrotate(NODE* root){ //right rotate at a node
	NODE* new_root=root->left;
	root->prnt=new_root;
	root->left=new_root->right;
	if(new_root->right!=NULL) new_root->right->prnt=root;
	new_root->right=root;
	return new_root;
}

NODE* makeskew(NODE* dummy){ //makes a tree completely right skewed
	if(dummy->right==NULL) return dummy;

	while(dummy->right->left!=NULL){
		dummy->right=rrotate(dummy->right);
	}
	dummy->right=makeskew(dummy->right);
	return dummy;
}

void nodeswap(NODE* root){ //function to swap two nodes when they are arranged in right skew
	swapchild(root->right);
	root->right=rrotate(root->right);
	swapchild(root->right->right);
	return;
}

void bsort(NODE* dummy){ //bubble sort for linked list
	NODE* temp=dummy;
	int swaps = 1;
	while(swaps!=0){
		swaps=0;
		temp=dummy;
		while(temp->right->right!=NULL){
			if(temp->right->key > temp->right->right->key){
				nodeswap(temp);
				swaps++;
			}
			temp=temp->right;
		}
	}
	return;
}

void bal_left(NODE* dummy){ //balances the left skewed  left subtree at a root
	NODE* temp;
	if(dummy==NULL) return;
	temp=dummy->left;
	int i, cnt=0;
	while(temp!=NULL){
		cnt++;
		temp=temp->left;
	}
	if(cnt%2) cnt/=2;
	else cnt=cnt/2-1;

	for(i=0;i<cnt;i++){
		dummy->left=rrotate(dummy->left);
	}
	return;
}

void bal_right(NODE* dummy){ //balances a right skewed right subtree at a root
	NODE* temp;
	if(dummy==NULL) return;
	temp=dummy->right;
	int i, cnt=0;
	while(temp!=NULL){
		cnt++;
		temp=temp->right;
	}
	cnt/=2;

	for(i=0;i<cnt;i++){
		dummy->right=lrotate(dummy->right);
	}
	return;

}

void balance(NODE* root){ //balances both subtrees at a root
	if(root==NULL) return;
	bal_left(root);
	bal_right(root);
	balance(root->left);
	balance(root->right);
	return;
}

void rebalance(NODE* dummy){ //get a BST from a right skewed tree
	int i, cnt=0;
	NODE* temp=dummy->right;
	while(temp!=NULL){
		cnt++;
		temp=temp->right;
	}
	cnt/=2;

	for(i=0;i<cnt;i++){
		dummy->right=lrotate(dummy->right);
	}
	
	balance(dummy->right);
	return;
}


int main(){

	//calling all functions
	NODE* dummy=readtree();
	cout<<"\n+++ Initial Tree\n";
	prntree(dummy->right);

	dummy = makeskew(dummy);
	cout<<"\n+++ Tree made skew\n";
	prntree(dummy->right);

	bsort(dummy);
	cout<<"\n+++ Tree after sorting\n";
	prntree(dummy->right);

	rebalance(dummy);
	cout<<"\n+++ Tree after rebalancing\n";
	prntree(dummy->right);

	return 0;
}