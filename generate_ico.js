const fs = require('fs');
const path = require('path');

const width = 32;
const height = 32;

// Create 32x32 pixel buffer (BGRA)
const pixels = new Uint8Array(width * height * 4);

function setPixel(x, y, r, g, b, a) {
  if (x < 0 || x >= width || y < 0 || y >= height) return;
  // BMP stores rows bottom-to-top!
  const row = (height - 1 - y);
  const idx = (row * width + x) * 4;
  pixels[idx] = b;
  pixels[idx + 1] = g;
  pixels[idx + 2] = r;
  pixels[idx + 3] = a;
}

// Distance to rounded rectangle
function roundedRectDist(x, y, w, h, r) {
  const dx = Math.max(r - x, 0, x - (w - 1 - r));
  const dy = Math.max(r - y, 0, y - (h - 1 - r));
  return Math.sqrt(dx * dx + dy * dy);
}

// 1. Draw rounded rectangle background (Black #0a0a0a with subtle border)
for (let y = 0; y < height; y++) {
  for (let x = 0; x < width; x++) {
    const r = 7;
    const dist = roundedRectDist(x, y, width, height, r);
    if (dist <= r) {
      if (dist > r - 1.2) {
        // Border highlight
        setPixel(x, y, 45, 45, 45, 255);
      } else {
        // Background black
        setPixel(x, y, 12, 12, 12, 255);
      }
    } else {
      // Transparent outside rounded rect
      setPixel(x, y, 0, 0, 0, 0);
    }
  }
}

// Helper: draw anti-aliased thick line
function drawThickLine(x0, y0, x1, y1, thickness, r, g, b, a) {
  const steps = Math.max(Math.abs(x1 - x0), Math.abs(y1 - y0)) * 4;
  for (let i = 0; i <= steps; i++) {
    const t = i / steps;
    const cx = x0 + (x1 - x0) * t;
    const cy = y0 + (y1 - y0) * t;
    const rad = thickness / 2;
    for (let dy = -Math.ceil(rad); dy <= Math.ceil(rad); dy++) {
      for (let dx = -Math.ceil(rad); dx <= Math.ceil(rad); dx++) {
        const d = Math.sqrt(dx * dx + dy * dy);
        if (d <= rad) {
          const px = Math.round(cx + dx);
          const py = Math.round(cy + dy);
          setPixel(px, py, r, g, b, a);
        }
      }
    }
  }
}

// Helper: draw filled circle
function drawCircle(cx, cy, radius, r, g, b, a) {
  for (let y = Math.floor(cy - radius); y <= Math.ceil(cy + radius); y++) {
    for (let x = Math.floor(cx - radius); x <= Math.ceil(cx + radius); x++) {
      const d = Math.sqrt((x - cx) ** 2 + (y - cy) ** 2);
      if (d <= radius) {
        setPixel(x, y, r, g, b, a);
      }
    }
  }
}

// 2. Draw Chevron: (9, 10) -> (16, 16) -> (9, 22)
drawThickLine(9, 10, 16, 16, 2.6, 255, 255, 255, 255);
drawThickLine(16, 16, 9, 22, 2.6, 255, 255, 255, 255);

// 3. Draw Underscore: (19, 22) -> (25, 22)
drawThickLine(19, 22, 25, 22, 2.6, 255, 255, 255, 255);

// 4. Draw Hub Circle Dot: (21, 11)
drawCircle(22, 11, 2.5, 255, 255, 255, 255);

// ICO Construction
const headerSize = 6;
const dirEntrySize = 16;
const bmpHeaderSize = 40;
const pixelDataSize = width * height * 4;
const maskDataSize = (width / 8) * height; // 32/8 * 32 = 128 bytes
const imageDataSize = bmpHeaderSize + pixelDataSize + maskDataSize;
const totalFileSize = headerSize + dirEntrySize + imageDataSize;

const icoBuffer = Buffer.alloc(totalFileSize);
let offset = 0;

// ICO Header
icoBuffer.writeUInt16LE(0, offset); offset += 2; // Reserved
icoBuffer.writeUInt16LE(1, offset); offset += 2; // Image type (1 = icon)
icoBuffer.writeUInt16LE(1, offset); offset += 2; // Image count = 1

// Directory Entry
icoBuffer.writeUInt8(width, offset++); // Width
icoBuffer.writeUInt8(height, offset++); // Height
icoBuffer.writeUInt8(0, offset++); // Color palette
icoBuffer.writeUInt8(0, offset++); // Reserved
icoBuffer.writeUInt16LE(1, offset); offset += 2; // Color planes
icoBuffer.writeUInt16LE(32, offset); offset += 2; // Bits per pixel
icoBuffer.writeUInt32LE(imageDataSize, offset); offset += 4; // Image data size
icoBuffer.writeUInt32LE(headerSize + dirEntrySize, offset); offset += 4; // Offset to BMP

// BITMAPINFOHEADER
icoBuffer.writeUInt32LE(40, offset); offset += 4; // Header size
icoBuffer.writeInt32LE(width, offset); offset += 4; // Width
icoBuffer.writeInt32LE(height * 2, offset); offset += 4; // Height * 2 (BMP in ICO)
icoBuffer.writeUInt16LE(1, offset); offset += 2; // Planes
icoBuffer.writeUInt16LE(32, offset); offset += 2; // Bits per pixel
icoBuffer.writeUInt32LE(0, offset); offset += 4; // Compression (BI_RGB)
icoBuffer.writeUInt32LE(pixelDataSize, offset); offset += 4; // Image size
icoBuffer.writeInt32LE(0, offset); offset += 4; // XPelsPerMeter
icoBuffer.writeInt32LE(0, offset); offset += 4; // YPelsPerMeter
icoBuffer.writeUInt32LE(0, offset); offset += 4; // Colors used
icoBuffer.writeUInt32LE(0, offset); offset += 4; // Important colors

// Copy pixel data
Buffer.from(pixels.buffer).copy(icoBuffer, offset);
offset += pixelDataSize;

// Write AND mask (all zeroes for 32-bit transparent icons)
icoBuffer.fill(0, offset, offset + maskDataSize);

const targetPath = path.join(__dirname, 'website', 'favicon.ico');
fs.writeFileSync(targetPath, icoBuffer);
console.log(`favicon.ico successfully generated at ${targetPath} (${totalFileSize} bytes)`);
