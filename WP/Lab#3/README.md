Laboratory Work Nr.3
====================
Title:
------
Basics of Working with Mouse. GDI Primitives. Bezier Curve.
------------
Contents:

•	Mouse

•	Device context

•	GDI Primitives

•	Line

•	Curve

•	Plane

•	Bitmap image

•	Bezier curve

Completed Mandatory Objectives:
--------------------
•	Draw few lines of different colors and weights

•	Draw a Bezier curve

•	Draw few plane objects (ex. circle, square, pie, polygon...) of different colors, weights, filled and not

•	Draw 2 different objects using mouse

Objectives With Points:
----------------------
•	Draw a custom bitmap image (1 pt)

•	Add a switch (button, select list...) that will change mouse ability to draw objects (2 pt)

•	Draw a Bezier curve using mouse (1 pt)

•	Fill an object with a gradient (1 pt)

•	Delete objects using mouse clicking (2 pt)

•	Use mouse as an eraser of an adjustable width (1 pt)

The theory of workflow:
--------------------------
For this laboratory work, I used Charles Petzold's book- *"Programming Windows"*, chapters 5 and 7 from the first section. Reading it helped me in understanding the main features for working with the mouse. Also I consulted [http://msdn.microsoft.com](http://msdn.microsoft.com) for an update to the existing functions. Also, some dilemmas were solved by [http://stackoverflow.com](http://http://stackoverflow.com). 


Features and functionality:
---------------------------
1.	In order to trigger the functionality, I've used radio buttons mostly as controls. I masked them using the `BS_PUSHLIKE` style which represents a radio button as a push button. This was done more for aesthetics.

2.	The “Fill” checkbox is used when you want to fill an object( ex.: ellipse, polygon) with a gradient.

3.	The “Stroke” label gives us the possibility to choose a value that will represent the width of the line or the object you want to draw.

4.	The “Eraser” tool is used for clearing a portion of the drawing area using the mouse. The dimensions are adjustable.

5.	The first tool is the “Pen” tool, which can be used for drawing continuous curve lines, having the stroke 1. Also the stroke and the color are adjustable.

6.	The “Line” tool is used for drawing straight lines. This is done by setting the starting point and the ending point.

7.	The “Ellipse” tool is used for drawing ellipses. This thing is done by left-clicking on the drawing area and then dragging and dropping the cursor.

8.	The “Polygon” tool is used to draw different squares or rectangles, by left-clicking the mouse to establish the first point, drag-and-drop the mouse to define the last point.

9.	The “Bezier” tool is used to draw Bezier curves. You can select the first point by left-clicking the mouse and then drag-and drop it; the second point is selected by right-clicking the mouse and also drag-and-drop it.

10.	The drawing area is the place where all the paintings are done. Also here you can delete an object (ellipse or polygon) by right-clicking on it.

11.	The bitmap image is added to the right of the drawing area and it divides the tools from the colors.

12. We also have two regions which display the curront foreground and background colors.

13.	Next, you see the RGB set of vertical gradients. The foreground color is chosed by left-clicking, while the background color by right-clicking on the specific color gradient.



Conclusion:
-----------

Doing this laboratory work made me value more the masterpiece of Microsoft. I enjoyed this work a lot because it helped me discover new features and utlities of a painting simulation application and made me think from a different perspective. Now I think that this ca be improved and made more object oriented.

