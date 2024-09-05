'use strict';

const common = require('../common.js');
const assert = require('assert');
const { xxhash32, crc32 } = require('internal/hash');

const bench = common.createBenchmark(main, {
  n: [1e5],
  size: [16, 64, 256, 1024, 4096],
  type: ['xxhash32', 'crc32']
});

function main({ n, size, type }) {
  const buffer = Buffer.alloc(size, 'a');
  
  let hashFunction;
  switch (type) {
    case 'xxhash32':
      hashFunction = xxhash32;
      break;
    case 'crc32':
      hashFunction = crc32;
      break;
    default:
      throw new Error(`Unknown hash type: ${type}`);
  }

  bench.start();

  for (let i = 0; i < n; i++) {
    hashFunction(buffer);
  }

  bench.end(n);
}