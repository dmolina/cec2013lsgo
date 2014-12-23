from cec2013lsgo import cec2013
import pytest
import numpy as np

@pytest.fixture
def bench():
    return cec2013.Benchmark();

def test_info(bench):
    expected_upper = [100, 100, 100, 100, 100, 32, 100, 100, 5, 32, 100, 100, 100, 100, 100]

    for i in range(1,16):
        info = bench.get_info(i, 10)
        assert info['lower'] == info['upper']*-1
        assert info['upper'] == expected_upper[i-1]
        assert info['threshold'] == 0
        assert info['best'] == 0

def test_values(bench):
    dim = 5
    sol = np.zeros(dim)
    f1 = bench.get_function(1)
    assert not np.isnan(f1(sol))
