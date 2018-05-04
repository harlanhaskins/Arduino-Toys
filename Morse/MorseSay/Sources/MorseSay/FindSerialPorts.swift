import Foundation
import IOKit
import IOKit.serial

private func findSerialDevices(deviceType: String, serialPortIterator: inout io_iterator_t ) -> kern_return_t {
  var result = KERN_FAILURE
  let classesToMatch = IOServiceMatching(kIOSerialBSDServiceValue)!
  var classesToMatchDict = (classesToMatch as NSDictionary) as! [String: Any]
  classesToMatchDict[kIOSerialBSDTypeKey] = deviceType
  let classesToMatchCFDictRef = (classesToMatchDict as NSDictionary) as CFDictionary
  result = IOServiceGetMatchingServices(kIOMasterPortDefault, classesToMatchCFDictRef, &serialPortIterator);
  return result
}

private func loadSerialPaths(portIterator: io_iterator_t) -> [String] {
  var serialService: io_object_t
  var paths = [String]()
  repeat {
    serialService = IOIteratorNext(portIterator)
    guard serialService != 0 else { break }
    let bsdPathAsCFtring = IORegistryEntryCreateCFProperty(
      serialService,
      "IOCalloutDevice" as CFString,
      kCFAllocatorDefault,
      /*options:*/ 0
    ).takeUnretainedValue()
    guard let path = (bsdPathAsCFtring as? NSString) as String? else {
      continue
    }
    paths.append(path)
  } while true
  return paths
}

func serialDevices() -> [String] {
  var iter = io_iterator_t()
  if findSerialDevices(deviceType: kIOSerialBSDAllTypes, serialPortIterator: &iter) == KERN_SUCCESS {
    return loadSerialPaths(portIterator: iter)
  }
  return []
}
