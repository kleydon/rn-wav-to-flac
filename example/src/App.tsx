import * as React from 'react'
import RNFS from 'react-native-fs'
import DeviceInfo from 'react-native-device-info'
import { 
  StyleSheet,
  View,
  Text,
  Button,
  Platform,
  Alert
} from 'react-native'
import { 
  multiply, 
  wavToFlac 
} from 'rn-wav-to-flac'

const ilog = console.log
const elog = console.log

// NOTE! Must keep this in sync with /cpp/ResultCode.h
const wavToFlacResultCodes = [  
  //Ok code
  'RESULT_CODE_OK',
  //Error codes
  'RESULT_CODE_ERR_UNSPECIFIED',
  'RESULT_CODE_ERR_ARGS',
  'RESULT_CODE_ERR_WAV_FILE_SUFFIX',
  'RESULT_CODE_ERR_FLAC_FILE_SUFFIX',
  'RESULT_CODE_ERR_FILE_OPEN',
  'RESULT_CODE_ERR_NO_RIFF_CHUNK_HEADER',
  'RESULT_CODE_ERR_READ_RIFF_CHUNK_HEADER',
  'RESULT_CODE_ERR_READ_RIFF_CHUNK_SIZE',
  'RESULT_CODE_ERR_READ_RIFF_CHUNK_WAVE',
  'RESULT_CODE_ERR_NO_RIFF_CHUNK_WAVE',
  'RESULT_CODE_ERR_READ_POST_RIFF_CHUNK_HEADER',
  'RESULT_CODE_ERR_READ_JUNK_CHUNK_SIZE',
  'RESULT_CODE_ERR_READ_JUNK_CHUNK_DATA',
  'RESULT_CODE_ERR_READ_FMT_CHUNK_HEADER',
  'RESULT_CODE_ERR_NO_FMT_CHUNK_HEADER',
  'RESULT_CODE_ERR_READ_FMT_CHUNK_SIZE',
  'RESULT_CODE_ERR_READ_FMT_CHUNK_TAG',
  'RESULT_CODE_ERR_READ_FMT_CHUNK_NUM_CHANNELS',
  'RESULT_CODE_ERR_READ_FMT_CHUNK_SAMPLE_RATE',
  'RESULT_CODE_ERR_READ_FMT_CHUNK_AVG_BYTES_PER_SEC',
  'RESULT_CODE_ERR_READ_FMT_CHUNK_BLOCK_ALIGN',
  'RESULT_CODE_ERR_READ_FMT_CHUNK_BITS_PER_SAMPLE',
  'RESULT_CODE_ERR_READ_POST_FMT_CHUNK_HEADER',
  'RESULT_CODE_ERR_READ_FLLR_CHUNK_SIZE',
  'RESULT_CODE_ERR_READ_FLLR_CHUNK_DATA',
  'RESULT_CODE_ERR_READ_BYTES_BEFORE_SAMPLE_DATA',
  'RESULT_CODE_ERR_READ_DATA_CHUNK_HEADER',
  'RESULT_CODE_ERR_NO_DATA_CHUNK_HEADER',
  'RESULT_CODE_ERR_READ_DATA_CHUNK_SIZE',
  'RESULT_CODE_ERR_READ_SAMPLE_DATA_BUFFER',
  'RESULT_CODE_ERR_ENCODER_ALLOC',
  'RESULT_CODE_ERR_ENCODER_SET_VERIFY',
  'RESULT_CODE_ERR_ENCODER_SET_COMPRESSION_LEVEL',
  'RESULT_CODE_ERR_ENCODER_SET_CHANNELS',
  'RESULT_CODE_ERR_ENCODER_SET_BITS_PER_SAMPLE',
  'RESULT_CODE_ERR_ENCODER_SET_SAMPLE_RATE',
  'RESULT_CODE_ERR_ENCODER_SET_TOTAL_SAMPLES_ESTIMATE',
  'RESULT_CODE_ERR_ENCODER_PROCESS_INTERLEAVED',
  'RESULT_CODE_ERR_ENCODER_FINISH',
  'RESULT_CODE_ERR_OUT_OF_MEMORY_OR_TAG_ERROR',
  'RESULT_CODE_ERR_ENCODER_SET_METADATA',
  'RESULT_CODE_ERR_ENCODER_INIT',
  'RESULT_CODE_ERR_INVALID_NUM_CHANNELS',
  'RESULT_CODE_ERR_INVALID_NUM_BITS_PER_SAMPLE',
]


