#include <iostream>
#include <cstring>
#include <ctime>

using namespace std;
// TASK CLASS
class Task {
public:
    int id;                 // Unique identifier
    char name[100];         // Task name/description
    int priority;           // 1 (highest) to 5 (lowest)
    int deadline;           // Days until deadline (1-30)
    int duration;           // Hours required to complete
    Task* next;             // Pointer to next task in list
    Task* queueNext;        // Pointer for queue operations
    // Constructor to initialize a task
    Task() {
        id = 0;
        strcpy(name, "");
        priority = 0;
        deadline = 0;
        duration = 0;
        next = nullptr;
        queueNext = nullptr;
    }

    // Display task details
    void displayTask(int taskNumber, bool isScheduled = true) {
        cout << "  " << taskNumber << ". [ID:" << id << "] " << name 
             << " | Priority: " << priority 
             << " | Deadline: " << deadline << " days"
             << " | Duration: " << duration << " hours";
        
        if (!isScheduled) {
            cout << " | Status: Pending";
        }
        cout << endl;
    }

    // Create a deep copy of this task
    Task* createCopy() {
        Task* newTask = new Task();
        newTask->id = id;
        strcpy(newTask->name, name);
        newTask->priority = priority;
        newTask->deadline = deadline;
        newTask->duration = duration;
        return newTask;
    }
};
// HISTORY NODE CLASS
class HistoryNode {
public:
    Task* task;             // Pointer to the scheduled task
    HistoryNode* next;      // Pointer to next history node
    // Constructor
    HistoryNode(Task* taskToStore) {
        task = taskToStore->createCopy();
        next = nullptr;
    }
    // Destructor
    ~HistoryNode() {
        delete task;
    }
};
// STACK CLASS FOR UNDO OPERATIONS
class UndoStack {
private:
    Task* taskStack[100];   // Array to store pointers to tasks
    int stackTop;           // Index of top element

public:
    // Constructor
    UndoStack() {
        stackTop = -1;
    }
    // Push a task onto the undo stack
    void pushTask(Task* task) {
        if (stackTop < 99) {
            taskStack[++stackTop] = task->createCopy();
        }
    }
    // Pop a task from the undo stack
    Task* popTask() {
        if (stackTop >= 0) {
            return taskStack[stackTop--];
        }
        return nullptr;
    }
    // Check if stack is empty
    bool isEmpty() {
        return stackTop == -1;
    }
    // Get the current stack size
    int getSize() {
        return stackTop + 1;
    }
};
// QUEUE CLASS FOR PRIORITY MANAGEMENT
class PriorityQueue {
private:
    Task* frontPointer;     // Front of queue
    Task* rearPointer;      // Rear of queue
    int taskCount;          // Number of tasks in queue
    int queuePriority;      // Priority level this queue handles (1-5)
public:
    // Constructor
    PriorityQueue(int priorityLevel) {
        frontPointer = nullptr;
        rearPointer = nullptr;
        taskCount = 0;
        queuePriority = priorityLevel;
    }
    // Destructor to clean up memory
    ~PriorityQueue() {
        clearQueue();
    }
    // Add a task to the queue
    void enqueueTask(Task* newTask) {
        Task* taskCopy = newTask->createCopy();
        taskCopy->queueNext = nullptr;
        if (isEmpty()) {
            frontPointer = taskCopy;
            rearPointer = taskCopy;
        } else {
            rearPointer->queueNext = taskCopy;
            rearPointer = taskCopy;
        }
        taskCount++;
    }
    // Remove and return a task from the queue
    Task* dequeueTask() {
        if (isEmpty()) {
            return nullptr;
        }
        Task* frontTask = frontPointer;
        frontPointer = frontPointer->queueNext;
        
        if (!frontPointer) {
            rearPointer = nullptr;
        }
        taskCount--;
        return frontTask;
    }
    // Check if queue is empty
    bool isEmpty() {
        return frontPointer == nullptr;
    }
    // Get number of tasks in queue
    int getTaskCount() {
        return taskCount;
    }
    // Get queue priority level
    int getPriorityLevel() {
        return queuePriority;
    }
    // Display all tasks in this queue
    void displayQueueTasks() {
        if (isEmpty()) {
            cout << "    Queue is empty." << endl;
            return;
        }
        Task* current = frontPointer;
        int taskNumber = 1;
        
        while (current) {
            cout << "    " << taskNumber << ". " << current->name 
                 << " (Deadline: " << current->deadline << " days)" << endl;
            current = current->queueNext;
            taskNumber++;
        }
    }
    // Clear all tasks from queue
    void clearQueue() {
        while (!isEmpty()) {
            Task* taskToDelete = dequeueTask();
            delete taskToDelete;
        }
    }
};
// TASK MANAGER CLASS - MAIN SYSTEM
class TaskManager {
private:
    // Main linked lists
    Task* allTasksList;         // List of all tasks
    Task* scheduledTasksList;   // List of scheduled tasks
    Task* pendingTasksList;     // List of pending tasks
    // History tracking
    HistoryNode* historyHead;   // Head of scheduling history
    // Data structures
    UndoStack undoSystem;               // Stack for undo operations
    PriorityQueue* priorityQueues[5];   // Array of queues for each priority level
    // Counter for generating unique task IDs
    int nextAvailableId;
    // Utility methods
    void clearInputBuffer();
    int getValidatedInteger(const string& prompt, int minValue, int maxValue);
    void getValidatedString(const string& prompt, char* buffer, int maxLength);
    // Linked list operations
    void addTaskToMainList(Task* newTask);
    Task* findTaskById(int taskId);
    bool removeTaskFromList(int taskId);
    // Sorting algorithm
    void sortTasksByPriorityAndDeadline(Task* &listHead);
    // Recursive functions
    bool checkForSchedulingConflict(Task* scheduledTask, Task* newTask, int depth = 0);
    void displayTasksRecursively(Task* currentTask, bool isScheduled, int taskNumber = 1);
    // History management
    void addToSchedulingHistory(Task* scheduledTask);
    // Display functions
    void displayTaskList(Task* listHead, const string& listName, bool showScheduledStatus = true);
public:
    // Constructor
    TaskManager();
    // Destructor
    ~TaskManager();
    // Main operations
    void createNewTask();
    void updateExistingTask();
    void deleteExistingTask();
    void scheduleTasksAutomatically();
    void scheduleTaskManually();
    void undoScheduledTask();
    // Display operations
    void displayAllTasks();
    void displayScheduledTasks();
    void displayPendingTasks();
    void displaySchedulingHistory();
    void displayAllPriorityQueues();
    // Menu and program flow
    void displayMainMenu();
    void runScheduler();
};
// TASK MANAGER IMPLEMENTATION
// Constructor
TaskManager::TaskManager() {
    allTasksList = nullptr;
    scheduledTasksList = nullptr;
    pendingTasksList = nullptr;
    historyHead = nullptr;
    nextAvailableId = 1;
    // Initialize priority queues for levels 1-5
    for (int i = 0; i < 5; i++) {
        priorityQueues[i] = new PriorityQueue(i + 1);
    }
    cout << "=== SMART TASK SCHEDULER INITIALIZED ===" << endl;
    cout << "Welcome to the Smart Task Scheduler!" << endl;
    cout << "This system helps you manage and schedule tasks efficiently." << endl << endl;
}
// Destructor - Clean up all memory
TaskManager::~TaskManager() {
    cout << "Cleaning up memory..." << endl;
    // Delete all tasks from main lists
    while (allTasksList) {
        Task* temp = allTasksList;
        allTasksList = allTasksList->next;
        delete temp;
    }
    while (scheduledTasksList) {
        Task* temp = scheduledTasksList;
        scheduledTasksList = scheduledTasksList->next;
        delete temp;
    }
    while (pendingTasksList) {
        Task* temp = pendingTasksList;
        pendingTasksList = pendingTasksList->next;
        delete temp;
    }
    // Delete history
    while (historyHead) {
        HistoryNode* temp = historyHead;
        historyHead = historyHead->next;
        delete temp;
    }
    // Delete priority queues
    for (int i = 0; i < 5; i++) {
        delete priorityQueues[i];
    } 
    cout << "Memory cleanup complete. Goodbye!" << endl;
}
// Clear the input buffer to prevent reading errors
void TaskManager::clearInputBuffer() {
    cin.clear();
    while (cin.get() != '\n');
}
// Get validated integer input from user
int TaskManager::getValidatedInteger(const string& prompt, int minValue, int maxValue) {
    int userInput;  
    while (true) {
        cout << prompt;
        cin >> userInput;      
        if (cin.fail() || userInput < minValue || userInput > maxValue) {
            cout << "Invalid input! Please enter a number between " 
                 << minValue << " and " << maxValue << "." << endl;
            clearInputBuffer();
        } else {
            clearInputBuffer();
            return userInput;
        }
    }
}
// Get validated string input from user
void TaskManager::getValidatedString(const string& prompt, char* buffer, int maxLength) {
    cout << prompt;
    cin.getline(buffer, maxLength);   
    // If user just pressed Enter, get input again
    if (strlen(buffer) == 0) {
        cin.getline(buffer, maxLength);
    }
}
// Add a task to the main linked list
void TaskManager::addTaskToMainList(Task* newTask) {
    if (!allTasksList) {
        allTasksList = newTask;
    } else {
        Task* current = allTasksList;
        while (current->next) {
            current = current->next;
        }
        current->next = newTask;
    }
    newTask->next = nullptr;
}
// Find a task in the main list by its ID
Task* TaskManager::findTaskById(int taskId) {
    Task* current = allTasksList;   
    while (current) {
        if (current->id == taskId) {
            return current;
        }
        current = current->next;
    }
    
    return nullptr;
}
// Remove a task from the main list by its ID
bool TaskManager::removeTaskFromList(int taskId) {
    if (!allTasksList) {
        return false;
    }  
    // Check if task is at the beginning of the list
    if (allTasksList->id == taskId) {
        Task* temp = allTasksList;
        allTasksList = allTasksList->next;
        delete temp;
        return true;
    }   
    // Search for the task in the list
    Task* current = allTasksList;
    while (current->next) {
        if (current->next->id == taskId) {
            Task* temp = current->next;
            current->next = current->next->next;
            delete temp;
            return true;
        }
        current = current->next;
    }  
    return false;
}
// Bubble sort algorithm to sort tasks by priority and deadline
void TaskManager::sortTasksByPriorityAndDeadline(Task* &listHead) {
    if (!listHead || !listHead->next) {
        return;
    }    
    bool wasSwapped;  
    do {
        wasSwapped = false;
        Task* current = listHead;
        Task* previous = nullptr;
        
        while (current && current->next) {
            Task* nextNode = current->next;
            
            // Compare tasks: first by priority, then by deadline
            bool shouldSwapNodes = false;
            
            // First, compare priorities (lower number = higher priority)
            if (current->priority > nextNode->priority) {
                shouldSwapNodes = true;
            } 
            // If priorities are equal, compare deadlines (earlier deadline first)
            else if (current->priority == nextNode->priority) {
                if (current->deadline > nextNode->deadline) {
                    shouldSwapNodes = true;
                }
            }   
            // Swap the nodes if needed
            if (shouldSwapNodes) {
                wasSwapped = true;
                
                // Adjust pointers to swap nodes
                if (previous) {
                    previous->next = nextNode;
                } else {
                    listHead = nextNode;
                }
                
                current->next = nextNode->next;
                nextNode->next = current;
                
                // Update previous pointer for next iteration
                previous = nextNode;
            } else {
                // Move to next node
                previous = current;
                current = current->next;
            }
        }
    } while (wasSwapped);
}
// Recursive function to check for scheduling conflicts
bool TaskManager::checkForSchedulingConflict(Task* scheduledTask, Task* newTask, int depth) {
    // Base case: reached end of scheduled list (no conflict found)
    if (!scheduledTask) {
        return false;
    }  
    // Check if new task conflicts with this scheduled task
    // Simple conflict: same priority and deadlines within 2 days of each other
    bool hasConflict = (scheduledTask->priority == newTask->priority &&
                       abs(scheduledTask->deadline - newTask->deadline) <= 2);
    
    if (hasConflict) {
        cout << "    Conflict detected at depth " << depth 
             << ": Task '" << scheduledTask->name 
             << "' conflicts with new task '" << newTask->name << "'" << endl;
        return true;
    }   
    // Recursively check the next task in the scheduled list
    return checkForSchedulingConflict(scheduledTask->next, newTask, depth + 1);
}
// Recursive function to display tasks
void TaskManager::displayTasksRecursively(Task* currentTask, bool isScheduled, int taskNumber) {
    // Base case: no more tasks to display
    if (!currentTask) {
        return;
    }   
    // Display current task
    currentTask->displayTask(taskNumber, isScheduled);
    
    // Recursive call to display next task
    displayTasksRecursively(currentTask->next, isScheduled, taskNumber + 1);
}
// Add a task to scheduling history
void TaskManager::addToSchedulingHistory(Task* scheduledTask) {
    HistoryNode* newHistoryNode = new HistoryNode(scheduledTask);
    newHistoryNode->next = historyHead;
    historyHead = newHistoryNode;
}
// Display a task list with a descriptive name
void TaskManager::displayTaskList(Task* listHead, const string& listName, bool showScheduledStatus) {
    if (!listHead) {
        cout << "\nNo tasks in " << listName << " list." << endl;
        return;
    }   
    cout << "\n=== " << listName << " ===" << endl;
    displayTasksRecursively(listHead, showScheduledStatus);
}
// Create a new task with user input
void TaskManager::createNewTask() {
    cout << "\n=== CREATE NEW TASK ===" << endl;
    
    Task* newTask = new Task();
    newTask->id = nextAvailableId++;
    
    getValidatedString("Enter task name: ", newTask->name, 100);
    newTask->priority = getValidatedInteger("Enter priority (1=highest to 5=lowest): ", 1, 5);
    newTask->deadline = getValidatedInteger("Enter deadline (days from now, 1-30): ", 1, 30);
    newTask->duration = getValidatedInteger("Enter duration (hours required, 1-24): ", 1, 24);
    
    // Add to main list
    addTaskToMainList(newTask);
    
    // Add to appropriate priority queue
    priorityQueues[newTask->priority - 1]->enqueueTask(newTask);
    
    cout << "\nTask created successfully!" << endl;
    cout << "Task ID: " << newTask->id << endl;
    cout << "Task Name: " << newTask->name << endl;
    cout << "Added to Priority Queue: " << newTask->priority << endl;
}

