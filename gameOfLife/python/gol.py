from playground import Playground
from plot import Plot


playground = Playground(10, random_flag=True)
plot = Plot()
plot.show_grid(playground.cells)

while True:
    playground.update()
    plot.show_grid(playground.cells)
