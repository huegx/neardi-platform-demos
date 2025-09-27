# neardi-platform-demos

This repository contains demo programs for Neardi embedded Linux platforms (e.g., LKD3576, LKD3588).
Demos are organized by functionality and language (Python / C).

## Current Demo

### `dual_screen`
- **Function:** Demonstrates dual-screen output. Left screen shows `glmark2-es2` with FPS and annotations, right screen shows a USB camera feed. If the camera fails to start, Chromium browser will open as fallback.
- **Languages:** Python (`dual_screen.py`) and C (`dual_screen.c`)
- **Dependencies:**
  - `glmark2-es2`
  - `GStreamer` (`gst-launch-1.0`)
  - USB camera device (`/dev/video*`)
  - `chromium` (optional fallback)
- **Usage:**
  ```bash
  # Python version
  python3 dual_screen/python/dual_screen.py

  # C version
  gcc dual_screen/c/dual_screen.c -o dual_screen/c/dual_screen
  ./dual_screen/c/dual_screen
  ```
