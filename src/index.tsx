import { NativeModules, Platform } from 'react-native'

const LINKING_ERROR =
  `The package 'rn-wav-to-flac' doesn't seem to be linked. Make sure: \n\n` +
  Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
  '- You rebuilt the app after installing the package\n' +
  '- You are not using Expo managed workflow\n';

const RnWavToFlac = NativeModules.RnWavToFlac  ? NativeModules.RnWavToFlac  : new Proxy(
      {},
      {
        get() {
          throw new Error(LINKING_ERROR);
        },
      }
    );

export function multiply(a: number, b: number): Promise<number> {
  return RnWavToFlac.multiply(a, b)
}

export function wavToFlac(inputWavFilePath: string, outputFlacFilePath: string): Promise<number> {
  return RnWavToFlac.wavToFlac(inputWavFilePath.replace('file://', ''), outputFlacFilePath)
}

