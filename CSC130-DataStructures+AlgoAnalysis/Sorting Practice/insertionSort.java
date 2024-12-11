
public class insertionSort {
	
	private static int[] list = {3,4,5,6,1,9,2,5,6};
	
	public static void main(String[] args) {
	// TODO Auto-generated method stub
	//Insertion Sort	
	System.out.print("Unsorted----------> ");
	print(list);
	
	iSort(list);	// call insertion sort
	
	System.out.print("Insertion Sorted--> ");
	print(list);
	System.out.println();
}

	
	
	
// INSERTION SORT
 public static void iSort(int[] list) {
	 //start our focus on index 1 because first element is considered sorted
	 for(int i = 1; i < list.length; i++) {
		 int j = i-1;			//predecessor index
		 int current = list[i]; //current element
		 
		 // loop while predecessor index is within bounds 
		 // and predecessor value is greater than current
		 while (j >= 0 && list[j] > current) {
			 list[j+1] = list[j];		// swap predecessor with the index to the right
			 j = j-1;					// move onto next predecessor
		 }
		 
		 list[j+1] = current;	// insert current element in correct slot
			 
		 }
	 }


//===============================================================================================
 //print 
 // prints every element of a list 
 public static void print(int[] list) {
	 
	 for (int i:list) {
		System.out.print(i+" ");
	 }
	 
	 System.out.println();
 }
}
