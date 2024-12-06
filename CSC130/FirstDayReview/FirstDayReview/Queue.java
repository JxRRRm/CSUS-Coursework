
public class Queue 
{
//	Constructor
	char queue[] = new char[5];
	//int size;
	int front; 
	int back;
//==========================================================================================
	public void enqueue(char x)
	{
		queue[back] = x;
		back++;
		//size++;
	}
//==========================================================================================
	public boolean isEmpty()
	{
		return front == back;
		//return size == 0;
	}
//==========================================================================================
	public void printElement(int index)
	{
//		Handle case for out of bounds index
		/*
		 if (index >= size+1)
			System.out.println("INDEX OUT OF BOUNDS!");
		*/
//		Handle case to print element at the back of queue (n-1)
		if (index == back)
		{
			System.out.println("Element at index " + (index-1) + " is: "  );
			System.out.print(queue[index-1] +"\n");
		}
//		Handle all normal cases
		else 
		{
			System.out.println("Element at index " + index + " is: "  );
			System.out.print(queue[index] +"\n");
		}
	}
//==========================================================================================
}
