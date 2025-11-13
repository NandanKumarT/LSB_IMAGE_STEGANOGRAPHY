# 🕵️‍♂️ LSB Image Steganography

## 📘 Overview
**LSB Image Steganography** is a C-based project that implements **Least Significant Bit (LSB)** encoding and decoding techniques to hide secret information inside image files.  
The project allows you to **encode a secret message or file inside an image** and later **decode** it back without noticeable changes to the image.

---

## 🎯 Objective
To demonstrate the concept of **digital steganography**, where information is concealed within an image using **LSB manipulation** — the smallest bit in each pixel — so that the hidden data remains invisible to the human eye.

---

## ⚙️ Features
- Hide secret data (text or file) inside an image.  
- Extract (decode) hidden data from a stego image.  
- Validate file formats and handle I/O efficiently.  
- CLI-based interface for encoding and decoding operations.  
- Modular C code for better readability and maintainability.

---

## 🧠 How It Works
1. **Encoding Phase**
   - Reads the source image pixel data.
   - Replaces the **least significant bits** of each pixel with bits of the secret data.
   - Writes the modified pixels to a new image (stego image).

2. **Decoding Phase**
   - Reads the LSBs from the stego image.
   - Reconstructs and extracts the original hidden message or file.

---

## 📁 Project Structure
LSB_IMAGE_STEGANOGRAPHY/
│
├── encode.c # Handles encoding (hiding) logic
├── decode.c # Handles decoding (extracting) logic
├── encode.h # Header for encoding functions
├── decode.h # Header for decoding functions
├── common.h # Shared constants and function prototypes
├── main.c # Entry point, handles user input and mode selection
├── Makefile # Compilation script
└── README.md # Project documentation
