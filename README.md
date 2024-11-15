# AutoCAD C++/CLI Plugin Sample

An  AutoCAD plugin demonstrating the integration of native C++ with .NET 8.0 using C++/CLI. 

This sample showcases best practices for AutoCAD plugin development, including proper memory management, thread safety, and modern UI design.

## Demo


https://github.com/user-attachments/assets/00dfe277-a9bf-44ed-a01a-155c763980aa




## Features

- Custom smart pointer implementation for AutoCAD database objects
- Thread-safe database operations
- Modern Windows Forms UI with async operations
- Professional error handling and user feedback
- RAII pattern for resource management
- Modeless dialog integration with AutoCAD

## Prerequisites

- AutoCAD 2025 or later
- ObjectARX SDK 2025
- Windows 10 or later
- .NET 8.0  or later
- Visual Studio 2022 17.8 and later

## Project Structure

```bash
├── StdAfx.h               # AutoCAD and System .NET references
├── ArxNetCore.cpp         # Windows DLL entry point
├── acrxEntryPoint.cpp     # ObjectARX entry point and command definition
└── Resource.h             # Resource definitions
```

## Building The Project

#### Using the Developer Command Line

```batch

git clone 
cd ArxNetCore
set ArxSdkDir = D:\ArxSdks\arx2025
msbuild /t:build /p:Configuration=Debug;Platform=x64

```

#### Using Visual Studio UI

- After cloning the repository, open the `ArxNetCore.sln` solution in **Visual Studio 2022**.

- **Open the Property Manager**:
  
  - Go to the **View** menu at the top of the window.
  - Select **Other Windows**.
  - Click on **Property Manager**.
  
  *Alternatively, you can use the keyboard shortcut **`Ctrl + Shift + F7`** to open the Property Manager window directly.*

- In the **Property Manager**, expand `Debug|x64` and double-click on the `ObjectARX 2025` property page.

- Navigate to **Common Properties** → **User Macros**.

- Edit the `ArxSdkDir` value to point to your ObjectARX SDK path.

- Save, click **OK**, and close the Property Manager.

- Build the project.

## Running the Plugin

1. Launch **AutoCAD 2025** (or later).

2. In AutoCAD, load the plugin:
   
   `netload ~x64\Debug\MADArxNetCore.dll`

3. Run the command:
   
   `LAUNCHTOOL`

4. Enter the desired circle raidus.

5. Click `Draw Circle` to create circle in current drawing.

6. Enjoy using the plugin! 🙂



## Key Components

### AcDbHelper Class

Provides thread-safe database operations and memory management through:

- Smart pointer implementation
- RAII pattern
- Factory methods for entity creation

### MainForm

Windows Forms dialog featuring:

- Asynchronous operations
- User input validation
- Simple UI design
- Proper error handling

### ObjectARX Integration

- Clean command registration
- Exception handling
- Resource management

## Best Practices Demonstrated

1. Memory Management:
   - Smart pointers for AutoCAD objects
   - Proper cleanup of resources
   - Prevention of memory leaks
2. Thread Safety:
   - Main thread execution for AutoCAD commands
   - Async operations for UI responsiveness
   - Proper synchronization
3. Error Handling:
   - Comprehensive exception management
   - User-friendly error messages
   - Proper cleanup on errors



## License

This project is licensed under the MIT License - see the LICENSE file for details.



Madhukar Moogala

Developer Advocate @ Autodesk Platform Services






