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

const defaultTC = {
  timezone: "+0000",
  years: 1,
  months: 1,
  days: 1,
  hours: 0,
  mins: 0,
  secs: 0,
  frames: 0,
}

class LTCEncoder {
  constructor(audioCtx, fps = 25, channels = 1, standard = 1, flags = 0) {
    initializationAssert();
    this.audioCtx = audioCtx;
    this.destinations = new Set();
    this.frameDuration = 1000 / fps;
    this.isStarted = false;
    this.isPaused = false;
    this.isReversed = false;
    this.channels = channels;
    this.latency = 2;
    this.encoder = new libltc.LTCEncoderJS(this.audioCtx.sampleRate, fps, channels, flags);
    this.setTimecode({
      hours: 0,
      mins: 0,
      secs: 0,
      frames: 0
    });
    this.isDestroyed = false;
  }

  connect(node) {
    this.destinations.add(node);
  }

  disconnect(node) {
    this.destinations.delete(node);
  }

  setTimecode(tc) {
    initializationAssert();
    this.destroyedAssert();
    tc = Object.assign({}, defaultTC, tc);
    const tcJS = new libltc.SMPTETimecodeJS(tc.timezone, tc.years, tc.months, tc.days, tc.hours, tc.mins, tc.secs, tc.frames);
    this.encoder.setTimecode(tcJS);
    tcJS.delete();
  }

  reverse() {
    initializationAssert();
    this.destroyedAssert();
    this.isReversed = !this.isReversed;
  }

  resume() {
    initializationAssert();
    this.destroyedAssert();
    this.isPaused = false;
  }

  pause() {
    initializationAssert();
    this.destroyedAssert();
    this.isPaused = true;
  }

  togglePause() {
    initializationAssert();
    this.destroyedAssert();
    this.isPaused = !this.isPaused;
  }

  start() {
    initializationAssert();
    this.destroyedAssert();
    if (!this.isStarted) {
      this.isStarted = true;
      this.nextFrameAt = Math.round(this.audioCtx.currentTime * 1000) + this.frameDuration;
      this.interval = setInterval(() => {
        // console.log ('next', this.nextFrameAt - Math.round(this.audioCtx.currentTime * 1000), this.frameDuration);
        if (this.nextFrameAt - Math.round(this.audioCtx.currentTime * 1000) <= this.frameDuration) {
          if (!this.isPaused) {
            if (this.isReversed) {
              this.encoder.decrementTimecode();
            } else {
              this.encoder.incrementTimecode();
            }
          }
          this.sendFrame();
          this.nextFrameAt += this.frameDuration;
        }
      }, 5);
    }
  }

  stop() {
    initializationAssert();
    this.destroyedAssert();
    if (this.isStarted) {
      clearInterval(this.interval);
      this.isStarted = false;
      this.encoder.flushBuffer();
    }
  }

  sendFrame () {
    initializationAssert();
    this.destroyedAssert();
    const buffer = this.audioCtx.createBuffer(2, Math.round(this.audioCtx.sampleRate * (this.frameDuration / 1000)), this.audioCtx.sampleRate);
    if (!this.isReversed) {
      this.encoder.encodeFrame();
    } else {
      this.encoder.encodeReversedFrame();
    }
    const sample = convertUInt8toFloat32(copyBufferFromLibLtcMemory(this.encoder.getBuffer()));
    // console.log('encoderBuffer', sample);
    for (let i = 0; i < this.channels; i++) {
      buffer.copyToChannel(sample, i);
    }
    const source = this.audioCtx.createBufferSource();
    source.buffer = buffer;
    source.connect(this.audioCtx.destination);
    for (const node of this.destinations) {
      source.connect(node);
    }
    source.start(this.nextFrameAt / 1000 + this.latency * this.frameDuration / 1000);
    source.onended = function () {
      source.disconnect();
    }
  }

  destroy() {
    if (!this.isDestroyed) {
      this.isDestroyed = true;
      clearInterval(this.interval);
      this.encoder.delete();
      this.encoder = null;
    }
  }

