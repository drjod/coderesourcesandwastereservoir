from square import *

def test_returns_square():
    some_number = 4.2
    squared_number = square(some_number)

    assert squared_number == some_number**2
