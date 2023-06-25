					Assignment 1
       				      Shreyansh Singh
       				        2020CS10385

(Note: The analysis is done on my local machine, while running on css most of my processes were killed and css stopped working many times) 

Approach 1:

Idea: 		Implemented a basic O(n^3) algorithm.
Time Taken: 	307 milliseconds
Drawback: 	The time complexity is high, and the code will take very large time for large files.


Approach 2:

Idea: 		Implemented a O(k2 ∗ m4) algorithm (where k is the number of blocks in the input file). In this approach, I multiplied every element of non-empty blocks with each other and then add it to the appropriate place in the answer matrix.
Why attempted: 	This method uses the sparsity of the matrix, if the matrix is not sparse then this method would have taken O(n4) time, but for a sparse matrix time taken will be much less.
Time Taken: 	80 milliseconds
Results: 	3.8 times speedup.
Drawback: 	This does not uses the fact that matrix is stored in blocks.


Approach 3:

Idea: 		Implemented a O(k2 ∗ m3) algorithm (where k is the number of blocks in the input file). In this approach, I multiplied blocks pairwise (when needed) and added them to the corresponding block in the answer matrix.
Why attempted: 	This method uses the sparsity of the matrix and the block structure of the matrix.
Time Taken: 	28 milliseconds
Results: 	2.9 times speedup.
Drawback: 	The code is sequential, thus it takes time.


Approach 4:

Idea: 		Optimised the previous approach using OpenMP, omp parallel for is used which makes the outer loop run parallelly, in this method task assigned to threads is managed by OpenMP.
Why attempted: 	This method runs the parallelly using multithreading, which reudces the time as compared to sequential execution.
Time Taken: 	16 milliseconds
Results: 	1.75 times speedup.
Drawback:	Diving of tasks among threads is handled by OpenMP, thus not very efficient.

Approach 5:

Idea: 		Optimised the previous approach using OpenMP, threads run parallelly to multiply different pairs of blocks and the multiplication of two blocks is defined as a task which could be run by threads parallelly independent of each other. In the end when accumulating the multiplied blocks critical is used so that 2 threads should not update same block.
Why attempted: 	This method divides the code into multiple tasks which could be run parallelly, thus improving the time.
Time Taken: 	4 milliseconds
Results: 	4 times speedup.

