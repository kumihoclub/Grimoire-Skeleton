
# Grimoire-Skeleton

Grimoire-Skeleton is a stripped down and barebones version of the main loop 
I am using in my personal Grimoire project in hopes it will help out anyone 
who is still dealing with issues in creating a main loop implementation that 
works and works well.

Grimoire-Skeleton features a deterministic lockstep and an update rate that 
is fully decoupled from the render rate via a simple multithreaded setup 
synchronized by only a single atomic value and a single condition var 
(with the condition var used only to prevent busy waiting on the render thread).
This loop was created with 2D games in mind, but could likely acomodate simple 3D
games as well.

##### This is _not_-

* A game engine
* A game related library that ~~makes the game~~ abstracts everything for you
* Perfect

##### This _is_-

* A starting point you can use for starting your indie game or trying to
implement the features this offers in your existing game.
* Tested. I use this very setup in my own game project.
* Not perfect. There are likely ways to improve upon this. Contributions are welcome.

# "The Usual Suspects" or rather "Why are you doing A instead of B/C/D/Z"

**_Why are you rendering outside of the main thread?_**

It is a common misconception that you can only call OpenGL procs on the main thread. 
This isnt entirely correct. The truth is that you can call OpenGL procs 
on any thread you want, **but the OpenGL context must be current on said thread**.
This is why in this repo the example renderer draws without issue despite the window 
being created on the main thread and the OpenGL context being created during 
the instantiation of the render thread. It is also possible to pass around the context
from thread to thread via a "make current" function call (SDL_GL_MakeCurrent in SDL2),
but in this I just go ahead and create the context in the render thread since it will 
be the only thing that every calls OpenGL procs.

**_Why not render in the main thread and run the "sim" in a seperate thread instead?_**

Lets create a hypothetical. You are creating a physics driven 2D game and want
to maximize accuracy so you decide to run the sim at an update rate of 120 fps.
You create a "sim" thread and render on the main thread because
every single internet search result stresses "**ONLY RENDER ON THE MAIN THREAD OR ELSE 
DRAGONS!**". You manage to get a prototype working and distribute it amoungst 
three testers, but all three say they are experiancing different levels of 
input latency. Your debug logs show all three instances are updating at the set 
update rate with zero hiccups so you are dumbfounded, unaware that by following the
infinitely wise internets advice to avoid the rendering **"DRAGONS!"**, you are
now dealing with completly different input delay **"DRAGONS!"** because tester 
one is running at a 30hz refresh rate, tester two at 60hz, and tester 
three at 144hz.

Unlike OpenGL procs, event polling **must** be done on the same thread that
created your window, and because you are both rendering and polling events in 
the main thread, the rate at which you can poll for input events is completely 
coupled to the speed at which you can render. Even though your "sim" is 
updating at a rate of 120 frames without issue, on a 30hz monitor (with vsync 
enabled) you are only able to feed it input events every 4 "sim" updates **at best**.
Extracting the rendering to a seperate thread and keeping event polling and sim updates
in the main thread fully decouples them from the render rate.

**_Dedicated subsystems are bad. Why-u-no job system?_**

A lot of modern engines used by AAA studios make heavy us of "jobs" or "tasks" in their
multithreaded architecture. Its modern, super efficient, extremly scalable, and completely
**overkill for a majority of indie game projects**. As a matter of fact, a majority of indie
games can be completely **single threaded**. The main reason this is threaded is to decouple 
the render rate from the update rate so framerate drops and vsync do not limit the update rate.