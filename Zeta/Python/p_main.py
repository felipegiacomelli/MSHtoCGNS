import sys

sys.path.append("../..//release/shared_libs")
import PybindCgnsInterface
from PybindCgnsInterface import (
    CgnsCreator,
    GridData,
    SectionData,
)


def Main():
    grid_data = GridData(2)

    coordinates = [
        [0.0, 0.0, 0.0],
        [0.0, 1.0, 0.0],
        [1.0, 1.0, 0.0],
        [0.0, 1.0, 0.0],
        [2.0, 0.0, 0.0],
        [2.0, 1.0, 0.0],
    ]
    grid_data.coordinates = coordinates

    connectivities = [
        [7, 0, 1, 2, 3, 0],
        [5, 1, 4, 2, 1],
        [5, 4, 5, 2, 2],
    ]
    grid_data.connectivities = connectivities

    grid_data.sections = [SectionData("A", 2, 0, len(connectivities))]

    cgns_creator = CgnsCreator(grid_data, "")
    print(cgns_creator.GetFileName())


if __name__ == "__main__":
    print()
    Main()
    print()
