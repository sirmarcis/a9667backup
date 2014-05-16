HOMEWORK 6: RICOCHET ROBOT RECURSION


NAME:  Anders



COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

none

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  15-20



ANALYSIS OF PERFORMANCE OF YOUR ALGORITHM:
(order notation & concise paragraph, < 200 words)

i & j = dimensions of the board
    r = number of robots on the board
    w = number of interior walls
    m = maximum total number of moves allowed
    
In my opinion, and please forgive me I am terrible at order notation, my algorithm is around O((i*j)*r*m^4),
but I may (and probably am) be wrong.  I know its at least to the forth power because there are four possible 
directions the robot may move in.




SUMMARY OF PERFORMANCE OF YOUR PROGRAM ON THE PROVIDED PUZZLES:
Preformance of the puzzle would be much mre efficent if not for the need to trace the moves and directions of all
the robots, but overall it is reasonably efficent, nut with obvius room for improvment.


MISC. COMMENTS TO GRADER:  
(optional, please be concise!)


