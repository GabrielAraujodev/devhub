const fs = require('fs');
const path = require('path');
const zlib = require('zlib');

function createPng(width, height) {
  // RGBA buffer: each row has 1 filter byte + width * 4 bytes
  const rowBytes = 1 + width * 4;
  const rawData = Buffer.alloc(rowBytes * height);

  function setPixel(x, y, r, g, b, a) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    const offset = y * rowBytes + 1 + x * 4;
    rawData[offset] = r;
    rawData[offset + 1] = g;
    rawData[offset + 2] = b;
    rawData[offset + 3] = a;
  }

  function roundedRectDist(x, y, w, h, r) {
    const dx = Math.max(r - x, 0, x - (w - 1 - r));
    const dy = Math.max(r - y, 0, y - (h - 1 - r));
    return Math.sqrt(dx * dx + dy * dy);
  }

  const radius = width * 0.22; // 22% border radius (modern squircle)
  for (let y = 0; y < height; y++) {
    for (let x = 0; x < width; x++) {
      const dist = roundedRectDist(x, y, width, height, radius);
      if (dist <= radius) {
        if (dist > radius - 2) {
          // Border highlight #2b2b2b
          setPixel(x, y, 43, 43, 43, 255);
        } else {
          // Background #0a0a0a
          setPixel(x, y, 10, 10, 10, 255);
        }
      } else {
        setPixel(x, y, 0, 0, 0, 0);
      }
    }
  }

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
            setPixel(Math.round(cx + dx), Math.round(cy + dy), r, g, b, a);
          }
        }
      }
    }
  }

  function drawCircle(cx, cy, rad, r, g, b, a) {
    for (let y = Math.floor(cy - rad); y <= Math.ceil(cy + rad); y++) {
      for (let x = Math.floor(cx - rad); x <= Math.ceil(cx + rad); x++) {
        const d = Math.sqrt((x - cx) ** 2 + (y - cy) ** 2);
        if (d <= rad) {
          setPixel(x, y, r, g, b, a);
        }
      }
    }
  }

  const scale = width / 64;
  // Chevron: (19, 20) -> (31, 32) -> (19, 44)
  drawThickLine(19 * scale, 20 * scale, 31 * scale, 32 * scale, 5.5 * scale, 255, 255, 255, 255);
  drawThickLine(31 * scale, 32 * scale, 19 * scale, 44 * scale, 5.5 * scale, 255, 255, 255, 255);

  // Underscore: (36, 44) -> (48, 44)
  drawThickLine(36 * scale, 44 * scale, 48 * scale, 44 * scale, 5.5 * scale, 255, 255, 255, 255);

  // Hub circle: (42, 22) r=4.5
  drawCircle(42 * scale, 22 * scale, 4.5 * scale, 255, 255, 255, 255);

  // PNG chunk builder
  function crc32(buf) {
    let table = new Uint32Array(256);
    for (let i = 0; i < 256; i++) {
      let c = i;
      for (let k = 0; k < 8; k++) {
        c = (c & 1) ? (0xEDB88320 ^ (c >>> 1)) : (c >>> 1);
      }
      table[i] = c;
    }
    let crc = 0xFFFFFFFF;
    for (let i = 0; i < buf.length; i++) {
      crc = (crc >>> 8) ^ table[(crc ^ buf[i]) & 0xFF];
    }
    return (crc ^ 0xFFFFFFFF) >>> 0;
  }

  function makeChunk(type, data) {
    const len = data.length;
    const chunk = Buffer.alloc(8 + len + 4);
    chunk.writeUInt32BE(len, 0);
    chunk.write(type, 4, 4, 'ascii');
    data.copy(chunk, 8);
    const crcVal = crc32(chunk.subarray(4, 8 + len));
    chunk.writeUInt32BE(crcVal, 8 + len);
    return chunk;
  }

  const pngSignature = Buffer.from([137, 80, 78, 71, 13, 10, 26, 10]);

  // IHDR
  const ihdrData = Buffer.alloc(13);
  ihdrData.writeUInt32BE(width, 0);
  ihdrData.writeUInt32BE(height, 4);
  ihdrData.writeUInt8(8, 8); // bit depth 8
  ihdrData.writeUInt8(6, 9); // color type 6: RGBA
  ihdrData.writeUInt8(0, 10); // compression
  ihdrData.writeUInt8(0, 11); // filter
  ihdrData.writeUInt8(0, 12); // interlace 0
  const ihdrChunk = makeChunk('IHDR', ihdrData);

  // IDAT
  const compressed = zlib.deflateSync(rawData, { level: 9 });
  const idatChunk = makeChunk('IDAT', compressed);

  // IEND
  const iendChunk = makeChunk('IEND', Buffer.alloc(0));

  return Buffer.concat([pngSignature, ihdrChunk, idatChunk, iendChunk]);
}

const png192 = createPng(192, 192);
const pngPath192 = path.join(__dirname, 'website', 'favicon-192.png');
fs.writeFileSync(pngPath192, png192);
console.log(`favicon-192.png generated at ${pngPath192} (${png192.length} bytes)`);

const appleTouch = path.join(__dirname, 'website', 'apple-touch-icon.png');
fs.writeFileSync(appleTouch, png192);
console.log(`apple-touch-icon.png generated at ${appleTouch}`);
