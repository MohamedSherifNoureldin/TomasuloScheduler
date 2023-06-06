# Tomasulo-Scheduler
A C++ implementation of the Tomasulo dynamic instruction scheduling algorithm without speculation. Implemented for the RiSC-16 architecture.

#### Inputs
Upon running, the simulator provides an option to change the standard reservation station number and number of cycles for each type. Then it provides an option to enter a memory file input, after which the main input (the instructions) file is requested. The simulator expects instructions waiting to be scheduled (and run) by the program. They can be entered through a file with the proper format (see the **test** folder for examples). The simulator supports instructions from the RiSC-16 instruction set, and register numbers should be submitted as just numbers (with no alphabetical characters included).

#### Outputs
The simulator outputs the reservation table values, the registers' statuses, the instructions' statuses, the registers' contents, and the memory contents at every cycle. At the end, it outputs the total execution time in cycles, the IPC, and the branch misprediction percentage (operating on a branch not taken predictor). In the end, the instrcutions are also output in a table format, with each instruction showing its status, and the cycle number (whenever applicable) at which it was issued, started executing, ended executing, and wrote its result. 

## Testing
For testing, refer to the **Testing** section in the *Report* pdf of this repository. Also see the pdf for screenshots on example runs.

## Contributors
- [Sara Mohamed](https://github.com/saraa-mohamedd)
- [Mohamed Sherif Noureldin](https://github.com/MohamedSherifNoureldin)
