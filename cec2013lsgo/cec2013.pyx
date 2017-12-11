#!python
#cython: language_level=2, boundscheck=False
from os import path
from collections import namedtuple
from pkg_resources import resource_filename
from libc.stdlib cimport malloc, free
import cython

cdef extern from "eval_func.h":
    void set_func(int funid)
    double eval_sol(double*)
    void set_data_dir(char * new_data_dir)
    void free_func()
    void next_run()


import sys
if sys.version < '3':
    def b(x):
        return x
else:
    import codecs
    def b(x):
        return codecs.latin_1_encode(x)[0]

def _cec2013_test_func(double[::1] x):
    cdef int dim
    cdef double fitness
    cdef double * sol

    dim = x.shape[0]

    sol = <double *> malloc(dim * cython.sizeof(double))

    if sol is NULL:
        raise MemoryError()

    for i in xrange(dim):
        sol[i] = x[i]

    fitness = eval_sol(sol)
    free(sol)
    return fitness

cdef class Benchmark:
    cpdef get_info(self, int fun):
        """
        Return the lower bound of the function
        """
        cdef double optimum
        cdef double range_fun

        optimum = 0

        if (fun == 6):
            range_fun = 32
        elif (fun == 9):
            range_fun = 5
        elif (fun == 10):
            range_fun = 32
        else:
            range_fun = 100

        return {'lower': -range_fun, 'upper': range_fun, 'threshold': 0,
                'best': optimum, 'dimension': 1000}

    def get_num_functions(self):
        return 15

    def __dealloc(self):
        free_func()

    cpdef next_run(self):
        next_run()

    cpdef get_function(self, int fun):
        """
        Evaluate the solution
        """
        set_func(fun)
        cdef bytes dir_name = resource_filename("cec2013lsgo", "cdatafiles").encode()
        set_data_dir(dir_name)
        return _cec2013_test_func
