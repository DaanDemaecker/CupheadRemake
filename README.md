## Motivation
In my Programming 2 class, the skills we learned in the first semester were put to the test.  
We had to choose a 2D platformer game and recreate it as best as possible using a framework that contained little more than an update loop and a renderer.  

I chose Cuphead as my game because I have always loved it and recently bought it. This posed a challenge, of course, Cuphead wasn't as simple as other games out there like the good-fashioned Mario and others, but I was ready for a challenge and the result speaks for itself.

## Challenges and mistakes I made
There were a lot of challenges that came with the making of this game of course like:
- Collisions  
   We were not using a physics library at the time, so the collisions existed out of self-defined boxes that had to be checked manually, this isn't ideal.
- Tracking bullets   
  Nearing the end of the project I added the different bullet types of Cuphead, including tracking bullets. The challenge was that I had to track them until they died. Trying to solve this problem, I ended up learning about and using smart pointers,  namely weak pointers to detect when the object had died. This is a choice I would never make again as weak pointers are not the best for this type of thing. In future projects, I would probably use an event system.
- Levels with specific input  
  In Cuphead, only a couple of levels need an "Enter" input to end the level, mainly the tutorial. Because we did not learn about event systems yet I manually checked if the level needed it and then checked the enter button, it was not Ideal.

## Conclusion
This is still a project I am ***Insanely*** proud of, but now that I'm a better programmer, a lot of the choices I made make me scream on the inside. But it was a project that I learned so much from and **really** enjoyed working on from start to finish.
