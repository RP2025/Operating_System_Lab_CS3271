Assignment to implement an interface (a set of functions) at the top of file(s) to achieve the following!

1. A file is conceived as collection of n  fixed sized  blocks of s bytes. For example, a file named "dd1" can be conceived as a collection of 2048 (n = 2048) blocks, where each block is of 4096 (s = 4096) bytes.
2. The n blocks in a file are numbered as 0 through n-1.
3. The blocks in a file can be accessed (read or written) in a random access manner. That is, the time to access the ith block in a file does not depend on the value of i.

A guideline for implementation of the functions are given below.

Say, the file dd1 is to contain 2048 number of blocks, each of size 4096 Bytes. The file can can be conceived as shown in the following figure.

![File as collection of blocks!](https://cs.iiests.ac.in/moodle/pluginfile.php/10054/mod_assign/intro/dd.png)

Note that the first 1024 Bytes is  kept for storing information like n (number of blocks), s (size of each block), etc.

1. Write a function "_int init\_File\_dd(const char \*fname, int bsize, int bno)_" that creates a file (if it does not exist) called fname of appropriate size (1024 + bsize\*bno Bytes) in your folder. If for some reason this functions fails, it returns -1. Otherwise it returns 0.
2. Write a function "_int read\_block(_const char \*fname_, int bno, char \*buffer)_" that reads the _bno_th block from the file _fname_ and puts the block in _buffer_. On success this function returns 1, returns 0 otherwise. This function gets the information n and s from the 1024 Bytes at the beginning of _fname_.
3. Write a function "_int write\_block__(const char \*fname__, int bno, char \*buffer)_" that overwrites the _bno_th block in the file _fname_ and with the data in _buffer_. On success this function returns 1, returns 0 otherwise. This function gets the information n and s from the 1024 Bytes at the beginning of _fname_.

Write a main function to demonstrate that your functions are working as desired.

Your code should properly indented and documented.

