# Motivation

This software has been created in order to help solving problems which are typically arising when developing complex UI software using the Qt/QML framework. This kind of system is typically designed in such a way that some C++ code exposes functionality to the QML engine. The typical challenges are the following:
 - The complexity of the code written in QML/JS tends to increase, which results in both maintenance and performance issues.
 - Exposing C++ interfaces to the QML engine requires writing large portions of boilerplate code, which typically results in inconsistencies in a project involving a large number of developers if this boilerplate code is written by hand.
 - Interfaces exposed to the code written in QML/JS code are typically spread over a large number of C++ classes.
 - Testing the code written in QML/JS code independently from the rest of the system is hard because of its dependency to the code written in C++.
 - Some of the interfaces might need to be available via an IPC, which is the case in a multi-process UI design.

# Requirements

This is a list of requirements which have been taken into account when designing this component:
 - Interfaces are defined using the QFace IDL
 - Interfaces can be implemented in C++
 - Interfaces can be implemented in QML/JS
 - C++ interface implementations should be clean from any hack required by the interfacing to QML (no use of variants, qabstractitemmodel, integer instead of enum, etc...)
 - The interface exposed to the UI code should be completely defined using the IDL.
 - The interface implementation details should not be visible to the UI code. Exceptions to that rule should be easily identifiable.
 - The framework should be able to produce proxy and adapter classes which enable an interface implementation to be accessible via an IPC channel.
 - A proxy class should implement the exact same interface as the object implementing the actual interface.
 - Proxy classes should be usable from both c++ and from QML code.
 - Proxy classes should use direct calls when the server object happens to be running in the same process as the proxy.

# FaceLift

Project folder structure:
 - lib : contains the C++ library which the generated code relies on
 - generator : contains the code generator based on the qface framework
 - qface : contains the qface framework (IDL parser, code generation frontend)
 - examples : contains example applications
     - AddressBook
         - interface : contains the interface definition of the model
         - models : contains various implementations of the model interfaces
         - ui : contains the UI code of the application

## Dependencies

* Cmake >= 3.1
* Python 3 (for qface)
* Qt >= 5.5
* Antlr4 python3 runtime

Ubuntu/Debian packages:
The required packages can be installed using the following commands:
```
$ sudo apt-get install python3-click python3-path python3-pip python3-jinja2 python3-yaml cmake qtdeclarative5-dev qml-module-qtquick-controls qtdeclarative5-private-dev
```

In addition, since no deb package is available for it, you need to install the antlr4 runtime using the following command line (for example):
```
$ pip3 install antlr4-python3-runtime
```

# Build

We use QFace as a submodule, which you need to fetch using the following command line:
```
$ git submodule init && git submodule update
```

You are now ready to build the package using the following commands:
```
$ mkdir build
$ cd build
$ cmake .. && make
```

If you want to build using your own version of Qt, you can specify its location using the following command instead:
```
$ cmake -DCMAKE_PREFIX_PATH=/path/to/Qt/gcc_64/lib/cmake .. && make
```

CCACHE can greatly improve build time in some situations. Configure the project with the following command to enable CCACHE:
```
$ CC="ccache gcc" CXX="ccache g++" cmake -DCMAKE_PREFIX_PATH=/path/to/Qt/gcc_64/lib/cmake .. && make
```

Start the example application with:
$ /path/to/Qt/gcc_64/bin/qmlscene ../examples/AddressBook/ui/AddressBookApp.qml  -I imports

## License and Copyright

This code is copyright (c) 2017 Pelagicore AB
Licensed under the LGPL 2.1, please see LICENSE file for more.
