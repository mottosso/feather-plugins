# feather-plugins

Feather Plugins
=======

A collection of plugins for Feather.


Introduction
---------------
Feather plugins are written in C++ and consist of node and/or commands. Nodes are blackboxes that can be connected together in the scenegraph and perform a single task. Nodes can only work on data provided to them via their input connections and can only provide output via their output connections. Commands are functions that are called by the UI or the user to perform a single task. There are two type of Commands - Core and UI. Core commands are written in C++ and are intended for working with large data sets or tasks where speed is important. UI commands are written in Qt's Qml scripting language and intended for simple high level tasks or modifying the UI.

Current Plugin List
---------------
Below is a list of current plugins and what they do
* base - blank plugin intended to be used as a starting point for creating new plugins
* common - collection of basic nodes and commands
* polygon - works on polygon mesh data
* obj - importing and exporting of the obj 3d format
 
