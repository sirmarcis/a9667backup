HOMEWORK 2: HOCKEY CLASSES


NAME:  Anders Maraviglia


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

Internet c++ website and forums for minor assistance

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  15


DESCRIPTION OF 3RD STATISTIC:
Please be concise! (< 200 words)

This statistic answers whether a team improves over the course of a season
in terms of more goals scored and more assists versus the number of penalties.
The statistic goes from game to game and commpares these factors in causal terms
by finding a positive or negative slope of improvement. the average of all of these
are then taken to find whether a team gets better or worse as the season goes on.
a positive percentage means the team consistantly improved from game to game,
while negative means otherwise.

What first needed to be organized was an accurate way to store the quality of
play a team had in a game, solved by a formula that converted it into a decimal value.
The second thing was the need to keep these values organized causally, rather easly solved with an array,
then the delta values could show if a team gets better relative to their last game.

What was interesting or challenging about the implementation of this statistic? 
The most interesting thing about this statistic was seeing the results of larger
data sets like 2012_all, which showed that almost all teams did indeed on average improve their quality of
play from week to week, basically giving a numerical value to what fans see as obvious,
but cannot usually explain throughly.



NAME OF FILE WITH SAMPLE OUTPUT FROM 3RD STATISTIC:
Be sure to select (or create) a dataset with interesting results

< insert filename, file is included with submission >


MISC. COMMENTS TO GRADER:  
Optional, please be concise!

