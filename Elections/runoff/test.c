#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() 
{ 
    int arr[] = {11, 15, 6, 8, 9, 10}; 
    int n = sizeof(arr)/sizeof(arr[0]); 
    int x = 6; 
  
    // Delete x from arr[] 
    n = deleteElement(arr, n, x); 
  
    cout << "Modified array is \n"; 
    for (int i=0; i<n; i++) 
       cout << arr[i] << " "; 
  
    return 0; 
}