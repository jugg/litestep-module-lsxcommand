                    *************************************
                    *            LSXCommand             *
                    *            by Visigoth            *
                    *    gandhimail@worldnet.att.net    *
                    *          ===============          *
                    *          Version: 1.7.9           *
                    *         November 23, 1999         *
                    *          ===============          *
                    * http://floach.pimpin.net/visigoth *
                    *************************************

Table of Contents
=================
i     Quick & Dirty Install
ii    Kudos
iii   Change Log
I.    About LSXCommand
II.   How to use LSXCommand

      II.1 Internet Searches
      II.2 Added RC Commands
           New to 1.7.6:
           -------------
           - CommandBottom/Top/Left/RightBorderSize

           New to 1.7.4:
           -------------
           - Changed CommandClock to CommandTime
           - CommandAddExternalsToHistory

           New to 1.7.2:
           -------------
           - CommandOffsetX
           - CommandOffsetY
           - CommandSearchEngineBrowser
           - CommandWinAmpDisappearsOnFocus
           - CommandHideOnUnfocus
           - CommandTabFileComplete

           New to 1.7.1:
           -------------
           - CommandContextMenuStandardItems
           - CommandScrollWinAmpTitle

           New to 1.7:
           -----------
           - CommandNoAutoComplete
           - CommandCommaDelimiter
           - CommandTransparentEditBox
           - CommandBackground
           - CommandRPNCalculator
           - CommandClock
           - CommandClockDisappearsOnFocus

           New to 1.5.3:
           -------------
           - CommandExplorePaths
           - CommandSelectAllOnMouseFocus

           New to 1.5.1:
           -------------
           - CommandUnixHistory

           New to 1.5:
           -----------
           - CommandNoWarnOnError
           - CommandHistoryMenuEntries
           - CommandNewestHistoryItemsOnTop
           - CommandContextMenuAboveBox
           - CommandContextMenuExecute
           - CommandContextMenuOrder
           - CommandNoTabMicroComplete
           - CommandClearHistoryOnStartup
           - CommandAssumeNetAddress

           New to LSXCommand:
           ------------------
           - CommandHistoryEntries
           - CommandSearchEngineList
           - CommandSelectAllOnFocus
           - *CommandAlias

      II.3 Added !Bang Commands
           New to 1.7.7:
           -------------
           - !CommandToggleTimer
           - !CommandSetText

           New to 1.7.5:
           -------------
           - !CommandMove

           New to 1.7.3:
           -------------
           - !CommandBrowseFile
           - !CommandBrowseFolder

           New to 1.7.1:
           -------------
           - !Command
           - !CommandShow
           - !CommandHide

           New to 1.5:
           -----------
           - !CommandClearHistory
           - !CommandShowContextMenu

           New to LSXCommand:
           ------------------
           - !CommandRescanEngines

      II.4 The engines.list File
      II.5 Tab-MicroComplete & Context Menu Access
      II.6 Calculator Functionality
      II.7 Backgrounds & Transparencies
      II.8 Clock Settings
      II.9 Tab File Completion
      II.10 LSXCommandClock module
      II.11 AlterScript module

III.  Known Bugs / Limitations

      III.1 Old history entries remain
      III.2 Quoted Names a Problem

IV.   Comments / Questions / Flames / Bug Reports
V.    Source Code
VI.   Source Code License


i. Quick & Dirty Install
========================

   i.1 Upgrading from LSCommand
   ----------------------------
   This is a pretty simple thing to do.  It's essentially just
   replacing a file, but a little more...

     1. Copy lsxcommand.dll to your modules directory
     2. Replace lscommand.dll with lsxcommand.dll in your step.rc
     3. Add the CommandHistoryEntries and CommandSearchEngineList
        RC Commands to your step.rc (see below for specifics)
        Note: CommandSearchEngineList must be an absolute path, as in
        C:\Litestep\engines.list
     4. Place the engines.list file to the place pointed to by
        CommandSearchEngineList
     5. I would really recommend at least browsing through the
        section below on Internet Searching with LSXCommand.
     6. Adding the CommandSearchEngineList and extracting the
        engines.list file is not mandatory - you will simply not have
        the ability to use the Internet search feature.


   i.2 New to LSXCommand
   ---------------------
   Read limpid's original readme (included with this distribution)
   on how to set up LSCommand - from which LSXCommand is based.
   Afterwords, go up to "i.1 Upgrading from LSCommand"


