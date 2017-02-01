# Parallel Programming Notes

# Course Info
## Computers got faster because...
 1. they got smaller
 2. we got smart about our algotithms
    - algorithm development
    - used brilliant code bases (low level engineering)
 3. hardware improvements
    - wider bus
    - fewer intersections
 4. factories got more efficient

## Help with compiling MPI
```
// Do this ONCE when you start up thomas
// module load openmpi-x86_64

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -np <numProcess> blah
```
## Frequently used MPI commands
``` C++
MPI_Send(<msgBuffer>, <msgLength>, <MPI_Datatype>, <destProcess>, tag, MPI_COMM_WORLD)
MPI_Recv(<msgBuffer>, <msgLength>, <MPI_Datatype>, <srcProcess>, tag, MPI_COMM_WORLD, &status)

```

---
&nbsp;

# 2017-01-24

## Parallel Memory Models
### PRAM (Parallel RAM)

P "synchronous" processors; shared memory  

For example, take the statement: $c = a + b$
This command is actually four operations, $a$ is read, $b$ is read, they are added, and then finally wrote to $c$.  
However, *in this class* it is considered an atomic operation (uses only one unit of time).  

#### Categories of PRAM
1. EREW - exclusive reads/writes
2. CREW - concurrent reads, exclusive writes
3. ERCW - exclusive reads, concurrent writes
    - stupid, why write to something you can't read?
4. CRCW - concurrent reads, concurrent writes

- Arbitrary CW - random process was the wrtie game
- Priority CW - the programmer assigns full hierarchy to the processors.  Most important process gets to write.
- Common CW - ONly allow writes is all process match the write value

### Distributed Memory Model (MRI)
1. $p$ synchronous processors
2. each process has its own private memory
3. communication among processors is expensive

We will use **distributed memory** in this course!

#### Work vs time
For $n$ number of processors...
``` C
for p where 1=<i=<n pardo [parallel do]
    B[i] = A[i]
````

## Summation example
Important: note this example is starting at 1 not 0  

_Starting with set:_
$A$ | 4 | 16 | 3 | 7 | 1 | 9 | 2 | 6 |
|---|---|---|---|---|---|---|---|---|---|
|  | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |

_yields:_

 4 | + | 16 | |  3 | + | 7 | |  1 | + |  9 | |  2 | + |  6 |
|---|---|---|---|--|--|---|---|---|---|---|--|--|--|--|--|
| | 20 | | + |  | 10 |  | | | 10 | | + | |  8 |
| | |  | 30 | | |   | + | |  | | 18 |  |  |
| |  | | | |  | | **48** |  |  |  | | | |


_...in log <sub>2</sub> n time._



## Summation Algorithm
Note: usually in this class:  
 $p$ = input size  
 $n$ = number of processors

However, *for this algorithm*, $p=n$ as a PRAM algorithm MUST provide exactly $p$ instructions per pulse of time (tick?). 

$B$ | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|---|---|---|---|---|---|---|---|---|---|
| **1** | 4 | 16 | 3 | 7 | 1 | 9 | 2 | 6 |
| **2** | 20 | 10 | 10 | 8 | | | | |
| **3** | 30 | 18 | | | | | | | |
| **4** | 48 | | | | | | | | |

``` C
for p from 1 =< i =< n pardo
    B[1][i] = A[i];
    for h from 1 to long n do{
        if (i =< n/(2^h)){
            B[h+1][i] = B[h][2i-1] + B[h][2i]
        }   
        else{
            do nothing
        }
    }

```

If $p \neq n$ , the code looks gross and is unreadable.  This is why PRAM is a terrible model to use to write code. However, it _is_ a good model to use to compute parallel run times -- basically, it just helps us figure out what "costs" something.



## Drawbacks of PRAM
_Question:_ Given an infinite number of processors, can I do this faster?
_Answer:_  **Nope**

 1. The model does not help to illuminate the dependence of the algorithms on $p$.  In other words, what effect do a different number of processors running time?
 2. Writing $p$ instructions every pulse is a pain.

Describe the set of tasks that everyone (all processors) needs to do is better than telling exactly what every specific person (processor) needs to do.

## Work-Depth Model

In a work depth model of computation, you are allowed to have a different number of things done in each stop.

The point is **not** to count how much time it takes to do an individual round of work.

The point **is** to count how many concurrent rounds of work are needed to solve a problem (or describe them) rather than focus on the time required in each round.

---
&nbsp;

# 2017-01-26

## Work-Depth Model

For algorithms utilizing work-depth, work & time can be modeled as: 

| | Non-parallel | Parallel | Theoretical |
| -- | -- | -- | -- |
| **Work** | $W(n) = T(n)$ | $W(n)$ | $W(n)$ |
| **Time** | $T(n) = W(n)$ | $T(n) \geq \frac {W(n)}{P}$ | $T(n)$ |

*In the theoretical world:*
$T(n)$ - How many pulses (ticks) are required for this algorithm?
$W(n)$ - "Optimal" work

*The algorithms in this class are based on Arbitrary
**CRCW** model (Which has an equivalent WD (representation)

---
&nbsp;



# 2017-01-31

## Parallelizing the Prefix Sum Problem

### Binary Tree technique

_Starting with set:_
Set A | 6 | 7 | 3 | 2 | 9 | 1 | 1 | 5 |
|---|---|---|---|---|---|---|---|---|---|
| **Prefix sum** | 6 | 13 | 16 | 18 | 27 | 28 | 29 | 34 |

In this algorithm, we will assume that $n = 2^k$ for some positive integer $k$, with an array of size $A$.

We will create `B[log(n) + 1][n]` and `C[log(n)+ 1][n]` that we will use. The answer will be in $C$.


*Tree demonstrating the prefix sum algorithm*:

 6| + | 7 | | 3 | + | 2 | |  9 | + |  1 | |  1 | + |  5 |
|---|---|---|---|--|--|---|---|---|---|---|--|--|--|--|--|
| | 13 | | + |  | 5 |  | | | 10 | | + | |  6 |
| | |  | 18 | | |   | + | |  | | 16 |  |  |
| |  | | | |  | | **34** |  |  |  | | | |


 Below is the $C$ matix and the $B$ matix that should follow the psum answer we want.  Refer to Binary tree example above. In this case go from the root down for $C$ and the opposite for $B$. 


### Prefix sum psuedocode
``` C
for i when 1 <= i <= n pardo:                          #
    B[1][i] = A[i]                                     #
