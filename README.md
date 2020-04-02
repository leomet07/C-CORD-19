# CORD-19 Parser in C

THE cJSON files arent mine. They are from https://github.com/DaveGamble/cJSON

This is the same program, designed to run the exact way as my python version [here](https://github.com/leomet07/Python-CORD-19), just faster

Some "problems":

-   Program speed bottlenecked by printf's
-   As a subsitute for there not being an easy way to use regex in c, i made the incubation/day retriever from scratch. This wont catch all possibilities.
o