Thoughts on storing custom animations in EEPROM
===============================================

Imagined usage:

-	Use desktop/mobile UI to develop, test and generally play with new animations
-	Once finished, click a button to store them into EEPROM with a unique identifier (e.g. number)
-	Trigger the stored animations using shortcut buttons, e.g. Custom 1, Custom 2

This would work best over a wifi connection from a web UI... and allows the bot to be extended/played-with without requiring any coding or cables.

Will need a way of listing commands already in EEPROM, how many commands stored, how many steps, etc.... generally a bunch of simple CRUD routines.

Storage scheme
--------------

-	Animation steps could be packed into 5 bytes, 4-bytes for relative joint positions and 1-byte for the duration in tenths of a second (range: 0.1sec to 25.5 seconds)
-	A single byte header would be enough to indicate number of steps in the animation, serving double-duty as an indicator of whether the animation is valid (i.e. zero steps = invalid)
-	The 328 chip has 1024k EEPROM, plenty of space for animations...
-	Simplest scheme (to avoid fragmentation) for memory mapping would have a fixed number of possible animations (e.g. 0..9) with a fixed maximum number of steps per animation (e.g. 10)
-	Using those numbers, we'd consume 10 x (1 + 10 * 5) = 510 bytes

Commands
--------

Store Keyframe command:

-	KEY <anim> <step> <servo positions 0..3> <dur>
-	Auto-extends the steps for <anim> if necessary, <step> numbered from zero

Do animation:

-	DO <anim>

Clear Animation command:

-	CA <anim>
-	Resets the given anim step count to zero

List custom animations:

-	LA <anim>
-	Optional anim id... if blank, will list all valid animations
-	Response in compact JSON (i.e. no whitespace, no line feed until the end):

```
{
    "animations":[
        {
            "id":0,
            "keyframes":[
                [1,2,3,4],
                [1,2,3,4]
            ],
            "durations":[50,50]
        }
    ]
}
```
