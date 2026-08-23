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