ii. Kudos
=========
   I just think it's appropriate to officially acknowledge that
   limpid did work on the original LSCommand, from which LSXCommand
   is based.  Thanks limpid!


   Credits
   -------
   A big thanks goes to Takayuki Kawamoto, as he is graciously
   translating this readme into Japanese.  His translation may be
   found at: http://litestep-jp.fsn.net/translation/lsxc.htm

   The following people gave me ideas after I released version 1.0:

   - Jody Franklin sent in the corrected Letterman Top 10 search engine
   - Jonas Sicking can take all the credit for the calculator functions in
     LSXCommand, now - he sent in entirely new calculator code which is
     much better than the stuff I had.
   - TAMUKI-TAM sent in a patch to LSXCommand.  I also found out he's
     localized LSXCommand to Japanese (that means you can use Japanese
     characters with LSXCommand).
   - technomancer sent in the code for different border sizes
   - Mike Thrall sent in the !bang commands to move LSXCommand
   - gizzmo also sent in the idea for switching between clock/amp mode
   - gizzmo first sent in the idea for a separate clock module
   - Kryten sent in the idea about Browse File and Folder
   - Ilkka Nisula sent in the request for multiple base support
   - Alasdair sent in a request for negative screen coordinates
   - John Kuhn sent in the WinAmp title scrolling idea
   - Reiner Meyer sent in the idea to add Clock functionality
   - Hugo Ahlenius sent in the idea for RPN mode and the comma
     delimiter
   - Francis Tyers wanted to be able to turn off AutoComplete for
     some reason.
   - SXW sent in the basic idea behind AssumeNetAddress and
     SelectAllOnFocus
   - Jay Kerr sent in the idea behind Aliases / Bindings
   - neep sent in a preliminary ideas about a context menu
   - tin_omen and Jay Kerr sent in fond thoughts of quoted paths and
     complex string parsing :P
   - Tomislav Mutak sent in the idea for a command line calculator
   - Kevin Werner sent in the idea for bash-like history navigator
   - Andrew Hornback sent in a reference to the need for multiple
     arguments (I'm working on it!!)

   The following people sent in lists of engines (I won't list each
   engine - it would just take too long)

   - bonez sent in 3 new ones for 1.7.1: UBL, OLGA, and CDNow
     All music related...  Cool stuff
   - Kryten sent in the Merriam-Webster stuff
   - bizquik sent in the File Demon engine
   - mirul sent in the MS Knowledge Base
   - Jason Murray sent in more than 230 engines!!!  That's just
     overboard, IMO
   - Scott Ferguson sent in MetaCrawler


iii. Change Log
===============

   Version 1.7.9
   -------------
     Bug Fixes
     `````````
     - Fixed DarkStep Compatibility
     - Linked against a fresh lsapi.dll, so this *should* fix problems
       people had loading LSXCommand 1.7.8.  Drop me a line if not

   Version 1.7.8
   -------------
     Documentation Changes
     `````````````````````
     - Calculator documentation entirely revamped to reflect all the new
       and cool features - CHECK IT OUT!
     - Added lsxcommand.rc to the package - just a list of all the
       RC commands and !Bang commands.
     - Documentation reflects new location of the Japanese translation

     Bug Fixes
     `````````
     - AutoComplete bug is fixed!  The fix *might* make AutoComplete work
       not *exactly* as planned, but at least it won't crash any more.

     New Features
     ````````````
     - New Timer management
     - Entirely new calculator code (Mad props to Jonas Sicking for coding
       this stuff - credit entirely belongs to him for the calculator now)

   Version 1.7.7
   -------------
     Documentation Changes
     `````````````````````
     - It turns out Mr. Kawamoto isn't a Professor after all ("merely
       a student" to put it in his words).  Well, at least he's a
       Ph. D. candidate :)
     - Kudos section fixed & updated - check out the new entry.
     - Updated CommandClock section to include new !Bangs supported
       by LSXCommandClock

     Bug Fixes
     `````````
     - Fixed CommandNoWarnOnError bug when entering a path
     - !CommandMove now automatically calls !FocusCommand *if*
       it was called without any arguments (ie you want it to move to
       the current mouse position)
     - Aliases now properly parse out stuff like the following:
       *CommandAlias IE "C:\program files\internet explorer\..."
       Thanks to TAMUKI-TAM for sending the patch to this bug.

     New Features
     ````````````
     - LSXCommand is now offered in Japanese.. TAMUKI-TAM is the guy
       who localized LSXCommand to use Japanese characters and
       Mr. Kawamoto, as I said before, has translated the readme to
       Japanese - thanks again!
     - Added following !Bang commands (see section II.3 for details):

       * !CommandToggleTimer / !CommandClockToggleTimer
       * !CommandSetText

   Version 1.7.6
   -------------

     Documentation Changes
     `````````````````````
     - Kudos section has a few new entries - check them out!
     - CommandBorderSize documentation added

     Bug Fixes
     `````````
     - Fixed !CommandMove bug
     - Fixed Working Directory problems

     New Features
     ````````````
     - When !CommandMove/!CommandClockMove is called without
       parameters, it moves to the current cursor position.  I would
       recommend using CommandHideOnUnfocus with this.
       Also, if you !CommandMove has the parameter "home", it moves
       back to the original position found in step.rc
     - Added following RC Commands:

       * Command + Bottom | Top | Left | Right + BorderSize
         (ie. CommandTopBorderSize, CommandLeftBorderSize...)

     Notes
     `````
     Be *sure* to read the CommandBorderSize documentation if you
     are interested in using CommandBottom/Top/Left/RightBorderSize

   Version 1.7.5
   -------------
   More bug fixes; new features

     Documentation Changes
     `````````````````````
     - Fixed Ilkka Nisula's name.
     - Updated Search Engine list documentation to reflect
       "categories"
     - Updated Section II.1 to note that new engines are only added
       to the master search engine list available on my web site.

     Bug Fixes
     `````````
     - Fixed !CommandBrowse* crashes

     New Features
     ````````````
     - Added Mike Thrall's code for moving LSXCommand via a !Bang
       (see below)
     - Added category feature to Search Engine list (see the end of
       section II.1 for details)
     - Added following !Bang Commands:

       * !CommandMove / !CommandClockMove

     - Added following Search Engines (see section II.1 for details)

       * DVDExpress
       * CodeGuru
       * ZDNet HotFiles

   Version 1.7.4
   -------------
   Bug fixes, bug fixes, bug fixes...

     Documentation Changes
     `````````````````````
     - Documentation now reflects change from CommandClock to
       CommandTime

     Bug Fixes
     `````````
     - Fixed AlterScript to work like Killarny's script.dll
       Meaning, understanding the LoadScript RC Commands
     - Fixed !CommandBrowseFile & !CommandBrowseFolder crashing bugs
     - Fixed CommandCommaDelimiter problems (stupid infinite loops)
     - Fixed the weird LSXCommand/CommandClock problem: if you have
       the *original* LSXCommand with a CommandClock entry, then
       you must change this to CommandTime (see Notes for more info)

     New Features
     ````````````
     - Added switch WinAmp/Clock mode on mouse click in
       LSXCommandClock (not in LSXCommand).
     - Added following RC Commands:

       * CommandAddExternalsToHistory

     - Added following search engines:

       * AudioFindMP3 (MP3 search engine - audiofind.com)
       * PalaVista    (MP3 metacrawler - palavista.com)

     Notes
     `````
     It turns out the problem lies in LSAPI, and not LSXCommand or
     LSXCommandClock.  In any case, the original CommandClock RC
     Command (before there ever was a LSXCommandClock) now is named
     CommandTime.  The LSXCommandClock RC Command "CommandClockString"
     is still the same.  So, if you have the original LSXCommand still
     with a CommandClock entry, change it to CommandTime.  NOTE:
     CommandClockDisappearsOnFocus is still the same.

       Example: CommandClock "#c"  -->  CommandTime "#c"

   Version 1.7.3
   -------------
   A couple of features, bug fixes

     Bug Fixes
     `````````
     - Fixed File AutoComplete junk characters error

     New Features
     ````````````
     - Separate AlterScript module (see section II.11 for details)
     - Separate clock module (see section II.10 for details)
     - Added following !Bang Commands:

       * !CommandBrowseFile
       * !CommandBrowseFolder

   Version 1.7.2
   -------------
   Mostly bug fixes & two feature additions

     Documentation Changes
     `````````````````````
     - Added section II.9 for Tab File Completion feature

     Bug Fixes
     `````````
     - Fixed !CommandHide & !CommandShow problems
     - Fixed CommandHiddenOnStart problems (again)
     - Fixed Paste menu item problems

     New Features
     ````````````
     - New positioning system (see Notes below)
     - Added file name tab completion like Re5ource's popups
       (see section II.9)
     - Added following RC Commands (see section II.2 for details)

       * CommandSearchEngineBrowser
       * CommandWinAmpDisappearsOnFocus
       * CommandHideOnUnfocus
       * CommandTabFileComplete
       * CommandOffsetX
       * CommandOffsetY

     Notes
     `````
     In the new positioning system, CommandOffsetX/Y are used to
     decide from where LSXCommand should compute its position.  The
     following table lists the combinations of CommandOffsetX/Y and
     CommandX/Y and what they do:

       CommandOffsetX/Y:
         0  -  Reference from Left/Top (default)
         1  -  Reference from Center of Horizontal/Vertical
         2  -  Reference from Right/Bottom

       CommandX/Y:
         These values now tell how many pixels away from the point
         described by CommandOffsetX/Y LSXCommand should be.

     For instance, using the following values will have LSXCommand
     be placed 2 pixels to the left of horizontal center, at the very
     bottom of the screen:

       CommandOffsetX 1
       CommandOffsetY 2
       CommandX      -2
       CommandY      -14  (Same as negative of CommandHeight)

     If you leave your settings the way they are, they will work as
     before.

     To turn on file name tab completion, be sure to turn *on*
     CommandTabFileComplete and CommandNoTabMicroComplete.  I know
     this is a bit clunky, but deal with it please... :)  I didn't
     test it too extensively, but it seems to be working quite
     well on my system..  Of course, we know what that means...

   Version 1.7.1
   -------------
   Just bug fixes and a few features

     Documentation Changes
     `````````````````````
     - Added number conversion information to Calculator
       Functionality section (section II.6)
     - Added multiple number bases information to Calculator
       Functionality section (section II.6)
     - Changed the original lscommand's readme file.  Many
       people asked about why CommandHideOnStart doesn't work.
       This is because there is no such thing.. it is
       CommandHiddenOnStart.  That solves that mess.

     Bug Fixes
     `````````
     - Fixed Comma Delimiter problem(s)
     - Caught up with the Kudos section of this readme. :)
     - Fixed bug with CommandContextMenuOrder
     - Fixed Clock that never came back after a command
     - Fixed any problems with CommandHiddenOnStart with
       CommandBackground

     New Features
     ````````````
     - Calculator supports limited number converion
     - Calculator supports binary & hexadecimal numbers (see section
       II.6)
     - You can now stop any of the History / Alias / Search Engine
       submenus from coming up by setting them to 9 instead of 0, 1,
       or 2.
     - Added following RC Commands (see section II.2 for details)

       * CommandScrollWinAmpTitle
       * CommandContextMenuStandardItems

     - Added following !Bang Commands (see section II.3 for details)

       * !Command
       * !CommandShow
       * !CommandHide

     - Added following search engines (see section II.1 for details)

       * OLGA  - On-Line Guitar Archive
       * UBL   - Ultimate Band List
       * CDNow - Search for CDs

     Notes
     `````
     CommandScrollWinAmpTitle scrolls the current WinAmp title *if*
     WinAmp's "Scroll Song Title in Windows Taskbar" preference is
     on.  Also, you *can* have a clock and the WinAmp scroller - but
     the WinAmp scroller takes precedence.  Therefore, the WinAmp
     scroller will show when there is a WinAmp to scroll.  If not,
     LSXCommand will show a clock (if the clock setting is specified)

   Version 1.7
   -----------
   New features...

     Documentation Changes
     `````````````````````
     NOTE: This section notifies you about changes to existing
     documentation.  New documentation is not listed here.  Pay
     close attention to Section II, since all major features have
     their own documentation added under this section.

     - Added note about RPN functionality to Calculator section II.6
     - Added shameless plug <g> for the full engines.list at my web
       site in section II.4.

     Bug Fixes
     `````````
     - Proper handling of border sizes (retheming may be required)

     New Features
     ````````````
     - Added following RC Commands (see section II.2 for details)

       * CommandNoAutoComplete
       * CommandCommaDelimiter
       * CommandTransparentEditBox
       * CommandBackground
       * CommandRPNCalculator
       * CommandClock
       * CommandClockDisappearsOnFocus

     Notes
     `````
     While doing some theming of LSXCommand myself, I noticed that
     when I set BorderSize to 0, it still had a border around it.
     I fixed that kind of stuff in this revision.  NOTE: Some re-
     theming may be required.

     If you set CommandTransparentEditBox on for whatever reason, you
     *WILL* see flashing while typing in the command box.  No doubt.
     I strongly suggest reading section II.2 for more about
     transparency and backgrounds.

   Version 1.5.3
   -------------
   Just a couple of bug fixes in the new command parser from 1.5.2
   and another feature.

     Bug Fixes
     `````````
     - Fixed the junk characters being launched as an Internet
       Address when CommandAssumeNetAddress is turned on.
     - Fixed history saving commands that were quoted paths alone
       multiple times
     - Fixed multiple explorer windows

     New Features
     ````````````
     - Added following RC Commands (see section II.2 for details)

       * CommandSelectAllOnMouseFocus
       * CommandExplorePaths

   Version 1.5.2
   -------------
   More bug fixes - most related to string parsing and memory
   allocation.  Programmers: forgive me!  I didn't check all the
   NULL pointers I was supposed to!  I only hope I caught all of
   them in this release...

   Quote of the release: What was I smoking when I released 1.5.1
   anyway?

     Bug Fixes
     `````````
     - Fixed Calculator () crashing bug
     - Fixed multiple history menu entries of same command
     - Fixed times when history menu wouldn't do anything
     - Fixed No-Argument searching (not possible now)
     - Fixed case when CommandNewestHistoryItemsOnTop was *not* on
     - Fixed LSXCommand hiding / showing itself on inappropriate
       commands from the context menu
     - Fixed command parsing by rewriting code - woopee!!

     New Features
     ````````````
     - Totally re-written command parser  (This is also a bug fix)
     - Added following engines:

       * IMDB	- Search for movies by title at imdb.com

     Notes
     `````
     This section will just comment on the new command parser, since
     it's not big enough for it's own section.

     The new command parser allows for anything to be quoted.
     However, do NOT try and use quoted names for aliases and search
     engines in your configuration files yet - they just aren't
     implemented.  I want this release to entirely stabilize before
     adding other features and moving on to other stuff for a while.

     Anyway, back to the parser.  Before, things like this would not
     run: (Let's say you have an alias of: nt notepad)

       "nt" c:\litestep\step.rc <ENTER>
       "nt c:\litestep\step.rc" <ENTER>

     The first, now, *will* run.  The second *will NOT* run.  This
     should be obvious: the first explicitly says nt is a command
     of its own.  The second says the whole line is a command of its
     own.  Since there is no command that fits this at all, nothing
     will happen.

     Also, quoted paths are now allowed.  So, you can now use

       "c:\program files\something"

     to launch files.

     LSXCommand now operates in the following fashion when given a
     command:

       - Try against Aliases
       - See if it fits !Bang / Calculator / Search
       - If not, try to execute the command and parsed arguments
       - If this does not work, but the command *was* a path,
         then try the whole line alone.  If it wasn't a path,
         your AssumeNetAddress setting kicks in and decides whether
         or not to launch your browser.


   Version 1.5.1
   -------------
   A QBF: Quick Bug Fix and one feature

     Bug Fixes
     `````````
     - Fixed all one word commands.  Seemed to be really broken, eh?

     New Features
     ````````````
     - Added following RC Commands (see Section II.2 for details)

       * CommandUnixHistory


   Version 1.5
   -----------
   Features, features, features:

     Documentation Changes
     `````````````````````
     NOTE: This section only relates to changes made to documentation
     that existed prior to the current version.

     - Added information about how to make engine entries for engines
       using POST method instead of GET.  (See section II.4)

     Bug Fixes
     `````````
     - Fixed memory leak on every autocomplete
     - Fixed bug where people couldn't edit their engines.list
       while LSXCommand was loaded.
     - Fixed bug where, if you don't have CommandClearOnHide or
       CommandNoClearOnCommand *was* on, the history wasn't in the
       correct place the next time you used lsxcommand.  (Didn't
       catch that, did ya?!)
     - Fixed bug where quoted paths / search phrases were ignored.

     New Features
     ````````````
     - Version Resource added for completeness.
     - Calculator Functionality (see Section II.6 for details)
     - Tab-MicroComplete (see Section II.5 for details
     - Context Menu Command Access (see Section II.5 for details)
     - Quoted Paths and Quoted Search Phrases
     - Added following RC Commands: (see Section II.2 for details)

       * CommandNoWarnOnError
       * CommandHistoryMenuEntries
       * CommandNewestHistoryItemsOnTop
       * CommandContextMenuAboveBox
       * CommandContextMenuExecute
       * CommandContextMenuOrder
       * CommandClearHistoryOnStartup
       * CommandAssumeNetAddress

     - Added following !Bang Commands: (see Section II.3 for details)

       * !CommandClearHistory
       * !CommandShowContextMenu

     - Added following engines:

       * BigBook - Find businesses in all 50 US states
       * BarnesAndNoble - Find books at Barnes & Nobles
       * MediaFindMP3 - Use Mediafind for MP3 searching
       * Reel - Find movies by title
       * Archieplex - Huge Archie (FTP) database
       * Filez - Search with Filez.com
       * Shareware - C|Net Shareware.com
       * NorthernLight - A real search engine.. :)
       * OpenText - Another search engine
       * InfoSpace - ''
       * GoTo - ''
       * Pathfinder - ''
       * MetaCrawler - ''
       * Top5 - Top 5% of all web sites (so they say..)
       * A2Z - The Lycos A2Z guide
       * AskJeeves - The famous ask jeeves
       * MerckMan - The Merck Manual
       * APWire - The Associated Press Wire
       * WhoWhere - E-Mail search
       * LibraryOfCongress - Search *the* Library
       * Finger - finger user@hostname
       * NSLookup - NSLookup...
       * LettermanTop10 - Search Dave Letterman's Top Ten
       * Anonymizer - Anonymize yourself!


   Version 1.1.1
   -------------
   Just one bug fix, but important:

     New Features
     ````````````
     - Added following search "engines":

       * Dictionary	- searches Merriam-Webster's Dictionary
       * Thesaurus	- searches Merriam-Webster's Thesaurus
       * OTH		- Archie search for MP3s, I think...
       * FOLDOC		- Free On-Line Dict. Of Computing
       * Quote		- Quote.Com stock quotes
       * FileDemon	- Software search of major sites

     Bug Fixes
     `````````
     - Bug where you couldn't edit your step.rc as long as
       lsxcommand.dll was loaded.
     - Same bug for engines.list


   Version 1.1
   -----------
   A few bug fixes and a little bit more...

     New Features
     ````````````
     - Added Select All on Focus by specifying
       CommandSelectAllOnFocus in the step.rc
     - Added 'Aliasing' - this allows you to "bind" a single word to
       a lengthy command.  For instance, you can specify 'step.rc'
       to be "notepad c:\litestep\step.rc"
     - Added following search "engines":

       * UPS		(?UPS 1Z99999999) - UPS shipment tracking
       * FedEx		(?FedEx 90923012) - FedEx shipment tracking
       * Airborne	(?Airborne 21039) - Airborne Express tracking

     Bug Fixes
     `````````
     - Fixed bug where number of entries reported was wrong
       (this is an internal code fix - just good bookkeeping)
     - Added the engines.list file to the archive


I. About LSXCommand
===================
   LSXCommand is a greatly extended version of LSCommand (by limpid).
   A quick list of the enhancements are:

     * Entirely rewritten History code (sorry limpid!)
     * Resizable History
     * History stored in Modules.ini (faster, makes more sense to me)
     * AutoComplete
     * Internet Search
     * Aliasing - or Binding, if you like
     * Calculator Functionality (Regular & RPN!)
     * Context Menu (Right-Click) Access
     * Backgrounds & Transparency
     * A Clock with Date functionality as well
     * A separate clock module
     * A 'replacement' for Killarny's script.dll for use with
       LSXCommand
     * Heck, it can even use WinAmp's scrolling title feature...
     * A few speed tweaks


II. How to use LSXCommand
=========================
   LSXCommand is used in the exact same fashion as LSCommand.  The
   only differences now are: 1. You have more (or less) history to
   deal with (depending on your settings)  2. AutoComplete works much
   like that of Internet Explorer / Netscape and  3. You can now
   search the net directly from LSXCommand!


   II.1 Internet Searches
   ------------------------
   Both Netscape and Internet Explorer support a small feature where
   you can type a question mark (?) and then a keyword, and they will
   search the net for you.  However, you could never tell which
   engine Netscape was going to use (Netcenter had deals with all the
   major engines) and IE always seemed a bit screwy to me.  Anyway,
   LSXCommand now has this functionality - and you always know which
   engine you are searching with!

   For instance, to search for "Litestep" with LSXCommand, enter the
   following and hit <Return>:

     ? Litestep

   The above will search your default search engine for the phrase
   Litestep.  (See the section below about the engines.list file to
   set up your default engine, and add more!).

   To use a specific search engine with your query, you must simply
   insert an engine ID after the question mark, like this (Note the
   lack of a space after the question mark - it's important!):

     ?Altavista Litestep

   This searches AltaVista for the phrase Litestep.

   The following is the list of engines that come preconfigured in
   the sample engines.list (Note - unless otherwise stated, these are
   configured to the default options these engines use.  Another note
   - engine names are not case sensitive, I just wrote them the
   proper way in the examples):

     * Default: Yahoo!
     * Yahoo!		(?Yahoo quake) [Note exclusion of !]
     * AltaVista	(?AltaVista phrase I can't find anywhere)
     * Lycos		(?Lycos insane asylum)
     * Excite		(?Excite dilbert)
     * InfoSeek		(?InfoSeek obey your thirst)
     * Google!		(?Google linux) [Note exclusion of ! again]
     * HotBot		(?HotBot nothing in particular)
     * DejaNews		(?DejaNews good compilers)
     * LycosMP3		(?LycosMP3 Offspring Gone Away)
     * ScourMP3		(?ScourMP3 Robert Miles Children)
     * Lyrics		(?Lyrics I wish I was a little bit taller)
     * LyricsByTitle    (?Lyrics I Wish)
     * Slashdot		(?Slashdot Quest for Cool Cases)
     * Floach		(?Floach lsxcommand)
     * MSDN		(?MSDN I didn't know that API call existed)
     * Amazon		(?Amazon Barbarians Led by Bill Gates)
     * SoftSeek		(?SoftSeek UltraEdit)
     * Hardware		(?Hardware AMD K7)
     * CMA		(?CMA phrase in that article I can't find)
     * Patent		(?Patent US05731339__)

     Added as of 1.1:

     * UPS		(?UPS 1Z99999999999)
     * FedEx		(?FedEx 1092831023)
     * Airborne		(?Airborne 123934309)

     Added as of 1.5: (sorry, I gave up on examples - too many!)

     * BigBook
     * BarnesAndNoble
     * MediaFindMP3
     * Reel
     * Archieplex
     * Filez
     * Shareware
     * NorthernLight
     * OpenText
     * InfoSpace
     * GoTo
     * Pathfinder
     * MetaCrawler
     * Top5
     * A2Z
     * AskJeeves
     * MerckMan
     * APWire
     * WhoWhere
     * LibraryOfCongress
     * Finger
     * NSLookup
     * LettermanTop10
     * Anonymizer

   Just a couple of notes:
     - LycosMP3 and ScourMP3 are customized versions of Lycos
       (lycos.com) and Scour (www.scour.net) to search strictly for
       MP3's.
     - Lyrics (by itself) searches www.lyrics.ch for the lyrics
       themselves.  LyricsByTitle searches for titles of songs
       available on the Lyrics server.
     - Hardware searches Tom's Hardware - a good site for hardware
       reviews.
     - CMA searches ZDNet's Computer Magazine Archive - which is cool
       if you don't remember where you saw an article.
     - Patent looks up the specified patent number on IBM's Patent
       server - www.patents.ibm.com
     - The last three are all shipment tracking searches - UPS, FedEx
       or Airborne Express

   For more information about adding, removing, or changing engines
   in the list file, see the section below.

   PLEASE NOTE: LSXCommand's default search engine list is quite
   large; however, there are tons of other good search engines out
   there.  After version 1.5, all engines which are said to be Added
   in the Change Log (above) are actually added to the master
   LSXCommand Search Engines list available at my web site:
   http://floach.pimpin.net/visigoth/litestep/lsxcommand-engines.list

   CATEGORY FUNCTIONALITY

   Before I start on the category functionality, I have to say the
   following: your default search engine (identified by the
   "Default ..." line in your search engines list) should be left
   alone on a line by itself. Try and make it the first non-comment
   line in your engines list.

   Just one more note: When a search engine is in a category, you
   access it *THE SAME WAY* when typing a search command.
   (ie. even if "Amazon" is in the "Books" category, you still use it
   by typing "?Amazon <book>")

   That said, we can get to the meat of the subject.  Categories are
   the same as sub-menus in the popup menu.  The only difference
   between search engines in a category and not in a category is the
   fact that one is in a sub-menu (such as "Books ->" or "MP3 ->") and
   the other is in the root "Search Engines ->" sub-menu.

   To create a category in your search engines list, all you have to
   do is type the name of the category and then an open brace ( "{" ).
   This must be on its own line.  Now you can place as many search
   engines as you like in that category, each one on its own line just
   as in the original engines.list.  To close a category, simply place
   a closing brace ( "}" ) on its own line.

   You can also have spaces in your category names (NOTE: This doesn't
   mean you can have spaces in your search engine names.  That has
   not been implemented).  To do this, enclose the category name in
   quotes, such as "Music and Movies".  Also, you can have a keyboard
   accelerator assigned to a sub-menu.  (You know how the File menu
   and the Edit menu have underlines under the 'F' and 'E'?  You can
   have that too - those are called keyboard accelerators).  To have
   a keyboard accelerator for a particular letter in the category
   name, place an ampersand ( "&" ) in front of the letter.  For
   instance, "Music and &Movies".  If you want to have an ampersand
   inside the name of a category, put two ampersands in a row.  As an
   example, "Music && Movies" will have an ampersand between "Music"
   and "Movies" but no keyboard accelerator.  "Music && &Movies" will
   have both an ampersand and an accelerator.

   You are allowed to have sub-categories ad-infinitum (meaning, you
   can make sub-menus on the sub-menus and just keep going as far as
   you like.  You could do something like Computers->Software->
   Download Libraries->.

   For human readability, you can also have white space in front of
   both search engine lines and category lines.

   For an example, see the included engines.list file in this
   distribution of LSXCommand.


   II.2 Added RC Commands
   ----------------------
     NOTE: Please see original LSCommand readme for previous
     RC Commands.

     CommandBottom/Top/Left/RightBorderSize
     ``````````````````````````````````````
     Description: These four commands specify separate borders for
     each side of the edit box.  The default for all of them is 2.
     You can only use these when CommandBorderSize is *not*
     soecified.

     Example:
       CommandTopBorderSize 4
       CommandBottomBorderSize 4
       CommandRightBorderSize 0

     This will set a 4 pixel border on top and bottom, a 2 pixel
     border on the left (default), and no border on the right.

     CommandBorderSize
     `````````````````
     Description: Sets the border size around the edit box.  This
     sets the border on each side to the same number.

     If you want to have different sized borders on each side, use
     CommandTopBorderSize, CommandBottomBorderSize, CommandLeft...,
     and CommandRight... (see above)

     If CommandBorderSize is specified, Command<SIDE>BorderSize
     commands are ignored.

     Default for all these commands is 2.

     CommandAddExternalsToHistory
     ````````````````````````````
     Description: If this is on, LSXCommand will add any commands
     launched with the !Command !Bang command to the history.  Since
     I've been using AlterScript with this a lot, it became
     annoying to have all of AlterScript's commands end up in
     LSXCommand's history.  NOTE: This is an LSXCommand RC Command
     (not for AlterScript).  It affects any command run with !Command
     and not just AlterScript's.


     CommandSearchEngineBrowser
     ``````````````````````````
     Description: Path to the browser to be used with search engines.
     If not specified, LSXCommand uses the default browser on your
     system.

     Example: CommandSearchEngineBrowser c:\opera\opera.exe


     CommandWinAmpDisappearsOnFocus
     ``````````````````````````````
     Description: Just like CommandClockDisappearsOnFocus, but for
     the WinAmp scrolling feature.


     CommandTabFileComplete
     ``````````````````````
     Description: Enables the completion of file names by using the
     tab key.  NOTE: You will want to also turn on the
     CommandNoTabMicroComplete RC Command


     CommandOffsetX/Y
     ````````````````
     Description: This value determines which point on the screen
     LSXCommand should use as a reference for the CommandX/Y values.
     The default is the upper left corner.

       0  -  Left / Top
       1  -  Horizontal / Vertical Center
       2  -  Right / Bottom

     Examples: CommandOffsetX 0   CommandOffsetY 2


     CommandHideOnUnfocus
     ````````````````````
     Description: Hides LSXCommand when it loses keyboard focus.
     This can be used to hide LSXCommand after it runs an app.

     Example: CommandHideOnUnfocus


     CommandContextMenuStandardItems
     ```````````````````````````````
     Description: This item does two things.  It first turns on the
     standard menu items: Cut, Copy & Paste.  Second, it tells where
     to put the standard items, above or below the LSXCommand
     specific menus.

     Example: CommandContextMenuStandardItems 1   ; Above
              CommandContextMenuStandardItems 2   ; Below
              CommandContextMenuStandardItems 0   ; Off (Default)

     CommandScrollWinAmpTitle
     ````````````````````````
     Description: If on, this setting takes WinAmp's scrolling title
     and displays it in LSXCommand.  Note that WinAmp's scroll song
     title preference must be on for this to happen.  A neat trick
     with this is to have both a clock and a WinAmp scroller.  The
     WinAmp scroller will show when WinAmp is running.  If not, the
     clock will show.  (Note that for the clock to show, you must use
     the CommandTime RC Command described below).

     Example: CommandScrollWinAmpTitle


     CommandNoAutoComplete
     `````````````````````
     Description: Turns *off* AutoComplete.  Someone asked for it, so
     I coded it :).

     Example: CommandNoAutoComplete


     CommandCommaDelimiter
     `````````````````````
     Description: Some Eurpoean countries use commas as decimal
     delimiters, so this changes decimals from periods to commas.

     Example: CommandCommaDelimiter


     CommandRPNCalculator
     ````````````````````
     Description: Makes the calculator an RPN calculator.  If you
     don't know what this is, don't use it!  You'll only confuse
     yourself.  Others call RPN "Postfix" notation.  If you want
     to learn more, obtain a Hewlett-Packard RPN calculator (the
     48/49 series are especially nice, IMO :).

     NOTE: This does not make the history a stack!  It merely
     means that you can write your expressions as postfix and it
     will be calculated correctly.  For instance, typing:

       5 <ENTER> 2 <ENTER> + <ENTER>

     will NOT work.  You must type "=5 2 +" to get the answer

     Example: CommandRPNCalculator


     CommandBackground
     `````````````````
     Description: Path to your background image.  If this is not an
     absolute path, LSXCommand looks in your images directory.  If
     this option is not specified, the background & border colors
     are used (see the original lscommand readme for info on these
     colors).

     NOTE: See section II.7 for more about backgrounds and their
     relationship to transparency.

     Example: CommandBackground "lsxcommand_background.bmp"


     CommandTransparentEditBox
     `````````````````````````
     Description: Makes the area in which you type transparent to the
     background.  If this option is not specified, the background
     image shows through.  If there is no background, the background
     color is used (see the original lscommand readme for info on
     these colors).

     NOTE: See section II.7 for more about backgrounds and
     transparency.

     Example: CommandTransparentEditBox


     CommandTime
     ```````````
     Description: Format string that tells LSXCommand how you want
     your time & date to look.

     NOTE: See section II.8 on different format strings and what
     effect they have.  It's quite long, but worth it.

     Example: CommandTime "%#c"

     NOTE: The above results in something like "Monday, June 14, 1999
     18:00:30" depending on the real date and time.


     CommandClockDisappearsOnFocus
     `````````````````````````````
     Description: If you want the clock to automatically disappear
     when you click in LSXCommand, set this on.  Otherwise, the
     clock will "pause" at the current time/date.  This makes it
     easier for you to copy/paste the time & date into any app.

     Example: CommandClockDisappearsOnFocus


     CommandSelectAllOnMouseFocus
     ````````````````````````````
     Description: Just like CommandSelectAllOnFocus, just that it
     only works when clicking to focus LSXCommand.  The reason
     these are two separate options is for people like me: I like to
     be able to just click once to put the cursor where I want it -
     not select the entire command.  But, some people like the
     explorer way of doing things better, so I added it.

     Example: CommandSelectAllOnMouseFocus  ; More explorer functions

     CommandExplorePaths
     ```````````````````
     Description: If enabled, any directory path you enter into
     LSXCommand will open a treed explorer window, instead of the
     standard folder window.

     Example: CommandExplorePaths	; Gimme a Tree!


     CommandUnixHistory
     ``````````````````
     Description: The most popular UNIX shell (bash) has the inverse
     history navigation: Up for previous, down for next.  You can
     use this system with this setting.

     Default: Down is previous; Up is next

     Example: CommandUnixHistory	; I like my old command line


     CommandNoWarnOnError
     ````````````````````
     Description: If there is an error (for instance, if the command
     you entered is invalid or the calculator couldn't evaluate the
     expression you entered), you will get an error message.  This,
     when turned on, will suppress it.

     Default: Off (meaning - you get the error message)

     Example: CommandNoWarnOnError	; Stops error messages


     CommandHistoryMenuEntries
     `````````````````````````
     Description: Sets the maximum number of history entries available
     from the context menu.

     Default: The max number of history entries

     Example: CommandHistoryMenuEntries 10	; 10 Entries available


     CommandNewestHistoryItemsOnTop
     ``````````````````````````````
     Description: This setting places newer history items at the top
     of the menu, instead of at the bottom.

     Default: Newest items go on the bottom

     Example: CommandNewestItemsOnTop	; Puts newest stuff on top


     CommandContextMenuAboveBox
     ``````````````````````````
     Description: If you have your LSXCommand window at the bottom of
     the screen, you will want to turn this on.  If at the top, leave
     this off.  This sets where the context menu will appear - above
     or below the LSXCommand box.

     Default: Context Menu appears below box

     Example: CommandContextMenuAboveBox	; Menu appears above box


     CommandContextMenuExecute
     `````````````````````````
     Description: Tells LSXCommand to execute a selection from the
     context menu instead of copying its data to the LSXCommand box.
     NOTE: Since the point of search engines is to have you enter a
     search phrase, it is not possible to have search engines
     execute.

     Default: Copies data (ie history / alias entry) to the box.

     Example: CommandContextMenuExecute	; Execute a command, no copy


     CommandContextMenuOrder
     ```````````````````````
     Description: Specifies which order each submenu should appear.
     For instance, you can have History, Alias, then Search Engines
     or Alias, History, Search Engines, or...

     The setting is 3 numbers: 0, 1, and 2.  The combination of
     these numbers sets in which order each submenu appears.  The
     first number is the position of the History submenu.  The
     second is the Alias submenu.  And the last is the Search
     Engines submenu.

     Default: 012

     Example: So, to have a menu where Search Engines are at the top,
     Aliases next and the History at the bottom, you would specify:

     CommandContextMenuOrder 210


     CommandNoTabMicroComplete
     ````````````````````````
     Description: Stops Tab-MicroCompletes from taking place.  See
     section II.5 on more about Tab-MicroComplete

     Default: Tab-MicroComplete is on

     Example: CommandNoTabMicroComplete	; I don't like fancy stuff


     CommandClearHistoryOnStartup
     ````````````````````````````
     Description: Clears the history on startup. NOTE: You *will*
     lose the history in modules.ini - not only the history in
     memory.

     Default: Keeps last session's history

     Example: CommandClearHistoryOnStartup


     CommandAssumeNetAddress
     ```````````````````````
     Description: If a command just doesn't cut it, you can have it
     start your browser with the address.

     Default: You will get an error message - this is suppressable by
     CommandNoWarnOnError

     Example: CommandAssumeNetAddress

     Example (in lsxcommand): home

     If home is not found in your path, your browser is loaded with
     http://home (which usually gets changed to http://home.com or
     http://www.home.com)


     CommandHistoryEntries
     `````````````````````
     Description: Sets number of history entries of which to keep
     track.

     Default: 10

     Note: I haven't really stress-tested LSXCommand, so I don't know
     how well it scales up, but it shouldn't be too taxing even at
     somewhere around 50 entries.  But I think that's a bit much
     anyway...

     Example: CommandHistoryEntries 20


     CommandSearchEngineList
     ```````````````````````
     Description: The path to your engines.list - the list of
     search engines to use with LSXCommand.

     Default: -

     Note: If none is supplied, or the file is unavailable, the
     search feature will be unavailable.
     IMPORTANT: This must be an absolute path.

     Example: CommandSearchEngineList C:\Litestep\engines.list


     CommandSelectAllOnFocus
     ```````````````````````
     Description: Selects everything in the command box on focus

     Default: True

     Example: CommandSelectAllOnFocus	; Selects all on lsxcommand


     *CommandAlias
     `````````````
     Description: Aliases (or Binds) a phrase to another command.

     Default: -

     Example: *CommandAlias step  notepad c:\litestep\step.rc


   II.3 Added !Bang Commands
   -------------------------
     NOTE: See Original LSCommand readme for previous !Bang Commands

     !CommandSetText
     ```````````````
     Description: This !Bang sets the text in LSXCommand to its
     arguments.  This makes for some really neat stuff (for instance,
     a shortcut that pops up LSXCommand with a question mark and a
     space already in it ready for an argument to search with).  Note
     that this !Bang command does not show LSXCommand if it is hidden.

     Example: !SetText "?Altavista "

     !CommandToggleTimer
     ```````````````````
     Description: Toggles the current timer.  If you use this, the
     WinAmp timer no longer takes precedence over the clock.  You have
     to call this !Bang again for the WinAmp scroller to appear.

     !CommandMove
     ````````````
     Description: Moves the LSXCommand window by the amount specified
     in the arguments.  The amount is a pair of numbers, separated by
     a space, specifying the number of pixels to move left/right or
     up/down.  If there is only one number, it is assumed to be
     horizontal movement.

       !CommandMove 0 100  ; moves it 100 pixels down

     When !CommandMove is called without parameters, it moves to the
     current mouse cursor position.  If !CommandMove is called with
     "home" as its parameter, it moves back to the original position
     it received from step.rc

     !CommandBrowseFile
     ``````````````````
     Description: Brings up an Open Dialog Box to browse for a file
     for LSXCommand to open.  It's just the same as typing in the
     file's name.  NOTE: You can send an argument to this !Bang
     command to specify the filter for the open box.  For instance,
     to see just Text Files, you might do this:

       !CommandBrowseFile    All Text Files (*.txt, *.ini)|*.txt;*.ini


     !CommandBrowseFolder
     ````````````````````
     Description: Brings up a folder selection dialog.  You can
     select a folder to browse to.  There are no arguments for this
     !Bang command.


     !CommandShow
     ````````````
     Description: Just shows LSXCommand.  NOTE: It does *not* give
     LSXCommand focus.  To do this, use !FocusCommand - this command
     gives focus to LSXCommand, and if it's not visible, it shows it
     also.


     !CommandHide
     ````````````
     Description: Just hides LSXCommand.  Nothing really special.


     !Command
     ````````
     Description: Now use the power of LSXCommand anywhere within
     Litestep!  Shortcuts, wharf items, whatever that can run !bang
     commands outside of its own can now use LSXCommand.

     Example: !Command notepad


     !CommandClearHistory
     ````````````````````
     Description: Clears both the history in memory *and* the history
     in modules.ini


     !CommandShowContextMenu
     ```````````````````````
     Description: Makes LSXCommand's Context Menu visible wherever
     the cursor is on the screen, regardless of LSXCommand's
     visibility.


     !CommandRescanEngines
     `````````````````````
     Description: Rescans the engines.list file.  Note: it does not
     load a new engines.list file if specified in the step.rc; it
     will attempt to reload the same engines.list file


   II.4 The engines.list File
   --------------------------
   This file tells LSXCommand how to use different search engines to
   search the Internet.  The basic structure of the file is like
   this (Note - it's probably a good idea to follow along with the
   sample engines.list):

     Default	<moniker>
     <moniker1>	<keyed url1>
     <moniker2>	<keyed url2>
     ...		...

   The <moniker> element is one of the monikers listed below
   (<moniker1>, <moniker2>, etc).  As explained above, the default
   engine that will be used is <moniker>.  The <keyed url#> elements
   are, basically, URLs that tell LSXCommand where to send your web
   browser.  However, there is one difference: the special "key"
   <args> is used to determine where to place your search string.
   For instance, let's take a look at Yahoo!'s line:

     Yahoo		http://search.yahoo.com/bin/search?p=<args>

   This means the following:

     * The user must begin a search with ?Yahoo to use this search
     * The user's search phrase (stuff after ?Yahoo) is placed in
       <args>.  So, the statement "?Yahoo something" gets converted
       to: http://search.yahoo.com/bin/search?p=something

   Multi-word searches are handled automatically by LSXCommand.  For
   this reason, LSXCommand can't quite use specialized engines to
   their full extent - for instance, the "Simple Search" function on
   the Lyrics server - www.lyrics.ch - has more than one search
   field.

   You can find the full engines.list (the one shipped with
   LSXCommand is by no means complete and neither is this one, but it
   is larger) here:

     http://floach.pimpin.net/visigoth/litestep/lsxcommand-engines.list

   TO MAKE YOUR OWN SEARCH ENTRIES:

   Go to the page where you enter a search phrase.  Type something
   and search for it.  If your URL bar has a lot of &'s and ='s, this
   site can be used with LSXCommand.  All you have to do is copy this
   URL and paste it into your engines.list file with a moniker of its
   own.  Then, replace the phrase you searched for with the key
   <args>.

   The above type of an engine is one that uses the GET method to
   send data to the web server.  The other type of method is called
   POST.  This is the one that doesn't have all the &'s and ='s in
   the URL.  What you have to do is go back to the search page and
   go into the actual HTML.  Find the place with the <FORM> tag and
   from here on, you have to set up your own &'s and ='s from the
   names and values listed, using the ACTION= property of the form
   tag as a base.  Sorry if it's confusing, but it's almost 5 AM.


   II.5 Tab-MicroComplete & Context Menu Access
   --------------------------------------------
   Tab-MicroComplete is a feature I made up on my own - not even
   Microsoft has it.  When you are typing in LSXCommand and get
   an autocompletion, you may want to automatically go on to the next
   word.  For instance, when you search with a particular search
   engine, and then want to search with the same engine again, but
   with a different phrase.

   With Tab-MicroComplete, you can do that.  If you hit tab while in
   LSXCommand, the cursor will be moved to the start of the next
   word, or to the end of the line (depending on where the current
   cursor is).

   LSXCommand now has Context Menu Access!  You can either right-
   click on LSXCommand's command box or call the !CommandContextMenu
   !bang command.  If you use the !bang command, the menu appears
   wherever your mouse is on screen.  The Context Menu contains
   past history entries, all your aliases, and all search engines.


   II.6 Calculator Functionality
   -----------------------------
   As of version 1.7.8, the calculator functionality has been entirely
   revamped, thanks to the work of Jonas Sicking.  Just like in older
   versions, calculator commands start with an = sign at the prompt, then
   the expression to evaluate.

   What's new?  Everything - from the supported operators to the base
   conversion, it's all been redone.

   LSXCommand handles float precision, so decimals are allowed.  The
   result will be placed right back into LSXCommand's command box
   with an equals sign in front of it and a space after it.  This
   enables you to use the result of the last command right off the
   bat!  (By the way - spaces are allowed)

   LSXCommand understands the following operators:

     + - * \ / ( ) ^ % ! E

     Addition, Subtraction, Multiplication, Division, Division,
     Open Parenthesis, Close Parenthesis, Power, Modulus (remainder),
     Factorial, Exponential

     Yes, there are two division operators.  HOWEVER, the \ means
     integer division, and the / means regular division.

     E is the 10 to the power operator.  For instance, 1E6 is 1 million.
     Note: don't try 1*E6 - that won't work.  Just do 1E6 or (1E6).  You
     can even do 1E(5+4), but don't try to multiply the two.

   FULL RECURSION

   With the new calculator, recursion is fully supported.  Meaning, as
   long as the expression follows the order of operations, it should be
   able to be calculated.  For instance, you could easily do the following
   calculation: =1E(log2(64)), which would return =1000000 (1 million).

   MATH LIBRARY FUNCTIONS

   With the revamping of the calculator functionality comes some really
   useful stuff: a library of mathematics functions!  The following is a
   list of the supported math functions:

      TRIG FUNCTIONS        ANALYTIC FUNCTIONS      CONSTANTS
      ``````````````        ``````````````````      `````````
      sin                   exp                     pi
      cos                   ln                      e
      tan                   log
      cot                   log2
      asin                  log10
      acos                  sqrt
      atan                  floor
      acot                  ceil
      sinh                  abs
      cosh                  fpart
      tanh                  ipart
                            sign
                            rand
                            rnd

   The trig functions should be self explanatory.  The log functions are
   a bit tricky: ln and log are both base e, while log2 and log 10 are
   base 2 and 10 respectively.  If you haven't heard of the other
   functions (like fpart and ipart), consult your graphing calculator's
   manual - the one you never bothered to read because you already thought
   you knew how to use a calculator :P

   To use these functions, you would type something like '=sin(0)'.  Note
   that since pi and e are constants, you don't put () after them.  For
   instance, =pi would result in =3.141..

   RPN CALCULATOR FUNCTIONALITY

   As of version 1.7, you can use LSXCommand as a "Postfix"
   calculator.  If you are not familiar with this expression, leave
   it be and don't worry about it!  If you really want to learn more,
   obtain a Hewlett-Packard RPN calculator and learn to use it.

   RPN mode does *not* turn your history into a stack.  It does,
   however, make LSXCommand accept postfix notation expressions. See
   the documentation for the CommandRPNCalculator RC Command above in
   section II.2.

   This functionality extends to the 1.7.8 revamping of the calculator
   code.  It should work fine, but I haven't played with it enough to say
   anything more than that.

   MULTIPLE NUMBER BASE FUNCTIONALITY

   As of version 1.7.1, you can specify numbers in different number
   bases.  With the 1.7.8 rewrite of the calculator, LSXCommand supports
   4 bases: decimal (10), binary (2), hexadecimal (16) and octal (8).

   To specify a number as binary, place a 'b' in front of it.  For
   instance, entering "=b10" in LSXCommand will return "=2" as an
   answer.  If you typed "=5 + b10", you would get "=7" as an
   answer.

   Append the following prefixes for its respective base:

   b - binary
   h - hexadecimal
   o - octal
   d - decimal (this is, of course, the default)

   NUMBER CONVERSION FUNCTIONALITY

   With the version 1.7.8 rewrite comes much more flexible number
   conversion.  You can convert between any of the bases supported by
   LSXCommand (listed above).

   To convert *from* decimal *to* hexadecimal (color conversion), you
   would do the following:

     =dh 255 255 255    -->   =hff hff hff

   So, to convert *from* a base *to* another base, place the
   identifier of the from base first, then the to base, then each
   number to convert with a space between each one.


   II.7 Backgrounds & Transparencies
   ---------------------------------
   I'm sure many people have been wanting these features, so I made
   my best attempt at added them to LSXCommand.

   Backgrounds are just that - backgrounds.  However, they only fill
   the background window, much like the background can be set in
   mIRC, but it doesn't show through the channel windows.  If you
   want a background to show through to the place you are typing,
   set the CommandTransparentEditBox RC Command.

   If there is no background set, CommandTransparentEditBox is
   automatically turned off since the only effect will be to slow
   down performance and other nasty things.

   If there is a background, but CommandTransparentEditBox is *not*
   set, the background acts like a border around the text box.

   DISCLAIMER: If you turn CommandTransparentEditBox *on*, you WILL
   see flashing as you are doing normal things with LSXCommand - such
   as typing.  Seriously, though, I have no doubts that you will get
   some type of flashing effect in LSXCommand no matter what you do.
   Until I write my own text output routines, you will just have to
   deal.


   II.8 Clock Settings
   -------------------
   The clock functionality in LSXCommand is quite robust, though hard
   to use since the format commands look nothing like what you would
   expect them to.  If you want a quick format command that gives you
   both the date and time, use "%#c".  If you want more
   customization, you will just have to read below.  Sorry...

   Okay, so you want more control, eh?  The following comes straight
   from the Microsoft Developer Network Library.  It is a table of
   format commands you must use to create your time & date string.
   Examples are listed below the table.

   %a                 Abbreviated weekday name
   %A                 Full weekday name
   %b                 Abbreviated month name
   %B                 Full month name
   %c                 Date & time representation for current location
   %d                 Day of month as number (01 - 31)
   %H                 Hour in 24-Hour format (00 - 23)
   %I                 Hour in 12-Hour format (01 - 12)
   %j                 Day of year as number (001 - 366)
   %m                 Month as number (01 - 12)
   %M                 Minute as number (00 - 59)
   %p                 AM / PM indicator for current location
   %S                 Second as number (00 - 59)
   %U                 Week of year as number, with Sunday as first
                      day of the week (00 - 53)
   %w                 Weekday as number, with Sunday as 0 (0 - 6)
   %W                 Week of year as number, with Monday as first
                      day of the week (00 - 53)
   %x                 Date representation for current location
   %X                 Time representation for current location
   %y                 Year without century (two digits) (00 - 99)
   %Y                 Year with century (four digits)
   %z                 Time-zone name / abbreviation
   %Z                 Same as %z
   %%                 Percent sign

   The # sign (pound, hash, whatever you like..) can be used as a
   prefix for most format commands.  For instance, %#<character>. The
   following lists the change in meaning for those commands that are
   affected:

   %#c                Long date & time representation for current
                      location.
   %#x                Long date representation only for current
                      location.
   %#d, %#H, %#I,     Remove leading zeros.
   %#j, %#m, %#M,
   %#S, %#U, %#w,
   %#W, %#y, %#Y

   So, what the heck does all that crap mean?  Basically, you think
   of how you want your date & time to look, and then substitute the
   proper codes where they are needed, for instance:

     Think Of..             Code Used
     ``````````             `````````
     05:30:00 PM            %I:%M:%S %p
     Monday 06/14/99        %A %x
     Mon. 6/14/1999         %a %#m/%#d/%Y  -  Removes leading zeros
     June 14, 1999          %B %d, %Y
     Monday, June 14, 1999  %#x

   I hope that's enough examples.  If you have any problems, see
   section IV about contacting me.


   II.9 Tab File Completion
   ------------------------
   If you turn on CommandTabFileComplete and
   CommandNoTabMicroComplete, you can have LSXCommand autocomplete
   filenames.  For instance, if you type c:\autoe and hit <TAB>, you
   will most likely get C:\AUTOEXEC.BAT  If not, you can hit <TAB>
   again to get the next matching filename.  Note that you do have
   to turn on both RC commands for this to work.


   II.10 LSXCommandClock module
   ----------------------------
   The LSXCommandClock module is a separate module from LSXCommand
   that only has the clock and winamp features enabled.  The RC
   Commands are similar, but have a CommandClock instead of Command
   prefix.  The following is a list of the RC Commands supported
   by LSXCommandClock (see section II.2 for details about these
   Commands)

     * CommandClockTextFontFace
     * CommandClockTextColor
     * CommandClockBorderColor
     * CommandClockBackground
     * CommandClockX, LSXCommandClockY
     * CommandClockWidth, LSXCommandClockHeight
     * CommandClockBGColor
     * CommandClockTextSize
     * CommandClockBorderSize
     * CommandClockOffsetX, LSXCommandClockOffsetY
     * CommandClockBevelBorder
     * CommandClockNotAlwaysOnTop
     * CommandClockNotMoveable
     * CommandClockHiddenOnStart
     * CommandClockNoCursorChange
     * CommandClockSelectAllOnFocus
     * CommandClockSelectAllOnMouseFocus
     * CommandClockTransparentEditBox
     * CommandClockScrollWinAmpTitle
     * CommandClockHideOnUnfocus
     * CommandClockString ****
     * !ToggleCommandClock
     * !FocusCommandClock
     * !CommandClockShow
     * !CommandClockHide
     * !CommandClockMove
     * !CommandClockToggleTimer

   **** NOTE: CommandClockString is the same as the "CommandTime"
   RC Command for LSXCommand (see section II.8 for details)


   II.11 AlterScript module
   ------------------------
   The AlterScript module is an alternative interpreter to Killarny's
   script.dll.  I was having problems getting his to work, so I wrote
   my own... go figure.  However, I'm sure the way he parses strings
   and the way I do differ - this is no substitute!!  There are many
   differences, which are described below.

   For instance, AlterScript does not interpret semi-colons that
   represent pauses in Killarny's script.dll.  Furthermore, every
   command MUST be separated by a pipe character '|'.  Everything
   between pipes is a command.

   AlterScript MUST be used with LSXCommand because it sends a
   !Command !Bang command with the specified command.  The actual
   command sent (stuff between the pipes) should be *exactly* what you
   would type into LSXCommand's text box.  If you type it without
   quotes into LSXCommand's text box, do the same between the pipes.
   Same goes for if you do it with quotes.

   Note that since I send the !Command !Bang command, you can use
   aliases and even search engine commands.


