#!/bin/bash
# Build simulator.html from modular source.
# Requires: node + npx (for esbuild)
# Output: simulator.html (self-contained, works by double-click)

set -e
cd "$(dirname "$0")"

echo "Building simulator..."

# Bundle JS with esbuild (downloads via npx if not installed)
npx --yes esbuild src/main.js \
  --bundle \
  --format=iife \
  --platform=browser \
  --outfile=dist/bundle.js \
  --log-level=warning

# Inline CSS + JS bundle into template.html -> simulator.html
node -e "
const fs = require('fs');
let html = fs.readFileSync('template.html', 'utf8');
const css  = fs.readFileSync('style.css', 'utf8');
const js   = fs.readFileSync('dist/bundle.js', 'utf8');
html = html.replace('<!-- INLINE_CSS -->', '<style>' + css + '</style>');
html = html.replace('<!-- INLINE_JS -->',  '<script>' + js + '</script>');
fs.writeFileSync('simulator.html', html);
const kb = (fs.statSync('simulator.html').size / 1024).toFixed(1);
console.log('simulator.html built (' + kb + ' KB)');
"
