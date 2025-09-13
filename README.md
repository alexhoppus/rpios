# RPiOS Kernel (Early Version)

This is an early version of **RPiOS kernel**, which currently includes:

- A simple **UART bootloader**
- The **kernel itself**

The project is aimed at experimenting with bare-metal development on Raspberry Pi boards.

---

## ⚙️ Features

Currently supported:

- **UART bootloader** for loading kernel images
- **Basic memory management**
- **Multiple address spaces**
- **Round-robin scheduler**
- **Loading multiple binaries**
- **Simple syscall handler**
- **Basic drivers**, including:
  - Interrupt controller
  - UART
  - Timer

---

## ⚙️ Requirements

Make sure you have the **ARM EABI toolchain** installed with `g++` support.
In particular, we use the `arm-none-eabi-*` cross toolchain.

---

## 🚀 How to Build and Run

### 1. Build UART Bootloader
```bash
cd ./uart_bootloader
./build.sh
```

This will produce a `kernel.bin` image.
Copy it to the **SD card boot partition** and rename it as follows:

- `kernel7.img` for **Raspberry Pi 2**

When properly set up, the bootloader should output:
```
: Waiting for image data on UART
```

---

### 2. Build Kernel
```bash
cd ./kernel
./build.sh
```

---

### 3. Send Kernel via UART
Use the `send.sh` script to communicate with the UART bootloader and upload the kernel:

```bash
./send.sh /dev/ttyUSB0
```

Expected output:
```
: Waiting for image data on UART
: Data transfer detected, downloading the image ...
: Image size (bytes): 4324

: Data transfer finished, giving up control to kernel
```

---

## 📌 Notes
- Supports **basic multitasking and system calls**.
- Currently only **basic UART bootloading and kernel execution** are supported.  
- This is an **early stage project**
- Tested on Raspberry Pi 2B.

---

## 📄 License
This project is released under the [MIT License](LICENSE).
