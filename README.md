Project Overview
The Line Editor is a command-line program built in C++ that allows users to read, edit, and write text files with functionalities like adding, updating, deleting lines, searching for words, and undo/redo operations. This program demonstrates the use of dynamic memory management, file I/O, and data structure manipulation to build a practical utility tool

Justification
Command-line Argument Parsing: In response to command-line inputs, the application modifies the file path.
Read and Write File Functions: File opening and content writing are handled by the read and write functions, which also create a new file if one doesn't already exist.
CRUD Operations: Functions that add, remove, and update lines in the buffer are included in CRUD operations.
Search Functionality: A simple function looks for a word in the buffer and, if it is found, outputs the line number.
Print Buffer: Shows the buffer's contents.

Extensions and Improvements
Error Handling: Additional checks for file operations and memory allocations could be added.
Dynamic Line Length: Currently, the line length is fixed. This could be dynamically adjusted based on the content.
Undo/Redo Operations: Implement a stack to keep track of operations for undo and redo functionality.

Basic Structure of the Program
1.	Command Line Argument Parsing: Handling different counts of arguments to manage file and directory operations.
2.	Buffer Management: Implement a buffer to store up to 25 lines of text.
3.	File Operations: Reading from and writing to files.
4.	CRUD Operations: Implement functions to create, read, update, and delete lines or words in the buffer.
5.	Search Functionality: Locate words in the buffer.
6.	Undo/Redo Operations: Implement simple undo/redo functionality.


Features
File Operations
•	Open an existing text file or create a new one if the file does not exist.
•	Save changes to the file after editing.

Buffer Management
•	Load up to 25 lines of text into a buffer for manipulation.
•	Dynamically allocate and manage memory for text storage.

CRUD Operations
•	Add a line at a specific position in the buffer.
•	Update an existing line.
•	Delete a line from the buffer.

Search Functionality
•	Search for a specific word in the text.
•	Display the line number and content where the word is found.

Undo/Redo
•	Undo the last three operations (add, delete, or update).
•	Redo the last three undone operations.

Interactive Menu
•	User-friendly menu-driven interface for performing actions.

