#include "MainGame.h"
void NewRow();
void CheckMatches();
void SetHoverers2(int firsthoverer, int hovertime, int addflags);
void SetHoverers(int firsthoverer, int hovertime, int addflags);
   // Stuff defined in this file:
   //  . the data structures that store the configuration of
   //    the stack of panels
   //  . the main game routine
   //    (rising, timers, falling, cursor movement, swapping, landing)
   //  . the matches-checking routine


int NewDir;
int GameTimeRender;
int P1ScoreRender;
int MrStopState;
int MrStopTimer;
int MrStopAni[25];
   // The stack of panels.
int P1StackPanels[96];
   // Twelve rows of 8 ints each, the first 6 representing
   // the panels on that row.
   // A panel's color can be retrieved using P1Stack[row<<3+col]



   // Each space on the stack of panels has a timer.
int P1StackTimers[96];
   // A panel's timer indicates for how many more frames it will:
   //  . be swapping
   //  . sit in the MATCHED state before being set POPPING
   //  . sit in the POPPING state before actually being POPPED
   //  . sit and be POPPED before disappearing for good
   //  . hover before FALLING
   // depending on which one of these flags is set on the panel.



   // Each space on the stack of panels has flags
   // that define the state it's in.
int P1StackFlags[96];
   
#define   FLAG_SWAPPING         1   // bit 0
#define   FLAG_FROMLEFT         2   // bit 1
   // Bit 0 is set if the panel is swapping.
   // The panel's timer then counts down from 3 to 0,
   // causing the swap to end 3 frames later.
   // The timer is also used to offset the panel's
   // position on the screen.
   //  Bit 1 indicates from which direction the panel
   //  is swapping:
   //   0 - from right
   //   1 - from left
#define   FLAG_DONTSWAP         4   // bit 2
   // In some cases a swapping panel shouldn't be allowed to swap
   // again until it's done with the swapping it's doing already.
   // In these cases bit 2, FLAG_DONTSWAP, should be set.

#define   FLAG_MATCHED          8   // bit 3
   // This flag is set when a panel's matched, then its timer counts
   // down until it reaches 0, at which time it will become:
#define   FLAG_POPPING         16   // bit 4
   // Popping.  All the panels in a match become popping at once,
   // but their timers for remaining in this state will be higher
   // depending on their place in the match (FLAG_MATCHINDEX).
#define   FLAG_POPPED          32   // bit 5
   // This flag indicates that panel has popped, but it's still
   // there until the last panel in the match pops.  When
   // its timer reaches 0 the panel is GONE!

#define   FLAG_HOVERING        64   // bit 6
   // This panel is floating and cannot be touched until
   // its timer reaches 0 and it begins to fall.
#define   FLAG_FALLING        128   // bit 7

#define   FLAG_CHAINING       256   // bit 8
   // A match made with a chain panel is a chain match...
   // Chain panels are de-flagged during the match-checking
   // routine, if they are eligible for a match but didn't make
   // one, and also are not sitting on top of a hovering or
   // swapping panel.  The chain flag must remain set on the panel
   // until it pops.

#define   FLAG_DIMMED         512   // bit 9
   // This panel is on the bottom row and isn't in play.
#define   FLAG_LANDING       1024   // bit 10
   // A panel that landed just recently.  This flag is set
   // so that the timer can be used to index the correct
   // frame to be drawn for the panel (the 'bounce' effect).
   // These next definitions exclude panels in certain states
   // from being acted upon in certain ways.
 // Bits 11-31 are reserved for possible future needs

 #define   EXCLUDE_HOVER       632
    // If a panel is matched, popping, popped, hovering,
    // or falling it won't get set hovering.
 #define   EXCLUDE_MATCH       761
    // If a panel is swapping, matched, popping, popped, hovering,
    // dimmed, or falling it isn't eligible for a match.
 #define   EXCLUDE_SWAP        636  
    // A panel is untouchable when matched, popping, popped, hovering,
    // dimmed, or is a swapping empty space below a panel (DONTSWAP).


   // And finally, each panel has a couple tags.
   // Unlike the timers and flags, they don't need to be cleared
   // when their data is no longer valid, because they should
   // only be checked when the data is surely valid.
int P1StackTags[96];
#define   TAG_COMBOINDEX        127   // bits 0-6
#define   TAG_COMBOSIZE       16256   // bits 7-13
#define   TAG_CHAININDEX 4294950912   // bits 14-31 (hahaha)




   // Stack displacement.
int P1StackDisplacement;
   // This variable indicates how far below the top of the play
   // area the top row of panels actually is.
   // This variable being decremented causes the stack to rise.
   // During the automatic rising routine, if this variable is 0,
   // it's reset to 15, all the panels are moved up one row,
   // and a new row is generated at the bottom.
   // Only when the displacement is 0 are all 12 rows "in play."

   int DoMatchesCheck;
      // if this gets set non-zero a matches-check will occur for this
      // frame.

int P1DangerCol[6];  // set 1 if this column is near the top
int P1DangerTimer;   // decided bounce frame when in danger


int P1DifficultyLV;
#define VEASY  0
#define EASY   1
#define NORMAL 2
#define HARD   3
#define VHARD  4


int P1SpeedLV;   // The player's speed level decides the amount of time
                 // the stack takes to rise automatically
int P1RiseTimer;   // When this value reaches 0, the stack will rise a pixel
int P1RiseLock;   // If the stack is rise locked, it won't rise until it is
                  // unlocked.
int P1HasRisen;   // set once the stack rises once during the game


int P1StopTime;
int P1StopTimeTimer;
int StopTime_Combo[2][5];
int StopTime_Chain[2][5];



int GameTime;
int GameTimeMode;
int GameTimeTimer;
#define TIME_ELAPSED 1
#define TIME_REMAINING 2



int ScoreMode;
#define SCOREMODE_TA      1
#define SCOREMODE_PDP64   2

int P1Score;          // der skore
int P1ChainCounter;   // how high is the current chain?



   // The following variables keep track of stuff:
int bottomrow;   // row number of the bottom row that's "in play"
int PanelsInTopRow;  // boolean, panels in the top row (danger)
int PanelsInSecondRow; // changes music state


int Count_ActivePanels;
int Count_ChainPanels;



   // These change depending on the difficulty and speed levels:
int FRAMECOUNT_HOVER;
int FRAMECOUNT_MATCH;
int FRAMECOUNT_FLASH;
int FRAMECOUNT_POP;
int FRAMECOUNT_RISE;





   // Player input stuff:
int P1ManualRaise;   // set until raising is completed
int P1ManualRaiseYet;  // if not set, no actual raising's been done yet
                       // since manual raise button was pressed
int P1PreventManualRaise;
int P1Swap1;   // attempt to initiate a swap on this frame
int P1Swap2;

int P1CurWaitTime;   // number of ticks to wait before the cursor begins
                     // to move quickly... it's based on P1CurSensitivity
int P1CurTimer;   // number of ticks for which a new direction's been pressed
int P1CurDir;     // the direction pressed
int P1CurRow;  // the row the cursor's on
int P1CurCol;  // the column the left half of the cursor's on

#define DIR_UP    1
#define DIR_DOWN  2
#define DIR_LEFT  3
#define DIR_RIGHT 4

int P1CurMoveAloud;  // this is set if the cursor movement sound should be played


 // score lookup tables
