//===================================================
//	Jeremy Vuong
//	Student ID: 302789539
//	Spring '22
//	CSC 130: Data Structures and Algorithm Analysis 
//	Last updated: 02/17/2022
//===================================================
public class LinkedListNode {
//	Instance variables
	public int data;
	public	LinkedListNode next;

//	Constructor
	public LinkedListNode(int data, LinkedListNode next) {
		setData(data);
		setNext(next);
	}

//	Getters
	public int getData() { 
		return data; 
	}
	
	public LinkedListNode getNext()	{ 
		return next; 
	}
	
//	Setters
	public void setData(int data) { 
		this.data = data; 
	}
	
	public void setNext(LinkedListNode next) { 
		this.next = next;	
	}
	
// toString() function returns this.data in form of a string
	public String toString() { 
		return data + ""; 
	}	
}
