# real-time-mining

The following is a project for the class Real-time programming at the faculty of Electrical Engineering, University of Belgrade. The project is a simulation of a real-time mining system involving environment sensors, a water tank and pump that controls the water level. The text of the assignment can be found on the [official class website](http://prv.etf.rs/prv/projekat/Projektni%20zadatak.pdf). 

## Prerequisites

The project was built and tested on Windows 10 x64. The real-time part of the application was built in C++ using Microsoft C++ Redistributable 2017 and Visual Studio Community 2017. The GUI part of the application was built using Python 3.6.

The project uses two additional open-source C++ libraries: [spdlog](https://github.com/gabime/spdlog) and [fmt](https://github.com/fmtlib/fmt).

## Installing

The `src` directory contains all `.h` and `.cpp` files. The `python` directory contains all `.py` files. To install the project, the contents of both directories need to be placed into a single directory along with the content of the `res` directory. The headers of the `spdlog` and `fmt` libraries need to be placed in the same directory as well.

The real-time part of the project is installed as a Python library using the following command:

```
pip3.6 install .
```

The project can then be ran from the current directory with the following command:

```
python3.6 real_time_mining.py
```

## Additional

The `doc` directory contains relevant UML diagrams and the project documentation.

## Built With

* [Visual Studio Community 2017](https://www.visualstudio.com/downloads/) - An open-source IDE
* [Python 3.6](https://www.python.org/downloads/) - The standard Python installation
* [spdlog](https://github.com/gabime/spdlog) - A fast C++ logging library
* [fmt](https://github.com/fmtlib/fmt) -A C++ formatting library

