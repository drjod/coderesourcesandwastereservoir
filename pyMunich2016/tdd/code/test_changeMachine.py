import pytest
from changeMachine import *

def test_calculate_amount_zeroValue():
    assert calculate_amount(Coins(0,0,0,0,0)) == 0

def test_CoinsClass_floatAndStringInputRaisesValueError():
    with pytest.raises(ValueError):
        Coins(1.0, 0, 0, 0, 0)
    with pytest.raises(ValueError):
        Coins(0, 1.0, 0, 0, 0)
    with pytest.raises(ValueError):
        Coins(0, 0, 1.0, 0, 0)
    with pytest.raises(ValueError):
        Coins(0, 0, 0, 1.0, 0)
    with pytest.raises(ValueError):
        Coins(0, 0, 0, 0, 1.0)
    with pytest.raises(ValueError):
        Coins("a", 0, 0, 0, 0)
    with pytest.raises(ValueError):
        Coins(0, "a", 0, 0, 0)
    with pytest.raises(ValueError):
        Coins(0, 0, "a", 0, 0)
    with pytest.raises(ValueError):
        Coins(0, 0, 0, "a", 0)
    with pytest.raises(ValueError):
        Coins(0, 0, 0, 0, "a")

def test_CoinsClass_negativeInputRaisesValueError():
    with pytest.raises(ValueError):
        Coins(-1, 0, 0, 0, 0)
    with pytest.raises(ValueError):
        Coins(0, -1, 0, 0, 0)
    with pytest.raises(ValueError):
        Coins(0, 0, -1, 0, 0)
    with pytest.raises(ValueError):
        Coins(0, 0, 0, -1, 0)
    with pytest.raises(ValueError):
        Coins(0, 0, 0, 0, -1)

def test_calculate_amount_negativeExchangeRateRaiseValueError():
    with pytest.raises(ValueError):
        calculate_amount(Coins(0, 0, 0, 0, 0), -1.)
    #with pytest.raises(ValueError):
    #    calculate_amount(Coins(0, 0, 0, 0, 0), "a")

def test_calculate_amount_reasonableValues():
    assert calculate_amount(Coins(0, 1, 2, 0, 3)) == 23
    assert calculate_amount(Coins(1, 1, 2, 0, 3)) == 43
    assert calculate_amount(Coins(0, 0, 0, 0, 3)) == 3
    assert calculate_amount(Coins(0, 1, 2, 2, 3)) == 27

def test_calculate_amount_reasonableValuesWithExchangeRate():
    assert calculate_amount(Coins(0, 1, 2, 0, 3), 2.) == 46
    assert calculate_amount(Coins(0, 1, 2, 0, 3), 2.5) == 57.5


def test_coins_equal():
    assert Coins(2, 1, 3, 5, 4) == Coins(2, 1, 3, 5, 4)

def test_coins_not_equal():
    assert (Coins(3, 1, 3, 5, 4) == Coins(2, 1, 3, 5, 4)) == False
    assert (Coins(2, 2, 3, 5, 4) == Coins(2, 1, 3, 5, 4)) == False
    assert (Coins(2, 1, 1, 5, 4) == Coins(2, 1, 3, 5, 4)) == False
    assert (Coins(2, 1, 3, 1, 4) == Coins(2, 1, 3, 5, 4)) == False
    assert (Coins(2, 1, 3, 5, 1) == Coins(2, 1, 3, 5, 4)) == False

def test_change_coins():
    assert change_coins(Coins(0, 2, 0, 0, 0)) == Coins(1, 0, 0, 0, 0)
    assert change_coins(Coins(1, 1, 2, 0, 3)) == Coins(2, 0, 0, 1, 1)
    assert change_coins(Coins(0, 0, 0, 0, 3)) == Coins(0, 0, 0, 1, 1)
    assert change_coins(Coins(0, 0, 0, 3, 0)) == Coins(0, 0, 1, 0, 1)
    assert change_coins(Coins(0, 0, 0, 3, 1)) == Coins(0, 0, 1, 1, 0)
    assert change_coins(Coins(0, 2, 0, 0, 0), 1.0001) == Coins(1, 0, 0, 0, 0)
    assert change_coins(Coins(0, 2, 0, 0, 0), 1.0001, Coins(1, 0, 0, 0, 0)) == Coins(2, 0, 0, 0, 0)
    assert change_coins(Coins(0, 2, 0, 0, 0), 1.0001, Coins(1, 0, 0, 1, 0)) == Coins(2, 0, 0, 1, 0)