  destroyedAssert() {
    if (this.isDestroyed) {
      throw new Error('This LTC encoder has been destroyed');
    }
  }
}

class LTCDecoder {
  constructor(audioCtx, source, channel = 0, apv = 1920, queue_size = 32) {
    initializationAssert();
    this.decoder = new libltc.LTCDecoderJS(apv, queue_size);
    this.isStarted = false;
    this.isDestroyed = false;
    this.audioCtx = audioCtx;
    this.channel = channel;
    this.source = source;
  }

  ontimecode = () => {};

  start() {
    initializationAssert();
    this.destroyedAssert();
    if (!this.isStarted) {
      this.isStarted = true;
      this.destination = this.audioCtx.createScriptProcessor(1024, 2, 2);
      if (this.source) {
        console.log('start dec')
        this.source.connect(this.destination);
      }
      console.log('start dec')
      this.destination.connect(this.audioCtx.destination);
      this.destination.onaudioprocess = (audioProcessingEvent) => {
        // The input buffer is a song we loaded earlier
        const inputBuffer = audioProcessingEvent.inputBuffer;
        const inputData = inputBuffer.getChannelData(this.channel);
        this.decoder.write(copyBufferToLibLtcMemory(inputData));
        let isValid = true;
        while (isValid) {
          const frame = this.decoder.read();
          console.log(frame.is_valid)
          if (frame.is_valid) {
            this.ontimecode({
              user1: frame.user1,
              frameUnits: frame.frame_units,
              user2: frame.user2,
              colFrame: frame.col_frame,
              dfbit: frame.dfbit,
              frameTens: frame.frame_tens,
              user3: frame.user3,
              secsUnits: frame.secs_units,
              user4: frame.user4,
              biphaseMarkPhaseCorrection: frame.biphase_mark_phase_correction,
              secsTens: frame.secs_tens,
              user5: frame.user5,
              minsUnits: frame.mins_units,
              user6: frame.user6,
              binaryGroupFlagBit0: frame.binary_group_flag_bit0,
              minsTens: frame.mins_tens,
              user7: frame.user7,
              hoursUnits: frame.hours_tens,
              user8: frame.user7,
              binaryGroupFlagBit2: frame.binary_group_flag_bit2,
              binaryGroupFlagBit1: frame.binary_group_flag_bit1,
              hoursTens: frame.hours_tens,
              syncWord: frame.sync_word,
              // offStart: frame.off_start,
              // offEnd: frame.off_end,
              reverse: frame.reverse,
              sampleMin: frame.sample_min,
              sampleMax: frame.sample_max,
              volume: frame.volume
            });
          }
          isValid = frame.is_valid;
          frame.delete();
        }
      };
    }
  }

  stop() {
    initializationAssert();
    this.destroyedAssert();
    if (this.isStarted) {
      this.isStarted = false;
      if (this.source) {
        this.source.disconnect();
      }
      this.destination.onaudioprocess = null;
      this.source = null;
      this.destination = null;
    }
  }

  destroyedAssert() {
    if (this.isDestroyed) {
      throw new Error('This LTC decoder has been destroyed');
    }
  }

  destroy() {
    if (!this.isDestroyed) {
      this.stop();
      this.decoder.delete();
      this.decoder = null;
    }
  }
}

function copyBufferToLibLtcMemory(buffer) {
  const typedArray = buffer;
  const pointer = libltc._malloc(typedArray.length * typedArray.BYTES_PER_ELEMENT);
  libltc.HEAPF32.set(typedArray, pointer >> 2)
  return {buf_ptr: pointer, size: typedArray.length};
}

function copyBufferFromLibLtcMemory(sample) {
  return libltc.HEAPU8.subarray(sample.buf_ptr, sample.buf_ptr + sample.size);
}

function convertUInt8toFloat32(incomingData) { // incoming data is a UInt8Array
  let i, l = incomingData.length;
  let outputData = new Float32Array(incomingData.length);
  for (i = 0; i < l; i++) {
    outputData[i] = (incomingData[i] - 128) / 128.0;
  }
  return outputData;
}
