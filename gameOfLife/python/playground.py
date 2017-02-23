import numpy as np
from sys import stdout


class Cell:
    def __init__(self, _x, _y):
        self.x, self.y = _x, _y


class Playground:
    def __init__(self, max_x, max_y):
        """

        :param max_x: (int)
        :param max_y: (int)
        """
        self.__cells_status = np.zeros((max_x, max_y))

    def get_cell_status(self, x, y):
        return self.__cells_status[x][y]

    def set_cell_status(self, x, y, value):
        self.__cells_status[x][y] = value

    def is_alive(self):
        """
        looks if at least one cell in the playground is alive
        :return: (bool)
        """
        return True if np.count_nonzero(self.__cells_status) > 0 else False

    def determine_new_cell_status(self, x, y):
        """
        looks who are neighbors and then how many neighbors are alive
        based on that it returns the new cell status
        :param x: (int)
        :param y: (int)
        :return: new status of cell - 0: dead, 1: alive
        """
        neighbors_alive = 0
        for item in self.determine_neighbors(x, y):
            neighbors_alive += self.get_cell_status(item.x, item.y)
        status_old = self.get_cell_status(x, y)

        if neighbors_alive == 3:
            return 1
        if neighbors_alive == 2 and status_old == 1:
            return 1
        return 0

    def determine_neighbors(self, x, y):
        """
        provides neighbors - 0 is neighbor of max (and vice versa) for x and y
        :param x: (int)
        :param y: (int)
        :return: list of tuples (x,y), which are the neighbors
        """
        max_x = self.__cells_status.shape[0]
        max_y = self.__cells_status.shape[1]

        result = list()
        result.append(Cell((x - 1 + max_x) % max_x, (y - 1 + max_y) % max_y))
        result.append(Cell((x - 1 + max_x) % max_x, y))
        result.append(Cell((x - 1 + max_x) % max_x, (y + 1) % max_y))
        result.append(Cell(x, (y - 1 + max_y) % max_y))
        result.append(Cell(x, (y + 1) % max_y))
        result.append(Cell((x + 1) % max_x, (y - 1 + max_y) % max_y))
        result.append(Cell((x + 1) % max_x, y))
        result.append(Cell((x + 1) % max_x, (y + 1) % max_y))

        return result

    def init_field(self, strings):
        """
        initializes cell_status based on lit of strings
        :param strings: list of strings - ' ': dead cell, '*': cell is alive
        :return:
        """
        for x, string_item in enumerate(strings):
            for y, char_item in enumerate(list(string_item)):
                if char_item == ' ':
                    self.__cells_status[x][y] = 0
                elif char_item == '*':
                    self.__cells_status[x][y] = 1
                # else ignore, e.g. '\n'

    def print(self):
        """
        console output
        :return:
        """
        for y in range(self.__cells_status.shape[1]):
            for x in range(self.__cells_status.shape[0]):
                if self.__cells_status[x][y] == 0:
                    stdout.write(' ')
                else:
                    stdout.write('*')
            stdout.write('\n')
        stdout.flush()

    def next_playground(self):
        """
        create and return new playground instance with new cell status
        :return:
        """
        playground_next = Playground(self.__cells_status.shape[0],
                                     self.__cells_status.shape[1])

        for x in range(self.__cells_status.shape[0]):
            for y in range(self.__cells_status.shape[1]):
                playground_next.set_cell_status(x, y, self.determine_new_cell_status(x, y))

        return playground_next
