# Assignment 5: Washing Machine Simulation
---
## Problem Description

The goal is to simulate the use of washing machines in a dormitory (OBH) using multi-threading. The program monitors students' use of the machines throughout a day and generates a report. Based on the statistics, it determines if more washing machines are required.

### Requirements:
1. **Inputs**:
   - `N`: Number of students.
   - `M`: Number of washing machines.
   - For each student:
     - `T_i`: Time of arrival (in seconds after program execution starts).
     - `W_i`: Time taken for washing (in seconds).
     - `P_i`: Maximum patience before leaving without washing (in seconds).

2. **Output**:
   - Events with specific colors:
     - **White**: A student arrives.
     - **Green**: A student starts washing.
     - **Yellow**: A student leaves after washing.
     - **Red**: A student leaves without washing.
   - Total number of students who left without washing.
   - Whether more washing machines are required ("Yes" or "No").

3. **Simulation Rules**:
   - Students use machines in **First-Come-First-Served (FCFS)** order.
   - If multiple students arrive simultaneously, the one with the lower index gets priority.
   - Use semaphores and mutex locks for thread synchronization. Avoid deadlocks and busy waiting.
   - A queue must not be used for managing threads.

4. **Final Decision**:
   - If at least 25% of students left without washing, more machines are needed.

---

## Solution Implementation

The solution is implemented in **C++**, using:
- **Threads** to represent students and washing machines.
- **Semaphores** for managing washing machine availability.
- **Mutex locks** for synchronized printing.

The solution ensures adherence to the FCFS policy while avoiding deadlocks and interleaved printing.

### Algorithm Outline:
1. Parse the input values (`N`, `M`, `T_i`, `W_i`, `P_i`).
2. Create threads for each student.
3. Simulate the arrival of students based on `T_i`.
4. Assign machines based on availability and FCFS rules.
5. Use semaphores and mutex locks to:
   - Assign washing machines.
   - Handle waiting time (`P_i`).
   - Print events in the specified colors.
6. Calculate and print the required statistics:
   - Total students who left without washing.
   - Final decision on whether more machines are needed.

---

## Steps to Compile and Execute

### Compilation
1. Open the **parent folder**, then type
   ```bash
   g++ solution.cpp -o solution -lpthread
   ```
2. This will create an executable named **solution**
3. Run the **solution** to exectute the solution to the washing machine synchronization problem
   
   
   
   
   
   
   
   
   
   
