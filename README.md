# CSC-698-Term-Project
Comparative Study of Naive/Serial MatMul Implementation to Parallel and Cache Optimised Methods.

Our team compared performance of matrix multiplication using the following approaches:

* Brute force O(n^3) solution
* Cache Optimized version using loop unrolling and SIMD
* Parallelization of brute force implementation
* Parallelization of cache optimized version with best performing parameters.

# To run the code

* Install cmake cli on the system.
* Clone the github repo in a folder.
* Go to the folder and do "mkdir build"
* Go to "build" direcotry and run "cmake ../"
* Inside build directory run "make clean" and then "make"
* Run the benchmark by excuting the command "./driver/benchmark" on terminal inside the "build" folder. 

You can go do your grocery shopping. 
It will take some time to complete the benchmark since all algorithms runs for 3 iterations for 12 different matrix sizes.

# Verify Output of Benchmarks
Use cmp -l "file1.csv" "file2.csv" to get a readout of all bytes that differ 

# Instructions for plotting the results using plotly (free API)

* First step is to sign-up for plotly  
`https://plot.ly/python/getting-started/`

* Setup the API key on your system by following the steps at  
`https://plot.ly/settings/api`  
(Click 'regenerate key' without adjust anything fields or settings.)

* Make sure the ouptfiles are at  `../output-files/` 
* Then run `python plot.py`


#### Note:   
* The plots will overwrite for every run.