III. Known Bugs / Limitations
=============================

   III.1 Old history entries remain
   --------------------------------
   When you change the number of history entries LSXCommand of which
   LSXCommand will keep track to a lower number, history entries that
   filled slots above this number will remain in modules.ini.  This
   is a known bug - I'll fix it some day...


   III.2 Quoted Names a Problem
   ----------------------------
   Quoted names for search engines and aliases are a problem.  Since
   this would require re-writing a lot of list management code, I
   decided to put it off until next revision.  Either way, plans I
   have for the next version of LSXCommand would require a massive
   rewrite.


IV. Comments/Questions/Flames/Bug Reports
=========================================
   All of these should be sent directly to me, visigoth.  My e-mail
   address is:  gandhimail@worldnet.att.net

   Seriously, though, I really would like to know if people are using
   this thing.  It'll help me determine whether it's worth continuing
   the development of it.


V. Source Code
==============
   Yes, source code is available.  All you have to do is send me an
   e-mail for it.  My e-mail is gandhimail@worldnet.att.net
   However, if you are going to edit the code, *please* read the
   Source Code License below.  There are certain restrictions.


VI. Source Code License
=======================

   Article I. Sure, Have the Code!
   -------------------------------
   I don't want to stop progress on this module.  So, I'm going to
   let anyone who wants to edit the code, do so at his/her free will.
   However, there are restrictions, as noted by the rest of the
   license.

   Article II. Redistribution
   --------------------------
   This file *must* be redistributed with the source code, with duly
   noted changes to sections I, II, III, and IV.  However, I request
   that my name be left in Section I as the original author of the
   module.

   Article III. Leave Credit where It's Due!
   -----------------------------------------
   All I ask is that Section I still have my name in it as the
   original author.  Besides this file and the comments, there isn't
   much to say that I did anything... :)

   Article IV. Send Me Your Cool Stuff
   -----------------------------------
   I'd like to know about your ideas for LSXCommand!  If you are
   working on an idea, I would like to know what it is.  I'm not like
   Microsoft (ie. I'm going to steal your idea, throw my time at it,
   and come out with a shoddier version first).  I'd just like to
   know what people are doing with my original code base.  So, if you
   do edit code for a purpose, please drop me a line as to what you
   are doing.

   Article V. Have Fun
   -------------------
   Remember, I'm not going to stop progress on this module, so go
   ahead and do what you want to it.  Just don't take credit for
   what I've already done.