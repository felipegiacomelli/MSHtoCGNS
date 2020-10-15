# MSHtoCGNS

Master: [![Build Status](https://ci.appveyor.com/api/projects/status/7eejp0wx0f0rf8qb/branch/master?svg=true)](https://ci.appveyor.com/project/felipegiacomelli/mshtocgns)
Develop: [![Build Status](https://ci.appveyor.com/api/projects/status/7eejp0wx0f0rf8qb/branch/develop?svg=true)](https://ci.appveyor.com/project/felipegiacomelli/mshtocgns)

Generate a msh grid using [gmsh 3.0.6](http://gmsh.info/)

<img src="Zeta/Images/grid_gmsh.png" alt="gmsh" height=250 width=600  />

Convert it to the CGNS format and open it with [paraview](https://www.paraview.org/)

<img src="Zeta/Images/paraview_grid.png" alt="paraview" height=250 width=600  />

This project currently supports

## MSH IO
- [msh 2.2](http://www.manpagez.com/info/gmsh/gmsh-2.2.6/gmsh_63.php/)
- [msh 4.1](https://gmsh.info/doc/texinfo/gmsh.html#MSH-file-format)
- read grids composed of linear elements

## CGNS IO
- [cgns 3.10 - 4.1.2](https://cgns.github.io/)
- HDF mothernode
- read/write unstructured grids
- write simulation results

<img src="Zeta/Images/paraview_hybrid_3d_with_well.png"  height=250 width=600  />

---

## Dependencies

To build, it is necessary

- [g++/gcc](https://gcc.gnu.org/) 8.4 (at least)
- [cmake](https://cmake.org/) 3.14.4
- [make](https://www.gnu.org/software/make/)
- [HDF5](https://www.hdfgroup.org/solutions/hdf5/) 1.10.0 (at least)
- [CGNS](https://cgns.github.io/index.html) 4.1.2
- [Boost](https://www.boost.org/) 1.74.0

In **Arch Linux/Manjaro**, all dependencies may be installed through [LibraryInstaller](https://github.com/felipegiacomelli/LibraryInstaller/). Just set the following options in
[Settings.py](https://github.com/felipegiacomelli/LibraryInstaller/blob/master/Settings.py)

```python

libraries = {
    "openmpi"  : {"version" : "4.0.2" , "install" : True},

    "boost"    : {"version" : "1.74.0", "install" : True},

    "metis"    : {"version" : "5.1.0" , "install" : False},
    "petsc"    : {"version" : "3.12.2", "install" : False},

    "hdf5"     : {"version" : "1.10.5", "install" : True},
    "cgns"     : {"version" : "4.1.2" , "install" : True},
    "mshtocgns": {"version" : "4.0.0" , "install" : True},

    "dei"      : {"version" : "1.0.0" , "install" : False},

    "muparser" : {"version" : "2.2.6" , "install" : False},

    "triangle" : {"version" : "1.6.0" , "install" : False},
    "tetgen"   : {"version" : "1.5.1" , "install" : False}
}
```

In **Ubuntu 18.04**, once the first four dependencies have been installed, you may install **Boost** and **CGNS** by executing [*setup.sh*](Zeta/CMakeModules/FindMSHtoCGNS.cmake). This script will install **shared libraries** in **release** variant.

## Build

Simply execute

```shell
$ mkdir build
$ cd build
$ cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=TRUE
$ make
$ make test
```
If you would like to **install**

```shell
$ make install
```
## Convert

The file [*MSHtoCGNS.json*](Zeta/MSHtoCGNS.json), located in *Zeta/*, specifies the paths to the msh files (**inputs**) and the path where the directory containing the CGNS files will be created (**output**). Thus, once you have set this, you may execute

```shell
$ ./MSHtoCGNS
```
## Simulate

Simulation input files are read through the **CgnsReader**.

```cpp
CgnsReader cgnsReader(std::string filePath);
boost::shared_ptr<GridData> gridData{cgnsReader.gridData};
```

The **GridData**, in turn, holds vertices coordinates, elements connectivity and the sections they belong to.

```cpp
struct GridData {
    int dimension;

    std::vector<std::array<double, 3>> coordinates;

    std::vector<std::vector<int>> connectivities;

    std::vector<SectionData> sections;

    int numberOfLocalVertices;
};
```

Note that the element type is in the connectivity *std::vector* front, and the element index is at the connectivity *std::vector* back. As in a CGNS file, the sections hold a contiguous partition of elements -- its information is stored in the **SectionData**.

```cpp
struct SectionData {
    SectionData() = default;

    SectionData(std::string name, int dimension, int begin, int end, std::vector<int> vertices) :
        name(name), dimension(dimension), begin(begin), end(end), vertices(vertices) {}

    std::string name;
    int dimension;
    int begin;
    int end;
    std::vector<int> vertices;
};
```
It is necessary to remark that: *boundaries* are sections where the boundary conditions are applied, *regions* are employed to define different physical properties throughout the domain, and *wells* may be associated to source terms.

Finally, the **CgnsWriter** is used to write the simulation results in the output file.

```cpp
class CgnsWriter : public CgnsOpener {
    public:
        CgnsWriter() = default;
        CgnsWriter(std::string filePath, std::string gridLocation);

        void writeSolution(std::string name);
        void writeSolution(double timeValue);

        void writeField(std::string name, const std::vector<double>& values);
        void writeField(std::string name, const std::vector<int>& values);

        void finalizeTransient();

        ...
    }
```

Easily visualised results with [paraview](https://www.paraview.org/).

<p float="left">
    <img src="Zeta/Images/2D/VelocityX.jpg" alt="gmsh" height=300 width=400  />
    <img src="Zeta/Images/2D/VelocityY.jpg" alt="gmsh" height=300 width=400  />
</p>

<p float="left">
    <img src="Zeta/Images/3D/DisplacementZ-first.png"  alt="gmsh" height=300 width=400  />
    <img src="Zeta/Images/3D/DisplacementZ-last.png" alt="gmsh" height=300 width=400  />
</p>

---

## Import

MSHtoCGNS may be easily imported to any project using **CMake**, just make sure [*FindMSHtoCGNS.cmake*](Zeta/CMakeModules/FindMSHtoCGNS.cmake) is on your CMAKE_MODULE_PATH. Hence, you may use this snippet

```cmake
set (MSHTOCGNS_DIR $ENV{MSHTOCGNS_DIR}/${BUILD_TYPE}/${LIBRARY_TYPE})
find_package (MSHtoCGNS REQUIRED)
if (MSHTOCGNS_FOUND)
    include_directories (${MSHTOCGNS_INCLUDE_DIR})
endif ()

...

target_link_libraries (${_target} ${MSHTOCGNS_LIBRARIES})
```
Usually,

*$ENV{MSHTOCGNS_DIR}* is set to a directory such as **/home/felipe/Libraries/mshtocgns-0.16.0**

*${BUILD_TYPE}* is the lower case ${CMAKE_BUILD_TYPE} - **release** OR **debug**

*${LIBRARY_TYPE}* is the lower case library type - **shared** OR **static**

You may also set ${MSHTOCGNS_DIR} (on your project's CMakeLists.txt) to the installation directory of MSHtoCGNS.

---

## Note

For MSH file format version 4.1, geometrical entities **must** be associated to **none** or a **single** physical entity. Otherwise, the MSH reader may not work properly.
