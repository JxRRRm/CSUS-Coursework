
public class radixSort {

	private static int[] list = {34, 37, 83, 51, 84, 22, 43};
	private static int radix = 10;
	public static void main(String[] args) {
		// TODO Auto-generated method stub
	    System.out.print("UNSORTED-------------->");
		print(list);
		rSort(list, list.length);
	    System.out.print("RADIX SORTED---------->");
		print(list);
		
	}
	
//===============================================================================================
//COUNT SORT
// start looking at current #'s place (i.e 1's 10s 100's), 
// count the # of occurrences of a digit in the #'s place
// and store # of occurences of a digit in count[digit]
// then changes count[] to store the values' location 
// then sorts the list according to #'s place
public static void countSort(int list[], int size, int place) {
	
	int sorted[] = new int[size];  	// sorted~ array
    int count[] = new int[radix];		// stores occurrences and location
    int i;		// iterator
    
    // fill count array with 0s    
    for (int j = 0; j < 10; j++) {
    	count[j] = 0;
    }
 
    // count occurrences of digit (0-9) in current number's place (1's, 10's, 100's place etc.)
    for (i = 0; i < size; i++) {			
    	count[(list[i] / place) % 10]++;	// 
    }
//    System.out.println("Digit Place: "+place+"'s Place");
//    System.out.print("index/digit----------->[");
//    System.out.print("0 1 2 3 4 5 6 7 8 9] \n");
//    System.out.print("Number of Occurrences-> ");
//    print(count);
    

    // Change count[i] so that count[i] now contains
    // actual position of this digit in sorted[]
    for (i = 1; i < 10; i++) {
    	count[i] += count[i - 1];
    }

//    System.out.print("actual position-------> ");
//    print(count);
//    System.out.print("Current List---------->");
//    print(list);
    
    // construct the sorted~ array
    for (i = size - 1; i >= 0; i--) {
//    	System.out.println("list index i = "+i);
    	int index = count[(list[i] / place) % 10] - 1;	// 
//    	System.out.println("sorted index = "+index);
    	sorted[index] = list[i];
        count[(list[i] / place) % 10]--;
//        System.out.print("sorted[]-------------->");
//        print(sorted);
    }
 
    // Copy the sorted~ list to original list, so that the list now
    // contains sorted~ numbers *according to current digit*
    for (i = 0; i < size; i++) {
    	list[i] = sorted[i];
    }
//    print(list);
}
//=============================================================================================== 
//RADIX SORT
// find max value in the list and perform radix sort
public static void rSort(int list[], int size) {
	
    int max = 0;
    
    // find max value in list
    for(int i = 0; i < size; i++) {
    	max = Math.max(max, list[i]);
    }    
//    System.out.println("Max: "+max);
 
    // perform counting sort for every possible #'s place 
    for (int place = 1; max/place > 0; place *= radix)
        countSort(list, size, place);
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
