
public class mergeSort {
	private static int[] list = {3,4,5,6,1,9,2,5,6};

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		//	Merge Sort
		System.out.print("Unsorted------> ");
		print(list);
		mSort(list);
		System.out.print("Merge Sorted--> ");
		print(list);
		System.out.println();

}

//===============================================================================================
// MERGE SORT
 public static void mSort(int[] list) {
	 int size = list.length;	// size of starting list
 
 if (size < 2) {	// return if we are unable to split the list again
	 return;
 }
 
 int mid = size/2;	// middle index of list
 
 int[] listL = new int[mid];			// new list to store left half of list
 int[] listR = new int[size-mid]; 		// new list to store right half of list
 
 // copy left half of list into new list(line45)
 for (int i = 0; i < mid; i++) {
	 listL[i] = list[i];
 }
 
 // copy right half list into new list(line46)
 for (int i = mid; i < size; i++) {
	 listR[i-mid] = list[i];
 }
 
 // recursively split list in half
 mSort(listL);
 mSort(listR);
 
 // merge when we return from the recursive calls
	 merge(list, listL, listR);
	 
 }
//===============================================================================================
// MERGE 
 // merges the lists split in mergeSort
 private static void merge(int[] list, int[] listL, int[] listR) {
	 int sizeL = listL.length;		// size of left half of list
	 int sizeR = listR.length;	// size of right half of list
	 int i = 0;	// left half iterator
	 int j = 0;	// right half iterator
	 int k = 0; // merged array iterator
	 
 /* while i and j are valid indexes for their respective lists
  * if the left value <= right value, insert left value into merged list
  * else insert right value into merged list
  * */
	 while (i < sizeL && j < sizeR) {	 
		 if(listL[i] <= listR[j]) {
			 list[k] = listL[i];
			 i++;	// increment left half iterator
		 }else {
			 list[k] = listR[j];
			 j++;	// increment left half iterator
		 }
		 k++;	// increment merged array iterator
	 }
	 
 // add remaining elements in left and right lists into merged list
		 while(i < sizeL) {
			 list[k] = listL[i];
			 i++;
			 k++;
		 }
		 
		 while(j < sizeR) {
			 list[k] = listR[j];
			 j++;
			 k++;
		 }
	 }
//===============================================================================================
//print 
//prints every element of a list 
public static void print(int[] list) {
	 for (int i:list) {
		System.out.print(i+" ");
	 }
	 System.out.println();
}
}