int Score_Combo_PdP64[40];
int Score_Combo_TA[31];
int Score_Chain_TA[14];
int Input_Swap1Pressed;
int Input_Swap2Pressed;
int Input_DirPressed;  // last dir pressed

int P1GameOver;



int Font_NumRed;
int Font_NumBlue;

int P1StackPosX;   // Position of the play area on the screen
int P1StackPosY;

int P1ScoreDisplay;

int P1ScoreDigits[5];

int GameTimeDisplay;
int GameTimeDisplayPosX;
int GameTimeDisplayPosY;
int GameTimeDigits[7];

int Graphics_TIME;

int Graphics_Ready321;

int P1SpeedLVDisplay;
int P1SpeedLVRender;
int P1SpeedLVDigits[3];


int Graphics_level;
int Graphics_MrStop[2];
int Graphics_Difficulty[5];

int Graphics_Panels[9];
int Graphics_Cursor[2];

int bouncetable[13];
int dangerbouncetable[30];

int TimeSlideAni[65];

int CardAni[50];
int Graphics_ComboCards;
int Graphics_ChainCards;
int ComboCardsQueue[50];
int ComboCardsQueueLength;
int ChainCards16Queue[50];
int ChainCards16QueueLength;
int ChainCards21Queue[50];
int ChainCards21QueueLength;



int NumConfettis;
#define MAXCONFETTIS     8

int Confettis[8][5];
#define CONFETTI_TIMER   0
#define CONFETTI_RADIUS  1
#define CONFETTI_ANGLE   2
#define CONFETTI_X       3
#define CONFETTI_Y       4
int ConfettiAni[48];
int ConfettiBuf[6][2];
#define CONFETTI_STARTTIMER   40
#define CONFETTI_STARTRADIUS 150


int Graphics_Controller;
int Graphics_Controller_Up;
int Graphics_Controller_Down;
int Graphics_Controller_Left;
int Graphics_Controller_Right;
int Graphics_Controller_ABXY;
int Graphics_Controller_L;
int Graphics_Controller_R;


void Controls_NewGame()
{
   Input_Swap1Pressed=0;
   Input_Swap2Pressed=0;
   Input_DirPressed=0;
   P1ManualRaise=0;
   P1ManualRaiseYet=0;
}
void SetupGame_Any()
{
   int a;

   P1StackPosX=75;
   P1StackPosY=21;

   switch(P1DifficultyLV)
   {
      case EASY:
         FRAMECOUNT_HOVER=12;
         FRAMECOUNT_MATCH=61;
         FRAMECOUNT_FLASH=16;
         FRAMECOUNT_POP=9;
      case NORMAL:
         FRAMECOUNT_HOVER=9;
         FRAMECOUNT_MATCH=50;
         FRAMECOUNT_FLASH=13;
         FRAMECOUNT_POP=8;
      case HARD:
         FRAMECOUNT_HOVER=6;
         FRAMECOUNT_MATCH=37;
         FRAMECOUNT_FLASH=8;
         FRAMECOUNT_POP=7;
   }

   P1GameOver=0;
   PanelsInTopRow=0;
   P1HasRisen=0;
   Count_ActivePanels=0;
   Count_ChainPanels=0;
   P1ChainCounter=0;

   ComboCardsQueueLength=0;
   ChainCards16QueueLength=0;
   ChainCards21QueueLength=0;
   NumConfettis=0;

   P1Score=0;
   P1ScoreRender=1;

   P1StopTime=0;
   P1StopTimeTimer=0;
   P1SpeedLVRender=1;

   P1DangerTimer=0;
   P1RiseTimer=FRAMECOUNT_RISE;

   for(a=0;a<96;a++)
   {
      P1StackPanels[a]=0;
      P1StackFlags[a]=0;
      P1StackTimers[a]=0;
      P1StackTags[a]=0;
   }

   P1StackDisplacement=0;

   P1CurRow=6;
   P1CurCol=2;
   Controls_NewGame();

}


   // The engine routine.
