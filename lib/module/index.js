"use strict";

import { NativeModules, Platform } from 'react-native';
const LINKING_ERROR = `The package 'rn-wav-to-flac' doesn't seem to be linked. Make sure: \n\n` + Platform.select({
  ios: "- You have run 'pod install'\n",
  default: ''
}) + '- You rebuilt the app after installing the package\n' + '- You are not using Expo Go\n';
const RnWavToFlac = NativeModules.RnWavToFlac ? NativeModules.RnWavToFlac : new Proxy({}, {
  get() {
    throw new Error(LINKING_ERROR);
  }
});
export function getWavMetaData(inputWavFilePath) {
  return RnWavToFlac.getWavMetaData(inputWavFilePath.replace('file://', '')).then(r => JSON.parse(r));
}
export function wavToFlac(inputWavFilePath, outputFlacFilePath) {
  return RnWavToFlac.wavToFlac(inputWavFilePath.replace('file://', ''), outputFlacFilePath);
}

// Example function
export function multiply(a, b) {
  return RnWavToFlac.multiply(a, b);
}
//# sourceMappingURL=index.js.map