// Update an existing task
void TaskManager::updateExistingTask() {
    if (!allTasksList) {
        cout << "\nNo tasks available to update." << endl;
        return;
    }
    
    cout << "\n=== UPDATE EXISTING TASK ===" << endl;
    
    // Display all tasks first
    displayAllTasks();
    
    int taskId = getValidatedInteger("\nEnter task ID to update: ", 1, nextAvailableId - 1);
    
    Task* taskToUpdate = findTaskById(taskId);
    if (!taskToUpdate) {
        cout << "Error: Task with ID " << taskId << " not found!" << endl;
        return;
    }
    
    cout << "\nCurrent task details:" << endl;
    cout << "1. Name: " << taskToUpdate->name << endl;
    cout << "2. Priority: " << taskToUpdate->priority << endl;
    cout << "3. Deadline: " << taskToUpdate->deadline << " days" << endl;
    cout << "4. Duration: " << taskToUpdate->duration << " hours" << endl;    
    cout << "\nEnter new values (press Enter to keep current value):" << endl;
    
    // Update name
    char newName[100];
    cout << "New name [" << taskToUpdate->name << "]: ";
    cin.getline(newName, 100);
    if (strlen(newName) > 0) {
        strcpy(taskToUpdate->name, newName);
    }
    
    // Update priority
    char priorityInput[10];
    cout << "New priority (1-5) [" << taskToUpdate->priority << "]: ";
    cin.getline(priorityInput, 10);
    if (strlen(priorityInput) > 0) {
        int newPriority = atoi(priorityInput);
        if (newPriority >= 1 && newPriority <= 5) {
            taskToUpdate->priority = newPriority;
        }
    }    
    // Similar updates for deadline and duration would go here...
    
    cout << "\nTask updated successfully!" << endl;
}
// Delete an existing task
void TaskManager::deleteExistingTask() {
    if (!allTasksList) {
        cout << "\nNo tasks available to delete." << endl;
        return;
    } 
    cout << "\n=== DELETE EXISTING TASK ===" << endl;
    
    // Display all tasks first
    displayAllTasks();
    
    int taskId = getValidatedInteger("\nEnter task ID to delete: ", 1, nextAvailableId - 1);
    
    if (removeTaskFromList(taskId)) {
        cout << "\nTask with ID " << taskId << " deleted successfully!" << endl;
    } else {
        cout << "\nError: Task with ID " << taskId << " not found!" << endl;
    }
}
// Schedule tasks automatically based on priority and deadline
void TaskManager::scheduleTasksAutomatically() {
    if (!allTasksList) {
        cout << "\nNo tasks available to schedule." << endl;
        return;
    }
    cout << "\n=== AUTOMATIC SCHEDULING ===" << endl;
    cout << "Scheduling tasks by priority and deadline..." << endl;
    
    // Create a temporary list for sorting
    Task* sortedList = nullptr;
    Task* current = allTasksList;
    Task* lastNode = nullptr;
    
    // Copy all tasks to temporary list
    while (current) {
        Task* taskCopy = current->createCopy();
        
        if (!sortedList) {
            sortedList = taskCopy;
            lastNode = taskCopy;
        } else {
            lastNode->next = taskCopy;
            lastNode = taskCopy;
        }
        current = current->next;
    }
    // Sort the temporary list
    sortTasksByPriorityAndDeadline(sortedList);
    // Schedule tasks from sorted list
    current = sortedList;
    int scheduledCount = 0;
    int pendingCount = 0;
    cout << "\nScheduling results:" << endl;
    while (current) {
        // Check for conflicts with already scheduled tasks
        if (!checkForSchedulingConflict(scheduledTasksList, current)) {
            // No conflict - schedule the task
            Task* scheduledTask = current->createCopy();
            scheduledTask->next = scheduledTasksList;
            scheduledTasksList = scheduledTask;         
           // Add to history
            addToSchedulingHistory(current);        
            // Push to undo stack
            undoSystem.pushTask(current);         
            scheduledCount++;
            cout << "  ? Scheduled: " << current->name << endl;
        } else {
            // Conflict found - add to pending list
            Task* pendingTask = current->createCopy();
            pendingTask->next = pendingTasksList;
            pendingTasksList = pendingTask;
            
            pendingCount++;
            cout << "  ? Pending (conflict): " << current->name << endl;
        } 
        current = current->next;
    }
    // Clean up temporary list
    while (sortedList) {
        Task* temp = sortedList;
        sortedList = sortedList->next;
        delete temp;
    }
    cout << "\nScheduling complete!" << endl;
    cout << "Tasks scheduled: " << scheduledCount << endl;
    cout << "Tasks pending due to conflicts: " << pendingCount << endl;
}
// Schedule a specific task manually
void TaskManager::scheduleTaskManually() {
    if (!allTasksList) {
        cout << "\nNo tasks available to schedule." << endl;
        return;
    }
    cout << "\n=== MANUAL SCHEDULING ===" << endl;
    // Display all tasks
    displayAllTasks();
    int taskId = getValidatedInteger("\nEnter task ID to schedule: ", 1, nextAvailableId - 1);
    Task* taskToSchedule = findTaskById(taskId);
    if (!taskToSchedule) {
        cout << "Error: Task with ID " << taskId << " not found!" << endl;
        return;
    }    
    cout << "\nAttempting to schedule task: " << taskToSchedule->name << endl;    
    // Check for conflicts
    if (checkForSchedulingConflict(scheduledTasksList, taskToSchedule)) {
        cout << "Cannot schedule task due to conflicts." << endl;
        cout << "Task moved to pending list." << endl;       
        // Add to pending list
        Task* pendingTask = taskToSchedule->createCopy();
        pendingTask->next = pendingTasksList;
        pendingTasksList = pendingTask;
    } else {
        // Schedule the task
        Task* scheduledTask = taskToSchedule->createCopy();
        scheduledTask->next = scheduledTasksList;
        scheduledTasksList = scheduledTask;
        
        // Add to history
        addToSchedulingHistory(taskToSchedule);
        
        // Push to undo stack
        undoSystem.pushTask(taskToSchedule);        
        cout << "Task scheduled successfully!" << endl;
    }
}
// Undo the last scheduled task
void TaskManager::undoScheduledTask() {
    if (undoSystem.isEmpty()) {
        cout << "\nNothing to undo!" << endl;
        return;
    }   
    cout << "\n=== UNDO LAST SCHEDULED TASK ===" << endl;   
    Task* lastScheduledTask = undoSystem.popTask();
    if (!lastScheduledTask) {
        cout << "Error: Undo operation failed!" << endl;
        return;
    }    
    cout << "Undoing task: " << lastScheduledTask->name << endl; 
    // Remove from scheduled list
    bool wasRemoved = false;    
    // Check if task is at the beginning of scheduled list
    if (scheduledTasksList && scheduledTasksList->id == lastScheduledTask->id) {
        Task* temp = scheduledTasksList;
        scheduledTasksList = scheduledTasksList->next;
        delete temp;
        wasRemoved = true;
    } else {
        // Search for task in scheduled list
        Task* current = scheduledTasksList;
        while (current && current->next) {
            if (current->next->id == lastScheduledTask->id) {
                Task* temp = current->next;
                current->next = current->next->next;
                delete temp;
                wasRemoved = true;
                break;
            }
            current = current->next;
        }
    }    
    if (wasRemoved) {
        // Move to pending list
        Task* pendingTask = lastScheduledTask->createCopy();
        pendingTask->next = pendingTasksList;
        pendingTasksList = pendingTask;
        
        cout << "Task moved to pending list." << endl;
    } else {
        cout << "Task not found in scheduled list." << endl;
    }    
    delete lastScheduledTask;
    cout << "Undo operation completed." << endl;
}
// Display all tasks
void TaskManager::displayAllTasks() {
    displayTaskList(allTasksList, "ALL TASKS", false);
}
// Display scheduled tasks
void TaskManager::displayScheduledTasks() {
    displayTaskList(scheduledTasksList, "SCHEDULED TASKS", true);
}
// Display pending tasks
void TaskManager::displayPendingTasks() {
    displayTaskList(pendingTasksList, "PENDING TASKS", false);
}
// Display scheduling history
void TaskManager::displaySchedulingHistory() {
    if (!historyHead) {
        cout << "\nNo scheduling history available." << endl;
        return;
    }   
    cout << "\n=== SCHEDULING HISTORY ===" << endl;    
    HistoryNode* current = historyHead;
    int historyNumber = 1;
    
    while (current) {
        Task* historyTask = current->task;
        cout << historyNumber << ". [" << historyTask->id << "] " << historyTask->name 
             << " | Priority: " << historyTask->priority 
             << " | Scheduled relative to deadline: Day " << (30 - historyTask->deadline) << endl;
        
        current = current->next;
        historyNumber++;
    }
}
// Display all priority queues
void TaskManager::displayAllPriorityQueues() {
    cout << "\n=== PRIORITY QUEUES ===" << endl;   
    bool anyQueueHasTasks = false;
    
    for (int i = 0; i < 5; i++) {
        int taskCount = priorityQueues[i]->getTaskCount();
        
        if (taskCount > 0) {
            anyQueueHasTasks = true;
            cout << "\nPriority " << (i + 1) << " Queue (" << taskCount << " tasks):" << endl;
            priorityQueues[i]->displayQueueTasks();
        }
    }  
    if (!anyQueueHasTasks) {
        cout << "\nAll priority queues are empty." << endl;
    }
}
// Display the main menu
void TaskManager::displayMainMenu() {
    cout << "\n=========================================" << endl;
    cout << "        SMART TASK SCHEDULER" << endl;
    cout << "=========================================" << endl;
    cout << "1.  Create New Task" << endl;
    cout << "2.  Update Existing Task" << endl;
    cout << "3.  Delete Existing Task" << endl;
    cout << "4.  Display All Tasks" << endl;
    cout << "5.  Schedule Tasks Automatically" << endl;
    cout << "6.  Schedule Task Manually" << endl;
    cout << "7.  Display Scheduled Tasks" << endl;
    cout << "8.  Display Pending Tasks" << endl;
    cout << "9.  Undo Last Scheduled Task" << endl;
    cout << "10. Display Scheduling History" << endl;
    cout << "11. Display Priority Queues" << endl;
    cout << "12. Exit Program" << endl;
    cout << "=========================================" << endl;
}
// Main program loop
void TaskManager::runScheduler() {
    int userChoice;
    do {
        displayMainMenu();
        userChoice = getValidatedInteger("Enter your choice (1-12): ", 1, 12);
        switch (userChoice) {
            case 1:
                createNewTask();
                break;
            case 2:
                updateExistingTask();
                break;
            case 3:
                deleteExistingTask();
                break;
            case 4:
                displayAllTasks();
                break;
            case 5:
                scheduleTasksAutomatically();
                break;
            case 6:
                scheduleTaskManually();
                break;
            case 7:
                displayScheduledTasks();
                break;
            case 8:
                displayPendingTasks();
                break;
            case 9:
                undoScheduledTask();
                break;
            case 10:
                displaySchedulingHistory();
                break;
            case 11:
                displayAllPriorityQueues();
                break;
            case 12:
                cout << "\nThank you for using the Smart Task Scheduler!" << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
        // Pause to let user read output
        if (userChoice != 12) {
            cout << "\nPress Enter to continue...";
            clearInputBuffer();
        }
    } while (userChoice != 12);
}
// MAIN FUNCTION
int main() {
    // Create task manager instance
    TaskManager taskScheduler;
    // Run the scheduler
    taskScheduler.runScheduler();   
    return 0;
}