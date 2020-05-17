# Threaded Lifts
Elevator simulator written in C18 including implementations using PThreads and forking.

### Description
The simulator accounts for floor to floor travel and each of the three lifts keeps track of its position, printing its actions to file. The simulator responds to instructions from an input file and prints all actions taken to file.

## Installation
#### Default view
```
make QUEUEV=1
```

#### Debug mode
```
make DEBUG=1
```
#### FILE IO testing
```
./tests
```
## Usage
#### Running simulator using PThreads
```
./lift_sim_A m t
```
#### Running simulator using forking (incomplete)
```
./lift_sim_B m t
```
##### m = buffer size
##### t = time taken to travel (uniform for all movements)


## Project status
PThread created and functional, some use cases cause segfault.
Forked version hangs on waiting for children to finish.
For both cases printing to file needs more refined mutual exclusion as lifts process all movements (visible in debug mode) but few are missing from sim_out.

## Known Issues
Seg fault on first run (not showing up in GDB)

## Authors and acknowledgment
### Jeffrey Dugmore - Project Dev
