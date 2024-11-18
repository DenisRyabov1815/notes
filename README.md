# Notes
A simple note-taking program for the terminal on the Linux operating system, written in C++.
This project primarily serves as a programming practice in C++. Therefore, I kindly ask you not to judge too harshly.

# Installation
To install, simply run the following command:
`make  &&  make  install` 

The application is written in pure C++ and does not require any additional dependencies.

# Uninstall
To remove, you need to use the following command:
`make uninstall`

# Usage
The program features a command-based interface. To start, you need to type the program name, followed by the command you wish to execute (for example, to add a note).

An example of usage looks like this:
`notes [COMMAND 1]  [COMMAND 2]  ...` 

Available commands:
-   **-a, --add "NOTES"**: Add a note.
-   **-d, --delete "INDEX"**: Delete the note with the specified index INDEX.
-   **-D, --delete-all Yes**: Delete all notes.
-   **-s, --show**: Display all notes.
-   **-v, --version**: Display the program version.
-   **-h, --help**: Show help information.
