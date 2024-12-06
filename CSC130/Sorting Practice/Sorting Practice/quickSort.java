
public class quickSort {

	private static int[] list = {3,4,5,6,1,9,2,5,6,5,3};
	

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.print("UNSORTED------> ");
		print(list);
		startQSort();
		System.out.print("QUICK SORTED--> ");
		print(list);
	}
	
// 
 // Quick sort with low index at 0 and high index at size-1
 public static void startQSort() {
	 //initialize high and low indexes
	 int low = 0;
	 int high = list.length-1;
	 qSort(low, high);
 }
//===============================================================================================
 //QUICK SORT
 // 
 public static void qSort(int low, int high) {
	 if(low < high) {
		 
		 int PI = median(low, high);	//set pivot index to median of 3 index

		 swap(low, PI);					// put pivot into list[0] (move pivot out the way)
		 PI = low;						// pivot index set to 0

		 // set pivot index to value returned 
		 //from calling partition method
		 PI = partition(PI, low, high);	
		 
		 //recursion
		 // quick sort left side
	     qSort(0, PI-1);
	     // quick sort right side
	     qSort(PI+1, high);
	     
	 }
 }
 
//===============================================================================================
 //PARTITION
 // partitions values less than pivot value into left side of the list
 // and partitions values greater than pivot into right side of list
 // pivot value is put in between at the end
 private static int partition(int PI, int low, int high){
     int leftPtr = low+1;	// left pointer 
     int rightPtr = high;	// right pointer
     int pivot = list[PI];	// pivot value
     
     // while not partitioned...
     while(leftPtr < rightPtr){ 	 
    	 
         if(list[leftPtr] <= pivot){	// if left pointed value is less than or equal to pivot
             leftPtr++;					// left pointer steps right
             
         }else if(list[rightPtr] > pivot){	// else if right pointed value is less than or equal to pivot
        	 rightPtr--;					// right pointer steps left

         }else {
             swap(leftPtr, rightPtr);		// else swap left pointed value with right pointed value
             leftPtr++;						// left pointer steps right
             rightPtr--;					// right pointer steps left
         }
     }
     
     while(list[leftPtr] > pivot) {		// while left pointed value is greater than the pivot
    	 leftPtr--;						// left pointer steps left
     }									// here, left pointer is less than or equal to pivot
     swap(low, leftPtr);				// swap pivot with left pointed value

     return leftPtr;					// return pivot index
 }
 
//===============================================================================================
//MEDIAN
// finds and returns the median index of a list given extreme and middle indexes
public static int median(int low, int high) {
	int mid = (high/2);		// middle index
	int a = list[low];		// low index value
	int b = list[mid];		// mid index value
	int c = list[high];		// high index value
	

	// if a < b < c OR c < b < a
	// middle index is median
	if ((a < b && b < c) || (c < b && b < a)) {
		return mid;		
	}
   // if b < a < c OR c < a < b 
	// low index is median
	else if ((b < a && a < c) || (c < a && a < b)) {
		return low;		
	}
	else
	// high index is median
		return high;	
}

//===============================================================================================
 //SWAP 
 // swaps list elements
 public static void swap(int left, int right) {
	 int temp = list[left];
	 list[left] = list[right];
	 list[right] = temp;
 }
//===============================================================================================
 // PRINT 
 // prints every element of a list 
 public static void print(int[] list) {
	 for (int i:list) {
		System.out.print(i+" ");
		 }
		 System.out.println();
	 }

}
