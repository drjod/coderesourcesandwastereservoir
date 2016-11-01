
class Coins():
    def __init__(self, twenty, ten, five, two, one):
        if not int(twenty) is twenty or not int(ten) is ten or not int(five) is five or not int(two) is two or not int(one) is one:
            raise ValueError("Values must be integer")
        if twenty < 0 or ten < 0 or five < 0 or two < 0 or one < 0:
            raise ValueError("Values must be positive")
        self.twenty = twenty
        self.ten = ten
        self.five = five
        self.two = two
        self.one = one
    def __add__(self, otherCoins):
        return Coins(self.twenty + otherCoins.twenty, self.ten + otherCoins.ten, self.five + otherCoins.five, self.two + otherCoins.two, self.one + otherCoins.one)
    def __eq__(self, otherCoins):
        if self.twenty == otherCoins.twenty and self.ten == otherCoins.ten and self.five == otherCoins.five and self.two == otherCoins.two and self.one == otherCoins.one:
            return True
        return False

def calculate_amount(coins, exchangeRate = 1.0):
    if exchangeRate < 0:
        raise ValueError("Exchange rate must have positive value")
    return (20 * coins.twenty + 10 * coins.ten + 5 * coins.five + 2 * coins.two + coins.one) * exchangeRate 

# exchange rate applies to coins (first argument)
def change_coins(coins, exchangeRate = 1.0, otherCoins = Coins(0, 0, 0, 0, 0)):
    amount = calculate_amount(coins, exchangeRate)
    newCoins = Coins(0, 0, 0, 0, 0)

    while 1 == 1:
        if amount - 20 >= 0: 
            newCoins.twenty += 1
            amount -= 20
        else:
            break

    while 1 == 1:
        if amount - 10 >= 0: 
            newCoins.ten += 1
            amount -= 10
        else:
            break

    while 1 == 1:
        if amount - 5 >= 0: 
            newCoins.five += 1
            amount -= 5
        else:
            break

    while 1 == 1:
        if amount - 2 >= 0: 
            newCoins.two += 1
            amount -= 2
        else:
            break

    while 1 == 1:
        if amount - 1 >= 0: 
            newCoins.one += 1
            amount -= 1
        else:
            break

    if not otherCoins is Coins(0, 0, 0, 0, 0): 
        newCoins += otherCoins

    return newCoins
