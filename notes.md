# 2025-Nov-23
On rendering engine:

I want rendering to be entirely decoupled from other systems. I want the "renderer" to simply receive a list of objects to render (Renderable). Each Renderable will have its function pointer executing its render. 

The game's other systems (level, UI) will return an array of objects to render, each with their own renderable function. This array could be chunked by entities that share the same rendering instructions/shader/model, so that instancing can be performed.

Outstanding items: 
- game state tracking (paused, in menu A or in menu B)?.
- Renderable interface that allows entities to be rendered.
- renderer itself.


# 2025-Nov-25
The game will have two modes: 

1. play mode, where players can move around an avatar around a level and engage with game mechanics..
2. edit mode, where players can move around an avatar, but:
    - use middle mouse button to pan camera
    - can open a UI where they can select items to place on the screen. This will be a side panel w/ image previews.

The event loop will have these two modes as their own loops.
- update GLFW callbacks for mouse/keyboard to change between editor/player controls.


need to look at [archive of ourmachinery](https://ruby0x1.github.io/machinery_blog_archive/) to get ideas for how to implement object picking/rotation.



