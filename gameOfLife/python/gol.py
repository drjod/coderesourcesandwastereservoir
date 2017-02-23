from playground import Playground
from time import sleep


class Game:
    def __init__(self):
        """
        initialize playground
        """
        self.__playground = Playground(20, 20)

        self.__playground.init_field(
            [" ",
             " ",
             " ",
             " ",
             " ",
             "                 ***",
             "                 * *",
             "                 * *",
             " ",
             "                 * *",
             "                 * *",
             "                 ***"]
        )

    def run(self):
        """
        march trough time as long as at least one cell is alive
        :return:
        """
        while self.__playground.is_alive():
            self.__playground.print()
            sleep(1)
            playground_new = self.__playground.next_playground()
            self.__playground = playground_new


if __name__ == '__main__':
    game = Game()
    game.run()
