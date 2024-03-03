# RayCaster
RayCaster is an implementation of the Wolfenstein 3D engine using the SDL (Simple DirectMedia Layer) library ;). This project was created as part of my journey to learn and understand the raycasting method, a fundamental technique used in early first-person shooter games.
# Installation and Running 
1. Clone the RayCaster repository:
      ```shell
   git clone https://github.com/Eual11/RayCaster.git
   ```
2. Install the SDL library:
   - For Linux:
     ```shell
     sudo apt-get install libsdl2-dev
     ```
   - For macOS:
     ```shell
     brew install sdl2
     ```
   - For Windows:
     You can download the SDL library from the official SDL website (https://www.libsdl.org/download-2.0.php). Make sure to select the appropriate version for your system and follow the installation instructions.

3. Compile the source code:
   - Navigate to the RayCaster project directory:
     ```shell
     cd RayCaster
     ```
   - Inorder to run the make file you have to first alias CPLUS_INCLUDE_PATH and C_LIBRARY_PATH as your include and library path on Linux and macOS you can do this with
     ```shell
     export C_LIBRARY_PATH=$(gcc -print-file-name=libc.so)
     export CPLUS_INCLUDE_PATH=$(dirname $(gcc -print-file-name=c++))/../include
     ```
   - For Windows, you need to add CPLUS_INCLUDE_PATH and C_LIBRARY_PATH in your enviorment variables and set the to your compilers include and library path
  
   - after this state simple compile by
     `Make all`
4. Run the Program:
   - Execute the compiled binary:
     ```shell
     ./build/e
     ```

