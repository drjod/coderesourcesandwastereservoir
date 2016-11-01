import pytest
from fraction import *

a = Fraction(1,1)
b = Fraction(20, 10)

def test_raises_when_denominator_is_zero():
    with pytest.raises(ValueError):
        Fraction(1,0)

def test_reduce_returns_same_fraction_when_fraction_is_irreducible():
    assert a.reduce().numerator == a.numerator
    assert a.reduce().denominator == a.denominator

def test_reduced_fraction_has_different_numerator_and_denominator_when_fraction_is_reducible():
    assert b.reduce().numerator != b.numerator
    assert b.reduce().denominator != b.denominator
