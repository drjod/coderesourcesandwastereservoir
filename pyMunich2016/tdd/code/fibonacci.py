def fib(number):

    if number < 0:
        raise ValueError

    if number == 0:
        return 1

    if number == 1:
        return 1

    return fib(number-2) + fib(number-1)
