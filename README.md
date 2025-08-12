# Steganography_Project

📒 This project is a command-line based Steganography application developed in C that enables users to hide and retrieve secret files within BMP images using the Least Significant Bit (LSB) technique. It supports encoding of .txt, .c, and .sh files, and decoding them back with full data recovery. Built with modular C programming, it includes robust file validation, error handling, and efficient binary file operations, simulating real-world data-hiding and information security techniques.

📌 Project Overview
In the digital age, securing sensitive information is crucial. This project provides a command-line solution for hiding secret files inside BMP images using the Least Significant Bit (LSB) steganography technique. It enables users to encode .txt, .c, or .sh files into images and decode them back with complete accuracy. The system ensures robust file validation, error handling, and efficient binary data processing, making it a practical demonstration of real-world data concealment and retrieval methods.

⚙️ Components / Tools Used

C Programming Language
GCC Compiler
Terminal / Command-line Interface
BMP Image Files for data embedding and extraction
Modular File Structure
main.c – Program entry & operation selection (encode/decode)
encode.c / encode.h – Core encoding logic using LSB technique
decode.c / decode.h – Core decoding logic to retrieve hidden files
common.h – Shared constants, macros, and utility definitions
types.h – Custom type definitions & enums for status handling

🧠 Working Principle
The system uses a command-line interface to perform data hiding (encoding) and data extraction (decoding) in BMP images using the Least Significant Bit (LSB) technique.
Encoding Process
Select Source Image & Secret File – Takes a BMP image and a file (.txt, .c, .sh) to hide.
Validate Files – Ensures the image has enough capacity to store the secret data.
Embed Magic String – Encodes a predefined signature to verify steganography presence during decoding.
Store File Metadata – Encodes file extension length, extension, and file size.
Embed Secret Data – Stores each bit of the secret file into the LSB of image pixels.
Generate Stego Image – Creates a new BMP file with the embedded data.
Decoding Process
Select Stego Image – Opens the BMP image containing hidden data.
Verify Magic String – Confirms that the image contains embedded data.
Retrieve File Metadata – Extracts the file extension size, extension, and file size.
Extract Secret Data – Reads each hidden bit from the image and reconstructs the original file.
Save Secret File – Writes the extracted content to a new file with the original extension.

🧠 Working Principle
The system uses a command-line interface to perform data hiding (encoding) and data extraction (decoding) in BMP images using the Least Significant Bit (LSB) technique.

 🔐 Encoding Process:

Select Source Image & Secret File – Takes a BMP image and a file (.txt, .c, .sh) to hide.
Validate Files – Ensures the image has enough capacity to store the secret data.
Embed Magic String – Encodes a predefined signature to verify steganography presence during decoding.
Store File Metadata – Encodes file extension length, extension, and file size.
Embed Secret Data – Stores each bit of the secret file into the LSB of image pixels.
Generate Stego Image – Creates a new BMP file with the embedded data.

🔓 Decoding Process:
Select Stego Image – Opens the BMP image containing hidden data.
Verify Magic String – Confirms that the image contains embedded data.
Retrieve File Metadata – Extracts the file extension size, extension, and file size.
Extract Secret Data – Reads each hidden bit from the image and reconstructs the original file.
Save Secret File – Writes the extracted content to a new file with the original extension.

🗂️ File Structure

📂 Steganography-Project
│
├── 📄 main.c                 # Entry point of the program, handles CLI arguments
├── 📄 encode.c               # Implementation of encoding functions
├── 📄 encode.h               # Header file for encoding
├── 📄 decode.c               # Implementation of decoding functions
├── 📄 decode.h               # Header file for decoding
├── 📄 common.h               # Common definitions (magic string, constants)
├── 📄 types.h                # User-defined types and enums
│
├── 📂 assets
│   ├── sample.bmp            # Example source BMP image
│   ├── secret.txt            # Example secret file to hide
│   └── stego.bmp             # Output BMP with hidden data
│
├── 📂 docs
│   └── README.md             # Project documentation
│
└── 📂 build
    └── executable            # Compiled binary output

📸 Project Media Sample Terminal Output:

Encoding Input Validation :
<img width="1224" height="157" alt="Screenshot 2025-08-12 140245" src="https://github.com/user-attachments/assets/36784503-7966-4671-b1d1-d462c1e8d8f9" />

Encoding Process :
<img width="1174" height="937" alt="Screenshot 2025-08-12 140331" src="https://github.com/user-attachments/assets/a8ca4d9c-dccc-4a53-8d41-4a8be7de3b65" />

Decoding Input Validation :
<img width="1240" height="122" alt="Screenshot 2025-08-12 140835" src="https://github.com/user-attachments/assets/e61d05b3-32e5-4932-b823-19acac77b148" />

Decoding Process :
<img width="1217" height="953" alt="Screenshot 2025-08-12 140905" src="https://github.com/user-attachments/assets/1d164369-f1d3-4d2d-b18d-0c4d427875d1" />

Project demo : https://drive.google.com/file/d/1hIsvHC2eOl8eufdUeUtYGN9rXpV9UBtR/view?usp=sharing

🚀 Future Enhancements

🔒 Encryption Before Embedding
Add encryption (AES/DES) to the secret file before encoding so even if extracted, it remains unreadable without a key.
🎨 Support for Multiple Image Format
Extend support from .bmp to .png and .jpg formats using appropriate libraries.
📁 Multiple File Encoding
Allow hiding multiple secret files in one image with a directory structure.
📦 Compression of Secret File
Implement compression before encoding to save space and embed larger files.
🖼 GUI Interface
Build a graphical user interface for easier usage instead of CLI commands.
📱 Mobile/Embedded Version
Port the project to Android or microcontroller platforms for secure on-device data hiding.
📊 Capacity Analyzer
Add a feature to analyze the image’s maximum capacity before encoding.
🛡 Integrity Check
Use checksums or hashes to verify that the hidden data was extracted without corruption.

👩‍💻 Developed By Pavan Kumar G V Graduate Engineer – Embedded Systems Enthusiast.
📧 gvpavanec008@gmail.com

