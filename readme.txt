                    *******************************
                    *         LSXCommand          *
                    *         by Visigoth         *
                    * gandhimail@worldnet.att.net *
                    *       ==============        *
                    *        Version: 1.0         *
                    *        May 25, 1999         *
                    *******************************

Table of Contents
=================
i     Quick & Dirty Install
ii    Kudos
I.    About LSXCommand
II.   How to use LSXCommand

      II.1 Internet Searches
      II.2 Added RC Commands
           - CommandHistoryEntries
           - CommandSearchEngineList
      II.3 Added !Bang Commands
           - !CommandRescanEngines
      II.4 The engines.list File

III.  Known Bugs / Limitations

      III.1 Old history entries remain

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


I. About LSXCommand
===================
   LSXCommand is a greatly extended version of LSCommand (by limpid).
   A quick list of the enhancements are:

     * Entirely rewritten History code (sorry limpid!)
     * Resizable History
     * History stored in Modules.ini (faster, makes more sense to me)
     * AutoComplete
     * Internet Search
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

     * Yahoo!		(?Yahoo quake) [Note exclusion of !]
     * AltaVista		(?AltaVista phrase I can't find anywhere)
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
     * MSDN			(?MSDN I didn't know that API call existed)
     * Amazon		(?Amazon Barbarians Led by Bill Gates)
     * SoftSeek		(?SoftSeek UltraEdit)
     * Hardware		(?Hardware AMD K7)
     * CMA			(?CMA phrase in that article I can't find)
     * Patent		(?Patent US05731339__)
     * Default: Yahoo!

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

   For more information about adding, removing, or changing engines
   in the list file, see the section below.


   II.2 Added RC Commands
   ----------------------

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


   II.3 Added !Bang Commands
   -------------------------

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


III. Known Bugs / Limitations
=============================

   III.1 Old history entries remain
   --------------------------------
   When you change the number of history entries LSXCommand of which
   LSXCommand will keep track to a lower number, history entries that
   filled slots above this number will remain in modules.ini.  This
   is a known bug - I'll fix it some day...


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