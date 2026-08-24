# Avionics Task 1 - Depth Sensor Graph
*By:* Aditya Siddhi | *ID:* 2026B3PS0477H

This is my code for the first avionics task where we had to graph the depth sensor data. 

### What this code does
When I first opened Depth Data.csv, the data was kind of messy. Sometimes it recorded random words instead of numbers, and other times the depth would randomly spike way too high in just a split second.( FOR BROWNIE POINTS)

To fix this, I wrote a script that goes through the data one line at a time. I figured that if the depth changes by more than 50.0( MY ESTIMATE FOR CORRECT THRESHOLD ) all of a sudden, it's definitely a sensor glitch. 

So, if my code sees a jump bigger than 50, or if it finds broken text instead of a number, it ignores that bad data. Instead of leaving a hole in the graph, it just replaces the bad point with the last normal reading it saw. This keeps the line smooth and connected.

### Making the Graph
At the end, the code uses matplotlib to draw the final graph. I set it up so that the normal, good data points and the ones I had to "fix" show up as completely different colors and shapes. 

I'm really happy with how it turned out because when you look at the graph, it's super easy to tell exactly where the sensor messed up and where my code fix it
                                                      ##TASK 2##
To figure out how to connect the components I used google,ai and gained idea of how to join the components , for the coding part I used C++ as the main language , I used light sensor intensity to find if it is storm( less than 50 %) or not , used distance sensor etc as specified , i have created diffrent function's like handlebutton , updatelcd for respective tasks , i was using a lcd screen which has 16 rows and 2 columns , i have specified why I used the particular code and why in comments 
