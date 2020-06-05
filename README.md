# Ising model simulation on C++, Qt 5.14.1

Семестровый проект по информатике студентов МФТИ Бондаря Р, Семененко А, Пяткина С.

## Building under Windows 10:
1) qmake
2) make
3) windeployqt .
4) ising_model.exe

## Building under Linux:
1) qmake
2) make
3) ./ising_model

## Usage and command line options:
For help: ```ising_model -help```

For simulation with GUI: ```ising_model [options]
options:
-lsize=<number> [default: 64]
        Sets lattice size```

For plot making in console: ```ising_model -plot input output [plot options]
in input file there should be number of points n at first (n > 0), and then n values!
plot options:
-lsize=<number> [default: 64]
-algo=<1|2> [default: 1]
        1 - Heat bath algorithm
        2 - Clasters algorithm
-steps=<number> [default: 100]
        Sets number of algorithm steps
-avg=<number> [default: 1]
        Sets averaging by results for each plot point```
        
License: GNU GPL
