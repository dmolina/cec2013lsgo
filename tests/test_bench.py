from cec2013lsgo import cec2013
import pytest
import numpy as np


@pytest.fixture
def bench():
    return cec2013.Benchmark()


@pytest.fixture
def sol():
    return np.zeros(1000)


def test_info(bench):
    """
    Test the information about each function
    """
    expected_upper = [100, 100, 100, 100, 100,
                      32, 100, 100, 5, 32, 100, 100, 100, 100, 100]
    num_funs = bench.get_num_functions()
    assert num_funs == 15

    for i in range(1, num_funs + 1):
        info = bench.get_info(i)
        assert info['lower'] == info['upper'] * -1
        assert info['upper'] == expected_upper[i - 1]
        assert info['threshold'] == 0
        assert info['best'] == 0
        assert info['dimension'] == 1000


def test_nonan(bench, sol):
    """
    Check that for all solutions is not nan
    """
    for fun_id in range(1, bench.get_num_functions() + 1):
        fn = bench.get_function(fun_id)
        assert not np.isnan(fn(sol))


def test_values(bench, sol):
    """
    Check the results for zero for each function
    """
    values_expected = [
        2.09833896353343505859E+11,
        4.76203116166061372496E+04,
        2.17290025349525492970E+01,
        1.07955147656065953125E+14,
        4.84191483329246416688E+07,
        1.07773246530947787687E+06,
        9.93826981321072625000E+14,
        5.72227150187806412800E+18,
        6.00160320250193595886E+09,
        9.81154816486999392509E+07,
        1.04485201647212016000E+17,
        1.71135423694972143555E+12,
        8.27380048985966720000E+16,
        4.40797968120962457600E+18,
        2.39389233661550150000E+15]

    for fun_id in range(1, 16):
        fn = bench.get_function(fun_id)
        assert fn(sol) == values_expected[fun_id - 1]

# def test_many_functions(bench,sol):
# print ""
#    import time
#
#    startglobal = time.time()
#
#    for fun_id in range(1, 16):
#        fn = bench.get_function(fun_id)
#        start = time.time()
#
#        for i in range(1, 10000):
#            assert fn(sol)>=0
#
#        end = time.time()
#        print "F%d: %.2f segs" %(fun_id,end-start)
#
#    endglobal = time.time()
#    print "Total: %.2f segs" %(endglobal-startglobal)
