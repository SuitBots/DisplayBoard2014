DisplayBoard2014
================

A display board to show images and videos

Requirements
============

You'll need [Oblong's](http://oblong.com/) [Greenhouse SDK](http://greenhouse.oblong.com/)
to build this. If you have it, just running `make` should be sufficient.

Running
=======

Currently, the switcher reads image (jpg, jpeg, png) and video (??) files off of
the command line. So if you have all of your image/video assets in the directory
`~/Desktop/SB`, you could run the system as follows:

    ./Suitbots ~/Desktop/SB/*

Of course you could list them by hand if order is important.  To run full screen,
you can set the FULLSCREEN environment variable:

    env FULLSCREEN=1 ./Suitbots ~/Desktop/SB/*

