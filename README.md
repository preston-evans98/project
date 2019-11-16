### Getting Started

1. Clone this repo, then use `make` to build from source.
1. Start compute in one terminal `./compute 1` (Fell free to replace `1` with a different number of threads)
1. Run package in another `./package matrix1.dat matrix1.dat output.dat` 

### Automated Testing
To run automated tests, `make clean`, then use `make runTests`. This will recompile package.c with the flag -DAUTO_TEST=1. When compiled with this flag, package changes its output format to include matrix dimension information. 
For example, result.date goes from
110 120 130 140 150 246 272 298 324 350 382 424 466 508 550
To 
3 5
110 120 130 140 150 246 272 298 324 350 382 424 466 508 550

After compilation has finished, the makefile will automatically run `test.c`, which runs `package` automatically and checks the results of four different matrix multiplcations against expected values calculated using Python and numpy. To finish the testing process, simply start `compute` in another terminal

The expected test results are found in:
1. expected3x5.dat
1. expected50x5.dat
1. expected50x50.dat
1. expected1x1.dat

To verify that the tests are truly running, feel free to change the inputs or expected outputs and watch the tests fail
For example, changing matrix1.dat from
3 4
1 2 3 4 5 6 7 8 9 10 11 12

To 
3 4
2 2 3 4 5 6 7 8 9 10 11 12

results in 
[111 122 133 144 155]
[246 272 298 324 350]
[382 424 466 508 550]
matrix1 * matrix2 != expected3x5.dat
FAILURE!
