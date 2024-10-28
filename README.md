# xv6-labs-2024

## Introduction

This repository contains the implementation of the xv6 operating system labs, aimed at enhancing understanding of operating systems concepts. The xv6 is a simple Unix-like teaching operating system developed at MIT, which serves as a useful platform for exploring fundamental OS principles.

## Getting Started

To get started with xv6-labs, follow the steps below:

### Prerequisites

- **Operating System**: Linux or macOS
- **Required Packages**:
  - `make`
  - `gcc`
  - `qemu` for virtualization
  - `git` for version control

### Installation

1. Clone the repository:

   ```bash
   git clone --branch lab1p2 https://github.com/ahmedanwar123/xv6-labs-2024.git
   cd xv6-labs-2024
   ```
2. Build the project:

   ```bash
   make
   ```
3. Run xv6 using QEMU

   ```bash
   make qemu
   ```
4. Start testing
   ```bash
   $ find . b
   ```
   ```bash
   $ echo hello too | xargs echo bye
   ```
   ```bash
   $ find . b | xargs grep hello
   ```
