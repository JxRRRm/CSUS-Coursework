//==========================================================================================
//Jeremy Vuong
//CSC 130 Section 4: Data Structures and Algorithm Analysis
//Spring 2022
//File: Main.java
//Last Updated: February 6, 2022
//==========================================================================================


public class Main 
{
	public static void	main(String []args) 
	{
		//Tests for Queue.java
		Queue q = new Queue();
		//Test isEmpty() for true case
		System.out.print(q.isEmpty()+"\n");
		//Fill array
		q.enqueue('a');
		q.enqueue('b');
		q.enqueue('c');
		q.enqueue('d');
		q.enqueue('e');
		//Print length of the array
		System.out.print(q.arr.length + "\n");
		//Print every index
		for (int i = 0; i < q.back; i++)
		{
			q.printElement(i);
		}
		//Test isEmpty() for false case
		System.out.print(q.isEmpty()+"\n");
		//Print front element
		q.printElement(q.front);
		//Print back element
		q.printElement(q.back);
		
		
		
		//Test for find() method
		//Create array literal for testing
		int arr[] = new int[] {0,1,2,3,4,5,6,7,8,9};
		
		//Test for true cases (0-9)
		for (int i = 0; i < arr.length; i++)
		{
			System.out.print(i+ ": ");
			System.out.print(find(arr, i )+"\n");
		}
		
		//Test for false cases (10-19)
		for (int i = 0; i < arr.length; i++) 
		{
			System.out.print((i + 10)+": ");
			System.out.print(find(arr, (i + 10))+"\n");
		}
	}
	
	
	
	
	
//==========================================================================================
//METHODS
	
//Find an integer k in a sorted array
//requires array is sorted
//returns if k is in the array
public static boolean find(int[] arr, int k)
{
		for (int i = 0; i < arr.length; i++) 
		{
			if (arr[i] == k) 
				return true;
		}
		return false;
}

}//END

