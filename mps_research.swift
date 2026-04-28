import Foundation
import Metal
import MetalPerformanceShaders

// 1. Setup Hardware
guard let device = MTLCreateSystemDefaultDevice() else {
    print("ERROR: NO_METAL_DEVICE")
    exit(1)
}

let size = 2048
let rowsA = size, colsA = size
let rowsB = size, colsB = size
let rowsC = size, colsC = size

// 2. Describe the Matrices
let descA = MPSMatrixDescriptor(rows: rowsA, columns: colsA, rowBytes: colsA * MemoryLayout<Float>.stride, dataType: .float32)
let descB = MPSMatrixDescriptor(rows: rowsB, columns: colsB, rowBytes: colsB * MemoryLayout<Float>.stride, dataType: .float32)
let descC = MPSMatrixDescriptor(rows: rowsC, columns: colsC, rowBytes: colsC * MemoryLayout<Float>.stride, dataType: .float32)

// 3. Allocate Metal Buffers
let bufferA = device.makeBuffer(length: rowsA * colsA * MemoryLayout<Float>.stride, options: .storageModeShared)!
let bufferB = device.makeBuffer(length: rowsB * colsB * MemoryLayout<Float>.stride, options: .storageModeShared)!
let bufferC = device.makeBuffer(length: rowsC * colsC * MemoryLayout<Float>.stride, options: .storageModeShared)!

let matrixA = MPSMatrix(buffer: bufferA, descriptor: descA)
let matrixB = MPSMatrix(buffer: bufferB, descriptor: descB)
let matrixC = MPSMatrix(buffer: bufferC, descriptor: descC)

// 4. Initialize Multiplier
let kernel = MPSMatrixMultiplication(device: device, transposeLeft: false, transposeRight: false, resultRows: rowsC, resultColumns: colsC, interiorColumns: colsA, alpha: 1.0, beta: 0.0)

// 5. Execute and Time
let commandQueue = device.makeCommandQueue()!
let commandBuffer = commandQueue.makeCommandBuffer()!

let start = CFAbsoluteTimeGetCurrent()
kernel.encode(commandBuffer: commandBuffer, leftMatrix: matrixA, rightMatrix: matrixB, resultMatrix: matrixC)
commandBuffer.commit()
commandBuffer.waitUntilCompleted()
let end = CFAbsoluteTimeGetCurrent()

let timeMS = (end - start) * 1000
let ops = Double(2 * size * size * size)
let gflops = (ops / (end - start)) / 1e9

print("MPS_RESULT: {\"timeMs\": \(timeMS), \"gflops\": \(gflops), \"device\": \"\(device.name)\"}")
