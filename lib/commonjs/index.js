"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports.getWavMetaData = getWavMetaData;
exports.multiply = multiply;
exports.wavToFlac = wavToFlac;
var _reactNative = require("react-native");
const LINKING_ERROR = `The package 'rn-wav-to-flac' doesn't seem to be linked. Make sure: \n\n` + _reactNative.Platform.select({
  ios: "- You have run 'pod install'\n",
  default: ''
}) + '- You rebuilt the app after installing the package\n' + '- You are not using Expo Go\n';
const RnWavToFlac = _reactNative.NativeModules.RnWavToFlac ? _reactNative.NativeModules.RnWavToFlac : new Proxy({}, {
  get() {
    throw new Error(LINKING_ERROR);
  }
});
function getWavMetaData(inputWavFilePath) {
  return RnWavToFlac.getWavMetaData(inputWavFilePath.replace('file://', '')).then(r => JSON.parse(r));
}
function wavToFlac(inputWavFilePath, outputFlacFilePath) {
  return RnWavToFlac.wavToFlac(inputWavFilePath.replace('file://', ''), outputFlacFilePath);
}

// Example function
function multiply(a, b) {
  return RnWavToFlac.multiply(a, b);
}
//# sourceMappingURL=index.js.map