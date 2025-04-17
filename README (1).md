# Advanced-File-Compressor-and-Decompressor-Tool-DSA-Project
# File Compressor Tool

A versatile file compression tool with a graphical user interface that supports both text and JPEG compression. The tool uses Huffman coding for text compression and the libjpeg library for JPEG compression.

## Features

- Text file compression and decompression using Huffman coding
- JPEG image compression with adjustable quality settings
- User-friendly graphical interface
- Real-time compression statistics
- Multi-threaded processing for JPEG compression
- Support for both command-line and GUI operations

## Prerequisites

### Required Software
1. MSYS2 MinGW64 (for C++ compilation)
2. Python 3.x (for GUI)
3. libjpeg library
4. C++ compiler (g++)
5. tkinter (Python GUI library)

### Installation Steps

1. **Install MSYS2:**
   - Download MSYS2 from https://www.msys2.org/
   - Run the installer and follow the setup instructions
   - Open MSYS2 MinGW64 terminal

2. **Update MSYS2 packages:**
   ```bash
   pacman -Syu
   ```

3. **Install required packages:**
   ```bash
   pacman -S mingw-w64-x86_64-gcc
   pacman -S mingw-w64-x86_64-libjpeg-turbo
   pacman -S make
   ```

4. **Add MinGW to System PATH:**
   - Open System Properties > Advanced > Environment Variables
   - Add `C:\msys64\mingw64\bin` to the System PATH variable

5. **Install Python Dependencies:**
   ```bash
   pip install tkinter
   ```

## Building the Project

1. **Compile the C++ backend:**
   ```bash
   g++ -o compressor.exe compressor.cpp -ljpeg
   ```

2. **Verify Python GUI:**
   - Ensure `compressor_gui.py` is in the same directory as `compressor.exe`

## Usage

### GUI Method
1. Run the GUI:
   ```bash
   python compressor_gui.py
   ```
2. Select file type (Text or JPEG)
3. Choose operation:
   - For text files: Compress or Decompress
   - For JPEG: Set quality (1-100)
4. Select input and output files
5. Click "Start Process"

### Command Line Method
1. **For text files:**
   ```bash
   compressor.exe text compress input.txt compressed.bin
   compressor.exe text decompress compressed.bin output.txt
   ```

2. **For JPEG files:**
   ```bash
   compressor.exe jpeg compress input.jpg output.jpg quality
   ```
   Where `quality` is a number between 1-100

## File Format Support

### Text Compression
- Input: Any text file (.txt, .csv, etc.)
- Output: Binary file containing compressed data

### JPEG Compression
- Input: JPEG/JPG images
- Output: Compressed JPEG/JPG images
- Quality: Adjustable from 1 (highest compression) to 100 (highest quality)

## Implementation Details

### Text Compression
- Uses Huffman coding algorithm
- Creates frequency table for characters
- Generates optimal binary codes
- Stores frequency table with compressed data
- Provides compression statistics

### JPEG Compression
- Uses libjpeg library
- Supports quality-based compression
- Maintains EXIF data
- Shows compression ratio
- Multi-threaded processing

## Error Handling
- Input file validation
- Quality parameter validation
- File access permissions check
- Compression process monitoring
- User-friendly error messages

## Performance
- Efficient memory usage
- Multi-threaded JPEG processing
- Progress feedback
- Compression ratio display

## Limitations
- JPEG compression requires libjpeg library
- Text compression may not be optimal for binary files
- Large files might require significant memory
- GUI requires Python and tkinter

## Troubleshooting

1. **libjpeg errors:**
   - Verify MSYS2 installation
   - Check PATH environment variable
   - Reinstall libjpeg if necessary

2. **GUI not launching:**
   - Verify Python installation
   - Check tkinter installation
   - Ensure compressor.exe is in the correct directory

3. **Compression fails:**
   - Check file permissions
   - Verify input file format
   - Ensure sufficient disk space
   - Check error messages in terminal
