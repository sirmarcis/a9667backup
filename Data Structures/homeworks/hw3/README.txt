HOMEWORK 3: UNDO ARRAY


NAME:  Anders Maraviglia


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

ALAC tutoring

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  3-4


ORDER NOTATION:
For each function, using the variables:
  n = size of the array
  h = the length of the longest history for one slot of the array
Include a short description of your order notation analysis 
for the non-trivial functions.

size: n^2

set:n

initialized: n

get: n

undo: n^2

print:n^2

basic constructor:n^2

copy constructor:n^2

destructor:n



EXTRA CREDIT:
Discuss your implementation & order notation of push_back and pop_back.

Both push_back and pop_back were just a matter of copying the UndoArray, either with one extra slot or one slot 
less. For the extra slot case, I just added on the new value at the last postition.

push_back:n^2

pop_back:n^2



MISC. COMMENTS TO GRADER:  
Too easy! jk






