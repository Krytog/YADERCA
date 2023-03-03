# YADERCA
Yet Another Directory Entities Regular Control Application (File Manager).

# Setup
Before building the app, you should make sure that you've already installed ncurses. To build the app, just copy the repository and go as usual:
<ul>
  <li>mkdir build<il>
  <li>cd build<il>
  <li>cmake ..<il>
  <li>make YADERCA<il>
  <li>./YADERCA<il>
</ul>
Notice that the default CMakeLists.txt enables address sanitizers.

# Guidance
<ul>
  <li>Use <b>up and down arrows</b> to navigate.<il>
  <li>Use <b>enter</b> to go into a directory or try open a file. No matter what the result is, the selector sets to 0.<il>
  <li>Use <b>qq</b> to exit.<il>
  <li>Use <b>h or H</b> to switch between hidden files displaying modes. The selector sets to 0.<il>
  <li>Use <b>c or C</b> to push a file path into the program buffer in copy mode.<il>
  <li>Use <b>x or X</b> to push a file path into the program buffer in cut mode.<il>
  <li>Use <b>v or V</b> to paste a file from the buffer. If the buffer mode is copy, then the path stays in the buffer. If the buffer mode is cut, then the path stays in the buffer on insert attempt failure and the buffer is cleared otherwise.<il>
</ul>

# Opening Files
The directory OpenFiles contains CONFIG.txt, this file mustn't be moved. To let the program open new file extensions, just add the line of type "<.EXTENSION> <OPEN_FUNCTION_NAME> <DYNAMIC_LIBRARY_NAME>". The program supposes that open functions have the signature of "int(const char *)". It's highly recommended that all open functions start a new terminal, instead of using the one YADERCA uses, as it can cause UI artifacts.