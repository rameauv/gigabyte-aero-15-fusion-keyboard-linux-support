# Gigabyte Aero 15 Fusion Keyboard Linux Support

Linux support for the Gigabyte Aero 15 laptop keyboard
Based on martin31821's work https://github.com/martin31821/fusion-kbd-controller and by reverse engineering the original Windows software

## Features
- lighteffect selection (implemented)
- color selection (implemented)
- lighteffect direction selection (TODO)
- FW update (TODO)
- macros (TODO)

I'll implement missing features if I need them.

Should I rewrite it in Rust? Zig? 🤔

## Building

### Build the TUI
```bash
bazel build //:tui
```

### Build for Release (Optimized)
```bash
bazel build --config=release -c opt //:tui
```

## Usage

Run the TUI directly:
```bash
# May require sudo for USB access
sudo ./bazel-bin/tui
