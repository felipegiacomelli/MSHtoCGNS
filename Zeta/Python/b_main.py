import sys

sys.path.append("/home/felipe/github/MSHtoCGNS/release/shared_libs")
from PyCgnsInterface import (
    AppendConnectivity,
    AppendCoordinate,
    AppendSection,
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
        # [2.0, 0.0, 0.0],
    ]
    for c in coordinates:
        AppendCoordinate(grid_data, c)

    connectivities = [
        [7, 0, 1, 2, 3, 0],
        # [5, 1, 4, 2, 1],
    ]
    for c in connectivities:
        AppendConnectivity(grid_data, c)

    AppendSection(grid_data, SectionData("A", 2, 0, len(connectivities)))

    cgns_creator = CgnsCreator(grid_data, "")
    print(cgns_creator.GetFileName())


if __name__ == "__main__":
    print()
    Main()
    print()
