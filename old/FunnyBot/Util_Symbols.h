	////////////////////////////
	// OpenHoldem Symbols
	////////////////////////////

	////////////////////////////
	//GENERAL
	////////////////////////////

	//ismanual: true if you're in manual mode, false otherwise
	//OH_VAR_BOOL(ismanual);
	//isppro: true if you're connected to a ppro server
	//OH_VAR_BOOL(isppro);
	//site: 0=user/ppro 1=scraped
	//site = gws(userchair,"site");
	//nchairs: the integer value for the Table Map symbol s$nchairs

	//isbring: true if OpenHoldem is attached to a Bring client window
	//OH_VAR_BOOL(isbring);
	//session: the current logging instance (0-9)
	//session = gws(userchair,"session");
	//handnumber: the site hand number if available 
	//OH_VAR(handnumber);
	//version: returns the version number of OpenHoldem that is currently running
	//OH_VAR(version);

	////////////////////////////
	//TABLE MAP
	////////////////////////////

	//sitename$abc: (bool) true if user defined string “abc” appears within the Table Map symbol s$sitename
	//sitename$abc = gws(userchair,"sitename$abc");
	//network$def: (bool) true if user defined string “def” appears within the Table Map symbol s$network
	//network$def = gws(userchair,"network$def");
	//swagdelay: (int?) Autoplayer delay in milliseconds between swag keystrokes and button click 
	//swagdelay = gws(userchair,"swagdelay");
	//allidelay: (int?) Autoplayer delay in milliseconds between alli slider jam and button click 
	//allidelay = gws(userchair,"allidelay");
	//swagtextmethod: (int)	the site interpretation for swag edit text (Table Map symbol) 1=f$srai 2=f$srai+call 3=f$srai+call+currentbet

	////////////////////////////
	//FORMULA FILE
	////////////////////////////

	//rake: (int?) percentage amount added/subtracted to/from the pot
	//OH_VAR(rake);
	//nit: (int?) number of iterations used to calculate the prwin/prtie/prlos and associated symbols.
	//OH_VAR(nit);
	//bankroll: (double?) the user defined, real world bankroll 
	//OH_VAR(bankroll);

	////////////////////////////
	//LIMITS
	////////////////////////////

	//bblind: (double) the big blind amount

	//sblind: (double) the small blind amount

	//ante: (double) the current pre-deal ante requirement
	//OH_VAR(ante);
	//lim: (int) the current table limit 0=NL 1=PL 2=FL
	//OH_VAR(lim);
	//isnl: (int) (lim==0)
	//OH_VAR(isnl);
	//ispl: (int) (lim==1)
	//OH_VAR(ispl);
	//isfl: (int) (lim==2)
	//isfl = gws(userchair,"isfl");
	//sraiprev: (double) the difference between the two largest unique wagers
	//sraiprev = gws(userchair,"sraiprev");
	//sraimin: (double) Scraped - (currentbet+call); PokerPro - the greater of bet and the current raise
	//sraimin = gws(userchair,"sraimin");
	//sraimax: (double) balance-call
	//sraimax = gws(userchair,"sraimax");
	//istournament: (bool) true if a tournament table is detected


	////////////////////////////
	//HAND RANK
	////////////////////////////

	//handrank: (int) one of the following based on the option specified in preferences
	//OH_VAR_INT(handrank);
	//handrank169: (int) your pocket holdem hand rank 1-169

	//handrank2652: (int?) your pocket holdem hand rank 12-2652
	//OH_VAR_INT(handrank2652);
	//handrank1326: (int?) your pocket holdem hand rank 6-1326 (handrank2652/2)
	//OH_VAR_INT(handrank1326);
	//handrank1000: (int?) your pocket holdem hand rank 4-1000 (1000*handrank2652/2652)
	//OH_VAR_INT(handrank1000);
	//handrankp: (int?) 2652 / (1+nopponents)
	//handrankp = gws(userchair,"handrankp");

	////////////////////////////
	//CHAIRS
	////////////////////////////

	//chair: (int) your chair number 0-9 ... 0 is usually top right
	//chair = gws(userchair,"chair");
	//userchair: (int) user chair number (0-9)
	//userchair = gws(userchair,"userchair");
	//dealerchair: (int) dealer chair number (0-9)

	//raischair: (int) raising chair number (0-9)
	//raischair = gws(userchair,"raischair");
	//chair$abc: (int) player “abc” chair number (0-9); -1 if not found 
	//chair$abc = gws(userchair,"chair$abc");
	//chairbit$abc: (int) player “abc” chairbit (1 << chair$abc); 0 if not found 
	//chairbit$abc = gws(userchair,"chairbit$abc");

	////////////////////////////
	//ROUNDS / POSITIONS
	////////////////////////////

	//betround: (int)	betting round (1-4) 1=preflop, 2=flop, 3=turn, 4=river

	//br: (int) abbreviation for betround
	//br = gws(userchair,"br");
	//betposition: (int) your bet position (1=sblind,2=bblind,...,nplayersdealt=dealer); betposition will change as players fold in front of you.
	//betposition = gws(userchair,"betposition");
	//dealposition: (int) your deal position (1=sblind,2=bblind ... nplayersdealt=dealer); dealposition will not change as players fold

	//originaldealposition: (int) a memory of deal position which retains its value through the hand, even if the user folds
	//originaldealposition = gws(userchair,"originaldealposition");
	//callposition: (int) your numbered offset from the raising player (who is 0)
	//callposition = gws(userchair,"callposition");
	//seatposition: (int) your seat position relative to the dealer
	//seatposition = gws(userchair,"seatposition");
	//dealpositionrais: (int) the deal position of the raising player (1-10)
	//dealpositionrais = gws(userchair,"dealpositionrais");
	//betpositionrais: (int) the bet position of the raising player (1-10)
	//betpositionrais = gws(userchair,"betpositionrais");

	////////////////////////////
	//PROBABILITIES
	////////////////////////////

	//prwin: (double) the probability of winning this hand (0.000 - 1.000) 
	//prwin = gws(userchair,"prwin");
	//prlos: (double) the probability of losing this hand (0.000 - 1.000)
	//prlos = gws(userchair,"prlos");
	//prtie: (double) the probability of pushing this hand (0.000 - 1.000).
	//prtie = gws(userchair,"prtie");
	//prwinnow: (double) probability that all opponents have a lower hand right now.
	//prwinnow = gws(userchair,"prwinnow");
	//prlosnow: (double) probability that any opponents have a higher hand right now.
	//prlosnow = gws(userchair,"prlosnow");
	//random: (double) random number between (0.000-1.000). Value is recalculated each time symbol appears in formula.
	//random = gws(userchair,"random");
	//randomhand: (double) random number between (0.000-1.000) for the hand. Value is calculated only once per hand.
	//randomhand = gws(userchair,"randomhand");
	//randomround: (double) random number between (0.000-1.000) for the current round. Value is calculated only once in current round.
	//randomround = gws(userchair,"randomround");
	//randomround1: (double) random number between (0.000-1.000) for round 1. Value is calculated only once in that round.
	//randomround1 = gws(userchair,"randomround1");
	//randomround2: (double) random number between (0.000-1.000) for round 2. Value is calculated only once in that round.
	//randomround2 = gws(userchair,"randomround2");
	//randomround3: (double) random number between (0.000-1.000) for round 3. Value is calculated only once in that round.
	//randomround3 = gws(userchair,"randomround3");
	//randomround4: (double) random number between (0.000-1.000) for round 4. Value is calculated only once in that round.
	//randomround4 = gws(userchair,"randomround4");

	////////////////////////////
	//f$P FORMULA
	////////////////////////////

	//defcon: (double?) defense level is typically used in the f$P formula and determines the number of analyzer opponents (0.000=maxoffense 1.000=maxdefense) (the Formula Editor parameters dialog uses values 0-10)
	//defcon = gws(userchair,"defcon");
	//isdefmode: (bool) true when defcon is at max
	//isdefmode = gws(userchair,"isdefmode");
	//isaggmode: (bool) true when defcon is at min 
	//isaggmode = gws(userchair,"isaggmode");

	////////////////////////////
	//CHIP AMOUNTS
	////////////////////////////

	//balance: (double) your balance
	//balance = gws(userchair,"balance");
	//balance0: (double) specific player/chair balance
	//balance0 = gws(userchair,"balance0");
	//balance1: (double) specific player/chair balance
	//balance1 = gws(userchair,"balance1");
	//balance2: (double) specific player/chair balance
	//balance2 = gws(userchair,"balance2");
	//balance3: (double) specific player/chair balance
	//balance3 = gws(userchair,"balance3");
	//balance4: (double) specific player/chair balance
	//balance4 = gws(userchair,"balance4");
	//balance5: (double) specific player/chair balance
	//balance5 = gws(userchair,"balance5");
	//balance6: (double) specific player/chair balance
	//balance6 = gws(userchair,"balance6");
	//balance7: (double) specific player/chair balance
	//balance7 = gws(userchair,"balance7");
	//balance8: (double) specific player/chair balance
	//balance8 = gws(userchair,"balance8");
	//balance9: (double) specific player/chair balance
	//balance9 = gws(userchair,"balance9");
	//stack0: (double) sorted playersplaying balances from 0=biggest to 9=smallest
	//stack0 = gws(userchair,"stack0");
	//stack1: (double) sorted playersplaying balances from 0=biggest to 9=smallest
	//stack1 = gws(userchair,"stack0");
	//stack2: (double) sorted playersplaying balances from 0=biggest to 9=smallest
	//stack2 = gws(userchair,"stack0");
	//stack3: (double) sorted playersplaying balances from 0=biggest to 9=smallest
	//stack3 = gws(userchair,"stack0");
	//stack4: (double) sorted playersplaying balances from 0=biggest to 9=smallest
	//stack4 = gws(userchair,"stack0");
	//stack5 = (double) sorted playersplaying balances from 0=biggest to 9=smallest
	//stack5 = gws(userchair,"stack0");
	//stack6: (double) sorted playersplaying balances from 0=biggest to 9=smallest
	//stack6 = gws(userchair,"stack0");
	//stack7: (double) sorted playersplaying balances from 0=biggest to 9=smallest
	//stack7 = gws(userchair,"stack0");
	//stack8: (double) sorted playersplaying balances from 0=biggest to 9=smallest
	//stack8 = gws(userchair,"stack0");
	//stack9: (double) sorted playersplaying balances from 0=biggest to 9=smallest
	//stack9 = gws(userchair,"stack0");
	//currentbet: (double) your current amount of chips in play
	//currentbet = gws(userchair,"balance");
	//currentbet0: (double) specific player/chair currentbet
	//currentbet0 = gws(userchair,"currentbet0");
	//currentbet1: (double) specific player/chair currentbet
	//currentbet1 = gws(userchair,"currentbet1");
	//currentbet2: (double) specific player/chair currentbet
	//currentbet2 = gws(userchair,"currentbet2");
	//currentbet3: (double) specific player/chair currentbet
	//currentbet3 = gws(userchair,"currentbet3");
	//currentbet4: (double) specific player/chair currentbet
	//currentbet4 = gws(userchair,"currentbet4");
	//currentbet5: (double) specific player/chair currentbet
	//currentbet5 = gws(userchair,"currentbet5");
	//currentbet6: (double) specific player/chair currentbet
	//currentbet6 = gws(userchair,"currentbet6");
	//currentbet7: (double) specific player/chair currentbet
	//currentbet7 = gws(userchair,"currentbet7");
	//currentbet8: (double) specific player/chair currentbet
	//currentbet8 = gws(userchair,"currentbet8");
	//currentbet9: (double) specific player/chair currentbet
	//currentbet9 = gws(userchair,"currentbet9");
	//call: (double) the amount you need to call
	//call = gws(userchair,"call");
	//bet: (double) the amount of a single initial bet or raise for current round
	//bet = gws(userchair,"bet");
	//bet1: (double) the amount of a single initial bet or raise for round 1
	//bet1 = gws(userchair,"bet1");
	//bet2: (double) the amount of a single initial bet or raise for round 2
	//bet2 = gws(userchair,"bet2");
	//bet3: (double) the amount of a single initial bet or raise for round 3
	//bet3 = gws(userchair,"bet3");
	//bet4: (double) the amount of a single initial bet or raise for round 4
	//bet4 = gws(userchair,"bet4");
	//pot: (double) the total amount of chips in play including player bets

	//potcommon: (double) the total amount of chips in the middle
	//potcommon = gws(userchair,"potcommon");
	//potplayer: (double) the total amount of chips in front of all players

	//callshort: (double) total amount that will be added to the pot if all players call 
	//callshort = gws(userchair,"callshort");
	//raisshort: (double) callshort + bet * nplayersplaying 
	//raisshort = gws(userchair,"raisshort");

	////////////////////////////
	//NUMBER OF BETS
	////////////////////////////

	//nbetstocall: (int?) total number of additional bets required to call.
	//nbetstocall = gws(userchair,"nbetstocall");
	//nbetstorais: (int?) total number of additional bets required to raise.
	//nbetstorais = gws(userchair,"nbetstorais");
	//ncurrentbets: (int?) total number of bets currently in front of you.
	//ncurrentbets = gws(userchair,"ncurrentbets");
	//ncallbets: (int?) total number of bets you would have on the table if you call.

	//nraisbets: (int?) total number of bets you would have on the table if you raise.
	//nraisbets = gws(userchair,"nraisbets");

	////////////////////////////
	//LIST TESTS
	////////////////////////////

	//islist0 - islist999: (bool)	true if your hand is in the numbered (0-999) list
	//islist0 - islist999 
	//islistcall: (bool)	true if your hand is in list 0
	//islistcall = gws(userchair,"islistcall");
	//islistrais: (bool)	true if your hand is in list 1
	//islistrais = gws(userchair,"islistrais");
	//islistalli: (bool)	true if your hand is in list 7
	//islistalli = gws(userchair,"islistalli");
	//isemptylistcall: (bool)	true if the call list is empty
	//isemptylistcall = gws(userchair,"isemptylistcall");
	//isemptylistrais: (bool)	true if the rais list is empty
	//isemptylistrais = gws(userchair,"isemptylistrais");
	//isemptylistalli: (bool)	true if the alli list is empty
	//isemptylistalli = gws(userchair,"isemptylistalli");
	//nlistmax: (int) highest list number in which your hand is listed
	//nlistmax = gws(userchair,"nlistmax");
	//nlistmin: (int) lowest list number in which your hand is listed
	//nlistmin = gws(userchair,"nlistmin");

	////////////////////////////
	//POKER VALUES
	////////////////////////////

	//pokerval: (?) absolute poker value for your 5 card hand
	//pokerval = gws(userchair,"pokerval");
	//pokervalplayer: (?) absolute poker value for your 2 card pocket hand only
	//pokervalplayer = gws(userchair,"pokervalplayer");
	//pokervalcommon: (?) absolute poker value for the common cards
	//pokervalcommon = gws(userchair,"pokervalcommon");
	//pcbits: (?) bit list of where your pocket cards are used in your 5 card hand
	//pcbits = gws(userchair,"pcbits");
	//npcbits: (?) number (0-2) of your pocket cards used in your 5 card hand 
	//npcbits = gws(userchair,"npcbits");

	////////////////////////////
	//POKER VALUES CONSTANTS
	////////////////////////////

	//hicard: (?) 1<< 0 (2 ** 0)
	//hicard 
	//onepair: (?) 1<<24 (2 ** 24)
	//onepair 
	//twopair: (?) 1<<25 (2 ** 25)
	//twopair 
	//threeofakind: (?) 1<<26 (2 ** 26)
	//threeofakind 
	//straight: (?) 1<<27 (2 ** 27)
	//straight 
	//flush: (?) 1<<28 (2 ** 28)
	//flush 
	//fullhouse: (?) 1<<29 (2 ** 29)
	//fullhouse 
	//fourofakind: (?) 1<<30 (2 ** 30)
	//fourofakind 
	//straightflush: (?) 1<<31 (2 ** 31)
	//straightflush 
	//royalflush: (?) 0x800edcba
	//royalflush 
	//fiveofakind: (?) 0xff000000 
	//fiveofakind 

	////////////////////////////
	//HAND TESTS
	////////////////////////////

	//$CCc: (?) references a hand symbol - see the {Hand Symbols} section below for details
	//$CCc 
	//$$AB#: (?) references a card symbol - see the {Card Symbols} section below for details
	//$$AB#
	//ishandup: (bool) true if your hand has gone up a level (i.e. from 1 pair to 2 pair)
	//ishandup 
	//ishandupcommon: (bool) true if common hand has gone up a level (i.e. from 1 pair to 2 pair)
	//ishandupcommon 
	//ishicard: (bool) true when you have hicard hand
	//ishicard 
	//isonepair: (bool) true when you have one pair
	//isonepair 
	//istwopair: (bool) true when you have two pair
	//istwopair 
	//isthreeofakind: (bool) true when you have three of a kind
	//isthreeofakind 
	//isstraight: (bool) true when you have a straight
	//isstraight 
	//isflush: (bool) true when you have a flush
	//isflush 
	//isfullhouse: (bool) true when you have a full house
	//isfullhouse 
	//isfourofakind: (bool) true when you have four of a kind 
	//isfourofakind 
	//isstraightflush: (bool) true when you have a straight flush
	//isstraightflush 
	//isroyalflush: (bool) true when you have a royal flush
	//isroyalflush 
	//isfiveofakind: (bool) true when you have a five of a kind 
	//isfiveofakind 

	////////////////////////////
	//POCKET TESTS
	////////////////////////////

	//ispair: (bool) true when your two dealt pocket cards are rank equal (0-1)
	//ispair 
	//issuited: (bool) true when your two dealt pocket cards are suit equal (0-1)
	//issuited 
	//isconnector: (bool) true when your two dealt pocket cards are rank adjacent (0-1) 
	//isconnector 

	////////////////////////////
	//POCKET / COMMON TESTS
	////////////////////////////

	//ishipair: (bool) true when you have hi pair (0-1)
	//ishipair 
	//islopair: (bool) true when you have lo pair (0-1)
	//islopair 
	//ismidpair: (bool) true when you have mid pair (0-1)
	//ismidpair 
	//ishistraight: (bool) true when you have the highest straight possible
	//ishistraight 
	//ishiflush: (bool) true when you have the highest flush possible 
	//ishiflush 

	////////////////////////////
	//PLAYERS, FRIENDS, OPPONENTS
	////////////////////////////

	//nopponents: (int) f$P function value for the Iterator
	//nopponents 
	//nopponentsmax: (int) maximum allowable value for nopponents (1-22 default=9)
	//nopponentsmax 
	//nplayersseated: (int) number of players seated (including you) (0-10)
	//nplayersseated 
	//nplayersactive: (int) number of players active (including you) (0-10)
	//nplayersactive 
	//nplayersdealt: (int) number of players dealt (including you) (0-10)

	//nplayersplaying: (int) number of players playing (including you) (0-10)
	//nplayersplaying 
	//nplayersblind: (int) number of players blind (including you) (0-10)
	//nplayersblind 
	//nfriendsseated: (int?) 1 if you are seated, 0 otherwise (0-1)
	//nfriendsseated 
	//nfriendsactive: (int?) 1 if you are active, 0 otherwise (0-1)
	//nfriendsactive 
	//nfriendsdealt: (int?) 1 if you are dealt, 0 otherwise (0-1) 
	//nfriendsdealt 
	//nfriendsplaying: (int?) 1 if you are playing, 0 otherwise (0-1)
	//nfriendsplaying 
	//nfriendsblind: (int?) 1 if you are in a blind, 0 otherwise (0-1)
	//nfriendsblind 
	//nopponentsseated: (int) number of opponents seated (not including you) (0-9) 
	//nopponentsseated 
	//nopponentsactive: (int) number of opponents active (not including you) (0-9)
	//nopponentsactive 
	//nopponentsdealt: (int) number of opponents dealt (not including you) (0-9) 

	//nopponentsplaying: (int) number of opponents playing (not including you) (0-9)
	//nopponentsplaying 
	//nopponentsblind: (int) number of opponents blind (not including you) (0-9) 
	//nopponentsblind 
	//nopponentschecking: (int) number of opponents playing with a zero current bet equal to the previous bettor (0-9)
	//nopponentschecking 
	//nopponentscalling: (int) number of opponents playing with a non-zero current bet equal to the previous bettor (0-9) 
	//nopponentscalling 
	//nopponentsraising: (int) number of opponents playing with a current bet greater than the previous bettor (0-9)
	//nopponentsraising 
	//nopponentsbetting: (int) number of opponents playing with a non zero current bet (0-9) 
	//nopponentsbetting 
	//nopponentsfolded: (int) number of opponents that have folded this hand (0-9)
	//nopponentsfolded 
	//nplayerscallshort: (int) number of players that must call to stay in the hand
	//nplayerscallshort 
	//nchairsdealtright: (int) number of chairs dealt before your chair

	//nchairsdealtleft: (int) number of chairs dealt after your chair
	//nchairsdealtleft 
	//playersseatedbits: (?) bits 9-0: 1=seated 0=unseated

	//playersactivebits: (?) bits 9-0: 1=active 0=inactive 
	//playersactivebits 
	//playersdealtbits: (?) bits 9-0: 1=dealt 0=notdealt
	//playersdealtbits 
	//playersplayingbits: (?) bits 9-0: 1=playing 0=notplaying

	//playersblindbits: (?) bits 9-0: 1=blind 0=notblind
	//playersblindbits 
	//opponentsseatedbits: (?) bits 9-0: 1=seated 0=unseated
	//opponentsseatedbits 
	//opponentsactivebits: (?) bits 9-0: 1=active 0=inactive
	//opponentsactivebits 
	//opponentsdealtbits: (?) bits 9-0: 1=dealt 0=notdealt
	//opponentsdealtbits 
	//opponentsplayingbits: (?) bits 9-0: 1=playing 0=notplaying

	//opponentsblindbits: (?) bits 9-0: 1=blind 0=notblind
	//opponentsblindbits 
	//friendsseatedbits: (?) bits 9-0: 1=seated 0=unseated, you only
	//friendsseatedbits 
	//friendsactivebits: (?) bits 9-0: 1=active 0=inactive, you only
	//friendsactivebits 
	//friendsdealtbits: (?) bits 9-0: 1=dealt 0=notdealt, you only
	//friendsdealtbits 
	//friendsplayingbits: (?) bits 9-0: 1=playing 0=notplaying, you only 
	//friendsplayingbits 
	//friendsblindbits: (?) bits 9-0: 1=blind 0=notblind, you only 
	//friendsblindbits 

	////////////////////////////
	//FLAGS
	////////////////////////////

	//fmax: (int) highest numbered flag button pressed 
	//fmax 
	//f0 - f19: (bol) true if flag 0 - flag 19 button is pressed, false otherwise
	//f0 - f19
	//fbits: (?) flag button bits 19-0 - 1=pressed 0=notpressed 
	//fbits 

	////////////////////////////
	//COMMON CARDS
	////////////////////////////

	//ncommoncardspresent: (int) number of common cards present
	//ncommoncardspresent 
	//ncommoncardsknown: (int) number of common cards present
	//ncommoncardsknown
	//nflopc: (int) short for ncommoncardsknown 
	//nflopc 

	////////////////////////////
	//(UN)KNOWN CARDS
	////////////////////////////

	//nouts: (int) the total number of unseen single cards that if dealt to the board might put your hand in the lead. to be counted as an out, the card must be able to bump your level and your new level must be higher than the resulting common level 
	//nouts 
	//ncardsknown: (int) total number of cards you can see (yours and commons)
	//ncardsknown 
	//ncardsunknown: (int) total number of cards you cannot see (deck and opponents) 
	//ncardsunknown 
	//ncardsbetter: (int) total number of single unknown cards that can beat you, e.g. if the board is four suited in hearts, and you have two spades, then ncardsbetter will be at least 9, because of the possible flush
	//ncardsbetter 

	////////////////////////////
	//NHANDS
	////////////////////////////

	//nhands: (int) total possible number of two-card hands using the unseen cards (nhandshi+nhandslo+nhandsti)
	//nhands 
	//nhandshi: (int) number of hands that can beat you in a showdown right now
	//nhandshi 
	//nhandslo: (int) number of hands that you can beat in a showdown right now
	//nhandslo 
	//nhandsti: (int) number of hands that can tie you in a showdown right now 
	//nhandsti 

	////////////////////////////
	//FLUSHES / STRAIGHTS / SETS
	////////////////////////////

	//nsuited: (int) total number of same suited cards you have (1-7)
	//nsuited 
	//nsuitedcommon: (int) total number of same suited cards in the middle (1-5)

	//tsuit: (int) specific card suit for nsuited (1-4)
	//tsuit 
	//tsuitcommon: (int) specific card suit for nsuitedcommon (1-4)
	//tsuitcommon 
	//nranked: (int) total number of same ranked cards you have (1-4)
	//nranked 
	//nrankedcommon: (int) total number of same ranked cards in the middle (1-4)
	//nrankedcommon 
	//trank: (int) specific card rank for nranked (2-14)
	//trank 
	//trankcommon: (int) specific card rank for nrankedcommon (2-14)
	//trankcommon 
	//nstraight: (int) total number of connected cards you have (1-7)
	//nstraight 
	//nstraightcommon: (int) total number of connected common cards (1-5)
	//nstraightcommon 
	//nstraightfill: (int) total number of cards needed to fill a straight (0-5)
	//nstraightfill 
	//nstraightfillcommon: (int) total number of cards needed to fill a common straight (0-5)
	//nstraightfillcommon 
	//nstraightflush: (int) total number of suited connected cards you have (1-7)
	//nstraightflush 
	//nstraightflushcommon: (int) total number of suited connected common cards (1-5)
	//nstraightflushcommon 
	//nstraightflushfill: (int) total number of cards needed to fill a straightflush (0-5)
	//nstraightflushfill 
	//nstraightflushfillcommon: (int) total number of cards needed to fill a common straightflush (0-5) 
	//nstraightflushfillcommon 

	////////////////////////////
	//RANK BITS (ACES ARE HI AND LO)
	////////////////////////////

	//rankbits: (?) bit list of card ranks (yours and commons)
	//rankbits 
	//rankbitscommon: (?) bit list of card ranks (commons)
	//rankbitscommon 
	//rankbitsplayer: (?) bit list of card ranks (yours)
	//rankbitsplayer 
	//rankbitspoker: (?) bit list of card ranks (pokerval)
	//rankbitspoker 
	//srankbits: (?) bit list of suited card ranks (yours and commons tsuit)
	//srankbits 
	//srankbitscommon: (?) bit list of suited card ranks (commons tsuitcommon)
	//srankbitscommon 
	//srankbitsplayer: (?) bit list of suited card ranks (yours tsuit)
	//srankbitsplayer 
	//srankbitspoker: (?) bit list of suited card ranks (pokerval tsuit) 
	//srankbitspoker 

	////////////////////////////
	//RANK HI (ACES ARE HI)
	////////////////////////////

	//rankhi: (int) highest card rank (14-2) (yours and commons)
	//rankhi 
	//rankhicommon: (int) highest card rank (14-2) (commons)
	//rankhicommon 
	//rankhiplayer: (int) highest card rank (14-2) (yours)
	//rankhiplayer 
	//rankhipoker: (int) highest card rank (14-2) (pokerval)
	//rankhipoker 
	//srankhi: (int) highest suited card rank (14-2) (yours and commons tsuit)
	//srankhi 
	//srankhicommon: (int) highest suited card rank (14-2) (commons tsuitcommon)
	//srankhicommon 
	//srankhiplayer: (int) highest suited card rank (14-2) (yours tsuit)
	//srankhiplayer 
	//srankhipoker: (int) highest suited card rank (14-2) (pokerval tsuit) 
	//srankhipoker 

	////////////////////////////
	//RANK LO (ACES ARE HI)
	////////////////////////////

	//ranklo: (int) lowest card rank (14-2) (yours and commons)
	//ranklo 
	//ranklocommon: (int) lowest card rank (14-2) (commons)
	//ranklocommon 
	//rankloplayer: (int) lowest card rank (14-2) (yours)
	//rankloplayer 
	//ranklopoker: (int) lowest card rank (14-2) (pokerval)
	//ranklopoker 
	//sranklo: (int) lowest suited card rank (14-2) (yours and commons tsuit)
	//sranklo 
	//sranklocommon: (int) lowest suited card rank (14-2) (commons tsuitcommon)
	//sranklocommon 
	//srankloplayer: (int) lowest suited card rank (14-2) (yours tsuit)
	//srankloplayer 
	//sranklopoker: (int) lowest suited card rank (14-2) (pokerval tsuit) 
	//sranklopoker 

	////////////////////////////
	//TIME
	////////////////////////////

	//elapsed: (int) time in seconds since sitting down

	//elapsedhand: (int) time in seconds since end of previous hand
	//elapsedhand 
	//elapsedauto: (int) time in seconds since autoplayer took action

	//elapsedtoday: (int) time in seconds since midnight GMT
	//elapsedtoday 
	//elapsed1970: (int) time in seconds since 1970-01-01 00:00:00 GMT (Thursday)
	//elapsed1970 
	//clocks: (int) number of CPU clocks since the last screen scrape
	//clocks 
	//nclockspersecond: (int) number of CPU clocks per second
	//nclockspersecond 
	//ncps: (int) number of CPU clocks per second
	//ncps 

	////////////////////////////
	//AUTOPLAYER
	////////////////////////////
	
	//myturnbits: (?) bits 43210 correspond to buttons KARCF (check alli rais call fold)

	//ismyturn: (int) (myturnbits & 7) (rais or call/chec or fold)

	//issittingin: (bool) true when you are not being dealt out

	//issittingout: (bool) true when you are being dealt out

	//isautopost: (bool) true when you are autoposting
	//isautopost 
	//isfinalanswer: (bool) true when autoplayer preparing to act; false any other time.
	//isfinalanswer 

	////////////////////////////
	//HISTORY
	////////////////////////////

	//nplayersround1: (int) number of players that began betting round 1
	//nplayersround1 
	//nplayersround2: (int) number of players that began betting round 2
	//nplayersround2
	//nplayersround3: (int) number of players that began betting round 3
	//nplayersround3
	//nplayersround4: (int) number of players that began betting round 4
	//nplayersround4
	//nplayersround: (int) number of players that began the current betting round
	//nplayersround 
	//prevaction: (int) record of previously attempted autoplayer action. (-1=fold 0=chec 1=call 2=rais 3=swag 4=alli)
	//prevaction 
	//didchec: (int) the number of times the autoplayer has checked during the current round
	//didchec 
	//didcall: (int) the number of times the autoplayer has called during the current round
	//didcall 
	//didrais: (int) the number of times the autoplayer has raised during the current round 
	//didrais 
	//didswag: (int) the number of times the autoplayer has swag'd during the current round
	//didswag 
	//nbetsround1: (double) the largest number of bets in front of any player during round 1
	//nbetsround1
	//nbetsround2: (double) the largest number of bets in front of any player during round 2
	//nbetsround2
	//nbetsround3: (double) the largest number of bets in front of any player during round 3
	//nbetsround3
	//nbetsround4: (double) the largest number of bets in front of any player during round 4
	//nbetsround4
	//nbetsround: (double) the largest number of bets in front of any player right now
	//nbetsround 
	//didchecround1: (bool) true if userchair checked during round 1
	//didchecround1
	//didchecround2: (bool) true if userchair checked during round 2
	//didchecround2
	//didchecround3: (bool) true if userchair checked during round 3
	//didchecround3
	//didchecround4: (bool) true if userchair checked during round 4
	//didchecround4
	//didcallround1: (bool) true if userchair called during round 1
	//didcallround1
	//didcallround2: (bool) true if userchair called during round 2
	//didcallround2
	//didcallround3: (bool) true if userchair called during round 3
	//didcallround3
	//didcallround4: (bool) true if userchair called during round 4
	//didcallround4
	//didraisround1: (bool) true if userchair raised during round 1
	//OH_VAR(didraisround1);
	//didraisround2: (bool) true if userchair raised during round 2
	//didraisround2
	//didraisround3: (bool) true if userchair raised during round 3
	//didraisround3
	//didraisround4: (bool) true if userchair raised during round 4
	//didraisround4
	//didswaground1: (bool) true if userchair swag'd during round 1 
	//didswaground1
	//didswaground2: (bool) true if userchair swag'd during round 2
	//didswaground2
	//didswaground3: (bool) true if userchair swag'd during round 3
	//didswaground3
	//didswaground4: (bool) true if userchair swag'd during round 4
	//didswaground4

	////////////////////////////
	//RON / RUN
	////////////////////////////

	//ron$royfl: (?) river opponent number : possible royal flush
	//ron$royfl 
	//ron$strfl: (?) river opponent number : possible straight flush
	//ron$strfl 
	//ron$4kind: (?) river opponent number : possible four of a kind
	//ron$4kind 
	//ron$fullh: (?) river opponent number : possible full house
	//ron$fullh 
	//ron$flush: (?) river opponent number : possible flush
	//ron$flush 
	//ron$strai: (?) river opponent number : possible straight
	//ron$strai 
	//ron$3kind: (?) river opponent number : possible three of a kind
	//ron$3kind 
	//ron$2pair: (?) river opponent number : possible two pair
	//ron$2pair 
	//ron$1pair: (?) river opponent number : possible one pair 
	//ron$1pair 
	//ron$hcard: (?) river opponent number : possible high card
	//ron$hcard 
	//ron$total: (?) river opponent number : sum of all possible river endings
	//ron$total 
	//ron$pokervalmax: (?) the maximum possible pokerval for the opponent
	//ron$pokervalmax 
	//ron$prnuts: (?) opponent chances of hitting the nuts on or before the river
	//ron$prnuts 
	//ron$prbest: (?) opponent chances of hitting pokervalmax on or before the river
	//ron$prbest 
	//ron$clocks: (?) total number of cpu clocks used to calculate the ron$ symbols
	//ron$clocks 
	//run$royfl: (?) river user number : possible royal flush
	//run$royfl 
	//run$strfl: (?) river user number : possible straight flush
	//run$strfl 
	//run$4kind: (?) river user number : possible four of a kind
	//run$4kind 
	//run$fullh: (?) river user number : possible full house
	//run$fullh 
	//run$flush: (?) river user number : possible flush
	//run$flush 
	//run$strai: (?) river user number : possible straight
	//run$strai 
	//run$3kind: (?) river user number : possible three of a kind
	//run$3kind 
	//run$2pair: (?) river user number : possible two pair
	//run$2pair 
	//run$1pair: (?) river user number : possible one pair
	//run$1pair 
	//run$hcard: (?) river user number : possible high card
	//run$hcard 
	//run$total: (?) river user number : sum of all possible river endings
	//run$total 
	//run$pokervalmax: (?) the maximum possible pokerval for the user
	//run$pokervalmax 
	//run$prnuts: (?) user chances of hitting the nuts on or before the river
	//run$prnuts 
	//run$prbest: (?) user chances of hitting pokervalmax on or before the river
	//run$prbest 
	//run$clocks: (?) total number of cpu clocks used to calculate the run$ symbols
	//run$clocks 

	////////////////////////////
	//VERSUS
	////////////////////////////

	//vs$nhands: (int) Total possible number of opponent hands
	//vs$nhands 
	//vs$nhandshi: (int) Number of opponent hands that have higher river chances
	//vs$nhandshi 
	//vs$nhandsti: (int) Number of opponent hands that have equal river chances
	//vs$nhandsti 
	//vs$nhandslo: (int) Number of opponent hands that have lower river chances
	//vs$nhandslo 
	//vs$prwin: (double) Probability (0.000 - 1.000) of winning versus all possible opponent hands
	//vs$prwin 
	//vs$prtie: (double) Probability (0.000 - 1.000) of chopping versus all possible opponent hands
	//vs$prtie 
	//vs$prlos: (double) Probability (0.000 - 1.000) of losing versus all possible opponent hands
	//vs$prlos 
	//vs$prwinhi: (double) Probability (0.000 - 1.000) of winning versus higher opponent hands
	//vs$prwinhi 
	//vs$prtiehi: (double) Probability (0.000 - 1.000) of chopping versus higher opponent hands
	//vs$prtiehi 
	//vs$prloshi: (double) Probability (0.000 - 1.000) of losing versus higher opponent hands
	//vs$prloshi 
	//vs$prwinti: (double) Probability (0.000 - 1.000) of winning versus equal opponent hands
	//vs$prwinti 
	//vs$prtieti: (double) Probability (0.000 - 1.000) of chopping versus equal opponent hands
	//vs$prtieti 
	//vs$prlosti: (double) Probability (0.000 - 1.000) of losing versus equal opponent hands
	//vs$prlosti 
	//vs$prwinlo: (double) Probability (0.000 - 1.000) of winning versus lower opponent hands
	//vs$prwinlo 
	//vs$prtielo: (double) Probability (0.000 - 1.000) of chopping versus lower opponent hands
	//vs$prtielo 
	//vs$prloslo: (double) Probability (0.000 - 1.000) of losing versus lower opponent hands
	//vs$prloslo 
	//vs$x$prwin: (double) Probability (0.000 - 1.000) of winnning versus hand list x 
	//vs$x$prwin 
	//vs$x$prlos: (double) Probability (0.000 - 1.000) of losing versus hand list x
	//vs$x$prlos 
	//vs$x$prtie: (double) Probability (0.000 - 1.000) of a tie versus hand list x 
	//vs$x$prtie 

	////////////////////////////
	//ACTION
	////////////////////////////

	//lastraised1: (int) which chair was the last to raise in round 1 
	//lastraised1
	//lastraised2: (int) which chair was the last to raise in round 2
	//lastraised2
	//lastraised3: (int) which chair was the last to raise in round 3
	//lastraised3
	//lastraised4: (int) which chair was the last to raise in round 4
	//lastraised4
	//raisbits1: (int) which chairs raised in round 1

	//raisbits2: (int) which chairs raised in round 2
	//raisbits2
	//raisbits3: (int) which chairs raised in round 3
	//raisbits3
	//raisbits4: (int) which chairs raised in round 4
	//raisbits4
	//callbits1: (int) which chairs called in round 1

	//callbits2: (int) which chairs called in round 2
	//callbits2
	//callbits3: (int) which chairs called in round 3
	//callbits3
	//callbits4: (int) which chairs called in round 4
	//callbits4
	//foldbits1: (int) which chairs folded in round 1
	//foldbits1
	//foldbits2: (int) which chairs folded in round 2
	//foldbits2
	//foldbits3: (int) which chairs folded in round 3
	//foldbits3
	//foldbits4: (int) which chairs folded in round 4
	//foldbits4
	//oppdealt: (?) trailing indicator for nopponentsdealt
	//oppdealt 
	//ac_aggressor: (int) which chair was aggressor (might be from previous round)
	//ac_aggressor 
	//ac_agchair_after: (?) does the aggressor chair act after me? 
	//ac_agchair_after 
	//ac_preflop_pos: (int) preflop position of the userchair (SB=1 BB=2 Early=3 Middle=4 Late=5 Dealer=6)
	//ac_preflop_pos 
	//ac_prefloprais_pos: (int) preflop position of the raiser (SB=1 BB=2 Early=3 Middle=4 Late=5 Dealer=6)
	//ac_prefloprais_pos 
	//ac_postflop_pos: (int) postflop position of the userchair (first=1 early=2 middle=3 late=4 last=5)
	//ac_postflop_pos 
	//ac_pf_bets: (int) 1 for no callers or blinds only, 2 for Called Pot - 1 bet to call, 3 for Raised Back - 1 more bet to call because someone behind you raised after you've already bet/called/raised, 4 for Raised Pot - 2 bets to call, 5 for Reraised Pot - 3+ bets to call 
	//ac_pf_bets 
	//ac_first_into_pot: (bool) returns true if you are first into the pot (first to act or checked to you)
	//ac_first_into_pot 
	//ac_betposx (x=0-9): (int) returns bet position of specified chair 
	//ac_betposx (x=0-9) 
	//ac_dealposx (x=0-9): (int) returns deal position of specified chair
	//ac_dealposx (x=0-9) 

	////////////////////////////
	//MYHAND
	////////////////////////////

	//mh_3straightxy: (bool) (x=1 for wheel, 0 not, y=1 for broadway, 0 not) - returns true if the board has a wheel straight draw or broadway straight draw, given the wheel/broadway parameters 
	//mh_3straightxy 
	//mh_bottomsd: (bool) true if I have a bottom straight draw (if you are contributing a single card to an open-ended straight draw and that card is the smallest, this symbol is true e.g. hole: T2 common: 345K)
	//mh_bottomsd 
	//mh_nsuitedbetter: (int) number of missing suited cards that are higher than my best suited card
	//mh_nsuitedbetter 
	//mh_kickerbetter: (int) number of cards that can beat your kicker
	//mh_kickerbetter 
	//mh_kickerrank: (int) rank of your kicker (returns 0 if kicker is shared [board] and thus useless) 
	//mh_kickerrank 
	//mh_nouts: (int) number of outs (HTC's formula)
	//mh_nouts 
	//mh_str_strflush: (int) 0-5 (5 best) of the relative strength of your straight flush
	//mh_str_strflush 
	//mh_str_quads: (int) 0-5 (5 best) of the relative strength of your four of a kind 
	//mh_str_quads 
	//mh_str_fullhouse: (int) 0-5 (5 best) of the relative strength of your full house
	//mh_str_fullhouse 
	//mh_str_flush: (int) 0-5 (5 best) of the relative strength of your flush
	//mh_str_flush 
	//mh_str_straight: (int) 0-5 (5 best) of the relative strength of your straight
	//mh_str_straight 
	//mh_str_trips: (int) 0-5 (5 best) of the relative strength of your three of a kind
	//mh_str_trips 
	//mh_str_twopair: (int) 0-5 (5 best) of the relative strength of your two pair
	//mh_str_twopair 
	//mh_str_onepair: (int) 0-5 (5 best) of the relative strength of your one pair
	//mh_str_onepair 

	////////////////////////////
	//TABLE STATISTICS
	////////////////////////////

	//floppct: (double) percentage of players seeing the flop for the last y minutes
	//floppct = gws(userchair, "floppct");
	//turnpct: (double) percentage of players seeing the turn for the last y minutes
	//turnpct 
	//riverpct: (double) percentage of players seeing the river for the last y minutes
	//riverpct 
	//avgbetspf: (double) average number of bets preflop for the last y minutes
	//avgbetspf 
	//tablepfr: (double) pfr percentage preflop for the last y minutes 
	//tablepfr 
	//maxbalance: (double) my highest balance during the session
	//maxbalance

	//handsplayed: (int) number of hands played this session 
	//handsplayed 
	//balance_rankx (x=0-9): (double) ranked list of player balances (includes players not currently in hand, and includes currentbet for each player as well). rank0 has highest balance. 
	//balance_rankx (x=0-9) 

	////////////////////////////
	//ICM CALCULATOR
	////////////////////////////

	//icm: (double) my tournament equity before any action is considered (just balances) 
	//icm 
	//icm_fold: (double) my tournament equity if I fold
	//icm_fold 
	//icm_callwin: (double) my tournament equity if I call and win
	//icm_callwin 
	//icm_calllose: (double) my tournament equity if I call and lose 
	//icm_calllose 
	//icm_calltie: (double) my tournament equity if I call and tie
	//icm_calltie 
	//icm_alliwin0 - icm_alliwin9: (double) my tournament equity if I push all-in and win against 0 - 9 callers
	//icm_alliwin0 - icm_alliwin9 
	//icm_allilose1 - icm_allilose9: (double) my tournament equity if I push all-in and lose against 0 - 9 callers 
	//icm_allilose1 - icm_allilose9 


	////////////////////////////
	//Poker Tracker Variables
	////////////////////////////

		////////////////////////////
		//GENERAL
		////////////////////////////

		//dirty_ismanual();
		//dirty_isppro();
		//dirty_site();
		//dirty_isbring();
		//dirty_session();
		//dirty_handnumber();
		//dirty_version();

		////////////////////////////
		//TABLE MAP
		////////////////////////////

		//dirty_sitename$abc();
		//dirty_network$def();
		//dirty_swagdelay();
		//dirty_allidelay();

		////////////////////////////
		//FORMULA FILE
		////////////////////////////

		//dirty_rake();
		//dirty_nit();
		//dirty_bankroll();

		////////////////////////////
		//LIMITS
		////////////////////////////

		//dirty_ante();
		//dirty_lim();
		//dirty_isnl();
		//dirty_ispl();
		//dirty_isfl();
		//dirty_sraiprev();
		//dirty_sraimin();
		//dirty_sraimax();

		////////////////////////////
		//HAND RANK
		////////////////////////////

		//dirty_handrank();

		//dirty_handrank2652();
		//dirty_handrank1326();
		//dirty_handrank1000();
		//dirty_handrankp();

		////////////////////////////
		//CHAIRS
		////////////////////////////

		//dirty_chair();
		//dirty_userchair();

		//dirty_raischair();
		//dirty_chair$abc();
		//dirty_chairbit$abc();

		////////////////////////////
		//ROUNDS / POSITIONS
		////////////////////////////

		//dirty_br();
		//dirty_betposition();

		//dirty_originaldealposition();
		//dirty_callposition();
		//dirty_seatposition();
		//dirty_dealpositionrais();
		//dirty_betpositionrais();

		////////////////////////////
		//PROBABILITIES
		////////////////////////////

		//dirty_prwin();
		//dirty_prlos();
		//dirty_prtie();
		//dirty_prwinnow();
		//dirty_prlosnow();
		//dirty_random();
		//dirty_randomhand();
		//dirty_randomround();
		//dirty_randomround1();
		//dirty_randomround2();
		//dirty_randomround3();
		//dirty_randomround4();

		////////////////////////////
		//f$P FORMULA
		////////////////////////////

		//dirty_defcon();
		//dirty_isdefmode();
		//dirty_isaggmode();

		////////////////////////////
		//CHIP AMOUNTS
		////////////////////////////

		//balance: (double) your balance
		//balance = gws(userchair,"balance");
		//balance0: (double) specific player/chair balance
		//balance0 = gws(userchair,"balance0");
		//balance1: (double) specific player/chair balance
		//balance1 = gws(userchair,"balance1");
		//balance2: (double) specific player/chair balance
		//balance2 = gws(userchair,"balance2");
		//balance3: (double) specific player/chair balance
		//balance3 = gws(userchair,"balance3");
		//balance4: (double) specific player/chair balance
		//balance4 = gws(userchair,"balance4");
		//balance5: (double) specific player/chair balance
		//balance5 = gws(userchair,"balance5");
		//balance6: (double) specific player/chair balance
		//balance6 = gws(userchair,"balance6");
		//balance7: (double) specific player/chair balance
		//balance7 = gws(userchair,"balance7");
		//balance8: (double) specific player/chair balance
		//balance8 = gws(userchair,"balance8");
		//balance9: (double) specific player/chair balance
		//balance9 = gws(userchair,"balance9");
		//stack0: (double) sorted playersplaying balances from 0=biggest to 9=smallest
		//stack0 = gws(userchair,"stack0");
		//stack1: (double) sorted playersplaying balances from 0=biggest to 9=smallest
		//stack1 = gws(userchair,"stack0");
		//stack2: (double) sorted playersplaying balances from 0=biggest to 9=smallest
		//stack2 = gws(userchair,"stack0");
		//stack3: (double) sorted playersplaying balances from 0=biggest to 9=smallest
		//stack3 = gws(userchair,"stack0");
		//stack4: (double) sorted playersplaying balances from 0=biggest to 9=smallest
		//stack4 = gws(userchair,"stack0");
		//stack5 = (double) sorted playersplaying balances from 0=biggest to 9=smallest
		//stack5 = gws(userchair,"stack0");
		//stack6: (double) sorted playersplaying balances from 0=biggest to 9=smallest
		//stack6 = gws(userchair,"stack0");
		//stack7: (double) sorted playersplaying balances from 0=biggest to 9=smallest
		//stack7 = gws(userchair,"stack0");
		//stack8: (double) sorted playersplaying balances from 0=biggest to 9=smallest
		//stack8 = gws(userchair,"stack0");
		//stack9: (double) sorted playersplaying balances from 0=biggest to 9=smallest
		//stack9 = gws(userchair,"stack0");
		//currentbet: (double) your current amount of chips in play
		//currentbet = gws(userchair,"balance");
		//currentbet0: (double) specific player/chair currentbet
		//currentbet0 = gws(userchair,"currentbet0");
		//currentbet1: (double) specific player/chair currentbet
		//currentbet1 = gws(userchair,"currentbet1");
		//currentbet2: (double) specific player/chair currentbet
		//currentbet2 = gws(userchair,"currentbet2");
		//currentbet3: (double) specific player/chair currentbet
		//currentbet3 = gws(userchair,"currentbet3");
		//currentbet4: (double) specific player/chair currentbet
		//currentbet4 = gws(userchair,"currentbet4");
		//currentbet5: (double) specific player/chair currentbet
		//currentbet5 = gws(userchair,"currentbet5");
		//currentbet6: (double) specific player/chair currentbet
		//currentbet6 = gws(userchair,"currentbet6");
		//currentbet7: (double) specific player/chair currentbet
		//currentbet7 = gws(userchair,"currentbet7");
		//currentbet8: (double) specific player/chair currentbet
		//currentbet8 = gws(userchair,"currentbet8");
		//currentbet9: (double) specific player/chair currentbet
		//currentbet9 = gws(userchair,"currentbet9");
		//call: (double) the amount you need to call
		//call = gws(userchair,"call");
		//bet: (double) the amount of a single initial bet or raise for current round
		//bet = gws(userchair,"bet");
		//bet1: (double) the amount of a single initial bet or raise for round 1
		//bet1 = gws(userchair,"bet1");
		//bet2: (double) the amount of a single initial bet or raise for round 2
		//bet2 = gws(userchair,"bet2");
		//bet3: (double) the amount of a single initial bet or raise for round 3
		//bet3 = gws(userchair,"bet3");
		//bet4: (double) the amount of a single initial bet or raise for round 4
		//bet4 = gws(userchair,"bet4");
		//pot: (double) the total amount of chips in play including player bets

		//potcommon: (double) the total amount of chips in the middle
		//potcommon = gws(userchair,"potcommon");
		//potplayer: (double) the total amount of chips in front of all players

		//callshort: (double) total amount that will be added to the pot if all players call 
		//callshort = gws(userchair,"callshort");
		//raisshort: (double) callshort + bet * nplayersplaying 
		//raisshort = gws(userchair,"raisshort");

		////////////////////////////
		//NUMBER OF BETS
		////////////////////////////

		//nbetstocall: (int?) total number of additional bets required to call.
		//nbetstocall = gws(userchair,"nbetstocall");
		//nbetstorais: (int?) total number of additional bets required to raise.
		//nbetstorais = gws(userchair,"nbetstorais");
		//ncurrentbets: (int?) total number of bets currently in front of you.
		//ncurrentbets = gws(userchair,"ncurrentbets");
		//ncallbets: (int?) total number of bets you would have on the table if you call.

		//nraisbets: (int?) total number of bets you would have on the table if you raise.
		//nraisbets = gws(userchair,"nraisbets");

		////////////////////////////
		//LIST TESTS
		////////////////////////////

		//islist0 - islist999: (bool)	true if your hand is in the numbered (0-999) list
		//islist0 - islist999 
		//islistcall: (bool)	true if your hand is in list 0
		//islistcall = gws(userchair,"islistcall");
		//islistrais: (bool)	true if your hand is in list 1
		//islistrais = gws(userchair,"islistrais");
		//islistalli: (bool)	true if your hand is in list 7
		//islistalli = gws(userchair,"islistalli");
		//isemptylistcall: (bool)	true if the call list is empty
		//isemptylistcall = gws(userchair,"isemptylistcall");
		//isemptylistrais: (bool)	true if the rais list is empty
		//isemptylistrais = gws(userchair,"isemptylistrais");
		//isemptylistalli: (bool)	true if the alli list is empty
		//isemptylistalli = gws(userchair,"isemptylistalli");
		//nlistmax: (int) highest list number in which your hand is listed
		//nlistmax = gws(userchair,"nlistmax");
		//nlistmin: (int) lowest list number in which your hand is listed
		//nlistmin = gws(userchair,"nlistmin");

		////////////////////////////
		//POKER VALUES
		////////////////////////////

		//pokerval: (?) absolute poker value for your 5 card hand
		//pokerval = gws(userchair,"pokerval");
		//pokervalplayer: (?) absolute poker value for your 2 card pocket hand only
		//pokervalplayer = gws(userchair,"pokervalplayer");
		//pokervalcommon: (?) absolute poker value for the common cards
		//pokervalcommon = gws(userchair,"pokervalcommon");
		//pcbits: (?) bit list of where your pocket cards are used in your 5 card hand
		//pcbits = gws(userchair,"pcbits");
		//npcbits: (?) number (0-2) of your pocket cards used in your 5 card hand 
		//npcbits = gws(userchair,"npcbits");

		////////////////////////////
		//POKER VALUES CONSTANTS
		////////////////////////////

		//hicard: (?) 1<< 0 (2 ** 0)
		//hicard 
		//onepair: (?) 1<<24 (2 ** 24)
		//onepair 
		//twopair: (?) 1<<25 (2 ** 25)
		//twopair 
		//threeofakind: (?) 1<<26 (2 ** 26)
		//threeofakind 
		//straight: (?) 1<<27 (2 ** 27)
		//straight 
		//flush: (?) 1<<28 (2 ** 28)
		//flush 
		//fullhouse: (?) 1<<29 (2 ** 29)
		//fullhouse 
		//fourofakind: (?) 1<<30 (2 ** 30)
		//fourofakind 
		//straightflush: (?) 1<<31 (2 ** 31)
		//straightflush 
		//royalflush: (?) 0x800edcba
		//royalflush 
		//fiveofakind: (?) 0xff000000 
		//fiveofakind 

		////////////////////////////
		//HAND TESTS
		////////////////////////////

		//$CCc: (?) references a hand symbol - see the {Hand Symbols} section below for details
		//$CCc 
		//$$AB#: (?) references a card symbol - see the {Card Symbols} section below for details
		//$$AB#
		//ishandup: (bool) true if your hand has gone up a level (i.e. from 1 pair to 2 pair)
		//ishandup 
		//ishandupcommon: (bool) true if common hand has gone up a level (i.e. from 1 pair to 2 pair)
		//ishandupcommon 
		//ishicard: (bool) true when you have hicard hand
		//ishicard 
		//isonepair: (bool) true when you have one pair
		//isonepair 
		//istwopair: (bool) true when you have two pair
		//istwopair 
		//isthreeofakind: (bool) true when you have three of a kind
		//isthreeofakind 
		//isstraight: (bool) true when you have a straight
		//isstraight 
		//isflush: (bool) true when you have a flush
		//isflush 
		//isfullhouse: (bool) true when you have a full house
		//isfullhouse 
		//isfourofakind: (bool) true when you have four of a kind 
		//isfourofakind 
		//isstraightflush: (bool) true when you have a straight flush
		//isstraightflush 
		//isroyalflush: (bool) true when you have a royal flush
		//isroyalflush 
		//isfiveofakind: (bool) true when you have a five of a kind 
		//isfiveofakind 

		////////////////////////////
		//POCKET TESTS
		////////////////////////////

		//ispair: (bool) true when your two dealt pocket cards are rank equal (0-1)
		//ispair 
		//issuited: (bool) true when your two dealt pocket cards are suit equal (0-1)
		//issuited 
		//isconnector: (bool) true when your two dealt pocket cards are rank adjacent (0-1) 
		//isconnector 

		////////////////////////////
		//POCKET / COMMON TESTS
		////////////////////////////

		//ishipair: (bool) true when you have hi pair (0-1)
		//ishipair 
		//islopair: (bool) true when you have lo pair (0-1)
		//islopair 
		//ismidpair: (bool) true when you have mid pair (0-1)
		//ismidpair 
		//ishistraight: (bool) true when you have the highest straight possible
		//ishistraight 
		//ishiflush: (bool) true when you have the highest flush possible 
		//ishiflush 

		////////////////////////////
		//PLAYERS, FRIENDS, OPPONENTS
		////////////////////////////

		//nopponents: (int) f$P function value for the Iterator
		//nopponents 
		//nopponentsmax: (int) maximum allowable value for nopponents (1-22 default=9)
		//nopponentsmax 
		//nplayersseated: (int) number of players seated (including you) (0-10)
		//nplayersseated 
		//nplayersactive: (int) number of players active (including you) (0-10)
		//nplayersactive 
		//nplayersdealt: (int) number of players dealt (including you) (0-10)

		//nplayersplaying: (int) number of players playing (including you) (0-10)
		//nplayersplaying 
		//nplayersblind: (int) number of players blind (including you) (0-10)
		//nplayersblind 
		//nfriendsseated: (int?) 1 if you are seated, 0 otherwise (0-1)
		//nfriendsseated 
		//nfriendsactive: (int?) 1 if you are active, 0 otherwise (0-1)
		//nfriendsactive 
		//nfriendsdealt: (int?) 1 if you are dealt, 0 otherwise (0-1) 
		//nfriendsdealt 
		//nfriendsplaying: (int?) 1 if you are playing, 0 otherwise (0-1)
		//nfriendsplaying 
		//nfriendsblind: (int?) 1 if you are in a blind, 0 otherwise (0-1)
		//nfriendsblind 
		//nopponentsseated: (int) number of opponents seated (not including you) (0-9) 
		//nopponentsseated 
		//nopponentsactive: (int) number of opponents active (not including you) (0-9)
		//nopponentsactive 
		//nopponentsdealt: (int) number of opponents dealt (not including you) (0-9) 

		//nopponentsplaying: (int) number of opponents playing (not including you) (0-9)
		//nopponentsplaying 
		//nopponentsblind: (int) number of opponents blind (not including you) (0-9) 
		//nopponentsblind 
		//nopponentschecking: (int) number of opponents playing with a zero current bet equal to the previous bettor (0-9)
		//nopponentschecking 
		//nopponentscalling: (int) number of opponents playing with a non-zero current bet equal to the previous bettor (0-9) 
		//nopponentscalling 
		//nopponentsraising: (int) number of opponents playing with a current bet greater than the previous bettor (0-9)
		//nopponentsraising 
		//nopponentsbetting: (int) number of opponents playing with a non zero current bet (0-9) 
		//nopponentsbetting 
		//nopponentsfolded: (int) number of opponents that have folded this hand (0-9)
		//nopponentsfolded 
		//nplayerscallshort: (int) number of players that must call to stay in the hand
		//nplayerscallshort 
		//nchairsdealtright: (int) number of chairs dealt before your chair

		//nchairsdealtleft: (int) number of chairs dealt after your chair
		//nchairsdealtleft 
		//playersseatedbits: (?) bits 9-0: 1=seated 0=unseated

		//playersactivebits: (?) bits 9-0: 1=active 0=inactive 
		//playersactivebits 
		//playersdealtbits: (?) bits 9-0: 1=dealt 0=notdealt

		//playersdealtbits 
		//playersplayingbits: (?) bits 9-0: 1=playing 0=notplaying

		//playersblindbits: (?) bits 9-0: 1=blind 0=notblind
		//playersblindbits 
		//opponentsseatedbits: (?) bits 9-0: 1=seated 0=unseated
		//opponentsseatedbits 
		//opponentsactivebits: (?) bits 9-0: 1=active 0=inactive
		//opponentsactivebits 
		//opponentsdealtbits: (?) bits 9-0: 1=dealt 0=notdealt
		//opponentsdealtbits 
		//opponentsplayingbits: (?) bits 9-0: 1=playing 0=notplaying

		//opponentsblindbits: (?) bits 9-0: 1=blind 0=notblind
		//opponentsblindbits 
		//friendsseatedbits: (?) bits 9-0: 1=seated 0=unseated, you only
		//friendsseatedbits 
		//friendsactivebits: (?) bits 9-0: 1=active 0=inactive, you only
		//friendsactivebits 
		//friendsdealtbits: (?) bits 9-0: 1=dealt 0=notdealt, you only
		//friendsdealtbits 
		//friendsplayingbits: (?) bits 9-0: 1=playing 0=notplaying, you only 
		//friendsplayingbits 
		//friendsblindbits: (?) bits 9-0: 1=blind 0=notblind, you only 
		//friendsblindbits 

		////////////////////////////
		//FLAGS
		////////////////////////////

		//fmax: (int) highest numbered flag button pressed 
		//fmax 
		//f0 - f19: (bol) true if flag 0 - flag 19 button is pressed, false otherwise
		//f0 - f19
		//fbits: (?) flag button bits 19-0 - 1=pressed 0=notpressed 
		//fbits 

		////////////////////////////
		//COMMON CARDS
		////////////////////////////

		//ncommoncardspresent: (int) number of common cards present
		//ncommoncardspresent 
		//ncommoncardsknown: (int) number of common cards present
		//ncommoncardsknown
		//nflopc: (int) short for ncommoncardsknown 
		//nflopc 

		////////////////////////////
		//(UN)KNOWN CARDS
		////////////////////////////

		//nouts: (int) the total number of unseen single cards that if dealt to the board might put your hand in the lead. to be counted as an out, the card must be able to bump your level and your new level must be higher than the resulting common level 
		//nouts 
		//ncardsknown: (int) total number of cards you can see (yours and commons)
		//ncardsknown 
		//ncardsunknown: (int) total number of cards you cannot see (deck and opponents) 
		//ncardsunknown 
		//ncardsbetter: (int) total number of single unknown cards that can beat you, e.g. if the board is four suited in hearts, and you have two spades, then ncardsbetter will be at least 9, because of the possible flush
		//ncardsbetter 

		////////////////////////////
		//NHANDS
		////////////////////////////

		//nhands: (int) total possible number of two-card hands using the unseen cards (nhandshi+nhandslo+nhandsti)
		//nhands 
		//nhandshi: (int) number of hands that can beat you in a showdown right now
		//nhandshi 
		//nhandslo: (int) number of hands that you can beat in a showdown right now
		//nhandslo 
		//nhandsti: (int) number of hands that can tie you in a showdown right now 
		//nhandsti 

		////////////////////////////
		//FLUSHES / STRAIGHTS / SETS
		////////////////////////////

		//nsuited: (int) total number of same suited cards you have (1-7)
		//nsuited 
		//nsuitedcommon: (int) total number of same suited cards in the middle (1-5)

		//tsuit: (int) specific card suit for nsuited (1-4)
		//tsuit 
		//tsuitcommon: (int) specific card suit for nsuitedcommon (1-4)
		//tsuitcommon 
		//nranked: (int) total number of same ranked cards you have (1-4)
		//nranked 
		//nrankedcommon: (int) total number of same ranked cards in the middle (1-4)
		//nrankedcommon 
		//trank: (int) specific card rank for nranked (2-14)
		//trank 
		//trankcommon: (int) specific card rank for nrankedcommon (2-14)
		//trankcommon 
		//nstraight: (int) total number of connected cards you have (1-7)
		//nstraight 
		//nstraightcommon: (int) total number of connected common cards (1-5)
		//nstraightcommon 
		//nstraightfill: (int) total number of cards needed to fill a straight (0-5)
		//nstraightfill 
		//nstraightfillcommon: (int) total number of cards needed to fill a common straight (0-5)
		//nstraightfillcommon 
		//nstraightflush: (int) total number of suited connected cards you have (1-7)
		//nstraightflush 
		//nstraightflushcommon: (int) total number of suited connected common cards (1-5)
		//nstraightflushcommon 
		//nstraightflushfill: (int) total number of cards needed to fill a straightflush (0-5)
		//nstraightflushfill 
		//nstraightflushfillcommon: (int) total number of cards needed to fill a common straightflush (0-5) 
		//nstraightflushfillcommon 

		////////////////////////////
		//RANK BITS (ACES ARE HI AND LO)
		////////////////////////////

		//rankbits: (?) bit list of card ranks (yours and commons)
		//rankbits 
		//rankbitscommon: (?) bit list of card ranks (commons)
		//rankbitscommon 
		//rankbitsplayer: (?) bit list of card ranks (yours)
		//rankbitsplayer 
		//rankbitspoker: (?) bit list of card ranks (pokerval)
		//rankbitspoker 
		//srankbits: (?) bit list of suited card ranks (yours and commons tsuit)
		//srankbits 
		//srankbitscommon: (?) bit list of suited card ranks (commons tsuitcommon)
		//srankbitscommon 
		//srankbitsplayer: (?) bit list of suited card ranks (yours tsuit)
		//srankbitsplayer 
		//srankbitspoker: (?) bit list of suited card ranks (pokerval tsuit) 
		//srankbitspoker 

		////////////////////////////
		//RANK HI (ACES ARE HI)
		////////////////////////////

		//rankhi: (int) highest card rank (14-2) (yours and commons)
		//rankhi 
		//rankhicommon: (int) highest card rank (14-2) (commons)
		//rankhicommon 
		//rankhiplayer: (int) highest card rank (14-2) (yours)
		//rankhiplayer 
		//rankhipoker: (int) highest card rank (14-2) (pokerval)
		//rankhipoker 
		//srankhi: (int) highest suited card rank (14-2) (yours and commons tsuit)
		//srankhi 
		//srankhicommon: (int) highest suited card rank (14-2) (commons tsuitcommon)
		//srankhicommon 
		//srankhiplayer: (int) highest suited card rank (14-2) (yours tsuit)
		//srankhiplayer 
		//srankhipoker: (int) highest suited card rank (14-2) (pokerval tsuit) 
		//srankhipoker 

		////////////////////////////
		//RANK LO (ACES ARE HI)
		////////////////////////////

		//ranklo: (int) lowest card rank (14-2) (yours and commons)
		//ranklo 
		//ranklocommon: (int) lowest card rank (14-2) (commons)
		//ranklocommon 
		//rankloplayer: (int) lowest card rank (14-2) (yours)
		//rankloplayer 
		//ranklopoker: (int) lowest card rank (14-2) (pokerval)
		//ranklopoker 
		//sranklo: (int) lowest suited card rank (14-2) (yours and commons tsuit)
		//sranklo 
		//sranklocommon: (int) lowest suited card rank (14-2) (commons tsuitcommon)
		//sranklocommon 
		//srankloplayer: (int) lowest suited card rank (14-2) (yours tsuit)
		//srankloplayer 
		//sranklopoker: (int) lowest suited card rank (14-2) (pokerval tsuit) 
		//sranklopoker 

		////////////////////////////
		//TIME
		////////////////////////////

		//dirty_elapsedhand();

		//dirty_elapsedtoday();
		//dirty_elapsed1970();
		//dirty_clocks();
		//dirty_nclockspersecond();
		//dirty_ncps();

		////////////////////////////
		//AUTOPLAYER
		////////////////////////////


		//dirty_isautopost();
		//dirty_isfinalanswer();

		////////////////////////////
		//HISTORY
		////////////////////////////

		//nplayersround1: (int) number of players that began betting round 1
		//nplayersround1 
		//nplayersround2: (int) number of players that began betting round 2
		//nplayersround2
		//nplayersround3: (int) number of players that began betting round 3
		//nplayersround3
		//nplayersround4: (int) number of players that began betting round 4
		//nplayersround4
		//nplayersround: (int) number of players that began the current betting round
		//nplayersround 
		//prevaction: (int) record of previously attempted autoplayer action. (-1=fold 0=chec 1=call 2=rais 3=swag 4=alli)
		//prevaction 
		//didchec: (int) the number of times the autoplayer has checked during the current round
		//didchec 
		//didcall: (int) the number of times the autoplayer has called during the current round
		//didcall 
		//didrais: (int) the number of times the autoplayer has raised during the current round 
		//didrais 
		//didswag: (int) the number of times the autoplayer has swag'd during the current round
		//didswag 
		//nbetsround1: (double) the largest number of bets in front of any player during round 1
		//nbetsround1
		//nbetsround2: (double) the largest number of bets in front of any player during round 2
		//nbetsround2
		//nbetsround3: (double) the largest number of bets in front of any player during round 3
		//nbetsround3
		//nbetsround4: (double) the largest number of bets in front of any player during round 4
		//nbetsround4
		//nbetsround: (double) the largest number of bets in front of any player right now
		//nbetsround 
		//didchecround1: (bool) true if userchair checked during round 1
		//didchecround1
		//didchecround2: (bool) true if userchair checked during round 2
		//didchecround2
		//didchecround3: (bool) true if userchair checked during round 3
		//didchecround3
		//didchecround4: (bool) true if userchair checked during round 4
		//didchecround4
		//didcallround1: (bool) true if userchair called during round 1
		//didcallround1
		//didcallround2: (bool) true if userchair called during round 2
		//didcallround2
		//didcallround3: (bool) true if userchair called during round 3
		//didcallround3
		//didcallround4: (bool) true if userchair called during round 4
		//didcallround4
		//didraisround1: (bool) true if userchair raised during round 1

		//didraisround2: (bool) true if userchair raised during round 2
		//didraisround2
		//didraisround3: (bool) true if userchair raised during round 3
		//didraisround3
		//didraisround4: (bool) true if userchair raised during round 4
		//didraisround4
		//didswaground1: (bool) true if userchair swag'd during round 1 
		//didswaground1
		//didswaground2: (bool) true if userchair swag'd during round 2
		//didswaground2
		//didswaground3: (bool) true if userchair swag'd during round 3
		//didswaground3
		//didswaground4: (bool) true if userchair swag'd during round 4
		//didswaground4

		////////////////////////////
		//RON / RUN
		////////////////////////////

		//ron$royfl: (?) river opponent number : possible royal flush
		//ron$royfl 
		//ron$strfl: (?) river opponent number : possible straight flush
		//ron$strfl 
		//ron$4kind: (?) river opponent number : possible four of a kind
		//ron$4kind 
		//ron$fullh: (?) river opponent number : possible full house
		//ron$fullh 
		//ron$flush: (?) river opponent number : possible flush
		//ron$flush 
		//ron$strai: (?) river opponent number : possible straight
		//ron$strai 
		//ron$3kind: (?) river opponent number : possible three of a kind
		//ron$3kind 
		//ron$2pair: (?) river opponent number : possible two pair
		//ron$2pair 
		//ron$1pair: (?) river opponent number : possible one pair 
		//ron$1pair 
		//ron$hcard: (?) river opponent number : possible high card
		//ron$hcard 
		//ron$total: (?) river opponent number : sum of all possible river endings
		//ron$total 
		//ron$pokervalmax: (?) the maximum possible pokerval for the opponent
		//ron$pokervalmax 
		//ron$prnuts: (?) opponent chances of hitting the nuts on or before the river
		//ron$prnuts 
		//ron$prbest: (?) opponent chances of hitting pokervalmax on or before the river
		//ron$prbest 
		//ron$clocks: (?) total number of cpu clocks used to calculate the ron$ symbols
		//ron$clocks 
		//run$royfl: (?) river user number : possible royal flush
		//run$royfl 
		//run$strfl: (?) river user number : possible straight flush
		//run$strfl 
		//run$4kind: (?) river user number : possible four of a kind
		//run$4kind 
		//run$fullh: (?) river user number : possible full house
		//run$fullh 
		//run$flush: (?) river user number : possible flush
		//run$flush 
		//run$strai: (?) river user number : possible straight
		//run$strai 
		//run$3kind: (?) river user number : possible three of a kind
		//run$3kind 
		//run$2pair: (?) river user number : possible two pair
		//run$2pair 
		//run$1pair: (?) river user number : possible one pair
		//run$1pair 
		//run$hcard: (?) river user number : possible high card
		//run$hcard 
		//run$total: (?) river user number : sum of all possible river endings
		//run$total 
		//run$pokervalmax: (?) the maximum possible pokerval for the user
		//run$pokervalmax 
		//run$prnuts: (?) user chances of hitting the nuts on or before the river
		//run$prnuts 
		//run$prbest: (?) user chances of hitting pokervalmax on or before the river
		//run$prbest 
		//run$clocks: (?) total number of cpu clocks used to calculate the run$ symbols
		//run$clocks 

		////////////////////////////
		//VERSUS
		////////////////////////////

		//vs$nhands: (int) Total possible number of opponent hands
		//vs$nhands 
		//vs$nhandshi: (int) Number of opponent hands that have higher river chances
		//vs$nhandshi 
		//vs$nhandsti: (int) Number of opponent hands that have equal river chances
		//vs$nhandsti 
		//vs$nhandslo: (int) Number of opponent hands that have lower river chances
		//vs$nhandslo 
		//vs$prwin: (double) Probability (0.000 - 1.000) of winning versus all possible opponent hands
		//vs$prwin 
		//vs$prtie: (double) Probability (0.000 - 1.000) of chopping versus all possible opponent hands
		//vs$prtie 
		//vs$prlos: (double) Probability (0.000 - 1.000) of losing versus all possible opponent hands
		//vs$prlos 
		//vs$prwinhi: (double) Probability (0.000 - 1.000) of winning versus higher opponent hands
		//vs$prwinhi 
		//vs$prtiehi: (double) Probability (0.000 - 1.000) of chopping versus higher opponent hands
		//vs$prtiehi 
		//vs$prloshi: (double) Probability (0.000 - 1.000) of losing versus higher opponent hands
		//vs$prloshi 
		//vs$prwinti: (double) Probability (0.000 - 1.000) of winning versus equal opponent hands
		//vs$prwinti 
		//vs$prtieti: (double) Probability (0.000 - 1.000) of chopping versus equal opponent hands
		//vs$prtieti 
		//vs$prlosti: (double) Probability (0.000 - 1.000) of losing versus equal opponent hands
		//vs$prlosti 
		//vs$prwinlo: (double) Probability (0.000 - 1.000) of winning versus lower opponent hands
		//vs$prwinlo 
		//vs$prtielo: (double) Probability (0.000 - 1.000) of chopping versus lower opponent hands
		//vs$prtielo 
		//vs$prloslo: (double) Probability (0.000 - 1.000) of losing versus lower opponent hands
		//vs$prloslo 
		//vs$x$prwin: (double) Probability (0.000 - 1.000) of winnning versus hand list x 
		//vs$x$prwin 
		//vs$x$prlos: (double) Probability (0.000 - 1.000) of losing versus hand list x
		//vs$x$prlos 
		//vs$x$prtie: (double) Probability (0.000 - 1.000) of a tie versus hand list x 
		//vs$x$prtie 

		////////////////////////////
		//ACTION
		////////////////////////////

		//dirty_lastraised1();
		//dirty_lastraised2();
		//dirty_lastraised3();
		//dirty_lastraised4();

		//dirty_raisbits2();
		//dirty_raisbits3();
		//dirty_raisbits4();

		//dirty_callbits2();
		//dirty_callbits3();
		//dirty_callbits4();
		//dirty_foldbits1();
		//dirty_foldbits2();
		//dirty_foldbits3();
		//dirty_foldbits4();
		//dirty_oppdealt();
		//ac_aggressor 
		//ac_agchair_after 
		//ac_preflop_pos 
		//ac_prefloprais_pos 
		//ac_postflop_pos 
		//ac_pf_bets 
		//ac_first_into_pot 
		//ac_betposx (x=0-9) 
		//ac_dealposx (x=0-9) 

		////////////////////////////
		//MYHAND
		////////////////////////////

		//mh_3straightxy: (bool) (x=1 for wheel, 0 not, y=1 for broadway, 0 not) - returns true if the board has a wheel straight draw or broadway straight draw, given the wheel/broadway parameters 
		//mh_3straightxy 
		//mh_bottomsd: (bool) true if I have a bottom straight draw (if you are contributing a single card to an open-ended straight draw and that card is the smallest, this symbol is true e.g. hole: T2 common: 345K)
		//mh_bottomsd 
		//mh_nsuitedbetter: (int) number of missing suited cards that are higher than my best suited card
		//mh_nsuitedbetter 
		//mh_kickerbetter: (int) number of cards that can beat your kicker
		//mh_kickerbetter 
		//mh_kickerrank: (int) rank of your kicker (returns 0 if kicker is shared [board] and thus useless) 
		//mh_kickerrank 
		//mh_nouts: (int) number of outs (HTC's formula)
		//mh_nouts 
		//mh_str_strflush: (int) 0-5 (5 best) of the relative strength of your straight flush
		//mh_str_strflush 
		//mh_str_quads: (int) 0-5 (5 best) of the relative strength of your four of a kind 
		//mh_str_quads 
		//mh_str_fullhouse: (int) 0-5 (5 best) of the relative strength of your full house
		//mh_str_fullhouse 
		//mh_str_flush: (int) 0-5 (5 best) of the relative strength of your flush
		//mh_str_flush 
		//mh_str_straight: (int) 0-5 (5 best) of the relative strength of your straight
		//mh_str_straight 
		//mh_str_trips: (int) 0-5 (5 best) of the relative strength of your three of a kind
		//mh_str_trips 
		//mh_str_twopair: (int) 0-5 (5 best) of the relative strength of your two pair
		//mh_str_twopair 
		//mh_str_onepair: (int) 0-5 (5 best) of the relative strength of your one pair
		//mh_str_onepair 

		////////////////////////////
		//TABLE STATISTICS
		////////////////////////////

		//dirty_floppct();
		//dirty_turnpct();
		//dirty_riverpct();
		//dirty_avgbetspf();
		//dirty_tablepfr();
		//dirty_maxbalance();
		//balance_rankx (x=0-9) 

		////////////////////////////
		//ICM CALCULATOR
		////////////////////////////

		//icm
		//icm_fold
		//icm_callwin
		//icm_calllose
		//icm_calltie
		//icm_alliwin0 - icm_alliwin9 
		//icm_allilose1 - icm_allilose9 