# 🎨 winchroma
[![Download](https://img.shields.io/badge/download-latest-41CD522?style=flat-square&logo=github)](https://github.com/anelkica/winchroma/releases/latest) <br />
![C++](https://img.shields.io/badge/C++-23-blue?style=flat-square&logo=c%2B%2B)
![Qt](https://img.shields.io/badge/Qt-6.10-41CD52?style=flat-square&logo=qt)
![Windows](https://custom-icon-badges.demolab.com/badge/Windows-0078D6?logo=windows11&logoColor=white)
> A modern Windows theming utility.

Qt C++ system tweaker for modifying and coloring fine-grained Windows features such as window borders, highlights, titlebars and such.

[⟹ See screenshots](#-screenshots)

## 🛠️ Installation
### Requirements
- C++23 compatible **MSVC** compiler
- CMake 3.16 or higher
- Qt 6

### Clone the repository
```bash
git clone https://github.com/anelkica/winchroma.git
cd winchroma
```
### Open in Qt Creator
Open the `winchroma` folder in Qt Creator IDE to build and run the application.

Make sure to compile with MSVC kit, not MinGW!

## 📕 Info
This project uses `C++23`.
<br>For customizing window appearance, it uses Microsoft's `dwmapi.h`.
<br>MSVC compiler recommended.

⚠️ This project changes registry values for features like highlight colors.

## 🔗 References
- [Desktop Window Manager (DWM)](https://learn.microsoft.com/en-us/windows/win32/api/_dwm/)
- [Use Modern C++ to Access the Windows Registry - GiovanniDicanio](https://learn.microsoft.com/en-us/archive/msdn-magazine/2017/may/c-use-modern-c-to-access-the-windows-registry)
- [WinReg - GiovanniDicanio](https://github.com/GiovanniDicanio/WinReg)

## 📝 Configuration
Per-process and per-window customization is done by editing the `config.toml` file, below are valid properties which can be customized.
`Hilight` `HilightText` and `HotTrackingColor` are registry values saved and loaded to/from the users registry, instead of `config.toml`.
```toml
schema_version = 1

# Global default colors applied to all windows
[globals]
border = "#ff99cc"                # Hex color (#RRGGBB or #RGB)
border_enabled = true             # Enable border customization
titlebar = "#191919"              # Titlebar background color
titlebar_enabled = false          # Enable titlebar customization
titlebar_text = "#ffffff"         # Titlebar text color
titlebar_text_enabled = false     # Enable titlebar text customization

[[rules]]
name = "winchroma app! :)"        # Rule identifier (required)
#process = "winchroma.exe"        # Process name regex (case-insensitive)
title = ".*winchroma.*"           # Window title regex (alternative to process)
border = "#ff99cc"                # Override border color
titlebar = "#191919"              # Override titlebar color
# titlebar_text = "#ffffff"       # Override titlebar text color

[[rules]]
name = "notepad green border"
process = "notepad.exe"
border = "#8bf7c5"
```

## 📸 Screenshots
<div align="center">
    <img width="257" height="184" src="https://github.com/user-attachments/assets/d83f1e44-713e-4f0c-924f-01f82bbfa8e6">
    <img width="257" height="184" src="https://github.com/user-attachments/assets/45bb109e-372b-4ae5-824f-fb8b8d1f7218">
    <p>Hilight Color + Hot Tracking Color</p>
</div>

<div align="center">
    <img width="843" height="483" alt="Image" src="https://github.com/user-attachments/assets/5144cba8-1202-4236-9a4c-f9803a1b63d0" />
    <p>Borders</p>
</div>

<div align="center">
    <img width="446" height="290" src="https://github.com/user-attachments/assets/21b197d7-8b10-49ce-aaa0-bb93104c0ee3">
    <img width="446" height="290" src="https://github.com/user-attachments/assets/f7ed90e0-bbd1-4d31-9737-21a3d7609ec2">
    <p>Titlebar Colors</p>
</div>

<div align="center">
    <img width="885" height="88" alt="Image" src="https://github.com/user-attachments/assets/a2845f9c-0126-4135-9c9d-6042c5b464bb" />
    <p>Titlebar Text Color</p>
</div>
