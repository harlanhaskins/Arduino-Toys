// swift-tools-version:4.1
import PackageDescription

let package = Package(
  name: "MorseSay",
  dependencies: [
    .package(url: "https://github.com/yeokm1/SwiftSerial", from: "0.0.13"),
  ],
  targets: [
    .target(
      name: "MorseSay",
      dependencies: ["SwiftSerial"]),
  ]
)
