# Image Compression & Steganography

A C project implementing two independent systems for image manipulation: **quadtree-based image compression** and **LSB steganography**. Both operate on grayscale PPM images and are built entirely from scratch using low-level C with no external image libraries.

---

## Features

### Quadtree Compression
Images are recursively decomposed into a quadtree structure based on pixel intensity variance. Regions with low variance (uniform areas) are stored as single leaf nodes, while high-variance regions are subdivided further — achieving compression by representing smooth areas with far fewer values.

- Recursive quadtree construction driven by a configurable **max RMSE threshold**
- Handles non-square and odd-dimension images via asymmetric splits (2-child and 3-child nodes)
- Compressed tree serialized to disk in **preorder traversal format** for compact storage
- Tree can be reloaded from file and reconstructed exactly
- Compressed quadtree rendered back to a PPM image via leaf-fill traversal

### LSB Steganography
Secret data is hidden inside a cover image by overwriting the **least significant bit** of each pixel's intensity value — a change imperceptible to the human eye.

**Two modes:**
- **Hide/reveal a text message** — encodes each character of a string bit-by-bit across sequential pixels; a null terminator signals end of message on extraction
- **Hide/reveal a secret image** — encodes the secret image's width and height into the first 16 pixels, then encodes each pixel of the secret image across 8 pixels of the cover image; fully recoverable on extraction

---

## How It Works

### Quadtree Compression

Each node stores: intensity (average of region), position (row/col), and dimensions (height/width). At construction time:

1. Compute the **average intensity** and **RMSE** of the current region
2. If `RMSE > max_rmse` and the region is splittable, subdivide into up to 4 quadrants
3. Otherwise, mark as a **leaf node** storing the average intensity

On reconstruction, each leaf fills its entire region with its stored intensity value, approximating the original image. Higher `max_rmse` = more compression, lower fidelity. Lower `max_rmse` = less compression, higher fidelity.

### LSB Steganography

Each pixel's intensity is modified at the bit level:
```
new_intensity = (original_intensity & ~0x1) | secret_bit
```
The LSB carries one bit of hidden data. To hide one byte of a message, 8 pixels are used. The change in each pixel's intensity is at most ±1, making it visually undetectable.

---

## Project Structure

```
Image_Compression/
├── src/
│   ├── image.c         # PPM loader, hide_message, reveal_message, hide_image, reveal_image
│   ├── qtree.c         # Quadtree construction, serialization, PPM rendering
│   └── hw3_main.c      # Demo driver: runs compression and steganography end-to-end
├── include/
│   ├── image.h         # Image struct and steganography API
│   └── qtree.h         # QTNode struct and quadtree API
├── images/
│   └── originals/      # Sample input PPM images
├── tests/
│   └── input/          # Preorder quadtree files for testing
├── CMakeLists.txt
└── .gitignore
```

---

## Build & Run

### Requirements
- GCC or Clang
- CMake ≥ 3.10
- A C99-compatible compiler

### Build
```bash
mkdir build && cd build
cmake ..
make
```

### Run
```bash
./hw3_main
```

The driver program demonstrates:
1. Loading a PPM image and building a quadtree with `max_rmse = 25`
2. Saving the compressed tree to a preorder text file
3. Hiding one image inside another using LSB steganography
4. Extracting and saving the hidden image

Input images are read from `images/originals/`. Output files are written to `tests/output/`.

---

## Image Format

All images use the **PPM P3 format** (plain-text RGB). Since images are grayscale, all three RGB channels per pixel are set to the same intensity value (0–255). The loader skips PPM comment lines (`#`) automatically.

---

## API Reference

### Image (`image.h`)
| Function | Description |
|----------|-------------|
| `load_image(filename)` | Load a PPM file into an `Image` struct |
| `delete_image(image)` | Free image memory |
| `get_image_width(image)` | Get image width |
| `get_image_height(image)` | Get image height |
| `get_image_intensity(image, row, col)` | Get pixel intensity at (row, col) |
| `hide_message(message, input, output)` | Encode a string into a PPM via LSB |
| `reveal_message(input)` | Decode and return the hidden string |
| `hide_image(secret, input, output)` | Encode a secret image into a cover PPM |
| `reveal_image(input, output)` | Decode and save the hidden image |

### Quadtree (`qtree.h`)
| Function | Description |
|----------|-------------|
| `create_quadtree(image, max_rmse)` | Build a quadtree from an image |
| `delete_quadtree(root)` | Free all quadtree nodes |
| `save_preorder_qt(root, filename)` | Serialize tree to file |
| `load_preorder_qt(filename)` | Reconstruct tree from file |
| `save_qtree_as_ppm(root, filename)` | Render compressed tree back to PPM |
| `get_child1/2/3/4(node)` | Access child nodes |
| `get_node_intensity(node)` | Get a node's stored intensity |
