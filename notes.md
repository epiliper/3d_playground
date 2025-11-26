# 2025-Nov-23
On rendering engine:

I want rendering to be entirely decoupled from other systems. I want the "renderer" to simply receive a list of objects to render (Renderable). Each Renderable will have its function pointer executing its render. 

The game's other systems (level, UI) will return an array of objects to render, each with their own renderable function. This array could be chunked by entities that share the same rendering instructions/shader/model, so that instancing can be performed.

Outstanding items: 
- game state tracking (paused, in menu A or in menu B)?.
- Renderable interface that allows entities to be rendered.
- renderer itself.
