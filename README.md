# MSHtoCGNS

Master: [![Build Status](https://ci.appveyor.com/api/projects/status/7eejp0wx0f0rf8qb/branch/master?svg=true)](https://ci.appveyor.com/project/felipegiacomelli/mshtocgns)

Develop: [![Build Status](https://ci.appveyor.com/api/projects/status/7eejp0wx0f0rf8qb/branch/develop?svg=true)](https://ci.appveyor.com/project/felipegiacomelli/mshtocgns)

Generate a msh grid using [gmsh 3.0.6](http://gmsh.info/):

<img src="Zeta/Images/grid_gmsh.png" alt="gmsh" height=250 width=600  />

Convert it to the CGNS format and open it with [paraview](https://www.paraview.org/):

<img src="Zeta/Images/paraview_grid.png" alt="paraview" height=250 width=600  />

This project currently supports:

## MSH IO
- [msh 2.2](http://www.manpagez.com/info/gmsh/gmsh-2.2.6/gmsh_63.php/)
- read 2D / 3D single element or hybrid unstructured grids
- supports Physical Line, Physical Surface and Physical Volume

## CGNS IO
- [cgns 3.10 - 3.4.0](https://cgns.github.io/)
- read 2D / 3D single element or hybrid unstructured grids
- write 2D / 3D single element or hybrid unstructured grids
- write simulation data
- **HDF mothernode**

<img src="Zeta/Images/paraview_hybrid_3d_with_well.png"  height=250 width=600  />

---

## Dependencies

To build, it is necessary:

- [g++/gcc](https://gcc.gnu.org/) 7.4 (at least)
- [cmake](https://cmake.org/) 3.14.4
- [make](https://www.gnu.org/software/make/)
- [HDF5](https://www.hdfgroup.org/solutions/hdf5/) 1.10.5
- [CGNS](https://cgns.github.io/index.html) 3.4.0
- [Boost](https://www.boost.org/) 1.70

Once you have installed the first three dependecies, you may install **Boost**, **HDF5**, **CGNS** by executing **setup.sh** located in *Zeta/Setup/*. This script will install **shared libraries** in **release** variant.

## Building

Simply execute:

```shell
$ mkdir build
$ cd build
$ cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=TRUE
$ make
```

If you would like **test**:
```shell
$ make test
```

If you would like to **install**:
```shell
$ make install
```

## Converting

The file **MSHtoCGNS.json**, located in *Zeta/*, specifies the paths to the msh files (**inputs**) and the path where the directory containing the cgns file will be created (**output**). Thus, once you have set this, you may execute:

```shell
$ ./MSHtoCGNS
```

## Simulate

Simulation results may be easily visualised.

<p float="left">
    <img src="Zeta/Images/2D/VelocityX.jpg" alt="gmsh" height=150 width=250  />
    <img src="Zeta/Images/2D/VelocityY.jpg" alt="gmsh" height=150 width=250  />
</p>

<p float="left">
    <img src="Zeta/Images/3D/DisplacementZ-first.png"  alt="gmsh" height=150 width=250  />
    <img src="Zeta/Images/3D/DisplacementZ-last.png" alt="gmsh" height=150 width=250  />
</p>

---

## Importing

MSHtoCGNS may be easily imported to any project using **CMake**, just make sure [*FindMSHtoCGNS.cmake*](Zeta/CMakeModules/FindMSHtoCGNS.cmake)  is on your CMAKE_MODULE_PATH. Hence, you may use this snippet:

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
