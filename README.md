# Snake
Popular Snake Game, made with QT and C++.

Coding your own version of the Snake Game seems like a rite of passage. Rather than use a Graphics View and Scene I wanted to try and do it by overriding the Paint Event on the Canvas. This does come with some overhead as the paint event calls for the entire window to be redrawn from frame to frame, so that the snake doesn't leave a persistant trail behind it.

The whole window is invisibly divided up into rows and columns so that the motion of the snake aligns to a grid. This means whenever the snake's food spawns randomly, it will be aligned with the snake's potential path. 

The snake starts by moving a little slower than usual so the player gets used to the mechanics of using the arrow keys. As soon as they have managed to captured a few food elements, the speed increases. With every capture of food the snake's body grows by one element.</p>

The body follows the path of the head, and all body elements are contained within a Vector that is continually shifted as the snake moves and the tail position is discarded thereby producing the effect that the snake is moving. The snake's food is spawned randomly but a check is made so that it doesn't appear within the current body or head of the snake.

The Game ends when the snake collides with either the boundary of the window, or its own body and this triggers the current score to appear in the title bar. The game is then automatically restarted once again.


https://user-images.githubusercontent.com/111576658/201324807-8930ce25-fc7c-4dea-bc99-9966b7251b32.mp4

