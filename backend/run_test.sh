#!/bin/sh
cd test
npm install ws
node nikon_raw_test.js
rm -rf node_modules
rm package-lock.json
rm package.json
cd ..
