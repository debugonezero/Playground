import Foundation
import Accelerate

let size = 2048
let n = Int32(size)
let ops = Double(2 * size * size * size)

var a = [Float](repeating: 0.5, count: size * size)
var b = [Float](repeating: 0.5, count: size * size)
var c = [Float](repeating: 0.0, count: size * size)

let start = CFAbsoluteTimeGetCurrent()
cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, n, n, n, 1.0, &a, n, &b, n, 0.0, &c, n)
let end = CFAbsoluteTimeGetCurrent()

let timeMS = (end - start) * 1000
let gflops = (ops / (end - start)) / 1e9
print("NATIVE_METAL_CPU_RESULT: \(timeMS) ms (\(gflops) GFLOPS)")
