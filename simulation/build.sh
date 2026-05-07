#!/bin/bash
# Build the self-contained browser simulator without downloading packages.

set -e
cd "$(dirname "$0")"

mkdir -p dist

node -e "
const fs = require('fs');
let model = fs.readFileSync('src/model.js', 'utf8');
let app = fs.readFileSync('src/app.js', 'utf8');
model = model.replace(/export /g, '');
app = app.replace(/import \\{[^}]+\\} from '\\.\\/model\\.js';\\n/, '');
fs.writeFileSync('dist/bundle.js', model + '\\n' + app);
let html = fs.readFileSync('template.html', 'utf8');
const css = fs.readFileSync('style.css', 'utf8');
const js = fs.readFileSync('dist/bundle.js', 'utf8');
html = html.replace('%%INLINE_CSS%%', '<style>' + css + '</style>');
html = html.replace('%%INLINE_JS%%', '<script>' + js + '</script>');
fs.writeFileSync('simulator.html', html);
const kb = (fs.statSync('simulator.html').size / 1024).toFixed(1);
console.log('simulator.html built (' + kb + ' KB)');
"
