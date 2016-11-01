from complex import *

arg1 = Complex(4,2)
arg2 = Complex(5,-9)

def test_add_returns_sum_of_coefficients():
    complex_sum = arg1 + arg2

    assert complex_sum == Complex(arg1.real + arg2.real, arg1.imaginary + arg2.imaginary)
    #assert complex_sum.real == arg1.real + arg2.real
    #assert complex_sum.imaginary == arg1.imaginary + arg2.imaginary

def test_sub_returns_difference_of_coefficients():
    difference = arg1 - arg2

    assert difference == Complex(arg1.real - arg2.real, arg1.imaginary - arg2.imaginary)
    #assert difference.real == arg1.real - arg2.real
    #assert difference.imaginary == arg1.imaginary - arg2.imaginary


def test_mul_returns_product_of_coefficients():
    product = arg1 * arg2

    assert product == Complex(arg1.real * arg2.real - arg1.imaginary * arg2.imaginary, arg1.real * arg2.imaginary + arg1.imaginary * arg2.real)


def test_conjugate_has_inverted_imaginary_coefficient():
    conjugate = arg1.conjugate()

    assert conjugate.imaginary == -arg1.imaginary


def test_number_equals_iteself():
    assert (arg1 == arg1) 

def test_two_different_numbers_do_not_equal():
    assert (arg1 == arg2) == False
