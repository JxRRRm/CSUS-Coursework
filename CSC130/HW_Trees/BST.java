//===================================================
//	Jeremy Vuong
//	Student ID: 302789539
//	Spring '22
//	CSC 130: Data Structures and Algorithm Analysis 
//===================================================
import java.util.Stack;
import java.lang.Integer;

//binary node class used to implement binary search tree
class binaryNode{
	
	int key;			// key
	binaryNode left;	// left child
	binaryNode right;	// right child
	
//	Constructors
	binaryNode(int key){
		this( key, null, null );
	}
	
	binaryNode(int theKey, binaryNode leftChild, binaryNode rightChild){
		key = theKey;
		left = leftChild;
		right = rightChild;
	}
	
	public String toString() {
		return "" +key;
	}
}

public class BST {
	
	// declare variables
	binaryNode root;
	Integer[] sortedArray = new Integer[25];
	
//	Methods
//	constructor
	public BST(binaryNode root){
		
		this.root = root; 
	}
	
//	i. Returns the key at the root
	public Integer getRoot() {
		
		if(root == null) {
			return null;
			
		}else {
			return root.key;
		}
		
	}
	
//	ii. Returns the left subtree of the root
	public BST getLeftTree() {
		
		// base case
		if(root == null || root.left == null) {
			
			return null;

		}else {
			BST leftTree = new BST(root.left);
			
			return leftTree;
		}

	}
	
//	iii. Returns the right subtree of the root
	public BST getRightTree() {
		
		// base case
		if(root == null || root.right == null) {
			
			return null;

		}else {
			BST rightTree = new BST(root.right);

			return rightTree;
		}

	}
	
//	iv.	Tests if the tree is empty
	public boolean isEmpty(){
		
		return root == null;
		
	}
	
//	v. Constructs an empty tree
	public BST(){
		
		root = null; 
		
	}
	
//	vi. Tests if the tree contains a given value
	public boolean contains(int key) {
		
		binaryNode focusNode = root;	// set pointer to start at the root
		
		if(focusNode == null)			//	handle base case (empty tree)
			return false;
		
		while(focusNode.key != key) {			
			if(key < focusNode.key) {			// if target key is smaller than focus key
				focusNode = focusNode.left;		// traverse left & update focus node
				
			}else {								// else, traverse right
				focusNode = focusNode.right;	// & update focus node
			}
			
			if(focusNode == null)		// if focus node == null, the key
				return false;			// was not found, return false
		}								// loop while key is not found
		
		return true;				// key found, return true
		
	}
	
//	vii. Insert a new integer into the BST
	public void insertNode(int key) {
		
		binaryNode newNode = new binaryNode(key);

		if(root == null) {		// base case (inserting in empty tree)
			root = newNode;		
			
		}else {
			
			binaryNode myNode = root;	// pointer to traverse tree, starts at root
			binaryNode parent;			// pointer to keep track of parent node
			
			while(true) {				// loop until node is inserted
				
				parent = myNode;		// update parent
				
				if(key < myNode.key) {		// if key is smaller than focus key
					myNode = myNode.left;	// traverse left
					
					if(myNode == null) {		// check if slot is taken
						parent.left = newNode;	// insert here
						return;		// exit 
					}
						
				}else {							// larger key
						myNode = myNode.right;	// so traverse right
						
						if(myNode == null) {			// check if slot is taken
							parent.right = newNode;		// insert here
							return;		// exit 
						}
					}
				}
			}
		
	}
	
//	c.	in-order traversal using stack
	public Integer[] inOrder() {

		binaryNode focusNode = root;		// focus node initialized to root
		int i = 0;
		
		// handle empty tree case
		if(focusNode == null) {
			System.out.println("Unable to traverse because the tree is empty!");
			return null;
		}
		
		Stack<binaryNode> myStack = new Stack<binaryNode>();	// create empty stack
		
		// loop until there is no more children or until we empty the stack 
		while(focusNode != null || myStack.isEmpty() != true) {
			// loop to push nodes into the stack until we reach most left node
			while(focusNode != null) {		
				myStack.push(focusNode);		// push focus node into stack
				focusNode = focusNode.left;		// go left/check if there is a smaller key
			}	// when we exit this loop, focusNode == null
			 
				focusNode = myStack.pop();		// remove the top node from the stack and focus it
				sortedArray[i] = focusNode.key;	// store in value in array
				i++;  // tracker
				focusNode = focusNode.right;	// now focus right child of that node
		}
		

		return sortedArray;		// return sorted array
	}
	
//	used to print tree
	public String toString() {
		
		return "Root---------->" +root+ 
				"\nLeft Child---->"  +root.left+ 
				"\nRight Child--->" +root.right+ "\n";
	}

}

// d. 
class sortIntegers {
	BST myTree = new BST();

	public 	Integer[] sortIntegerArray(Integer[] array) {

		// for loop to insert every element in the array into a BST
		for(int i = 0; i < array.length; i++) {
			myTree.insertNode(array[i]);	
		}
									// use in-order traversal method to sort
		return myTree.inOrder();	// & return sorted array
	}
}
	
