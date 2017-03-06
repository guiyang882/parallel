## 并行化OMP算法

### 在未优化前，测试情况如下：
```bash
➜  bin git:(master) ✗ time ./testsparse 
During loadDicts 1324.19 ms !
Dict Size is 24000 * 128
Detect Image Feature size is 896 * 128
24, 26, 39, 10, 37, 1, 22, 17, 20, 13, 31, 12, 14, 25, 30, 40, 4, 41, 36, 42, 19, 8, 43, 44, 27, 34, 35, 18, 0, 11, 29, 47, 16, 46, 32, 7, 3, 9, 33, 23, 28, 6, 38, 21, 5, 45, 2, 
During SRClassify 99108.1 ms !
./testsparse  100.25s user 0.32s system 99% cpu 1:41.13 total
```

### 在mac中使用openmp进行优化

**cpu time: 100s ---> 55.579s**

```bash
➜  cmake-build-debug git:(master) ✗ time ./testsparse
During loadDicts 1844.42 ms !
Dict Size is 24000 * 128
Detect Image Feature size is 896 * 128
24, 26, 39, 10, 37, 1, 22, 17, 20, 13, 31, 12, 14, 25, 30, 40, 4, 41, 36, 42, 19, 8, 43, 44, 27, 34, 35, 18, 0, 11, 29, 47, 16, 46, 32, 7, 3, 9, 33, 23, 28, 6, 38, 21, 5, 45, 2,
During SRClassify 188377 ms !
./testsparse  189.94s user 0.90s system 343% cpu 55.579 total
```

### 在Mac中使用openmp+clang-omp++配合编译器优化O3

**cpu time: 55.579s ---> 12.590s **

```bash
➜  bin git:(master) ✗ time ./testsparse 
During loadDicts 451.212 ms !
Dict Size is 24000 * 128
Detect Image Feature size is 896 * 128
24, 26, 39, 10, 37, 1, 22, 17, 20, 13, 31, 12, 14, 25, 30, 40, 4, 41, 36, 42, 19, 8, 43, 44, 27, 34, 35, 18, 0, 11, 29, 47, 16, 46, 32, 7, 3, 9, 33, 23, 28, 6, 38, 21, 5, 45, 2, 
During SRClassify 42580 ms !
./testsparse  42.94s user 0.22s system 342% cpu 12.590 total
```