void PdP()
{
      // The main game routine has five phases:
      //  1. Decrement timers, act on expired ones
      //  2. Move falling panels down a row
      //  3. Do things according to player input
      //  4. Make falling panels land
      //  5. Possibly do a matches-check

      // During these phases the entire StackPanels will be examined
      // several times, from first to last (or last to first).
      // Here are defined the necessary counters for iterating through
      // the StackPanels and StackTimers:
   int row, col;   // used for iterating through the StackPanels
   int panel;      // used when row and col are avoidable
   int counter;    // an extra general-purpose counter

      // other general-purpose things:
   int whatever, something, somethingelse;

   if(P1StopTime)
   {
      P1StopTimeTimer--;
      if(!P1StopTimeTimer)
      {
         P1StopTime--;
         if(P1StopTime) P1StopTimeTimer=60;
      }
   }


   if(P1StackDisplacement) bottomrow=10;
   else bottomrow=11;   // the 12th row (row 11) is only "in play"
                        // when the stack displacement is 0
                        // and there are panels in the top row

      // count the number of panels in the top row (danger)
   PanelsInTopRow=0;   
   for(panel=0;panel<6;panel++)
   {
      if(P1StackPanels[panel])
      {
         PanelsInTopRow=1;
         P1DangerCol[panel]=1;
      }
      else P1DangerCol[panel]=0;
   }
   if(PanelsInTopRow)
   {
      if(!P1StopTime)
      {
         P1DangerTimer--;
            if(P1DangerTimer<0) P1DangerTimer=17;
      }
   }

   PanelsInSecondRow=0;
   for(panel=8;panel<14;panel++)
   {
      if(P1StackPanels[panel])
         PanelsInSecondRow=1;
   }

   if(!P1StackDisplacement && P1HasRisen)
   {
      if(!PanelsInTopRow)
      {
         NewRow();
      }
   }

   if( Count_ActivePanels ) P1RiseLock=1;
   else P1RiseLock=0;


   if(!P1StackDisplacement && PanelsInTopRow && !P1RiseLock && !P1StopTime)
      P1GameOver=1;

      // Phase 0 //////////////////////////////////////////////////////////////
      // Stack automatic rising


   if(P1SpeedLV && !P1ManualRaise)   // only rise if speed LV is non-zero
   {
      if(!P1StopTime && !P1RiseLock)
      {
         P1RiseTimer--;
         if(!P1RiseTimer)   // try to rise
         {
            if(!P1StackDisplacement)
            {
               if(P1HasRisen || PanelsInTopRow)
               {
                  //P1GameOver=1;
               }
               else
               {
                  NewRow();
                  P1StackDisplacement=23;
                  P1HasRisen=1;
               }
            }
            else
            {
               P1StackDisplacement--;
               if(!P1StackDisplacement)
               {
                  P1PreventManualRaise=0;
                  if(PanelsInTopRow)
                  {
                     for(panel=88;panel<94;panel++)
                        if(P1StackFlags[panel] & FLAG_DIMMED)
                           P1StackFlags[panel]-=FLAG_DIMMED;                  
                     bottomrow=11;
                  }
                  else
                  {
                     NewRow();
                  }
               }
            }
            P1RiseTimer=FRAMECOUNT_RISE;
         }
      }
   }



      // Phase 1 . ///////////////////////////////////////////////////////
      //  Falling

   for(row=bottomrow;row>=0;row--)
   {
      panel=row<<3;
      for(col=0;col<6;col++)
      {
         if(P1StackFlags[panel] & FLAG_FALLING)
         {
               // if there's no panel below a falling panel,
               // it must fall one row.
               // I'm gonna assume there's no panel below,
               // because the falling panel should've landed on
               // it during the last frame if there was.
            P1StackPanels[panel+8]=P1StackPanels[panel];
            P1StackFlags[panel+8]=P1StackFlags[panel];
            P1StackTimers[panel+8]=0;
            P1StackPanels[panel]=0;
            P1StackFlags[panel]=0;
               // the timer can be left behind because it should be 0.
               // the tags can be left behind because they're not important
               // until a panel is stuck in position.
         }
         panel++;
      }
   }



      // Phase 2. /////////////////////////////////////////////////////////////
      //  Timer-expiring actions


   for(row=bottomrow;row>=0;row--)
   {
      panel=row<<3;
      for(col=0;col<6;col++)
      {
            // first of all, we do Nothin' if we're not even looking
            // at a space with any flags.
         if(P1StackFlags[panel])
         {
            if(P1StackTimers[panel])
            {
               P1StackTimers[panel]--;
               if(!P1StackTimers[panel])
               {
                  if(P1StackFlags[panel] & FLAG_SWAPPING)
                  {
                        // a swap has completed here.
                     P1StackFlags[panel]-=FLAG_SWAPPING;
                     if(P1StackFlags[panel] & FLAG_DONTSWAP)
                        P1StackFlags[panel]-=FLAG_DONTSWAP;
                     if(P1StackFlags[panel] & FLAG_FROMLEFT)
                     {
                        P1StackFlags[panel]-=FLAG_FROMLEFT;
                        something=1;
                     }  else something=0;
                     // Now there are a few cases where some hovering must
                     // be done.
                     if(P1StackPanels[panel])
                     {
                        if(row!=bottomrow)
                        {
                           if(!P1StackPanels[panel+8])
                           {
                              SetHoverers2(panel,FRAMECOUNT_HOVER,0);
                                 // if there is no panel beneath this panel
                                 // it will begin to hover.
              // CRAZY BUG EMULATION:
              // the space it was swapping from hovers too
                              if(something)
                              {
                                 if(P1StackFlags[panel-1] & FLAG_FALLING)
                                    SetHoverers2(panel-1,FRAMECOUNT_HOVER,0);
                              }
                              else
                              {
                                 if(P1StackFlags[panel+1] & FLAG_FALLING)
                                    SetHoverers(panel+1,FRAMECOUNT_HOVER+1,0);
                              }

                           }
                           else
                           {
                                 // swap may have landed on a hover
                              if(P1StackFlags[panel+8] & FLAG_HOVERING)
                                 SetHoverers2(panel,FRAMECOUNT_HOVER,0);
                           }
                        }
                     }
                     else
                     {
                           // an empty space finished swapping...
                           // panels above it hover
                        SetHoverers(panel-8,FRAMECOUNT_HOVER+1,0);
                     }
                     // swap completed, a matches-check will occur this frame.
                     DoMatchesCheck=1;
                  }
                  else
                  {
                     if(P1StackFlags[panel] & FLAG_HOVERING)
                     {
                           // This panel is no longer hovering.
                           // it will now fall without sitting around
                           // for any longer!
                        P1StackFlags[panel]-=FLAG_HOVERING;
                        if(P1StackPanels[panel+8])
                        {
                           P1StackFlags[panel]=P1StackFlags[panel] | FLAG_LANDING;
                           P1StackTimers[panel]=12;
                           DoMatchesCheck=1;
                        }
                        else
                        {
                           P1StackFlags[panel]+=FLAG_FALLING;
                           P1StackPanels[panel+8]=P1StackPanels[panel];
                           P1StackFlags[panel+8]=P1StackFlags[panel];
                           P1StackTimers[panel+8]=0;
                           P1StackPanels[panel]=0;
                           P1StackFlags[panel]=0;
                        }
                     }
                     else
                     {
                        if(P1StackFlags[panel] & FLAG_LANDING)
                           P1StackFlags[panel]-=FLAG_LANDING;
                        else
                        {
                           if(P1StackFlags[panel] & FLAG_MATCHED)
                           {
                              P1StackFlags[panel]-=FLAG_MATCHED;
                                 // This panel's match just finished the whole
                                 // flashing and looking distressed thing.
                                 // It is given a pop time based on its place
                                 // in the match.
                              P1StackFlags[panel]+=FLAG_POPPING;
                              something=P1StackTags[panel] & TAG_COMBOINDEX;
                              P1StackTimers[panel]=something*FRAMECOUNT_POP;
                           }
                           else
                           {
                              if(P1StackFlags[panel] & FLAG_POPPING)
                              {
                                 P1Score+=10;
                                 P1ScoreRender=1;
                                    // this panel just popped
                                    // Now it's invisible, but sits and waits
                                    // for the last panel in the combo to pop
                                    // before actually being removed.
                                 something=P1StackTags[panel] & TAG_COMBOSIZE;
                                 something=something>>7;
                                 whatever=P1StackTags[panel] & TAG_COMBOINDEX;
                                    // If it is the last panel to pop,
                                    // it should be removed immediately!
                                 if(something==whatever) //size==index
                                 {
                                    P1StackPanels[panel]=0;
                                    if(P1StackFlags[panel] & FLAG_CHAINING)
                                       Count_ChainPanels--;
                                    P1StackFlags[panel]=0;
                                    SetHoverers(panel-8,FRAMECOUNT_HOVER+1,FLAG_CHAINING);
                                 }
                                 else
                                 {
                                    P1StackFlags[panel]-=FLAG_POPPING;
                                    P1StackFlags[panel]+=FLAG_POPPED;
                                    P1StackTimers[panel]
                                       =(something-whatever)*FRAMECOUNT_POP;
                                 }
                                 something=P1StackTags[panel] & TAG_CHAININDEX;
                                 something=something>>14;
                                 if(!something) something=1;
//                                 SFX_Pop_Play[0]=something;
                            //     SFX_Pop_Play[1]=whatever;
                              }
                              else
                              {
                                 if(P1StackFlags[panel] & FLAG_POPPED)
                                 {
                                       // It's time for this panel
                                       // to be gone forever.
                                    P1StackPanels[panel]=0;
                                    if(P1StackFlags[panel] & FLAG_CHAINING)
                                       Count_ChainPanels--;
                                    P1StackFlags[panel]=0;
                                       // Any panels sitting on top of it
                                       // hover and are flagged as CHAINING
                                    SetHoverers(panel-8,FRAMECOUNT_HOVER+1,FLAG_CHAINING);
                                 }
                                 else
                                 {
                                    // what the heck.
                                    // if a timer runs out and the routine can't
                                    // figure out what flag it is, tell brandon.

                                 }
                              }
                           }
                        }
                     }
                  }

                  // the timer-expiring action has completed
               }
            }
         }
         panel++;
      }
   }

      // Phase 3. /////////////////////////////////////////////////////////////
      //  Actions performed according to player input

                                                     // CURSOR MOVEMENT
      P1CurMoveAloud=0;
      something=0; whatever=0;
      if(!P1CurTimer) something=1;
      if(P1CurTimer==P1CurWaitTime) whatever=1;
      if(something || whatever)
      {
         if(P1CurDir==DIR_UP)
         {  if(P1CurRow>0)
            {  P1CurRow--;
               P1CurMoveAloud=1;
            }
         } else
         {  if(P1CurDir==DIR_DOWN)
            {  if(P1CurRow<bottomrow)
               {  P1CurRow++;
                  P1CurMoveAloud=1;
               }
            } else
            {  if(P1CurDir==DIR_LEFT)
               {  if(P1CurCol>0)
                  {  P1CurCol--;
                     P1CurMoveAloud=1;
                  }
               } else
               { if(P1CurDir==DIR_RIGHT)
                 {  if(P1CurCol<4)
                    {  P1CurCol++;
                       P1CurMoveAloud=1;
                    }
                 }
               }
            }
         }
      }
      if(!whatever)
      {
         P1CurTimer++;
//         if(P1CurMoveAloud && something) SFX_P1Cursor_Play=1;
      }

                                                     // SWAPPING
   if(P1Swap1 || P1Swap2)
   {
      panel=(P1CurRow<<3)+P1CurCol;

         // in order for a swap to occur, one of the two panels in
         // the cursor must not be a non-panel.
      if(P1StackPanels[panel] || P1StackPanels[panel+1])
      {
            // also, both spaces must be swappable.
         something=P1StackFlags[panel] & EXCLUDE_SWAP;
         whatever=P1StackFlags[panel+1] & EXCLUDE_SWAP;
         if(!something && !whatever)
         {
            something=0;
            whatever=0;
            if(P1CurRow>0)
            {
               something=P1StackFlags[panel-8] & FLAG_HOVERING;
               whatever=P1StackFlags[panel-7] & FLAG_HOVERING;
            }
            if(!something && !whatever)
            {
               something=P1StackPanels[panel];
               P1StackPanels[panel]=P1StackPanels[panel+1];
               P1StackPanels[panel+1]=something;

               something=P1StackFlags[panel] & FLAG_CHAINING;
               whatever=P1StackFlags[panel+1] & FLAG_CHAINING;
               P1StackFlags[panel]=FLAG_SWAPPING | whatever;
               P1StackFlags[panel+1]=FLAG_SWAPPING | something | FLAG_FROMLEFT;

               P1StackTimers[panel]=3;
               P1StackTimers[panel+1]=3;

//               SFX_Swap_Play=1;

               if(P1CurRow!=bottomrow)
               {
                  something=0;
                  if(!P1StackPanels[panel+8]) something=1;
                  else 
                  { if(P1StackFlags[panel+8] & FLAG_FALLING) something=1; }
                  if(P1StackPanels[panel] && something)
                     P1StackFlags[panel]+=FLAG_DONTSWAP;
                  something=0;
                  if(!P1StackPanels[panel+9]) something=1;
                  else
                  { if(P1StackFlags[panel+9] & FLAG_FALLING) something=1; }
                  if(P1StackPanels[panel+1] && something)
                     P1StackFlags[panel+1]+=FLAG_DONTSWAP;
               }

               if(P1CurRow>0)
               {
                  if(!P1StackPanels[panel])
                  {  if(P1StackPanels[panel-8])
                        P1StackFlags[panel]+=FLAG_DONTSWAP;
                  }
                  if(!P1StackPanels[panel+1])
                  {  if(P1StackPanels[panel-7])
                        P1StackFlags[panel+1]+=FLAG_DONTSWAP;
                  }
               }
            }
         }
      }
      P1Swap1=0;
      P1Swap2=0;
   }


 
                                               // MANUAL STACK RAISING
   if(P1ManualRaise)
   {
      if(!P1RiseLock)
      {
         if(!P1StackDisplacement)
         {
            if(P1HasRisen)
            {
               if(PanelsInTopRow)
               {
                  P1GameOver=1;
               }
            }
            else
            {
               NewRow();
               P1StackDisplacement=23;
               P1HasRisen=1;
            }
         }
         else
         {
            P1HasRisen=1;
            P1StackDisplacement--;
            if(P1StackDisplacement==1)
            {
               P1ManualRaise=0;
               P1RiseTimer=1;
               if(!P1PreventManualRaise){P1Score++;P1ScoreRender=1;}
               P1PreventManualRaise=1;
            }
         }
         if(!P1ManualRaiseYet) P1ManualRaiseYet=1;  //ehhhh
         P1StopTime=0;
         P1StopTimeTimer=0;
      }
      else
       if(!P1ManualRaiseYet) P1ManualRaise=0;
       // if the stack is rise locked when you press the raise button,
       // the raising is cancelled
   }



      // Phase 4. /////////////////////////////////////////////////////////////
      //  Now falling panels will land if they have something to land on that
      //  isn't falling as well.

   for(row=bottomrow;row>=0;row--)
   {
      panel=row<<3;
      for(col=0;col<6;col++)
      {
         if(P1StackFlags[panel] & FLAG_FALLING)
         {
               // if it's on the bottom row, it should surely land
            if(row==bottomrow)
            {
               P1StackFlags[panel]-=FLAG_FALLING;
               P1StackFlags[panel]=P1StackFlags[panel] | FLAG_LANDING;
               P1StackTimers[panel]=12;
               DoMatchesCheck=1;
//               SFX_Land_Play=1;
            }
            else
            {
               if(P1StackPanels[panel+8])
               {
                  // if there's a panel below, this panel's gonna land
                  // unless the panel below is falling.
                  something=P1StackFlags[panel+8] & FLAG_FALLING;
                  if(!something)
                  {
                     P1StackFlags[panel]-=FLAG_FALLING;

                        // if it lands on a hovering panel, it inherits
                        // that panel's hover time.
                     something=P1StackFlags[panel+8] & FLAG_HOVERING;
                     if(something)
                     {
                        SetHoverers(panel,P1StackTimers[panel+8],0);
                     }
                     else
                     {
                        P1StackFlags[panel]=P1StackFlags[panel] | FLAG_LANDING;
                        P1StackTimers[panel]=12;
                     }
                     DoMatchesCheck=1;
//                     SFX_Land_Play=1;
                  }
               }
            }
         }
         panel++;
      }
   }
 
      // Phase 5. /////////////////////////////////////////////////////////////
      // If a swap completed, one or more panels landed, or a new row was
      // generated during this tick, a matches-check is done.
   if(DoMatchesCheck) CheckMatches();


   // if at the end of the routine there are no chain panels, the chain ends.
   if(P1ChainCounter)
   {
      if(!Count_ChainPanels)
      {
         P1ChainCounter=0;

      }
   }

   if(P1Score>99999) P1Score=99999;


   GameTimeTimer--;
   if(!GameTimeTimer)
   {
      GameTimeTimer=60;
      switch(GameTimeMode)
      {
         case TIME_REMAINING:
            GameTime--;
            if(GameTime<0) GameTime=0;

//            if(!GameTime) SFX_Bell_Play=25;
         //   else if(GameTime<15) SFX_Bell_Play=24;

         case TIME_ELAPSED:
            GameTime++;
            if(GameTime>359999) GameTime=359999;
      }
      GameTimeRender=1;
   }

}


   ////////////////////////////////////////////////////////////////////////////
   // The matches-checking routine.

   // This array is used to remember which panels were matched.
