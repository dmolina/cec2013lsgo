Introduction
============
This is a Python wrapping using the C++ Implementation of the test suite for the Special Session on Large Scale Global Optimization at 2013 IEEE Congress on Evolutionary Computation.


Note
----
If you are to use any part of this code, please cite the following publications:
X. Li, K. Tang, M. Omidvar, Z. Yang and K. Qin, "Benchmark Functions for the CEC'2013 Special Session and Competition on Large Scale Global Optimization," Technical Report, Evolutionary Computation and Machine Learning Group, RMIT University, Australia, 2013. 
http://goanna.cs.rmit.edu.au/~xiaodong/cec13-lsgo/competition/

Requirements
------------
- GNU Make
- GNU G++
- Python
- Cython

Testing Environment
-------------------
- Debian GNU/Linux jessie/sid
- GNU Make 3.81
- g++ (Debian 4.7.3-4) 4.7.3
- Python 2.7
- numpy 1.8.1
- cython 0.20.1

Instalation
-----------

Very easy, *pip install cec2013lsgo* ;-). 

You can also download from https://github.com/dmolina/cec2013lsgo, and do *python setup.py install [--user]*.
(the option *--user* is for installing the package locally, as a normal user (interesting when you want to 
run the experiments in a cluster/server without administration permissions).

To compile the source code in C++
----------------------------------

The source code in C++ is also available. If you want to compile only the C++
version type in 'make' in the root directory of source code. 

There are two equivalents demo executables: demo and demo2. 

**REMEMBER: To run the C++ version the directory cdatafiles must be available in the working directory**. 
In the python version, these files are included in the packages, so it is not needed. 

To run
------
./demo.out

To clean up
-----------
make clean

Contact
-------

Python package 
  Daniel Molina @ Computer Science Deparment, University of Cadiz
  Please feel free to contact me at <daniel.molina@uca.es> for any enquiries or suggestions.

C++ source code 
  Wenxiang Chen @ Computer Science Department, Colorado State University
  Please feel free to contact me at <chenwx@cs.colostate.edu> for any enquiries or suggestions.

Last Updated: 

- C++ version
  <2013-05-28 Tue 06:28>

- Python wrapping
  <2014-12-23>
