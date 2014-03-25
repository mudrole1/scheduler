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
TODO:
==============
1. Test the code with newer SCIP version
2. Solve the bug that scheduler will handle preconditions of task
3. Simplify the equations (if the pre is set to one, we dont need to have both equations)
4. Run more tests



--------------------------
g++ -std=c++0x main.cpp task.cpp scheduler.cpp distWrapper.cpp -o main
