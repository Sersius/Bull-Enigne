# Bull-Enigne

A 3d game engine developed with OpenGL in c ++. This project has been done by two students of CITM - UPC (Terrasa, Spain) for the subject Game Engines.

- GitHub repository: [Bull Engine](https://github.com/Sersius/Bull-Engine)
 
## Students

- Víctor Tirado [GitHub](https://github.com/VictorTirado)
- Sergio Gómez [GitHub](https://github.com/Sersius)

### Contribution

- Víctor Tirado
  - Hierarchy
  - Serialization
  - File format
  - Time manager
  - Inspector
  - Audio banks
  - Audio states
  
- Sergio Gómez
  - Frustum culling
  - Camera component
  - "Assets" window
  - Mouse picking
  - Inspector
  - Reverb zone

# Features

- Configuration options (Application, window, hardware and add primitives).
- Inspector shows transform propierties, mesh information and texture information.
- Console with LOG control.
- Drag & Drop to load and visualization of geometry and textures.
- Hierachy Panel where you can see all the GameObjects.
- GameObjects can be picked from the world using the mouse.
- Camera is a component with FOV that can be modified.
- Application/Scene window can be resized and the rendering window adapts to the new aspect ratio.
- All meshes use a bounding volume (AABB).
- Frustum Culling.
- Scene is serialized.
- Start/Pause/Stop Game mode.
- Can open scenes from Assets on the editor.

# Camera controls

- Mouse right click: Move Camera with static position
- Mouse right click + arrow keys: Navigate arround the scene
- Mouse right click + Q: Move up the camera
- Mouse right click + E: Move down the camera
- Mouse left + Alt-left: Move Camera arround model pivot
- Mouse wheel: Move Camera forward and backward
- Press Mouse wheel: Move camera
- F: Center the Camera on an object

# Tools used

- Visual Studio 2017

# Libraries used

 - SDL
 - OpenGL
 - Glew
 - Assimp
 - DevIL
 - Imgui
 - MathGeoLib
 - Parson
 - MMGR
 - PCG Random
 - Par Shapes

# MIT License

Copyright (c) 2019 Sersius & Víctor Tirado

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
