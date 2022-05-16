/*
 * Binary Search Tree #2
 *
 * Data Structures
 *
 * Department of Computer Science
 * at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>



typedef struct node {
	int key;
	struct node *left;
	struct node *right;
} Node;

/* for stack */
#define MAX_STACK_SIZE		20
Node* stack[MAX_STACK_SIZE];
int top = -1;

Node* pop();
void push(Node* aNode);

/* for queue */
#define MAX_QUEUE_SIZE		20
Node* queue[MAX_QUEUE_SIZE];
int front = -1;
int rear = -1;

Node* deQueue();
void enQueue(Node* aNode);


int initializeBST(Node** h);

/* functions that you have to implement */
void recursiveInorder(Node* ptr);	  /* recursive inorder traversal */
void iterativeInorder(Node* ptr);     /* iterative inorder traversal */
void levelOrder(Node* ptr);	          /* level order traversal */
int insert(Node* head, int key);      /* insert a node to the tree */
int deleteNode(Node* head, int key);  /* delete the node for the key */
int freeBST(Node* head); /* free all memories allocated to the tree */

/* you may add your own defined functions if necessary */


void printStack();



int main()
{
	char command;
	int key;
	Node* head = NULL;

	printf("-----허정윤 2021041047-----\n");
	do{
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z                                       \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initializeBST(&head);
			break;
		case 'q': case 'Q':
			freeBST(head);
			break;
		case 'i': case 'I':
			printf("Your Key = ");
			scanf("%d", &key);
			insert(head, key);
			break;
		case 'd': case 'D':
			printf("Your Key = ");
			scanf("%d", &key);
			deleteNode(head, key);
			break;

		case 'r': case 'R':
			recursiveInorder(head->left);
			break;
		case 't': case 'T':
			iterativeInorder(head->left);
			break;

		case 'l': case 'L':
			levelOrder(head->left);
			break;

		case 'p': case 'P':
			printStack();
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initializeBST(Node** h) {
	/* if the tree is not empty, then remove all allocated nodes from the tree*/
	if(*h != NULL)
		freeBST(*h);

	/* create a head node */
	*h = (Node*)malloc(sizeof(Node));
	(*h)->left = NULL;	/* root */
	(*h)->right = *h;
	(*h)->key = -9999;

	//stack에 쓸 top
	top = -1;
	//queue에 쓸 front, rear
	front = rear = -1;

	return 1;
}



void recursiveInorder(Node* ptr)
{
	if(ptr) {
		//inorder 순서대로.
		recursiveInorder(ptr->left);
		printf(" [%d] ", ptr->key);
		recursiveInorder(ptr->right);
	}
}

/**
 * textbook: p 224
 */
void iterativeInorder(Node* node)
{
	for(;;node=node->right){
		//오른쪽으로 노드를 옮기면서 반복.
		//왼쪽으로 노드를 옮기면서 push, 마지막에 pop
		
		for(; node; node=node->left) push(node);
		node = pop();

		//leaf노드는 왼쪽으로 옮겨지지 않고 다시 pop하여 올라감.
		//그리고 다시 push-pop을 반복하면 부모노드가 아니라 그 위로 올라가서 다시 진행됨.

		//pop된 노드가 없으면 break
		if(!node) break;
		//노드의 key 출력
		printf(" [%d] ", node->key);
	}
}

/**
 * textbook: p 225
 */
void levelOrder(Node* ptr)
{
	//ptr이 없으면 return
	if(!ptr) return;

	enQueue(ptr);

	for(;;){
		ptr = deQueue();
		if(ptr){
			printf(" [%d] ", ptr->key);

			//선입선출이기때문에 left->right순서로 빠져나오게 된다.
			//그 아래 노드들은 저 둘이 dequeue된 이후 빠져나온다.
			if(ptr->left) enQueue(ptr->left);
			if(ptr->right) enQueue(ptr->right);
		}
		//ptr이 없으면 break
		else break;
	}
}


int insert(Node* head, int key)
{
	if(head == NULL){
		printf("Error : head is NULL\n");
		return 1;
	}
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;

	if (head->left == NULL) {
		head->left = newNode;
		return 1;
	}

	/* head->left is the root */
	Node* ptr = head->left;

	Node* parentNode = NULL;
	while(ptr != NULL) {

		/* if there is a node for the key, then just return */
		if(ptr->key == key) return 1;

		/* we have to move onto children nodes,
		 * keep tracking the parent using parentNode */
		parentNode = ptr;

		/* key comparison, if current node's key is greater than input key
		 * then the new node has to be inserted into the right subtree;
		 * otherwise the left subtree.
		 */
		if(ptr->key < key)
			ptr = ptr->right;
		else
			ptr = ptr->left;
	}

	/* linking the new node to the parent */
	if(parentNode->key > key)
		parentNode->left = newNode;
	else
		parentNode->right = newNode;
	return 1;
}


int deleteNode(Node* head, int key)
{
	//head나 head->left가 NULL이면 에러 메시지 출력후 return
	//head는 연결이 안되었단 이야기고, head->left는 root가 없단 의미.
	if(head == NULL || head->left == NULL) {
		printf("\nError : Nothing to delete\n");
		return -1;
	}

	Node* root = head->left;

	Node* parent = NULL;
	Node* ptr = root;

	while((ptr !=NULL) && (ptr->key != key)) {
		if(ptr->key != key) {
			parent = ptr;

			//key의 크기에 따라 ptr을 이동.
			//결과적으로 ptr->key == key인 ptr을 찾아간다.
			if(ptr->key >key) ptr = ptr->left;
			else ptr = ptr->right;
		}
	}

	//못 찾은 경우(위의 반복문이 ptr== NULL로 종료된 경우)
	if(ptr == NULL){
		printf("No node for key [%d]\n", key);
		return -1;
	}

	//좌우 노드가 모두 NULL인 경우(그러나 ptr은 NULL이 아닌 경우.)
	if(ptr->left == NULL && ptr->right == NULL)
	{	
		//parent가 존재하면 
		if(parent != NULL) {
			//parent를 이용해서 ptr과 연결을 끊고 
			if(parent->left == ptr) parent->left = NULL;
			else parent->right = NULL;
		} 
		//parent가 없으면 (위의 while문 안 if문이 실행되지 않음. ==처음 ptr에서 key를 찾음)
		else {
			//head에서 연결 해제
			head->left = NULL;
		}
		//ptr 자체도 해제
		free(ptr);
		return 1;
	}

	//왼쪽 또는 오른쪽 하나만 NULL인 경우.(둘다이거나, ptr이 NULL은 아님.)
	if ((ptr->left == NULL || ptr->right == NULL))
	{
		Node* child;
		//NULL이 아닌 노드를 child로 받아둔다.
		if (ptr->left != NULL) child = ptr->left;
		else child = ptr->right;

		//parent가 NULL이 아닌경우(root가 ptr이 아닌경우.)
		if(parent != NULL)
		{
			if(parent->left == ptr) parent->left = child;
			else parent->right = child;
		} 
		//root가 ptr
		else {
			//child를 root로 한다.
			root = child;
		}

		//ptr을 해제한다.
		free(ptr);
		return 1;
	}

	//여기를 왔다는 것은 좌우 노드가 모두 NULL이 아닌 경우.
	Node* candidate;
	parent = ptr;

	candidate = ptr->right;

	//candidate의 왼쪽이 NULL일 때까지 내려간다.
	//이때는 당연히 candidate가 가장 작은 노드가 된다.
	while(candidate->left != NULL)
	{
		parent = candidate;
		candidate = candidate->left;
	}

	//parent의 right가 candidate면 candidate의 오른쪽을 parent의 오른쪽에 연결.
	//그렇지 않으면 candidate의 오른쪽을 parent 오른쪽에 연결
	//위의 while문에 의해 candidate의 왼쪽은 NULL이다.
	if (parent->right == candidate) parent->right = candidate->right;
	else parent->left = candidate->right;

	//ptr의 key를 candidate의 key로 바꾼다.
	ptr->key = candidate->key;

	//candidate를 해제한다.
	free(candidate);
	return 1;
}


void freeNode(Node* ptr)
{
	if(ptr) {
		freeNode(ptr->left);
		freeNode(ptr->right);
		free(ptr);
	}
}

int freeBST(Node* head)
{

	if(head->left == head)
	{
		free(head);
		return 1;
	}

	Node* p = head->left;

	freeNode(p);

	free(head);
	return 1;
}



Node* pop()
{
	//top<0이란 것은 비어있단 의미.
	if(top<0) return NULL;
	//비어있지 않으면 stack의 top값을 리턴하고, top 값에서 1을 뺀다.
	return stack[top--];
}

void push(Node* aNode)
{
	//top을 1을 넣고 top을 인덱스로 한 stack의 값에 aNode를 넣는다.
	stack[++top] = aNode;
}



Node* deQueue()
{
	//front와 rear가 같으면 빈 queue
	if(front == rear) {
		// printf("\nError : Queue is Empty\n");
		return NULL;
	}
	//front가 최대 size를 넘으면 다시 1부터
	//안넘으면 나머지 연산이니 그대로.
	front = (front+1) % MAX_QUEUE_SIZE;
	return queue[front];
}

void enQueue(Node* aNode)
{
	//size를 넘으면 1부터, 아니면 그대로.
	rear = (rear+1) % MAX_QUEUE_SIZE;

	//'위의 연산을 거쳐서' front==rear면 이미 full이었던 queue
	//이미 front==rear이면 빈 queue
	if(front==rear){
		//printf("\nError : Queue is full\n");
		return;
	}
	queue[rear] = aNode;
}

//stack 출력 함수
void printStack()
{
	printf("--- stack ---\n");
	for(int i=0;i <= top;i++){
		printf("stack[%d] = %d\n", i, stack[i]->key);
	}
}




