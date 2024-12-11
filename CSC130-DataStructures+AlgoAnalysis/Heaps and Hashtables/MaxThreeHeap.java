//===================================================
//	Jeremy Vuong
//	Student ID: 302789539
//	Spring '22
//	CSC 130: Data Structures and Algorithm Analysis 
//===================================================
import java.util.Arrays;
import java.lang.*;

/*
 * public MaxThreeHeap();				// constructor
 * public void insert(int x);			// method to insert element into MaxThreeheap
 * public void deleteMax();				// method to delete the maximum value in MaxThreeHeap
 * private void percolateUp();			// method to correct heap order after insertion
 * private void percolateDown();		// method to correct heap order after deletion
 * public int parent(int childIndex);	// method to get the index of the parent of the given a child's index
 * public int largestChild(int parent);	// method to get the index of largest child given the parent's index
 * public void print();					// method to print MaxThreeHeap
 */

public class MaxThreeHeap{
	
	int heapSize = 0;		// # of elements in the heap
	private int[] heap;		// array to store the heap
	
	// constructor
	public MaxThreeHeap() {
		heapSize = 0; 
		heap = new int[heapSize+1];
	}
	
	// method to insert element into MaxThreeheap
	public void insert(int x) {
		heapSize++;						// update heap size
		int[] tempArr = heap;			// temporary array to store old heap
		heap = new int[heapSize+1];		// new heap of size+1
		
		// for loop to copy old heap into new heap
		for (int i = 1; i < tempArr.length; i++) {
			heap[i] = tempArr[i];
		}
		
		heap[heapSize] = x;	// insert element into tail of heap
		
		if(heapSize > 1) {	// if there is more than 1 element in the heap
			percolateUp();	// correct order of heap
		}

	}
	// method to delete the maximum value in MaxThreeHeap
	public void deleteMax() {
		heap[1] = heap[heapSize];		// swap values with last element
		heapSize--;						// reduce heap size
		int[] tempArray = heap;			// temporary array to store old heap
		heap = new int[heapSize+1];		// new heap of size-1
		
		// for loop to copy old heap into new heap
		for (int i = 1; i < tempArray.length-1; i++) {
			heap[i] = tempArray[i];
		}
		
		percolateDown();	// correct order of heap
		
	}
	
	// method to correct heap order after insertion
	private void percolateUp() {
		int currIndex = heapSize; 		// start at end of array
		int parent = parent(currIndex);	// parent index
		
		while (heap[currIndex] > heap[parent] // while current node is bigger than its parent 
				&& heap[parent] > 0) {		// and the parent is not index 0 (index 0 is empty)
			
				int temp = heap[parent]; 		// set temp = parent value
				heap[parent] = heap[currIndex];	// swap current node with... 
				heap[currIndex] = temp;			// its parent
				currIndex = parent;				// update current index
				parent = parent(currIndex);		// update parent
		}
	}
	
	// method to correct heap order after deletion
	private void percolateDown() {
		int parent = 1;								// index of current parent
		int largestChild = largestChild(parent);	// index of largest child of current parent								
		
		
		while(largestChild <= heapSize && heap[parent] < heap[largestChild]) {	// while parent is less than largest child 
			int temp = heap[parent];					// set temp = root value
			heap[parent] = heap[largestChild];			// swap root with... 
			heap[largestChild] = temp;					// largest child 
			parent = largestChild;						// proceed to next sub tree
			largestChild = largestChild(parent);		// largest child of next parent
		}
	}
	
	// method to get the index of parent given a child's index
	public int parent(int childIndex) {
		return ((int) Math.ceil((childIndex-1.0)/3.0));	
	}
	
	// method to get the index of largest child
	public int largestChild(int parent) {
		int child1 = (parent*3)-1;		// index of left child
		int child2 = (parent*3);		// index of middle child
		int child3 = (parent*3)+1;		// index of right child
		
		if (child1 == heapSize) {		//if there is only one child
			return child1;				//return left child's index
		}else if (child3 <= heapSize){	// use this algorithm if there are 3 children
			if (heap[child1] > heap[child2] && heap[child1] > heap[child3]) {	// if left child is largest
				return child1;													// return left child's index
			}else if (heap[child2] > heap[child3]) {	// else compare middle child and right child 
				return child2;							// then return the index of larger child
			}else {
				return child3;
			}
		}else if(child2 <= heapSize) {			// if there are only two children
			if(heap[child2] > heap[child1]) {	// compare the two children and return the index of the larger child
				return child2;
			}else {
				return child1;
			}
		}
		
		return heapSize+2;		// return an out of bounds index because there are no children

	}
	
	// method to print MaxThreeHeap
	public void print() {
		for (int i = 1; i <= heapSize; i ++) {
			System.out.print(heap[i] + " ");
		}
	}
}
