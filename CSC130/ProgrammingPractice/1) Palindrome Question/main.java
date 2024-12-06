//===================================================
//	Jeremy Vuong
//	Student ID: 302789539
//	Spring '22
//	CSC 130: Data Structures and Algorithm Analysis 
//	Last updated: 02/17/2022
//===================================================
import java.util.Stack;

public class main {
	public static void main(String[] args) {

//		Create even linked list
		LinkedListNode node7 = new LinkedListNode(1, null);
		LinkedListNode node6 = new LinkedListNode(2, node7);
		LinkedListNode node5 = new LinkedListNode(3, node6);
		LinkedListNode node4 = new LinkedListNode(4, node5);
		LinkedListNode node3 = new LinkedListNode(4, node4);
//		LinkedListNode node2 = new LinkedListNode(3, node3);
		LinkedListNode node1 = new LinkedListNode(2, node3);
		LinkedListNode head = new LinkedListNode(1, node1);
		
//		Create odd linked list
		LinkedListNode nodeF = new LinkedListNode(3, null);
		LinkedListNode nodeE = new LinkedListNode(2, nodeF);
		LinkedListNode nodeD = new LinkedListNode(1, nodeE);
		LinkedListNode nodeC = new LinkedListNode(0, nodeD);
		LinkedListNode nodeB = new LinkedListNode(1, nodeC);
		LinkedListNode nodeA = new LinkedListNode(2, nodeB);
		LinkedListNode head2 = new LinkedListNode(3, nodeA);
		
//		Print even list
		System.out.print("List = ");
		System.out.print(head);
		System.out.print(node1);
//		System.out.print(node2);
		System.out.print(node3);
		System.out.print(node4);
		System.out.print(node5);
		System.out.print(node6);
		System.out.print(node7);
		System.out.println();
		System.out.println(isPalindrome(head));		//call method, print result	
		
//		Print odd list
		System.out.print("List = ");
		System.out.print(head2);
		System.out.print(nodeA);
		System.out.print(nodeB);
		System.out.print(nodeC);
		System.out.print(nodeD);
		System.out.print(nodeE);
		System.out.print(nodeF);
		System.out.println();
		System.out.println(isPalindrome(head2));	//call method, print result	
	}
//=========================================================================================================
//	METHODS
			public static boolean isPalindrome(LinkedListNode head) {
		//		Initialize two node pointers
				LinkedListNode slow = head;	// slow pointer
				LinkedListNode fast = head;	// fast pointer
				
				Stack<Integer> stack = new Stack<Integer>();	// create integer stack
				
		//		Push elements from first half of linked list onto stack
				while (fast != null && fast.next != null) {
					stack.push(slow.data);
					slow = slow.next;
					fast = fast.next.next;	// fast pointer should move 2 positions 
											// for ever 1 position that slow moves
				}
		//		Has odd num of elements, so skip middle
				if (fast != null) {
					slow = slow.next;
				}
			
				while (slow != null) {
					int top = stack.pop().intValue();
					
		//			If values are different, then its not a palindrome
					if (top != slow.data) {
						return false;
					}
					slow = slow.next;
				}
				return true;
			}
	
}
		
	
	
	
