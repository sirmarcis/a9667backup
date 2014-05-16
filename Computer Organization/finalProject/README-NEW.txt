Anders Maraviglia
5/12/2014
marava2
661242888

Daniel Lowe
5/12/2014
lowed2
661185723

Computer organization final project: MIPS processor simulator

All MIPS instructions specified in the project details have been completed, which includes the MIPS-1 instruction set.

System calls 1-12 are handled, according to the MIPS SPIM simulator system call instruction set.  

Program output is printed to the terminal where it is run, while details of the actual simulation, including time elapsed, number of instructions simulated,
memory allocation, and any generated error messages were printed to simlog.txt.  

Various enums were added on to, and the context struct now has three additional variables.  Two to handle hi and lo registers in multiplication, and one to count
the number of instructions simulated.  Along with this, all functions in sim.c received the additional argument output, so that the output file could be passed in.

The function SimulateItypeInstruction was added to handle all non-branching itype instructions (except jr).

In order to get clock_gettime() to work, the flag --std=c99 in the sim Makefile had to be changed to --std=gnu99.  This was the easiest and most logical fix we could 
come up with, and only benefits in regards to this instance.
