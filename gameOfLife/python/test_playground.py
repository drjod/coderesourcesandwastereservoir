from playground import Playground
from mock import Mock


def test_determine_new_cell_status():
    playground = Playground(10, 10)  # 10x10 field
    position_x, position_y = 2, 2
    # 1 alive
    playground.get_cell_status = Mock(side_effect=[0, 0, 1, 0, 0, 0, 0, 0,  # 8 neighbor cells
                                                   0])  # old status
    assert playground.determine_new_cell_status(position_x, position_y) == 0
    # 2 alive
    playground.get_cell_status = Mock(side_effect=[0, 0, 1, 0, 1, 0, 0, 0,  # 8 neighbor cells
                                                   1])  # old status
    assert playground.determine_new_cell_status(position_x, position_y) == 1

    playground.get_cell_status = Mock(side_effect=[0, 0, 1, 0, 0, 0, 0, 1,  # 8 neighbor cells
                                                   0])  # old status
    assert playground.determine_new_cell_status(position_x, position_y) == 0
    # 3 alive
    playground.get_cell_status = Mock(side_effect=[1, 1, 1, 0, 0, 0, 0, 0,  # 8 neighbor cells
                                                   0])  # old status
    assert playground.determine_new_cell_status(position_x, position_y) == 1
    # 4 alive
    playground.get_cell_status = Mock(side_effect=[1, 1, 1, 0, 0, 0, 1, 0,  # 8 neighbor cells
                                                   0])  # old status
    assert playground.determine_new_cell_status(position_x, position_y) == 0
