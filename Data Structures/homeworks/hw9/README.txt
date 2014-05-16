HOMEWORK 9: DISTANCE FIELDS & FAST MARCHING METHOD


NAME:  Anders Maraviglia


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

TA Rebecca  

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.



ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  13-15



NAIVE ALGORITHM:

Order Notation: (w*h)^2

Timing Experiment Data:

FILE:                           USER TIME:
tiny_5x5.ppm                    0.015s
small_10x10.ppm                 0.031s
squiggle_30x30.ppm              0.080s
lines_100x100.ppm               2.869s
data_structures_300x300.ppm     3m54.390s

Discussion:

First, it is important to note that user time should have been used in order to test the efficency of the algorithm, 
not the system.  What happens is that each pixel is looped over, which gives an order notation of width * height,
and at each pixel the entire image is again looped over while looking for black pixels (again, width * height).
Thus the final order notation is (w*h)^2.



IMPROVED ALGORITHM: 

Order Notation: (w*h*p)

Timing Experiment Data:

FILE:                           USER TIME:
tiny_5x5.ppm                    0.015s
small_10x10.ppm                 0.015s
squiggle_30x30.ppm              0.077s
lines_100x100.ppm               1.465s
data_structures_300x300.ppm     2m50.836s

Discussion:

What seperates this implementation from the last is that the black pixels are first found (notation of width * height)
through one iteration of the image, and then afterwards the image is looped over again, the same as before, but this
time instead of looping over the entire image at each pixel, only the black pixels are looped over, giving a 
notation of width * height * black pixels (p).  Thus when combined we get an order notation of (w*h) + (w*h*p), but 
this can be simplified since the first iteration (represented by the first term) is so small as to be insignificant 
in running time relative to the second term, leaving a final notation of (w*h*p).


FAST MARCHING METHOD (with a map):

Order Notation: (w*h)*log(w*h)

Timing Experiment Data:

FILE:                           USER TIME:
tiny_5x5.ppm                    0.047s
small_10x10.ppm                 0.030s
squiggle_30x30.ppm              0.030s
lines_100x100.ppm               0.280s
data_structures_300x300.ppm     2.355s
dots_1000x1000.ppm             37.969s

Discussion:

Going over every pixel, the queue must, in the wort case, have all the pixels in the queue at one point and thus
the order notation is every pixel in the image times the log of every pixel (every other smaller term can be ignored
as insignificant). thus, (w*h)*log(w*h)



DISTANCE FIELD VISUALIZATIONS FOR EXTRA CREDIT:




FAST MARCHING METHOD (with a hash table) FOR EXTRA CREDIT:

Order Notation: w*h

Timing Experiment Data:

FILE:                           USER TIME:
tiny_5x5.ppm                    0.047s
small_10x10.ppm                 0.030s
squiggle_30x30.ppm              0.030s
lines_100x100.ppm               0.233s
data_structures_300x300.ppm     1.590s
dots_1000x1000.ppm              9.437s

Discussion:

the diffrence between this and using a map is that instead of taking log(width * height) for each pixel while using
the priority queue it takes time 1, due to the use of a hash function instead of tree traversal.  thus, the order
notation simplifies to simply w*h.


MISC. COMMENTS TO GRADER:  
The dots_1000x1000.ppm is crazy! looks so good #newwallpaper #dataswag






