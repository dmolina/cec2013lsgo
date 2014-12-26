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
- Python 2.7 and Python 3.2
- numpy 1.8.1
- cython 0.20.1

Results with Travis-CI
~~~~~~~~~~~~~~~~~~~~~~
.. image:: https://api.travis-ci.org/dmolina/cec2013lsgo.svg?branch=master

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

Tests
-----

The source code has tests to check the information about each function, and the results obtained
with the C version using the solution np.zeros(1000) (a solution of zeros).

Quickstart
----------

The package is very simple to use. There is a class Benchmark with two functions:

- Give information for each function: their optimum, their dimensionality, the domain search, and the
  expected threshold to achieve the optima.

- Give a fitness function to evaluate solutions. It expect that these solutions are numpy arrays
  (vectors) but it can also work with normal arrays.

These two functionalities are done with two methods in Benchmark class:

- **get_num_functions()**

  Return the number of functions in the benchmarks (15)

- **get_info(function_id)**

  Return an array with the following information, where /function_id/ is the identifier of the function, a int value between 1 and 15.

    - lower, upper
        *lower* and *upper* boundaries of the domain search. 

    - best
        Optimum to achieve, it is always zero, thus it can be ignored.

    - threshold
        Threshold to obtain, it is always zero, thus it can also be ignored.

    - dimension
        Dimension for the function, it is always 1000.

    It can be noticed that several data are the same for all functions. It is made for maintaining the 
    same interface to other cec20xx competitions.

- **get_function(function_id)**

  *function_id* is the same parameter than in **get_info**, an integer value between 1 and 15.
  
  It returns the fitness function to evaluate the solutions.

Examples of use
---------------

Obtain information about one function
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

>>> from cec2013lsgo.cec2013 import Benchmark
>>> bench = Benchmark()
>>> bench.get_info(1)
{'best': 0.0,
 'dimension': 1000,
 'lower': -100.0,
 'threshold': 0,
 'upper': 100.0}

Create random solution for the search
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

>>> from numpy.random import rand
>>> info = bench.get_info(1)
>>> dim = info['dimension']
>>> sol = info['lower']+rand(dim)*(info['upper']-info['lower'])

Evaluate a solution
~~~~~~~~~~~~~~~~~~~
>>> fun_fitness = bench.get_function(1)
>>> fun_fitness(sol)
464006824710.75995

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