int MatchedPanels[96];

void CheckMatches()
{
   int row = 0, col = 0, panel = 0,count = 0,oldpanel = 0;
   int ischain = 0;
   int firstpanelrow = 0, firstpanelcol = 0;
   int comboindex = 0, combosize = 0;
   int something = 0, whatever = 0;

   for(panel=0;panel<96;panel++) MatchedPanels[panel]=0;

                                                 // check vertical matches
   count=0;
   for(col=0;col<6;col++)
   {
      panel=col;
      oldpanel=0;
      for(row=0;row<bottomrow+1;row++)
      {
         something=P1StackFlags[panel] & EXCLUDE_MATCH;
         if(P1StackPanels[panel] && !something)
         {
            if(!count) count=1;
            else
            {
               if((P1StackPanels[panel])==(oldpanel))
               {
                  count++;
                  if(count>2)
                  {
                     if(!MatchedPanels[panel])
                     {
                        MatchedPanels[panel]=1;
                        if(P1StackFlags[panel] & FLAG_CHAINING)
                           ischain=1;
                        combosize++;
                     }
                     if(count==3)
                     {
                        if(!MatchedPanels[panel-8])
                        {
                           MatchedPanels[panel-8]=1;
                           if(P1StackFlags[panel-8] & FLAG_CHAINING)
                              ischain=1;
                           combosize++;
                        }
                        if(!MatchedPanels[panel-16])
                        {
                           MatchedPanels[panel-16]=1;
                           if(P1StackFlags[panel-16] & FLAG_CHAINING)
                              ischain=1;
                           combosize++;
                        }
                     }
                  }
               }
               else // not the same, but matchable
                  count=1;
            }
         }
         else count=0;
         oldpanel=P1StackPanels[panel];
         panel+=8;
      }
   }

                                                 // check horizontal matches
   count=0;
   panel=0;
   for(row=0;row<bottomrow+1;row++)
   {
      oldpanel=0;
      panel=row<<3;
      for(col=0;col<6;col++)
      {
         something=P1StackFlags[panel] & EXCLUDE_MATCH;
         if(P1StackPanels[panel] && !something)
         {
            if(!count) count=1;
            else
            {
               if(P1StackPanels[panel]==oldpanel)
               {
                  count++;
                  if(count>2)
                  {
                     if(!MatchedPanels[panel])
                     {
                        MatchedPanels[panel]=1;
                        if(P1StackFlags[panel] & FLAG_CHAINING)
                           ischain=1;
                        combosize++;
                     }
                     if(count==3)
                     {
                        if(!MatchedPanels[panel-1])
                        {
                           MatchedPanels[panel-1]=1;
                           if(P1StackFlags[panel-1] & FLAG_CHAINING)
                              ischain=1;
                           combosize++;
                        }
                        if(!MatchedPanels[panel-2])
                        {
                           MatchedPanels[panel-2]=1;
                           if(P1StackFlags[panel-2] & FLAG_CHAINING)
                              ischain=1;
                           combosize++;
                        }
                     }
                  }
               }
               else // not the same, but matchable
                  count=1;
            }
         }
         else count=0;
         oldpanel=P1StackPanels[panel];
         panel++;
      }
   }

if(ischain)
{
   if(P1ChainCounter) P1ChainCounter++;
   else P1ChainCounter=2;
}

comboindex=combosize;
for(row=bottomrow;row>=0;row--)
{
   panel=(row<<3)+5;
   for(col=5;col>=0;col--)
   {
      if(MatchedPanels[panel])
      {
         if(P1StackFlags[panel] & FLAG_LANDING)
            P1StackFlags[panel]-=FLAG_LANDING;
         P1StackFlags[panel]+=FLAG_MATCHED;
         P1StackTimers[panel]=FRAMECOUNT_MATCH;
         if(ischain)
         {
            if(!(P1StackFlags[panel] & FLAG_CHAINING))
            {
               P1StackFlags[panel] += FLAG_CHAINING;
               Count_ChainPanels++;
            }
         }
         something=combosize<<7;
         whatever=P1ChainCounter<<14;
         P1StackTags[panel]= whatever | something | comboindex;
         comboindex--;
         if(!comboindex)
         {
            firstpanelcol=col;
            firstpanelrow=row;
         }
      }
      else
      {
         if(P1StackPanels[panel])
         {
               // if a panel wasn't matched but was eligible,
               // we might have to remove its chain flag...!
            something=P1StackFlags[panel]&EXCLUDE_MATCH;
            if(!something)
            {
               if(row!=bottomrow)
               {
                  something=P1StackFlags[panel+8] & FLAG_SWAPPING;
                  if(!something)   
                  {     // no swapping panel below
                        // so this panel loses its chain flag
                     if(P1StackFlags[panel] & FLAG_CHAINING)
                     {
                        P1StackFlags[panel]-=FLAG_CHAINING;
                        Count_ChainPanels--;
                     }
                  }
               }
               else  // a panel landed on the bottom row, so it surely
               {     // loses its chain flag.
                  if(P1StackFlags[panel] & FLAG_CHAINING)
                  {
                     P1StackFlags[panel]-=FLAG_CHAINING;
                     Count_ChainPanels--;
                  }
               }
            }
         }
      }
      panel--;
   }
}

if(combosize)
{
   if(combosize>3)
   {
      switch(ScoreMode)
      {
         case SCOREMODE_TA:
            if(combosize>30) combosize=30;
            P1Score+=Score_Combo_TA[combosize];

         case SCOREMODE_PDP64:
            if(combosize<41) P1Score+=Score_Combo_PdP64[combosize];
            else P1Score+=20400+((combosize-40)*800);
      }

    //  EnqueueComboCard(firstpanelcol,firstpanelrow,combosize<<4);
     // EnqueueConfetti(firstpanelcol<<4+P1StackPosX+4,
     //                 firstpanelrow<<4+P1StackPosY+P1StackDisplacement-9);
      firstpanelrow--; // offset chain cards
   }

   if(ischain)
   {
      something=P1ChainCounter;
      if(ScoreMode==SCOREMODE_TA)
      {
            if(P1ChainCounter>13) something=0;
      }

//      EnqueueChainCard(firstpanelcol,firstpanelrow,something);
     // EnqueueConfetti(firstpanelcol<<4+P1StackPosX+4,
     //                 firstpanelrow<<4+P1StackPosY+P1StackDisplacement-9);
   }

   something=P1ChainCounter;
   switch(ScoreMode)
   {
      case SCOREMODE_TA:
         if(P1ChainCounter>13) something=0;
         P1Score+=Score_Chain_TA[something];
   }

   if(combosize>3 || ischain)
   {
      if(P1StopTime) P1StopTime++;
      else
      {
         if(ischain) P1StopTime+=StopTime_Chain[PanelsInTopRow][P1DifficultyLV];
         else P1StopTime+= StopTime_Combo[PanelsInTopRow][P1DifficultyLV];

         MrStopState=1;
         MrStopTimer=MrStopAni[P1StopTime];
         P1StopTimeTimer=60;
      }
      if(P1StopTime>99) P1StopTime=99;

//      SFX_Buddy_Play=P1Stage;
//      SFX_Land_Play=0;
   }

   P1ManualRaise=0;
   P1ScoreRender=1;
}

}



