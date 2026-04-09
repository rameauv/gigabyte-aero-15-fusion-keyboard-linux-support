# Gigabyte Aero 15 Fusion Keyboard Linux Support

linux support for the Gigabyte Aero 15 laptop HW

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
