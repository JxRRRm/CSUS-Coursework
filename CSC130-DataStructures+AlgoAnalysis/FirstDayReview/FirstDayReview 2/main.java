
//==========================================================================================
//Jeremy Vuong
//CSC 130 Section 4: Data Structures and Algorithm Analysis
//Spring 2022
//File: Main.java
//Last Updated: February 6, 2022
//==========================================================================================

import java.util.Arrays;

public class main 
{
	public static void	main(String []args) 
	{
//		Tests for Queue.java
		Queue q = new Queue();
		
		//Test q.isEmpty() for true case
		System.out.print(q.isEmpty()+"\n");
//		Fill array
		q.enqueue('a');
		q.enqueue('b');
		q.enqueue('c');
		q.enqueue('d');
		q.enqueue('e');
		
//		Print length of the array
		System.out.print(q.queue.length + "\n");
		/*
//		Print every index
		for (int i = 0; i < q.back; i++)
		{
			q.printElement(i);
		}
		
//		Test q.isEmpty() for false case
		System.out.print(q.isEmpty()+"\n");
		
		//Print front element
		q.printElement(q.front);
		
		//Print back element
		q.printElement(q.back);
		
		
		
//		Test for find() method
//		Create array literal for testing (unsorted)
		int arr[] = new int[] {8,2,7,3,1,0,9,4,6,5};
		for (int i = 0; i < arr.length; i++) 
		{
			System.out.print(arr[i]+", ");
		}
			System.out.print("\n");
			
//		Test for true cases (0-9)
		for (int j = 0; j < arr.length; j++)
		{
			System.out.print("Is " +arr[j]+ " in the array?\n");
			System.out.print(find(arr, arr[j])+"\n");		
		}
		
//		Test for false cases (10-19)
		for (int j = 0; j < arr.length; j++)
		{
			System.out.print("Is " +(arr[j]+10)+ " in the array?\n");
			System.out.print(find(arr, arr[j]+10)+"\n");		
		}
		
//		Check to see if the array is sorted after calling find()
		for (int k = 0; k < arr.length; k++)
		System.out.print(arr[k]+", ");

	}
	
	*/
	
	
	
//==========================================================================================
//FUNCTIONS
	
//Find an integer k in a sorted array
//requires array is sorted
//returns if k is in the array
	public static boolean find(int[] arr, int k) 
	{		
//		Check if array is sorted
		for (int i = 0; i < arr.length-1; i++)
		{
//			If the array is not sorted
			if (arr[i] > arr[i+1]) 
//				Sort the array
				Arrays.sort(arr);
			break;
		}
//		Find key
		for (int j = 0; j < arr.length; j++) 
		{
			if (arr[j] == k) 
				{
//					Return if if key is found
					return true;
				}
		}
//		Return false if key is not found
		return false;
	}
}//END

