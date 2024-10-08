
class Processor extends AudioWorkletProcessor {

  constructor() {
    super();
  }
  process([input], [output]) {
    this.port.postMessage({

    })
    return true;
  }
}

registerProcessor("processor", Processor);