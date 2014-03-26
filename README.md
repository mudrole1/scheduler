scheduler
=========
1. Clone the repository to your structure
2. Download Mixed Integer Programming solver: http://scip.zib.de/download.php?fname=scipoptsuite-3.0.2.tgz
3. Unpack it to the cloned repository
4. Follow the installation instructions: http://scip.zib.de/doc-3.0.2/html/MAKE.shtml
5. Rewrite the file scipoptsuite-3.0.2/scip-3.0.2/make/make.project by the file from cloned repository (setting compiler to follow c++11 standard)
6. Run "make" in the cloned repository to compiled the code
7. Go to the bin folder and run main to observed behaviour

--------------------------------------------
How to use it
==============
See the main.cpp for example, you should create a vector of pointers to tasks. Then create a new scheduler object passing the address of the vector with tasks
Scheduler sch(&schedT);

Then, method solve return true, if the solution is found and set execution_time in each tasks. Otherwise return false.

bool worked = sch.solve();

TODO:
==============
1. Solve the bug that scheduler will handle preconditions of task
2. Simplify the equations (if the pre is set to one, we dont need to have both equations)
3. Run more tests



--------------------------
g++ -std=c++0x main.cpp task.cpp scheduler.cpp distWrapper.cpp -o main