for h from 1 to log n:                                 #    Calculate B
    for i when 1 <= i <= n/2^h pardo:                  #
        B[h+1][i] = B[h][2i-1] + B[h][2i]              #


for h in range(log n+1, 1):                            #
    for i=1 pardo:                                     #
        C[h][i]=B[1][1]                                #
    for even i from 1 <= i <= n/2^(h-1)  pardo:        #    Calculate C
        C[h][i]=C[h+1][i/2]                            #
    for odd i from 3 <= i <= n/2^(h-1)  pardo:         #
        C[h][i]=C[h+1][(i-1)/2]+B[h][i]                #
```


_Result:_


$B$ | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|---|---|---|---|---|---|---|---|---|---|
| **1** | 6 | 7 | 3 | 2 | 9 | 1 | 1 | 5|
| **2** | 13 | 5 |10 | 6 | | | | | |
| **3** | 18  |16 | | | | | | | |
| **4** | 34  | | | | | | | | |


$C$ | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|---|---|---|---|---|---|---|---|---|---|
| **1** | 6 | 13 | 16 | 18 | 27 | 28 | 29 | 34 |
| **2** | 13 | 18 | 28 | 34 |  |  |  |  |
| **3** | 18 | 34 |  |  |  |  |  |  |
| **4** | 34 |    |  |  |  |  |  |  | 


### Run time of Prefix sum:


_To calculate work $W(n)$:_

1. Ignore the `pardo` directive
2. Use concepts from CS 248... 
&nbsp;
$log (n) \cdot (\frac{n}{2} + \frac{n}{4} + \frac{n}{8} + \dots + 1)$
$= (1 + 2 + 4 + 8 + \dots + \frac{n}{2})$
$= \sum_{i=0}^{log \frac{n}{2}} 2^i = 2^{log(\frac{n}{2})}$

&nbsp;


#### Work for each code block:
``` C                                          
for i when 1 <= i <= n pardo:                          #____Work___|___Time___|
    B[1][i] = A[i]                                     #    O(n)   |   O(1)   |
for h from 1 to log n:                                 #-----------|----------|  
    for i when 1 <= i <= n/2^h pardo:                  #    O(n)   | O(log n) |
        B[h+1][i] = B[h][2i-1] + B[h][2i]              #-----------|----------|


for h in range(log n+1, 1):                            #
    for i=1 pardo:                                     #----------|----------|
        C[h][i]=B[1][1]                                #  O(nlogn)   O(log n)
    for even i from 1 <= i <= n/2^(h-1)  pardo:        #----------|----------|
        C[h][i]=C[h+1][i/2]                            #
    for odd i from 3 <= i <= n/2^(h-1)  pardo:         #
        C[h][i]=C[h+1][(i-1)/2]+B[h][i]                #
```
##### Sequential Sum:
- $W(n) = O(n)$
- $T(n) = O(n)$

##### Parallel Prefix Sum:
- $W(n)= O(n log(n))$
- $T(n)= O(log(n))$

---
&nbsp;


# Exam 1 Prep

| $A$ | 6 | 7 | 3 | 2 | 9 | 1 | 1 | 5 |
|---|---|---|---|---|---|---|---|---|
| **psum** | 6 | 6 | 3 | 2 | 2 | 1 | 1 | 1 |

1. **Try to find the min of an array $A$:**
     - $W(n)= O(n)$
     - $T(n)=O(log(n))$

2. **Find the Prefix min of an array $A$**

&nbsp;

| $B$ | 1| 0 | 0 | 1 | 0 | 0 | 1 | 1 |
|---|---|---|---|---|---|---|---|---|
| **psum** | 1 | 1 | 1 | 4 | 4 | 4 | 7 | 8 |

3. **Given a bitvector $B$, find the postion of the nearest leftmost 1.**

     - $W(n)= O(nlogn)$
     - $T(n)=O(log(n))$