export default function App() {

  const cacheDirPath = RNFS.CachesDirectoryPath
  ilog('cacheDirPath: ' + cacheDirPath)
  const inWavFilePath = cacheDirPath + '/inputWavFile.wav'
  const outFlacFilePath = cacheDirPath + '/outputFlacFile.flac'
  ilog('inWavFilePath: ' + inWavFilePath)
  ilog('outFlacFilePath: ' + outFlacFilePath)

  const [conversionResult, setConversionResult] = React.useState<number | undefined>();
  const [multiplyResult, setMultiplyResult] = React.useState<number | undefined>();

  const isAndroid = Platform.OS === 'android';

  const doWavToFlac = ():void => {
    wavToFlac(inWavFilePath, outFlacFilePath).then(setConversionResult).catch((e:any) => {
      elog(e)
      Alert.alert('Error', 'wavToFlac() erred with: ' + e)
    })
  }

  const a = 7;
  const b = 3;
  const doMultiply = ():void => {
    multiply( a, b).then(setMultiplyResult).catch((e:any) => {
      elog(e)
      Alert.alert('Error', 'multiply() erred with: ' + e)
    })
  }

  if (!DeviceInfo.isEmulator()) {
    return (
      <View style={ss.container}>
        <Text>
          Run this demo on the {isAndroid ? 'Emulator' : 'Simulator'}, 
          so that input / output audio files are accessible via 
          { isAndroid ? 'Android Studio\'s "Device File Explorer"' : ' your desktop file system'}. 
        </Text>
      </View>
    );
  }

  const prompt = 'Use ' + (isAndroid ? 'Android Studio\'s Device File Explorer' : 'the desktop file system') +
                 ' to store/retreive audio files at the paths below.' + 
                 (isAndroid ? ' (You may need to refresh Device File Explorer to see them.)\n' : '')

  const multiplyButtonTitle = `multiply ${a} * ${b}`

  const wavToFlacResultCode = (conversionResult !== undefined) ? 
      `(${wavToFlacResultCodes[conversionResult]})` : ''

  ilog(`Audio File Paths (${Platform.OS}):`)
  ilog('inWavFilePath: ' + inWavFilePath)
  ilog('outFlacFilePath: ' + outFlacFilePath)

  return (
    <>
      <View style={{height: 35}}/>
      <Text style={ss.title}>rn-wav-to-flac</Text>
      <View style={ss.container}>
        <Text style={ss.convertPromptText}>{prompt}</Text>
        <Text style={ss.label}>Input .wav file path:</Text>
        <Text style={ss.path}>{inWavFilePath}{'\n'}</Text>
        <Text style={ss.label}>Output .flac file path:</Text>
        <Text style={ss.path}>{outFlacFilePath}{'\n'}</Text>
        <View style={{height:10}} />
        <Button title='convert' onPress={doWavToFlac}></Button>
        <Text style={ss.label}>result:</Text>
        <Text style={ss.result}>{String(conversionResult)} {wavToFlacResultCode}</Text>
        <View style={{height:50}} />
        <Text style={ss.multiplyPromptText}>A native "multiply" test:</Text>
        <Button title={multiplyButtonTitle} onPress={doMultiply}></Button>
        <Text style={ss.label}>
          result:{' '} 
          <Text style={ss.result}>{String(multiplyResult)}
          </Text>
        </Text>
      </View>
    </>
  );
}

const ss = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
    paddingHorizontal: 20,
  },
  title: {
    color: '#000',
    fontSize: 18,
    fontWeight: '500',
    textAlign: 'center',
  },
  convertPromptText: {
    color: '#000',
    fontSize: 20,
    textAlign: 'center',
    paddingBottom: 0
  },
  multiplyPromptText: {
    color: '#000',
    fontSize: 20,
    textAlign: 'center',
    paddingBottom: 15
  },
  label: {
    color: '#000',
    fontSize: 18,
    fontWeight: '500',
    textAlign: 'center',
    paddingTop: 5
  },
  result: {
    color: '#000',
    fontSize: 18,
    textAlign: 'center',
    fontWeight: 'normal',
  },
  path: {
    color: '#000',
    fontSize: 15,
    textAlign: 'center'
  },
});
