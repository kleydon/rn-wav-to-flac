export interface WavMetaData {
    sampleRate: number;
    numChannels: number;
    bitsPerSample: number;
    numSamples: number;
}
export interface GetWavMetaDataResult {
    resultCode: number;
    wavMetaData: WavMetaData;
}
export declare function getWavMetaData(inputWavFilePath: string): Promise<GetWavMetaDataResult>;
export declare function wavToFlac(inputWavFilePath: string, outputFlacFilePath: string): Promise<number>;
export declare function multiply(a: number, b: number): Promise<number>;
//# sourceMappingURL=index.d.ts.map