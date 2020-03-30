# K18KR-OS-Project

Problem given:  

Write a multithreaded program that implements the banker's algorithm. Create n threads that request and release resources from the bank. The banker will grant the request only if it leaves the system in a safe state. It is important that shared data be safe from concurrent access. To ensure safe access to shared data, you can use mutex locks.  




Test case 1: 
Currently available resources (3 3 3)
Process   Allocated   Maximum Req. 
1 	      2 1 0	      6 4 3
2 	      4 3 5 	    7 3 1 
3 	      2 2 1	      3 2 3


Test case 2: 
Currently available resources (2 2 1)
Process 	Allocated 	Maximum Req. 
1 	      1 1 0     	6 7 8 
2 	      2 1 1 	    7 6 9 
3 	      0 1 2 	    6 5 7 
4 	      3 1 2 	    7 6 5 
