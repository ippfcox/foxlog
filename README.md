# foxlog

>  A lightweight and fast header-only logging library for C projects.

## Features

- Header-only, easy to integrate
- Colorized output (configurable)
- Macro-based logging API
- Optional logger instance support
- Cross-platform: Linux / Windows / embedded

## Usage

```c
#define FOXLOG_IMPLEMENTATION
#include "foxlog.h"

int main() {
    foxloginfo("hello, world!");
    return 0;
}
