import numpy as np


class Playground:

    @property
    def cells(self):
        return self.__cells

    def __init__(self, size, random_flag):
        if random_flag:
            self.__cells = (np.random.rand(size, size) > 0.5).astype('uint8')
        else:
            self.__cells = np.zeros((size, size)).astype('uint8')
            strings =[" ",
                      " ",
                      "     ***",
                      "     * *",
                      "     * *",
                      " ",
                      "     * *",
                      "     * *",
                      "     ***"]

            for x, string_item in enumerate(strings):
                for y, char_item in enumerate(list(string_item)):
                    if char_item == ' ':
                        self.__cells[x][y] = 0
                    elif char_item == '*':
                        self.__cells[x][y] = 1
                    # else ignore, e.g. '\n'

        ndx = np.r_[0:size]
        self.__up = np.roll(ndx, -1)
        self.__down = np.roll(ndx, 1)

    def update(self):
        neighbors = self.__cells[self.__up, :] + self.__cells[self.__down, :] + \
                    self.__cells[:, self.__up] + self.__cells[:, self.__down] + \
                    self.__cells[np.ix_(self.__up, self.__up)] + self.__cells[np.ix_(self.__up, self.__down)] + \
                    self.__cells[np.ix_(self.__down, self.__up)] + self.__cells[np.ix_(self.__down, self.__down)]

        self.__cells = (neighbors == 3) | (self.__cells & (neighbors == 2))

