CS 349- Assignment 1
	John Speare
	 20393111
    September 27-29, 2013

This assignment involved the creation of a simple sidescrolling game using Xlib, and focussed on the use of animation.

Program is accessible via the command 'make run'.
'Mario's' movement speed, jumping speed, the sun's movement speed and the frame rate are tuneable by altering the parameter values in the makefile (currently set to default values. )

Enhancements include:
	-pseudo-randomly generated levels tending towards increased difficulty the farther the player progresses accross them
	-sun separated into sun and moon, which affect the spawn of certain items
	-primitive animations with the intention of informing the player of the rules without having to interrupt gameplay to explain them. (e.g., 'Mario' will face the screen and hop to denote having taken enemy damage, after taking enemy damage 'Mario' will start temporarily flashing to indicate invulnerability, collection of a flower/non-poisonous mushroom will cause 'Mario' to be drawn with bolder lines to indicate resistance to damage, etc.)