void SetHoverers(int firsthoverer, int hovertime, int addflags)
{
   int hoverstime = 0, panel = 0, break2 = 0, something = 0, nonpanel = 0;
   panel=firsthoverer;
   if(firsthoverer<0) break2++;
   hoverstime=hovertime;
   while(!break2)
   {
      nonpanel=0;
      if(!P1StackPanels[panel]) nonpanel++;
      something=P1StackFlags[panel] & EXCLUDE_HOVER;
      if(nonpanel || something) break2++;
      else
      {
         if(!(P1StackFlags[panel] & FLAG_SWAPPING))
            {}
         else hoverstime+=P1StackTimers[panel];
         something=P1StackFlags[panel] & FLAG_CHAINING;
         P1StackFlags[panel]=FLAG_HOVERING | addflags | something;
         P1StackTimers[panel]=hoverstime;
         if(!something)
           if(addflags & FLAG_CHAINING) Count_ChainPanels++;
      }
      panel-=8;
      if(panel<0) break2++;
   }
}

void SetHoverers2(int firsthoverer, int hovertime, int addflags)
{
   // this version of the SetHoverers routine is for use during Phase 1&2,
   // when panels above the first should be given an extra tick of hover time.
   // This is because their timers will be decremented once on the same tick
   // they are set, as Phase 1&2 iterates backward through the stack.

   int notfirst = 0;   // if set, the current panel isn't the first one
   int hoverstime = 0, panel = 0, break2 = 0, something = 0, nonpanel = 0;
   panel=firsthoverer;
   if(firsthoverer<0) break2++;
   hoverstime=hovertime;
   while(!break2)
   {
      nonpanel=0;
      if(!P1StackPanels[panel]) nonpanel++;
      something=P1StackFlags[panel] & EXCLUDE_HOVER;
      if(nonpanel || something) break2++;
      else
      {
         if(!(P1StackFlags[panel] & FLAG_SWAPPING))
            {}
         else hoverstime+=P1StackTimers[panel];
         something=P1StackFlags[panel] & FLAG_CHAINING;
         P1StackFlags[panel]=FLAG_HOVERING | addflags | something;
         P1StackTimers[panel]=hoverstime+notfirst;
         if(!something)
           if(addflags & FLAG_CHAINING) Count_ChainPanels++;
         notfirst=1;
      }
      panel-=8;
      if(panel<0) break2++;
   }
}


