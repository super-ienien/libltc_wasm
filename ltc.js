let initialized = false;
let resolveDeferred;

const wasmMemory = new WebAssembly.Memory({
  initial: 4096,
  maximum: 8128
});

let libltc = {
  // wasmMemory,
  onRuntimeInitialized() {
    console.log('ready');
    initialized = true;
    if (resolveDeferred && !resolveDeferred.resolved) {
      resolveDeferred.resolved = true;
      resolveDeferred.resolve();
    }
  }
};

function initializationAssert() {
  if (!initialized) {
    throw new Error('you must call initLibLTC() first');
  }
}

function initLibLTC() {
  if (!resolveDeferred) {
    resolveDeferred = {
      resolved: false
    };
    resolveDeferred.promise = new Promise(function (resolve) {
      resolveDeferred.resolve = resolve;
    });
    loadLibLTC(libltc);
  }
  return resolveDeferred.promise;
}

class LTCDecoder {
  constructor(apv = 1920, queue_size = 32) {
    initializationAssert();
    this.decoder = libltc._ltc_decoder_create(apv, queue_size);
    this.released = false;
  }

  flushQueue() {
    initializationAssert();
    this.releasedAssert();
    return libltc._ltc_decoder_queue_flush(this.decoder);
  }

  write(buffer) {
    initializationAssert();
    this.releasedAssert();
    return libltc._ltc_decoder_write_float(this.decoder, copyBufferToLibLtMemory(buffer), buffer.length);
  }

  read() {
    initializationAssert();
    this.releasedAssert();
    const frame = new libltc.LTCFrameJS();
    console.log('read result', frame);
    const r = libltc.__Z18ltc_decoder_read_2P10LTCDecoderR10LTCFrameJS(this.decoder, frame);
    console.log('read result', r, frame);
  }

  releasedAssert() {
    if (!this.decoder) {
      throw new Error('This LTC decoder has been released');
    }
  }

  release() {
    if (this.decoder) {
      libltc._ltc_decoder_free(this.decoder);
      this.decoder = null;
    }
  }
}

function copyBufferToLibLtMemory(buffer) {
  const typedArray = new Float32Array(buffer.length);
  for (let i=0; i<buffer.length; i++) {
    typedArray[i] = buffer[i];
  }
  const pointer = libltc._malloc(typedArray.length * typedArray.BYTES_PER_ELEMENT);
  libltc.HEAPF32.set(typedArray, pointer >> 2)
  return pointer;
}

function copyBufferFromLibLtcMemory(pointer, length) {
  const arrayData = []
  for (let i= 0; i < length + 1; i++) {
    arrayData.push(libltc.HEAPF32[pointer/Float32Array.BYTES_PER_ELEMENT+i])
  }
  new Float32Array()
}