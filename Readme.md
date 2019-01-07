# ❄️ ElfUnknown's - Glacierfight

This is my solution for [the third homework](https://ocw.cs.pub.ro/courses/programare/teme_2018/tema3_2018_ca) of the Computer Programming Course

## Project hierarachy
- utils.h/utils.c:  it contains some usefull functions that calculate 
the distance to the center of an elf, can return a specific bite from 
a bitset. Also, it contains all the libraries & macro's used by the
program, as well as the structures that contain the elves and map 
information, along with functions used to manipulate the data. 
(sort of objects, with their attributes and methods )


- main.c: the game logic

## Used algorithms

Bubble sort - printLeaderboard function
- it is a adapted version of the algorithm because I use 3 parameters for
sorting, with different priorities.


## Encountered problems

- In particular, a very difficult task was to figure a way to "melt" 
the glacier but keep the elves coordinates(cells) as they were. 
So, in the end, I found a formula to get around this problem - `cell 
searched(x,y) =  the cell that has x = x - the number of meltdowns & y 
= y - the number of meltdowns`. However, the difficult part was then
to adapt all existing functions to use this method of accessing cells.
(took quite a lot of time)

- Another difficult thing was to free all the memory when the game was ended.
If I would have cheated a little bit, I could have left the program to read
all input, even if the game was won by someone, and then free the memory using
a function. But, because I wanted to be able to free it at any time, I had to
rewrite a lot of the function in main.c so they would pass on the arguments 
needed by the releaseMemory function (pointer to the map, players and their 
total number, input & output files).

© 2018 Grama Nicolae, 312CA