void NewRow()
{
   int panel= 0;
   int something= 0, somethingelse= 0, anotherthing= 0, whatever= 0;
   int break2 = 0;

                     // move cursor up
                  if(P1CurRow) P1CurRow--;

                     // move panels up
                  for(panel=0;panel<86;panel++)
                  {
                     P1StackPanels[panel]=P1StackPanels[panel+8];
                     P1StackFlags[panel]=P1StackFlags[panel+8];
                     P1StackTimers[panel]=P1StackTimers[panel+8];
                     P1StackTags[panel]=P1StackTags[panel+8];
                  }

                     // put bottom row into play
                  for(panel=80;panel<88;panel++)
                     if(P1StackFlags[panel] & FLAG_DIMMED)
                        P1StackFlags[panel]-=FLAG_DIMMED;                  
 
                     // generate a new row
                  for(panel=88;panel<90;panel++)
                  {
                     break2=0;
                     while(!break2)
                     {
                        P1StackPanels[panel]=rand() % 6 + 1;
                        break2=1;
                        if(P1StackPanels[panel]==P1StackPanels[panel-8])
                           break2=0;
                     }

                     P1StackFlags[panel]=FLAG_DIMMED;
                     P1StackTimers[panel]=0;
                  }
                  for(panel=90;panel<94;panel++)
                  {
                     whatever=0;
                     if(P1StackPanels[panel-1]==P1StackPanels[panel-2])
                        whatever=1;
                     break2=0;
                     while(!break2)
                     {
                        P1StackPanels[panel]=rand() % 6 + 1;
                        something=0;
                        if(whatever)
                        {  if(P1StackPanels[panel]==P1StackPanels[panel-1])
                              something=1;
                        }
                        somethingelse=0;
                        if(P1StackPanels[panel]==P1StackPanels[panel-8])
                           somethingelse=1;
                        break2=1;
                        if(something || somethingelse) break2=0;
                     }

                     P1StackFlags[panel]=FLAG_DIMMED;
                     P1StackTimers[panel]=0;
                  }
                  P1StackDisplacement=24;
                  bottomrow=10;                  

                  DoMatchesCheck=1;
}



void QuietCursorMovement()
{
   int something, whatever;

   if(P1CurTimer)
   {     // the cursor will move if a direction's was just pressed or has been
         // pressed for at least the P1CurWaitTime
      P1CurMoveAloud=0;
      something=0; whatever=0;
      if(P1CurTimer==1) something=1;
      if(P1CurTimer==P1CurWaitTime) whatever=1;
      if(something || whatever)
      {
         if(P1CurDir==DIR_UP)
         {  if(P1CurRow>0) P1CurRow--;
         } else
         {  if(P1CurDir==DIR_DOWN)
            {  if(P1CurRow<bottomrow) P1CurRow++;
            } else
            {  if(P1CurDir==DIR_LEFT)
               {  if(P1CurCol>0) P1CurCol--;
               } else
               { if(P1CurDir==DIR_RIGHT)
                 {  if(P1CurCol<4) P1CurCol++;
                 }
               }
            }
         }
      }
      if(!whatever) P1CurTimer++;      
   }
}


