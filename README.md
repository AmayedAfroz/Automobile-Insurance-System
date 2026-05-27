# Automobile Insurance System 🛡️

An enterprise-grade, dual-architecture C++ information system designed to demonstrate strict 3-tier decoupling (Presentation, Business Logic, Data Access).

Unlike standard CRUD applications that mix UI rendering with database calls, this system isolates its domain logic entirely. To prove this architectural decoupling, this repository contains **two distinct applications** that run on the exact same backend engine.

## 🏗️ The Evolution & Dual Architecture

The pure terminal-based **`SDA_AutoInsurance_CLI`** was engineered first as the baseline to establish a rock-solid, bug-free domain logic layer.

Once the backend was perfected, the **`SDA_AutoInsurance_GUI`** was developed as the final, definitive upgrade. It is a hardware-accelerated, 60-FPS Immediate Mode graphical interface built with **Dear ImGui**, **OpenGL 3**, and **GLFW**.

Because of the strict 3-tier architecture, both versions share the exact same `UserController`, `ClaimController`, and `FileHandler` backend classes. The presentation layer was completely swapped from a terminal to a GPU-accelerated GUI without rewriting a single line of business logic.

## ⚙️ Tech Stack

- **Language:** C++
    
- **UI Framework:** Dear ImGui (Immediate Mode GUI)
    
- **Graphics API:** OpenGL 3 / GLFW
    
- **Build System:** CMake
    
- **Storage:** Custom Data Access Object (DAO) utilizing pipe-delimited (|) flat files to natively prevent CSV string corruption.
    

## 📐 Design Patterns Implemented

- **3-Tier Separation of Concerns**
    
- **Data Access Object (DAO)** for isolated file I/O operations.
    
- **Facade Pattern** via Domain Controllers.
    
- **GRASP Controller Pattern** to prevent UI-coupling.
    
- **Abstract Polymorphism** for dynamic User Role instantiation.
    

## 🚀 How to Build & Run

This project requires CMake and a C++17 compatible compiler (MSVC via Visual Studio Developer Command Prompt is recommended).

### 1. Build the Project

> cd SDA_AutoInsurance_GUI cmake -S . -B build cmake --build build --config Debug

### 2. Launch the Application

Once compiled, you can run the executable directly from the terminal or by double-clicking it in your file explorer.

> .\build\Debug\AutoInsuranceSystem.exe

## 👤 Author

**Amayed Afroz**