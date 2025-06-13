# 🌄 Horizon Mapping — Procedural Terrain Renderer with Horizon-Based Lighting

A C++ / OpenGL graphics programming project built on top of a provided starter framework to demonstrate real-time horizon mapping techniques, procedural mesh generation, and GLSL shader programming.

---

## 📖 Project Context

This project was developed as part of an advanced computer graphics course.  
- A base OpenGL rendering framework was provided to all students.
- The core task was to **design and implement an eight-channel horizon mapping solution** as described in *Section 7.8 of the course textbook*.
- Additional work involved GLSL shader development, procedural terrain adjustments, and lighting models.

---

## 🎯 Implemented Features

- 🏞 **Eight-Channel Horizon Mapping**
  - C++ logic computes horizon maps for procedural terrain using provided height maps.
  - GLSL fragment shaders apply horizon-based lighting calculations per pixel.

- 💡 **Real-Time Lighting Model**
  - Implements slope shading, self-occlusion, and horizon-based ambient light simulation.
  - Enhanced terrain surface realism using directional light computations.

- 🔢 **Procedural Terrain Generation**
  - Dynamically generated height maps processed into terrain mesh data.
  - Supports adjustable grid resolution for different detail levels.

- 🎯 **Custom Camera Controller**
  - First-person camera navigation with pitch/yaw controls.
  - Full freedom of movement to explore terrain output.

- 🎮 **Modern OpenGL Rendering Pipeline**
  - Vertex Buffer Objects (VBO), Vertex Array Objects (VAO), and uniform management for efficient rendering.

---

## 🛠️ Tech Stack

| Technology | Purpose |
|-------------|---------|
| `C++` | Graphics programming logic |
| `OpenGL` | Rendering pipeline |
| `GLSL` | Custom shaders for horizon mapping |
| `GLFW` | Window/context management |
| `GLAD` | OpenGL function loading |
| `GLM` | Math operations (vectors, matrices) |

---

### Requirements:

- C++17 compatible compiler (GCC, Clang, MSVC)
- CMake >= 3.10
- OpenGL 3.3+
- GLFW, GLAD, GLM

📸 Screenshots
Horizon Lighting	Wireframe Mesh	Camera View
(Insert screenshots here)		

📁 Visual results demonstrate real-time shading on procedural terrain.

💡 Learning Objectives
- Implementation of advanced shading techniques using horizon mapping.
- Shader programming via GLSL.
- 3D procedural terrain processing.
- Real-time camera and input system design.
- Low-level OpenGL pipeline management.
- Fundamentals of ambient occlusion rendering.

👤 Author
Mateen Aminian
Senior Software Engineer
📍 Los Angeles, CA
🔗 Portfolio Website
📫 mateenaminian@gmail.com

📝 License
This project is open-source for educational and demo purposes.
Original framework code provided via course materials. Horizon mapping logic and shader implementation written by author.
