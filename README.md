# EADS_Dictionary
Algorithms and Data Structures project with graphical representation.
### Goal
The main goal of the project was to develop an AVL tree.
##### Data-structure functionalities.
Additional functionalities are fully functional iterator/const_iterator classes furthermore there is reverse_iterator/const_reverese_iterator class. To traverse the tree is a O(n) there is a executeForAllNodes method. Thanks to the recursive design it reduces the traversing to minimum. 
To properly use method u need to specify "method" as one of the imput argument. It is recomended to pass lambda constructed in a following way:
[...](Dictionary<a, b>::const_iterator& argument)->void{...}. U can also specify the TravelType (TravelType::preorder by default).
##### Additional trasks.
###### Imput from .txt
readFile method reads a set of words from the file. Where word is a set of capital and lower case letters. If a particular infividual word repeats intelf the info part of the Dictionary is increased by 1. Output is a newly created AVL tree.
###### printMaxFrequency and printMinFrequency
Those two metodes accepts dictionary of a type <string, int> and outputs the maximal for ...Max... and minimal ...Min... frequency. Where frequency is a int part of the dictionary.
