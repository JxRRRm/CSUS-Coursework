//===================================================
//	Jeremy Vuong
//	Student ID: 302789539
//	Spring '22
//	CSC 130: Data Structures and Algorithm Analysis 
//	Last updated: 02/17/2022
//===================================================
import java.io.*;
import java.util.*;

public class Solution {
    public static void main(String[] args) {
//    	initialize variables
        int n = 0;	//# of cities
        int m = 0;  //# of space stations
        Scanner sc = new Scanner(System.in);	//	initialize scanner object to read/write input (stdin)
//      get n and m from stdin
//      constraints: 1 <= n <= 10^5 
//                   1 <= m <= n
        while ((n < 1 || n > Math.pow(10, 5)) || (m < 1 || m > n)) {
                n = sc.nextInt();	
                m = sc.nextInt();	
            }
// 		Initialize array of size m
        int[] c = new int[m];
//      Call function to store space station locations in array
        storeLocations(n, c, sc);	
// 		Call function
        System.out.println(flatlandSpaceStations(n, c));	// print result to stdout
    }
//	METHODS 
//  
public static int flatlandSpaceStations(int n, int[] c) {
    int max = 0;    
    Arrays.sort(c);
//  For loop to calculate distance between space stations excluding end points
    for (int i = c.length-1; i > 0; i--) {           
        max = Math.max(max, (c[i]-c[i-1])/2);  
    }
// 	end-point cases
    max = Math.max(max, c[0] - 0);	
    max = Math.max(max, ((n-1) - c[c.length-1]));	
    return max;        //return largest distance
    
}
// Recursive function to store space station locations into array
public static void storeLocations(int n, int[] c, Scanner sc) {
	int size = 0;
	while (size != c.length){		// while array is not full
//		For loop to fill array
		for (int i = 0; i < c.length; i++){
			c[i] = sc.nextInt();	
//			if values are out of bounds, invoke recursion
			if (c[i] < 0 || c[i] > n-1){
				storeLocations(n, c, sc);
			}
			size++;
		}

	}
}
}