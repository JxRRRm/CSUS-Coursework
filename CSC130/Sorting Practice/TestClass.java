import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.*;
class TestClass {
    public static void main(String args[] ) throws Exception {
        //BufferedReader
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        
        // Read input N from STDIN
        int N = Integer.parseInt(br.readLine());                
        while (1 > N || N > Math.pow(10,6)){
        	N = Integer.parseInt(br.readLine());
        }
        
        int[] A = new int[N];	// initialize integer array of size N
        
        // get array values from user input 
        // and use them to fill the array
        getList(A);
        
        int[] index = new int[N];		// integer array to store index
        
        // find indexes of each element 
        // as if the array was sorted
        for (int i = 0; i < N; i++) {		// start @ i = 0; for every element in A
        	for(int j = 0; j < N; j++) {	// compare with all other elements in A
        		if(A[i] > A[j])				// if A[i] > A[j]
        			index[i]++;				// index of A[i] +=1 for each A[j] < A[i]
        	}
        }
        
        // print answer
   	 	for (int i:index) {
   	 		System.out.print(i+" ");
   	 	}
    }
    
//  reads N space separated integers from STDIN
//  and inserts them into array A in the same order
    public static void getList(int[] list) {
    	BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
    	String inLine = null;
		try {
			inLine = br.readLine();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}    
        String[] inStr = inLine.trim().split("\\s+");
                
        for (int i = 0; i < inStr.length; i++) {
        	list[i] = Integer.parseInt(inStr[i]);
        }
        for (int i:list) {
        	if(i < 1 || i > Math.pow(10,6)) {
        		System.out.println("Try Again!");
        		getList(list);
        	}
        }
    }
}