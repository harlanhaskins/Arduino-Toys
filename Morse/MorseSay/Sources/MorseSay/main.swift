import Foundation
import SwiftSerial
import Darwin

func invalid(_ value: String) {
  print("invalid choice: \(value)")
}

func printMenu(_ devices: [String]) {
  print("Choose device:\n")
  for (idx, device) in devices.enumerated() {
    print("  [\(idx)]: \(device)")
  }
  print("  [q]: Quit")
  print()
  print("> ", terminator: "")
}

func selectDevice() -> String {
  var choice = ""
  let devices = serialDevices()
  while true {
    printMenu(devices)
    choice = readLine() ?? ""
    if ["q", "quit", "exit"].contains(choice) { exit(0) }
    guard let value = Int(choice), value < devices.count else {
      invalid(choice)
      continue
    }
    return devices[value]
  }
}

struct Sample {
  var potentiometerValue: UInt8 = 0
  var data = Data()
}

func readSample(port: SerialPort) throws -> Sample {
  var sample = Sample()
  print("Waiting for percentage...")
  sample.potentiometerValue = try port.readByte()
  print("Percentage: \(sample.potentiometerValue)")
  while let byte = try? port.readByte(), byte != 0 {
    sample.data.append(byte)
  }
  print("Read \(sample.data.count) samples...")
  return sample
}

let port = SerialPort(path: selectDevice())
port.setSettings(receiveRate: .baud115200, transmitRate: .baud115200,
                 minimumBytesToRead: 0)
try port.openPort()

while true {
  var str = ""
  while let char = try? port.readChar(), char != "\n" {
    let c = String(char)
    print(c, terminator: "")
    str.append(c)
  }
  if str.isEmpty { continue }
  print()
  print("saying: \(str)")
  let process = Process()
  process.launchPath = "/usr/bin/env"
  process.arguments = ["say", str]
  process.launch()
}
