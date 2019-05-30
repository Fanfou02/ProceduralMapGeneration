RandomTilemapGeneration
========================
Wave function Collapse to build 3D tilemaps


Building under Linux/macOS
--------------------------
Inside the top-level directory, execute the following commands:

    mkdir build
    cd build
    cmake ..
    make

The last command -- i.e. `make` -- compiles the application. Rerun it whenever you have added/changed code in order to recompile.

Run under Linux/macOS
--------------------------
Inside the build folder, execute the following command:

    ./RandomTilemapGeneration [filename.vox]
    
If you specify a vox file in argument, the program will directly render this file, if you don't it will create a new model and render it. You can find the newly genereted model under the build/ directory.

Keyboard Settings
-----------------
  * arrow keys: 	Navigation of the camera angle.
  * W,A,S,D, Q, E:	Navigation of the camera position.
  * B:	    		Stop/Start Bezier camera mouvement 
  * ENTER:        	Show the entire model directly, skip step by step
  * F:         	 	Speed up the step by step rendering
  * R:          	Slow down the step by step rendering
  * ESCAPE:         Stop the step by step rendering
