lscommand v1
limpid - limpid@w2s.com
http://www.v-active.com/lscommand/
-----------------------------------

the lscommand LoadModule is a text box that sits on your desktop.
it will let you type !bangs, folder names, documents, apps, urls, etc an execute them
think of it as a very customizable address bar :)
if you specify a border width and you dont have CommandNotMoveable set, you can drag it around by the border.
there is a problem that some people are having where the mouse cursor is not set back to a normal cursor
after it is off the text box. to fix this, use CommandNoCursorChange to stop lscommand from changing the cursor.
just a tip: lots of programs names work in lscommand.. you can type stuff like 'notepad', 'icq', 'netscape', to start
some programs instead of the full path.

--new in this release (3/19/99)
lscommand now has history!!
use the up and down arrow keys to scroll through the history
added default colors so it will now work with no configuration (why would you do that though?)
added CommandNoCursorChange
up to about 7.5k now.. not too bad :)

--new in this release (3/18/99)
text is now centered vertically... looks a *lot* better now
its way smaller now... down to a mere 6k (thats small)



--in step.rc
LoadModule lscommand.dll	load it like this

CommandX 10			x position on the screen
CommandY 10			y position on the screen
CommandWidth 125		width in pixels
CommandHeight 15		height in pixels
CommandBorderSize 2		width in pixels of the border
CommandTextSize 14		pixel size of the text
--all colors are BBGGRR (unless you have colorRGB set) (not my fault - ls does them that way)
CommandBGColor 000000		background color
CommandTextColor ffffff		text color
CommandBorderColor ffffff	color of the border
CommandTextFontFace "Arial"	font for the text
CommandNotMoveable		if this is set you will not be able to drag it around by the border
CommandNotAlwaysOnTop		overides the default of being Always On Top of other windows
CommandNoClearOnCommand		wont clear the input after you hit enter
CommandHideOnStart		start hidden
CommandClearOnHide		clear the text when mod is hidden
CommandNoCursorChange		does not change mouse cursor when over the text box

umm i think thats all of em so far.

!ToggleCommand	toggle lscommand visibility
!FocusCommand	set focus to command (nice for binding to hotkeys)

--coming soon
i dunno, tell me what you want.
someone suggested a background image, i might throw that in
drag and drop support (i had it in sort-of then took it out)
transparency? no.

--thanks to...
jugg for doing a ton of beta testing
everyone else who's giving me comments/complaints/suggestions/etc..
crank- if it wasnt for him creating some competition, i dont think either of our apps would be where they are now :)
my cat, bob, who stood around even as i neglected to feed him while coding the history functions
re5ources popups... i peeked at the code a few times when i couldnt figure something out
killarny.. for the sys* series which inspired me or something