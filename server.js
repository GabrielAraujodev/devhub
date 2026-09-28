const http = require('http');
const fs = require('fs');
const path = require('path');

const MIME_TYPES = {
  '.html': 'text/html; charset=utf-8',
  '.css': 'text/css; charset=utf-8',
  '.js': 'application/javascript; charset=utf-8',
  '.svg': 'image/svg+xml',
  '.ico': 'image/x-icon',
  '.png': 'image/png',
  '.zip': 'application/zip',
  '.exe': 'application/octet-stream',
  '.json': 'application/json'
};

const PORT = 8085;
const WEBSITE_DIR = path.join(__dirname, 'website');

const server = http.createServer((req, res) => {
  let decoded = decodeURIComponent(req.url.split('?')[0]);
  let filePath = path.join(WEBSITE_DIR, decoded === '/' ? 'index.html' : decoded);

  fs.stat(filePath, (err, stats) => {
    if (err || !stats.isFile()) {
      res.writeHead(404, { 'Content-Type': 'text/plain; charset=utf-8' });
      res.end('404 Not Found');
      return;
    }

    const ext = path.extname(filePath).toLowerCase();
    const contentType = MIME_TYPES[ext] || 'application/octet-stream';
    const isBinaryDownload = ext === '.zip' || ext === '.exe';

    const headers = {
      'Content-Type': contentType,
      'Content-Length': stats.size,
      'Cache-Control': isBinaryDownload ? 'no-cache' : 'public, max-age=3600'
    };

    if (isBinaryDownload) {
      headers['Content-Disposition'] = `attachment; filename="${path.basename(filePath)}"`;
    }

    res.writeHead(200, headers);
    fs.createReadStream(filePath).pipe(res);
  });
});

server.listen(PORT, () => {
  console.log(`DevHub Download Server online at http://localhost:${PORT}`);
});
