import matplotlib.pyplot as plt


class Plot:
    __grid = None
    __ax = None

    def __init__(self):
        pass

    def show_grid(self, grid):
        fig, self.__ax = plt.subplots()
        self.__ax.format_coord = self.format_coord
        self.__grid = grid
        self.__ax.imshow(self.__grid, interpolation='nearest')
        plt.show()

    def format_coord(self, x, y):
        col = int(x + 0.5)
        row = int(y + 0.5)
        if col >= 0 and col < self.__grid.shape[0] and row >= 0 and row < self.__grid.shape[1]:
            z = self.__grid[row, col]
            return 'x=%1.4f, y=%1.4f, z=%1.4f' % (x, y, z)
        else:
            return 'x=%1.4f, y=%1.4f' % (x, y)
