                    *******************************
                    *         LSXCommand          *
                    *         by Visigoth         *
                    * gandhimail@worldnet.att.net *
                    *       ==============        *
                    *       Version: 1.5.2        *
                    *        May 31, 1999         *
                    *******************************

Table of Contents
=================
i     Quick & Dirty Install
ii    Kudos
iii   Change Log
I.    About LSXCommand
II.   How to use LSXCommand

      II.1 Internet Searches
      II.2 Added RC Commands
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
   The following people gave me ideas after I released version 1.0:

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

   - Kryten sent in the Merriam-Webster stuff
   - bizquik sent in the File Demon engine
   - mirul sent in the MS Knowledge Base
   - Jason Murray sent in more than 230 engines!!!  That's just
     overboard, IMO
   - Scott Ferguson sent in MetaCrawler


iii. Change Log
===============

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

     to launch files.  NOTE: If you left the quotes *off* of this,
     the command would still work - like the explorer way of doing
     things.

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
     * Calculator Functionality
     * Context Menu (Right-Click) Access
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


   II.2 Added RC Commands
   ----------------------
     NOTE: Please see original LSCommand readme for previous
     RC Commands.

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
   Currently, the calculator functionality in LSXCommand is a bit
   skimpy, though it does support order of operations and
   parentheses.  To invoke a calculation, the procedure is much like
   a search or !bang command.  Just type an = sign at the prompt and
   enter the expression to calculate.

   LSXCommand handles float precision, so decimals are allowed.  The
   result will be placed right back into LSXCommand's command box
   with an equals sign in front of it and a space after it.  This
   enables you to use the result of the last command right off the
   bat!  (By the way - spaces are allowed)

   LSXCommand understands the following operators:

     + - * \ / ^ ( )

     Addition, Subtraction, Multiplication, Division, Division,
     Power, Open Parenthesis, Close Parenthesis.

     Yes, there are two division operators - some people like \ and
     others like /.


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