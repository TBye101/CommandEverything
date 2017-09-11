# CommandEverything
A command line to do everything and anything.
Written in C++, and optimizied for performance, this command line is as powerful and efficient as possible.
It currently only supports x64 windows, but if anyone wants to port to other platforms, that is always welcome.

## Features

### Custom Scripts
We now support file based scripts. Simply create a file, save it.
Then use the run script command on your file.
Watch as your script runs!
Any command that is valid in the console is valid in the file.

### Logging
Your entire command line history for each instance of the command line is saved to a text file under the "Logs" folder, in the same directory as your executable.

### Command Line Arguments

This terminal now parses command line arguments.

Ex: "Help Tree" would launch the help command, and the tree command.

Ex: "CD C:/Program Files" would change the working directory to "C:/Program", and then try and execute a command called "Files" which doesn't exist. Yet.

## Libraries Consumed

[Dirent](https://github.com/tronkko/dirent)

[ThreadPool](https://github.com/progschj/ThreadPool)
