Smart Task Scheduler
C++ Data Structures Console App
A menu-driven console-based Task Management System implemented in pure standard C++ (no STL containers, no vectors). This project demonstrates the practical use of fundamental data structures for efficient task scheduling.
Perfect for learning and presenting Linked Lists, Stacks, Queues, Sorting, and Recursion in Data Structures & Algorithms courses.
Features

Add, Update, Delete Tasks – Each task has ID, Name, Priority (1-5), Deadline (days), Duration (hours)
Priority Queues – Separate linked-list-based queue for each priority level (1 to 5)
Automatic Scheduling – Sorts tasks using Bubble Sort (priority first, then deadline) and schedules them
Conflict Detection – Uses recursion to detect deadline conflicts for same-priority tasks
Undo Last Scheduled Task – Implemented using a fixed-size stack (array-based)
Scheduling History – Linked list to track previously scheduled tasks
Display Options – All tasks, scheduled, pending, priority queues, and history
Recursive Display – Clean recursive function to print task lists
Input Validation – Robust integer and string input handling
Manual Memory Management – Uses new/delete with proper cleanup in destructor

Data Structures Used

























StructurePurposeSingly Linked ListStore all tasks, scheduled tasks, pending tasksQueue (Linked List)One queue per priority level (5 queues total)Stack (Array-based)Undo last scheduled taskLinked List (History)Maintain scheduling history
Screenshots (Sample Output)
text=========================================
     SMART TASK SCHEDULER
=========================================
1.  Create New Task
2.  Update Task
3.  Delete Task
4.  Show All Tasks
5.  Auto Schedule Tasks
6.  Schedule Task Manually
7.  Show Scheduled Tasks
8.  Show Pending Tasks
9.  Undo Last Scheduled Task
10. Show Scheduling History
11. Show Priority Queues
12. Exit
=========================================
How to Run
Requirements

Any C++ compiler (g++, clang++, MSVC)
C++11 or later

Compilation
Bashg++ -o scheduler main.cpp -std=c++11
Run
Bash./scheduler          # On Linux/Mac
scheduler.exe         # On Windows
The program will start with a welcome message and display the main menu.
Project Structure
textSmart-Task-Scheduler/
├── main.cpp              # Complete source code (single file)
├── README.md             # This file
└── (Optional) report.pdf # Project report (if submitted)
Suitable For

Data Structures & Algorithms coursework
College mini/major projects
Demonstrating OOP + manual data structures in C++

Limitations & Future Scope

Console-only (no GUI)
No file persistence (data lost on exit)
Fixed undo stack size

Future Enhancements:

Save/Load tasks from file
Graphical interface
Real-time date integration
Merge Sort for better performance

Author
[Muhammad Danish Wahab]
danishwahabkhan.1@gmail.com
Artificial Intellegence Student
Feel free to fork, star ⭐, and modify!