void Init()
{

 P1StackPosX=120;
 P1StackPosY=31;

 ScoreMode = SCOREMODE_TA;

   Score_Combo_PdP64[4] =    30;
   Score_Combo_PdP64[5] =    50;
   Score_Combo_PdP64[6] =   150;
   Score_Combo_PdP64[7] =   190;
   Score_Combo_PdP64[8] =   230;
   Score_Combo_PdP64[9] =   270;
   Score_Combo_PdP64[10]=   310;
   Score_Combo_PdP64[11]=   400;
   Score_Combo_PdP64[12]=   450;
   Score_Combo_PdP64[13]=   500;
   Score_Combo_PdP64[14]=   550;
   Score_Combo_PdP64[15]=   700;
   Score_Combo_PdP64[16]=   760;
   Score_Combo_PdP64[17]=   850;
   Score_Combo_PdP64[18]=   970;
   Score_Combo_PdP64[19]=  1120;
   Score_Combo_PdP64[20]=  1300;
   Score_Combo_PdP64[21]=  1510;
   Score_Combo_PdP64[22]=  1750;
   Score_Combo_PdP64[23]=  2020;
   Score_Combo_PdP64[24]=  2320;
   Score_Combo_PdP64[25]=  2650;
   Score_Combo_PdP64[26]=  3010;
   Score_Combo_PdP64[27]=  3400;
   Score_Combo_PdP64[28]=  3820;
   Score_Combo_PdP64[29]=  4270;
   Score_Combo_PdP64[30]=  4750;
   Score_Combo_PdP64[31]=  5260;
   Score_Combo_PdP64[32]= 15000;
   Score_Combo_PdP64[33]= 15570;
   Score_Combo_PdP64[34]= 16170;
   Score_Combo_PdP64[35]= 16800;
   Score_Combo_PdP64[36]= 17460;
   Score_Combo_PdP64[37]= 18150;
   Score_Combo_PdP64[38]= 18870;
   Score_Combo_PdP64[39]= 19620;

   Score_Combo_TA[4]=      20;
   Score_Combo_TA[5]=      30;
   Score_Combo_TA[6]=      50;
   Score_Combo_TA[7]=      60;
   Score_Combo_TA[8]=      70;
   Score_Combo_TA[9]=      80;
   Score_Combo_TA[10]=    100;
   Score_Combo_TA[11]=    140;
   Score_Combo_TA[12]=    170;
   Score_Combo_TA[13]=    210;
   Score_Combo_TA[14]=    250;
   Score_Combo_TA[15]=    290;
   Score_Combo_TA[16]=    340;
   Score_Combo_TA[17]=    390;
   Score_Combo_TA[18]=    440;
   Score_Combo_TA[19]=    490;
   Score_Combo_TA[20]=    550;
   Score_Combo_TA[21]=    610;
   Score_Combo_TA[22]=    680;
   Score_Combo_TA[23]=    750;
   Score_Combo_TA[24]=    820;
   Score_Combo_TA[25]=    900;
   Score_Combo_TA[26]=    980;
   Score_Combo_TA[27]=   1060;
   Score_Combo_TA[28]=   1150;
   Score_Combo_TA[29]=   1240;
   Score_Combo_TA[30]=   1330;


   Score_Chain_TA[2]=      50;
   Score_Chain_TA[3]=      80;
   Score_Chain_TA[4]=     150;
   Score_Chain_TA[5]=     300;
   Score_Chain_TA[6]=     400;
   Score_Chain_TA[7]=     500;
   Score_Chain_TA[8]=     700;
   Score_Chain_TA[9]=     900;
   Score_Chain_TA[10]=   1100;
   Score_Chain_TA[11]=   1300;
   Score_Chain_TA[12]=   1500;
   Score_Chain_TA[13]=   1800;


   StopTime_Combo[0][VEASY]=3;
   StopTime_Chain[0][VEASY]=7;
   StopTime_Combo[1][VEASY]=13;
   StopTime_Chain[1][VEASY]=13;

   StopTime_Combo[0][EASY]=2;
   StopTime_Chain[0][EASY]=5;
   StopTime_Combo[1][EASY]=10;
   StopTime_Chain[1][EASY]=10;

   StopTime_Combo[0][NORMAL]=2;
   StopTime_Chain[0][NORMAL]=3;
   StopTime_Combo[1][NORMAL]=7;
   StopTime_Chain[1][NORMAL]=7;

   StopTime_Combo[0][HARD]=2;
   StopTime_Chain[0][HARD]=2;
   StopTime_Combo[1][HARD]=4;
   StopTime_Chain[1][HARD]=4;

   StopTime_Combo[0][VHARD]=1;
   StopTime_Chain[0][VHARD]=1;
   StopTime_Combo[1][VHARD]=1;
   StopTime_Chain[1][VHARD]=1;


 P1DifficultyLV=EASY;


 P1SpeedLV=1;
 FRAMECOUNT_RISE=5;
 P1CurWaitTime=25;
 

//   Controls_SetDefaults();
  // Graphics_Init();
  // Sound_Init();

}
void MainGame::DrawFPS(Display *display) {

    // Update FPS
    m_FPSTicks[ m_FPSCounter & 15 ] = PocketPC::GetTickCount();
    if (m_FPSCounter > 15)
    {
        uint32_t totalTime = m_FPSTicks[ m_FPSCounter & 15 ] - m_FPSTicks[ (m_FPSCounter+1) & 15 ];
        if (totalTime == 0) totalTime = 1;
        uint32_t fps  = 16000 / totalTime;
        uint32_t n100 = fps / 100; fps -= n100 * 100;
        uint32_t n10  = fps / 10;  fps -= n10 * 10;
        uint32_t n1   = fps;

        if (n100 > 9) { n100 = 9; n10 = 9; n1 = 9; }
        
        // Display fps
        display->Blit(  0, 0, i_numbers, &Rect( n100 * 8, 0, (n100+1)*8, 11 ) );
        display->Blit(  8, 0, i_numbers, &Rect( n10 * 8, 0, (n10+1)*8, 11 ) );
        display->Blit( 16, 0, i_numbers, &Rect( n1 * 8, 0, (n1+1)*8, 11 ) );
    }
    ++m_FPSCounter;
}
void MainGame::Render_1P(Display *display)
{
   int row=0, col=0, panel=0;
   int drawpanel=0, drawframe=0, drawx=0, drawy=0;
   int something=0, whatever=0;


   Count_ActivePanels=0;
   for(row=0;row<12;row++)
   {
      panel=row<<3;
      for(col=0;col<6;col++)
      {
         whatever=P1StackFlags[panel];
         something=0;
         if(P1StackPanels[panel])
         {
            if(whatever & FLAG_SWAPPING) something=1;
            else
            {  if(whatever & FLAG_MATCHED) something=1;
               else
               {  if(whatever & FLAG_POPPING) something=1;
                  else
                  {  if(whatever & FLAG_POPPED) something=1;
                     else
                     {  if(whatever & FLAG_HOVERING) something=1;
                        else
                           if(whatever & FLAG_FALLING) something=1;
                     }
                  }
               }
            }
         }
         else if(whatever & FLAG_SWAPPING) something=1;
         if(something) Count_ActivePanels++;

         if(P1StackPanels[panel])
         {
            if(!(P1StackFlags[panel] & FLAG_POPPED))
            {
               drawpanel=P1StackPanels[panel];
               drawframe=0;
               drawx=(col<<4)+P1StackPosX;
               drawy=(row<<4)+P1StackPosY+P1StackDisplacement;
               if(P1StackFlags[panel] & FLAG_MATCHED)
               {  if(P1StackTimers[panel] < FRAMECOUNT_FLASH)
                     drawframe=5;
                  else
                  {  if(P1StackTimers[panel] & 1) drawframe=4;
                     else drawframe=0;
                  }
               }
               else
               {  if(P1StackFlags[panel] & FLAG_POPPING)
                     drawframe=5;
                  else
                  {  if(P1StackFlags[panel] & FLAG_LANDING)
                        drawframe=bouncetable[P1StackTimers[panel]];
                     else
                     {  if(P1StackFlags[panel] & FLAG_SWAPPING)
                        { 
                           if(P1StackFlags[panel] & FLAG_FROMLEFT)
                              something=0-(P1StackTimers[panel]<<2);
                           else
                              something=P1StackTimers[panel]<<2;
                           drawx+=something;
                        }
                        else
                        {  if(row<=bottomrow) something=1;
                           else something=0;
                           if(P1DangerCol[col] && something)
                              drawframe=dangerbouncetable[P1DangerTimer];
                           else
                           {
                              if(P1StackFlags[panel] & FLAG_DIMMED)
                                 drawframe=6;
                              else
                                 drawframe=0;
                           }
                        }
                     }
                  }
               }
               drawframe=drawframe<<4;

				drawx += col * 8;
				drawy += row * 8;
			   switch (drawpanel) {
				   case 1:
					   display->Blit(drawx,drawy,i_redblock);
					   break;
				   case 2:
					   display->Blit(drawx,drawy,i_purpleblock);
					   break;
				   case 3:
					   display->Blit(drawx,drawy,i_blueblock);
					   break;
				   case 4:
					   display->Blit(drawx,drawy,i_cyanblock);
					   break;
				   case 5:
					   display->Blit(drawx,drawy,i_greenblock);
					   break;
				   case 6:		   
					   display->Blit(drawx,drawy,i_yellowblock);
					   break;
			   }
			   
			   if (col == 0) {
					TCHAR paused2[200];
					_stprintf(paused2, TEXT("%d"), P1StackPanels[(10<<3)+5]);
					shared->bigfont->DisplayText(display,100,100,paused2,Color(255,0,255));
			   }
              // GrabRegion(drawframe,0,drawframe+15,15,drawx,drawy,
             //             Graphics_Panels[drawpanel],screen);
            }
         }
         panel++;
      }
   }


}
MainGame::MainGame(void)
{
}

