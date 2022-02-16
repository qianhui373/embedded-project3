
#ifndef APPLICATION_H_
#define APPLICATION_H_



#include <HAL/HAL.h>
//////
#include <HAL/Graphics.h>
#include <HAL/Timer.h>
///////

///////////////////////////////////////////////////////////////////////////////
#define TITLE_SCREEN_WAIT   3000  //3 seconds
#define RESULT_SCREEN_WAIT  3000  // 3 seconds

#define NUM_TEST_OPTIONS    3
#define NUM_RANDOM_NUMBERS  5

#define BACK_TO_BEGIN 100//1 second


#define caltime 2000//1 seconds
#define TIME_START 0//sta
#define judgetime 8

#define divisor 12

#define remaina 1
#define remainb 2
#define remainc 3
#define remaind 4
#define remaine 5
#define remainf 6
#define remaing 0

#define pointtime 5000
#define hazardtime 2000
#define dangertime 4000



enum _GameState
{
    TITLE_SCREEN, MENU_SCREEN, HOWTOPLAY_SCREEN, HIGHSCORES_SCREEN, GAME_SCREEN, RESULT_SCREEN
};
typedef enum _GameState GameState;

enum _Cursor
{
    CURSOR_0 = 0, CURSOR_1 = 1, CURSOR_2 = 2,
};
typedef enum _Cursor Cursor;

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////



struct _Application
{
 // Put your application members and FSM state variables here!
    // =========================================================================
    //UART_Baudrate baudChoice;
    bool firstCall;
    bool consumeEnergy;
    bool original;


    //////////////////////////
    GameState state;  // Determines which screen is currently shown
    SWTimer timer;    // General-purpose timer for when screens must disappear
    SWTimer ballTimer;

    SWTimer orbTimer;

    SWTimer pointTimer;
    SWTimer hazardTimer;
    SWTimer dangerTimer;
    SWTimer judgeTimer;

    SWTimer tenSecondTimer;
    // Booleans to determine which colors the user has picked
    bool playgame;
    bool highscores;
    bool howtoplay;

    bool moveToRight;
    bool moveToLeft;

    int cotime;
    int accelx;
    int accely;



    int randomnumber;
    int remainder;
    int remaindere;
    int remaindera;
    int remainderb;
    int remainderc;
    int remainderd;



    int score;
    int life;

    int circlecollide_x[200];
    int circlecollide_y[200];
    int pointcollide_x[200];
    int pointcollide_y[200];
    int hazardcollide_x[200];
    int hazardcollide_y[200];
    int dangercollide_x[200];
    int dangercollide_y[200];

    int circle_counter;
    int point_counter;
    int hazard_counter;
    int danger_counter; // in the construct needs to be = 0

    int pointcount;
    int hazardcount;
    int dangercount;
    // Cursor tracker variable
    Cursor cursor;

    int result;
    int sc1;
    int sc2;
    int sc3;
    int sc4;


        //////////////////////////////////////////////////////////////////////////////

};


typedef struct _Application Application;

// Called only a single time - inside of main(), where the application is constructed

// Called once per super-loop of the main application.


////////////////////////////////////////////////////////
Application Application_construct(HAL* hal_p);
void Application_loop(Application* app_, HAL* hal_p);


void Application_handleTitleScreen(Application* app_, HAL* hal);
void Application_handleMenuScreen(Application* app_, HAL* hal_p);
void Application_handleGameScreen(Application* app_, HAL* hal_p, GFX* gfx_p);
void Application_handleResultScreen(Application* app_, HAL* hal_p);


// Helper functions which clear the screen and draw the text for each state
void Application_showTitleScreen(GFX* gfx_p);
void Application_showMenuScreen(Application* app_p, GFX* gfx_p);
void Application_showGameScreen(Application* app_p, HAL* hal_p, GFX* gfx_p);
void Application_showResultScreen(Application* app_p, HAL* hal_p, GFX* gfx_p);



void Application_updateGameScreen(Application* app_p, GFX* gfx_p);

// Used to reset the internal game variables
void Application_initGameVariables(Application* app_p, HAL* hal_p);

////////////////////////////////the new functions is set here
void Application_handleHowToPlayScreen(Application* app_p, HAL* hal_p);
void Application_handleHighScoresScreen(Application* app_p, HAL* hal_p);

void Application_showHowToPlayScreen(Application* app_p, GFX* gfx_p);
void Application_showHighScoresScreen(Application* app_, GFX* gfx_p);

void Application_updateMenuScreen(Application* app_p, GFX* gfx_p);


void random(Application* app_p, HAL* hal_p, GFX* gfx_p);

void storecircle(Application* app_p, HAL* hal_p, GFX* gfx_p);

void circlecollide(Application* app_p, HAL* hal_p, GFX* gfx_p);



void highscore(Application* app_p, HAL* hal_p);







// Called whenever the UART module needs to be updated
void Application_updateCommunications(Application* app_, HAL* hal_p);

// Interprets an incoming character and echoes back to terminal what kind of
// character was received (number, letter, or other)


// Generic circular increment function
uint32_t CircularIncrement(uint32_t value, uint32_t maximum);


#endif /* APPLICATION_H_ */