MainGame::~MainGame(void)
{
}

void MainGame::GameInit(Display *display)
{	

	wave = 0;
	int x;

		for(x = 0; x <= 360; x++)
		{
			sin_look[x] = sin(DEG_TO_RAD(x));
			cos_look[x] = cos(DEG_TO_RAD(x));
		}

    m_FPSCounter = 0;
	m_lastTime = 0;
	whatRow = -1;
	lastY=6;
	lastX=2;
	nextState= GAME;
	Init();
	

	 SetupGame_Any();
	GameTimeMode = TIME_REMAINING;
	GameTimeTimer=60;
	GameTime = 120;
	GameTimeRender=1;
	Input_Swap1Pressed=0;
	Input_Swap2Pressed=0;
	Input_DirPressed=0;
	P1ManualRaise=0;
	P1ManualRaiseYet=0;
	lock60 = 0;

}

void MainGame::GameEnd()
{
	
}

void MainGame::GameLoop(Display *display)
{

	NewDir = 0;
	if (nextState != GAME) {
		shared->gamestate = nextState;
		nextState= GAME;
	}

	if (shared->trial && shared->trialtime > 1800) {
		// do nothing
	} else {
			/**/
			// Gameplay
			Input();
			if (shared->input->ButtonPressed() == shared->m_keys->vkB) 
				nextState = MAINMENU;
			PdP(); // the engine 
   
	}
	
	// Rendering
	Render(display);
	//DrawFPS(display);
	if (shared->trial && shared->trialtime > 1800) {
		TCHAR paused2[200];
		_stprintf(paused2, TEXT("TRIAL PERIOD ENDED!"));
		shared->bigfont->DisplayText(display,10,10,paused2,Color(255,0,255));
	}

}
void MainGame::Input() {
	/*if (shared->input->getLastCoordinates().x > shared->input->getCoordinates().x) {
		P1CurRow = 5;
		P1CurCol = 3;
		P1Swap1=1;
	}/**/
	/*if (shared->input->StylusIsDown()) {
		P1CurRow = (shared->input->getCoordinates().y - P1StackPosY - P1StackDisplacement) / 24;
		P1CurCol = (shared->input->getCoordinates().x - P1StackPosX) / 24;

		if (P1CurRow > 10)
			P1CurRow= 10;
		if (P1CurRow < 0)
			P1CurRow= 0;
		if (P1CurRow < 0)
			P1CurRow= 0;
	}   /**/
	if (shared->input->StylusIsPressed()) {
		whatRow = (shared->input->getCoordinates().y - P1StackPosY - P1StackDisplacement) / 24;
	}
	if (shared->input->StylusPressed()) {
		lastX = lastY = -10;
	}
	if (shared->input->StylusIsDown()) {
		P1CurRow = (shared->input->getCoordinates().y - P1StackPosY - P1StackDisplacement) / 24;
		P1CurCol = (shared->input->getCoordinates().x - P1StackPosX) / 24;

		if (P1CurRow > 11)
			P1CurRow= 11;
		if (P1CurRow < 0)
			P1CurRow= 0;
		if (P1CurCol > 5)
			P1CurCol = 5;
		if (P1CurCol < 0)
			P1CurCol = 0;

		if (lastX == P1CurCol - 1 && lastY == whatRow) {
			if(!Input_Swap1Pressed) {
				P1CurCol--;
				P1Swap1=1;
				Input_Swap1Pressed=1;
			}
			lastX = P1CurCol+1;
			lastY = P1CurRow;
		}
		else {
			if (lastX == P1CurCol + 1 && lastY == whatRow) {
				if(!Input_Swap1Pressed) {
					P1CurCol;
					P1Swap1=1;
					Input_Swap1Pressed=1;
				}
				lastX = P1CurCol;
				lastY = P1CurRow;
			}
			else {
				Input_Swap1Pressed=0;/**/
			}
		}

		if ((lastX != P1CurCol - 1) && (lastX != P1CurCol + 1)) {
				lastX = P1CurCol;
				lastY = P1CurRow;
		}
	}
		if (shared->input->ButtonIsDown() == shared->m_keys->vkC)
         {
            if(!P1PreventManualRaise)
            {
               P1ManualRaise=1;
               P1ManualRaiseYet=0;
            }
         }

          if(shared->input->ButtonIsDown() == shared->m_keys->vkA)
         {
            if(!Input_Swap1Pressed)
            {  P1Swap1=1;
               Input_Swap1Pressed=1;
            }
         }
          else Input_Swap1Pressed=0;

		NewDir =0;
		if(shared->input->ButtonPressed() == shared->m_keys->vkUp)
		{  
			NewDir=DIR_UP;
		} else {
			if(shared->input->ButtonPressed() == shared->m_keys->vkDown)
			{  
				NewDir=DIR_DOWN;
			} else {
				if(shared->input->ButtonPressed() == shared->m_keys->vkLeft)
				{  
					NewDir=DIR_LEFT;
				} else {
					if(shared->input->ButtonPressed() == shared->m_keys->vkRight)
						NewDir=DIR_RIGHT;
					}
				}
		}
		if(NewDir==P1CurDir)
		{
			if(P1CurTimer!=P1CurWaitTime)
				P1CurTimer++;
			}
			else
			{
				P1CurDir=NewDir;
				P1CurTimer=0;
			}
	
}
void MainGame::Render(Display *display) {
	display->Clear(Color(0,0,0));
	
//	display->Blit(0,0,i_background);
	Render_1P(display);

/**	display->SetBlending(128);
	display->FillRect((P1CurCol<<4)+P1StackPosX+(P1CurCol*8), 
		(P1CurRow<<4)+P1StackPosY+P1StackDisplacement+(P1CurRow*8),
(P1CurCol<<4)+P1StackPosX + 48+(P1CurCol*8), 
(P1CurRow<<4)+P1StackPosY+P1StackDisplacement+24+(P1CurRow*8)
,Color(0,0,0));
	display->SetBlending(255);
	*/ // cursor
	

	RenderText(display);
}


void MainGame::StylusDown( Point stylus )
{
 
}

void MainGame::StylusUp( Point stylus )
{
}

void MainGame::StylusMove( Point stylus )
{

}

void MainGame::ButtonUp(int button) {

}

void MainGame::ButtonDown(int button) {

};

void MainGame::DebugInfo(Display* display) {

	TCHAR buffer[200];

	_stprintf(buffer, TEXT("a %d"), lastY);
	shared->bigfont->DisplayText(display,60,150,buffer,Color(0,255,255));
}
void MainGame::RenderText(Display *display) {

	float y;
	wave += shared->delta_time * 100;
	if (wave >= 360)
		wave = 0;
	y = cos_look[(int)wave] * 20;
	TCHAR paused2[200];
	_stprintf(paused2, TEXT("%f, %d"),  wave, y);
	shared->bigfont->DisplayText(display,10,50+y,paused2,Color(255,0,255));
}	

void MainGame::FinishAnimation() {

}

void MainGame::StartAnimation() {

}