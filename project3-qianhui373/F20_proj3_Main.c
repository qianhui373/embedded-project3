/**
 * Starter code for Project 3. Good luck!
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include <HAL/HAL.h>
#include <HAL/Timer.h>
#include <Application.h>
#include <HAL/Graphics.h>



////////////the following const are for my images
extern const Graphics_Image title_image8BPP_UNCOMP;
extern const Graphics_Image menu_image8BPP_UNCOMP;
extern const Graphics_Image Game_over8BPP_UNCOMP;
extern const Graphics_Image high_score8BPP_UNCOMP;


// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void InitNonBlockingLED()
{
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
}

// Non-blocking check. Whenever Launchpad S1 is pressed, LED1 turns on.
static void PollNonBlockingLED()
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
    if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == 0) {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
    }
}

int main()
{
    WDT_A_holdTimer();

    // Do not remove this line. This is your non-blocking check.
    InitNonBlockingLED();
    InitSystemTiming();

    HAL hal_p = HAL_construct();
    Application app_p = Application_construct(&hal_p);
    Application_showTitleScreen(&hal_p.gfx);
    while (1)
    {
        // Do not remove this line. This is your non-blocking check.
        Application_loop(&app_p, &hal_p);
        HAL_refresh(&hal_p);
        PollNonBlockingLED();
    }
}



Application Application_construct(HAL* hal_p)
{
    // The App_GuessTheColor object to initialize
    Application app_p;

    // Predetermined random numbers for this application. In an actual project,
    // you should probably use some form of noise generator instead, like the
    // noise from your ADC.
    app_p.timer = SWTimer_construct(TITLE_SCREEN_WAIT);
        SWTimer_start(&app_p.timer);

    app_p.ballTimer = SWTimer_construct(BACK_TO_BEGIN);// set the countdown timer to decrease from start by 1 second



    app_p.orbTimer = SWTimer_construct(caltime);

    app_p.cotime = 0;
//////////////////////////////////////////////////////////////
    app_p.pointTimer = SWTimer_construct(pointtime);


    app_p.hazardTimer = SWTimer_construct(hazardtime);


    app_p.dangerTimer = SWTimer_construct(dangertime);

    app_p.judgeTimer = SWTimer_construct(judgetime);

    // Initialization of FSM variables
    app_p.state = TITLE_SCREEN;


    Application_initGameVariables(&app_p, hal_p);

    app_p.cursor = CURSOR_0;
    app_p.accelx = 63;
    app_p.accely = 63;



    app_p.randomnumber = 0;
    app_p.remainder = 0;
    app_p.remaindere = 0;
    app_p.remaindera = 0;
    app_p.remainderb = 0;
    app_p.remainderc = 0;
    app_p.remainderd = 0;




    app_p.score = 0;
    app_p.life = 3;

    app_p.result = 0;
    app_p.sc1 = 0;
    app_p.sc2 = 0;
    app_p.sc3 = 0;
    app_p.sc4 = 0;


    app_p.circle_counter = 0;
    app_p.point_counter = 0;
    app_p.hazard_counter = 0;
    app_p.danger_counter = 0;


    app_p.pointcount = 0;
    app_p.hazardcount = 0;
    app_p.dangercount = 0;

    // Return the completed Application struct to the user
    return app_p;
}

/**
 * The main super-loop function of the application. We place this inside of a
 * single infinite loop in main. In this way, we can model a polling system of
 * FSMs. Every cycle of this loop function, we poll each of the FSMs one time.
 */
void Application_loop(Application* app_p, HAL* hal_p)
{




    switch (app_p->state)
    {
    case TITLE_SCREEN:
        Application_handleTitleScreen(app_p, hal_p);
        break;

    case MENU_SCREEN:
        Application_handleMenuScreen(app_p, hal_p);
        break;

    case HOWTOPLAY_SCREEN:
        Application_handleHowToPlayScreen(app_p, hal_p);
        break;

    case HIGHSCORES_SCREEN:
        Application_handleHighScoresScreen(app_p, hal_p);
        break;

    case GAME_SCREEN:
        Application_handleGameScreen(app_p, hal_p, &hal_p->gfx);
        break;

    case RESULT_SCREEN:
        Application_handleResultScreen(app_p, hal_p);
        break;

        default:
            break;
    }






}


void Application_showTitleScreen(GFX* gfx_p)//show my title screen
{

    GFX_clear(gfx_p);
    Graphics_drawImage((const Graphics_Context *)gfx_p, &title_image8BPP_UNCOMP, 0, 30);//insert a image by using image reformer

    GFX_print(gfx_p, "ECE 2564 Project3", 0, 0);
    GFX_print(gfx_p, "---------------------", 1, 0);
    GFX_print(gfx_p, "By: Qianhui Xie", 2, 0);

}

/**
 * A helper function which resets all the game variables to their unselected
 * states and resets the cursor position.
 */
void Application_initGameVariables(Application* app_p, HAL* hal_p)
{
    // Reset the cursor
    app_p->cursor = CURSOR_0;

    // Deselect each option
    app_p->playgame = false;
    app_p->highscores = false;
    app_p->howtoplay = false;

    app_p->moveToRight = false;
    app_p->moveToLeft = false;

    // Turn off all LEDs - they don't turn on until a random number is generated
    LED_turnOff(&hal_p->boosterpackRed);
    LED_turnOff(&hal_p->boosterpackGreen);
    LED_turnOff(&hal_p->boosterpackBlue);
}

/**
 * Callback function for when the game is in the TITLE_SCREEN state. Used to
 * break down the main App_GuessTheColor_loop() function into smaller
 * sub-functions.
 */
void Application_handleTitleScreen(Application* app_p, HAL* hal_p)
{
    if (SWTimer_expired(&app_p->timer))
    {
        app_p->state = MENU_SCREEN;
        Application_showMenuScreen(app_p, &hal_p->gfx);

    }
}

/**
 * Callback function for when the game is in the INSTRUCTIONS_SCREEN state. Used
 * to break down the main App_GuessTheColor_loop() function into smaller
 * sub-functions.
 */
void Application_handleMenuScreen(Application* app_p, HAL* hal_p)
{
    if (Joystick_isTappedToUp(&hal_p->joystick) && ((int) app_p->cursor) > 0) {////////////tapp the joystick to up to move the cursor
        app_p->cursor = (Cursor) (((int) app_p->cursor - 1) % NUM_TEST_OPTIONS);
        Application_updateMenuScreen(app_p, &hal_p->gfx);
    }
    if (Joystick_isTappedToDown(&hal_p->joystick)) {/////////////////////tap the joystick to down to move the cursor
        app_p->cursor = (Cursor) (((int) app_p->cursor + 1) % NUM_TEST_OPTIONS);
        Application_updateMenuScreen(app_p, &hal_p->gfx);
    }

    // If B1 is pressed, either add a selection to the proper color choice OR
    // transition to the SHOW_RESULT state if the user chooses to end the test.
    if (Joystick_isTappedToDown(&hal_p->joystick))
    {
        switch (app_p->cursor)
        {
        // In the first three choices, we need to re-display the game screen
        // to reflect updated choices.
        // -----------------------------------------------------------------
        case CURSOR_0: // Red choice
            app_p->playgame = true;/////////////at cursor 0, point to cake time option in menu screen
            Application_updateMenuScreen(app_p, &hal_p->gfx);
            break;

        case CURSOR_1: // Green choice
            app_p->highscores = true;//////////////at cursor 1, point to highscores option in menu screen
            Application_updateMenuScreen(app_p, &hal_p->gfx);
            break;

        case CURSOR_2: // Blue choice
            app_p->howtoplay = true;//////////////at cursor 2, point to howtowork option in menu screen
            Application_updateMenuScreen(app_p, &hal_p->gfx);
            break;



            // In the final choice, we must setup a transition to RESULT_SCREEN
            // by starting a timer and calling the proper draw function.
            // -----------------------------------------------------------------

        default:
            break;
        }
    }

    if (Button_isTapped(&hal_p->boosterpackJS) && app_p->cursor == CURSOR_2){//////////////at cursor 2, tap joystick will turn the menu to howtoplay screen
        app_p->state = HIGHSCORES_SCREEN;
        Application_showHighScoresScreen(app_p, &hal_p->gfx);
    }

    if (Button_isTapped(&hal_p->boosterpackJS) && app_p->cursor == CURSOR_1){//////////////at cursor 1, tap joystick will turn the menu to jighscores screen
        app_p->state = HOWTOPLAY_SCREEN;
        Application_showHowToPlayScreen(app_p, &hal_p->gfx);

    }

    if (Button_isTapped(&hal_p->boosterpackJS) && app_p->cursor == CURSOR_0){//////////////at cursor 0, tap joystick will turn the menu to the game screen

        app_p->state = GAME_SCREEN;
        Application_showGameScreen(app_p, hal_p, &hal_p->gfx);
    }

}



void Application_showHowToPlayScreen(Application* app_p, GFX* gfx_p){//howtoplayscreen text
    GFX_clear(gfx_p);

    // Display the text
    GFX_print(gfx_p, "How to Play", 0, 0);
    GFX_print(gfx_p, "---------------------", 1, 0);
    GFX_print(gfx_p, "tilt the board to", 2, 0);
    GFX_print(gfx_p, "move a ball around.", 3, 0);
    GFX_print(gfx_p, "              orbs to", 4, 0);
    GFX_print(gfx_p, "gain life. If you hit", 5, 0);
    GFX_print(gfx_p, "         orb, you'll", 6, 0);
    GFX_print(gfx_p, "lose life. If your", 7, 0);
    GFX_print(gfx_p, "life hits 0 OR if you", 8, 0);
    GFX_print(gfx_p, "touch a RED orb, your", 9, 0);
    GFX_print(gfx_p, "game is over.", 10, 0);


    GFX_print(gfx_p, "Press JS to return...", 13, 0);

    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_GREEN);
    GFX_print(gfx_p, "        GREEN", 4, 0);
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    GFX_print(gfx_p, "Collect", 4, 0);
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_YELLOW);
    GFX_print(gfx_p, "  YELLOW", 6, 0);
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    GFX_print(gfx_p, "a", 6, 0);
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_RED);
    GFX_print(gfx_p, "        RED", 9, 0);
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    GFX_print(gfx_p, "touch a", 9, 0);

}

void Application_showHighScoresScreen(Application* app_, GFX* gfx_p){//hishacore screen text
    GFX_clear(gfx_p);
    Graphics_drawImage((const Graphics_Context *)gfx_p, &high_score8BPP_UNCOMP, 0, 40);
        // Display the text
        GFX_print(gfx_p, "High Scores", 0, 0);
        GFX_print(gfx_p, "---------------------", 1, 0);
        GFX_print(gfx_p, "1:", 2, 0);
        GFX_print(gfx_p, "2:", 3, 0);
        GFX_print(gfx_p, "3:", 4, 0);


        GFX_print(gfx_p, "Press JS to return....", 15, 0);
}

void Application_handleHighScoresScreen(Application* app_p, HAL* hal_p){

    char buffer4[15];
        snprintf(buffer4, 15, "%02d", app_p->sc1);
        GFX_print(&hal_p->gfx, buffer4, 2, 2);

        char buffer5[15];
        snprintf(buffer5, 15, "%02d", app_p->sc2);
        GFX_print(&hal_p->gfx, buffer5, 3, 2);

        char buffer6[15];
        snprintf(buffer6, 15, "%02d", app_p->sc3);
        GFX_print(&hal_p->gfx, buffer6, 4, 2);


    if (Button_isTapped(&hal_p->boosterpackJS)) {//at how to play screen, tap joystick button will turn to menu screen
        app_p->state = MENU_SCREEN;
        Application_showMenuScreen(app_p, &hal_p->gfx);
    }
}

void Application_handleHowToPlayScreen(Application* app_p, HAL* hal_p){
    if (Button_isTapped(&hal_p->boosterpackJS)) {//at how to play screen, tap joystick button will turn to menu screen
        app_p->state = MENU_SCREEN;
        Application_showMenuScreen(app_p, &hal_p->gfx);
    }
}

void Application_updateMenuScreen(Application* app_p, GFX* gfx_p){

    GFX_print(gfx_p, "  ", 2, 3);
    GFX_print(gfx_p, "  ", 3, 3);
    GFX_print(gfx_p, "  ", 4, 3);

    GFX_print(gfx_p, ">", 2 + app_p->cursor, 3);///set the cursor's start location

    // Draw the stars for LED guesses

}


void Application_handleGameScreen(Application* app_p, HAL* hal_p, GFX* gfx_p)
{


    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    char buffer1[15];
    snprintf(buffer1, 15, "%02d", app_p->score);
    GFX_print(&hal_p->gfx, buffer1, 15, 6);

    char buffer2[15];
    snprintf(buffer2, 15, "%02d", app_p->life);
    GFX_print(&hal_p->gfx, buffer2, 15, 18);
    if (SWTimer_expired(&app_p->orbTimer)){//set a timer for the game screen
        app_p->cotime++;

        SWTimer_start(&app_p->orbTimer);
    }

    if (SWTimer_expired(&app_p->ballTimer)){//blue ball move velocity
        Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
        Graphics_fillCircle(&gfx_p->context, app_p->accelx, app_p->accely, 6);
        if (Accel_isMovedToLeft(&hal_p->accel) && app_p->accelx >= 8){//there are three different titl angles with each corrsponding speed
            app_p->accelx = app_p->accelx - 1;//in this tilt angle, the circle will move in speed 1
        }
        if (Accel_isMovedToLefta(&hal_p->accel) && app_p->accelx >= 8){
            app_p->accelx = app_p->accelx - 2;//in this tilt angle, the circle will move in speed 1
        }
        if (Accel_isMovedToLeftb(&hal_p->accel) && app_p->accelx >= 8){
            app_p->accelx = app_p->accelx - 3;
        }
        //////////////////////////////////////////////////////////////////////////////////
        if (Accel_isMovedToRight(&hal_p->accel) && app_p->accelx <= 120){//there are three different titl angles with each corrsponding speed
            app_p->accelx = app_p->accelx + 1;
        }
        if (Accel_isMovedToRighta(&hal_p->accel) && app_p->accelx <= 120){
            app_p->accelx = app_p->accelx + 2;
        }
        if (Accel_isMovedToRightb(&hal_p->accel) && app_p->accelx <= 120){
            app_p->accelx = app_p->accelx + 3;
        }
        ///////////////////////////////////////////////////////////////////////////////////
        if (Accel_isMovedToUp(&hal_p->accel) && app_p->accely >= 20){//there are three different titl angles with each corrsponding speed
            app_p->accely = app_p->accely - 1;
        }

        if (Accel_isMovedToUpa(&hal_p->accel) && app_p->accely >= 20){
            app_p->accely = app_p->accely - 2;
        }

        if (Accel_isMovedToUpb(&hal_p->accel) && app_p->accely >= 20){
            app_p->accely = app_p->accely - 3;
        }


        ///////////////////////////////////////////////////////////////////////
        if (Accel_isMovedToDown(&hal_p->accel) && app_p->accely <=105){//there are three different titl angles with each corrsponding speed
            app_p->accely = app_p->accely + 1;//in this tilt angle, the circle will move in speed 1
        }

        if (Accel_isMovedToDowna(&hal_p->accel) && app_p->accely <=105){
            app_p->accely = app_p->accely + 2;//in this tilt angle, the circle will move in speed 1
        }

        if (Accel_isMovedToDownb(&hal_p->accel) && app_p->accely <=105){
            app_p->accely = app_p->accely + 3;
        }

//        if (Button_isTapped(&hal_p->boosterpackS1)) {
//            Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_PINK);
//            Graphics_drawCircle(&gfx_p->context, app_p->accelx, app_p->accely, 8);
//        }

        Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLUE);
        Graphics_fillCircle(&gfx_p->context, app_p->accelx, app_p->accely, 6);




        SWTimer_start(&app_p->ballTimer);

    }


    random(app_p, hal_p, &hal_p->gfx);
    circlecollide(app_p, hal_p, &hal_p->gfx);





}






void Application_handleResultScreen(Application* app_p, HAL* hal_p)
{
    if (Button_isTapped(&hal_p->boosterpackJS)) {//at how to play screen, tap joystick button will turn to menu screen
        highscore(app_p, hal_p);
        //i need to refresh all the data to start a second turn game.
        app_p->score = 0;
        app_p->life = 3;
        app_p->cotime = 0;

        app_p->randomnumber = 0;
        app_p->remainder = 0;
        app_p->remaindere = 0;
        app_p->remaindera = 0;
        app_p->remainderb = 0;
        app_p->remainderc = 0;
        app_p->remainderd = 0;


        app_p->circle_counter = 0;
        app_p->point_counter = 0;
        app_p->hazard_counter = 0;
        app_p->danger_counter = 0;


        app_p->pointcount = 0;
        app_p->hazardcount = 0;
        app_p->dangercount = 0;

        app_p->state = MENU_SCREEN;
        Application_showMenuScreen(app_p, &hal_p->gfx);
    }
}

/**
 * A helper function which clears the screen and prints the instructions for how
 * to play the game.
 */
void Application_showMenuScreen(Application* app_p, GFX* gfx_p)
{
    // Clear the screen from any old text state
    GFX_clear(gfx_p);
    Graphics_drawImage((const Graphics_Context *)gfx_p, &menu_image8BPP_UNCOMP, 0, 45);

    // Display the text
    GFX_print(gfx_p, "Main Menu", 0, 0);
    GFX_print(gfx_p, "---------------------", 1, 0);
    GFX_print(gfx_p, "Play Game", 2, 5);
    GFX_print(gfx_p, "How to Play", 3, 5);
    GFX_print(gfx_p, "High Scores", 4, 5);

    GFX_print(gfx_p, ">", 2 + app_p->cursor, 3);

}


void Application_showGameScreen(Application* app_p, HAL* hal_p ,GFX* gfx_p)
{
    // Clear the screen from any old text state
    GFX_clear(gfx_p);


        // Display the text
        GFX_print(gfx_p, "Play the Game", 0, 0);
        GFX_print(gfx_p, "---------------------", 1, 0);
        GFX_print(gfx_p, "Score:       Life:", 15, 0);
        GFX_print(gfx_p, "---------------------", 14, 0);




}




void Application_showResultScreen(Application* app_p, HAL* hal_p, GFX* gfx_p)
{
    // Print the splash text
    GFX_clear(&hal_p->gfx);
    Graphics_drawImage((const Graphics_Context *)gfx_p, &Game_over8BPP_UNCOMP, 0, 15);
    Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_WHITE);
    GFX_print(&hal_p->gfx, "Result               ", 0, 0);
    GFX_print(&hal_p->gfx, "---------------------", 1, 0);
    GFX_print(&hal_p->gfx, "Your Score:          ", 2, 0);
    GFX_print(&hal_p->gfx, "Press JS to return...", 15, 0);

    char buffer7[15];
    snprintf(buffer7, 15, "%02d", app_p->score);
    GFX_print(&hal_p->gfx, buffer7, 2, 12);


}


/////////////////////////////

void random(Application* app_p, HAL* hal_p, GFX* gfx_p){
    //this is the randome number function
    if (app_p->cotime >= 2)
    {
        app_p->remainder = hal_p->accel.x % 105 + hal_p->accel.x % 15;//set a function for the number generated from the accel, and set the remainder as the random number.
        app_p->remaindere = hal_p->accel.y % 100 + hal_p->accel.y % 5;//set a function for the number generated from the accel, and set the remainder as the random number.
        app_p->remaindera = hal_p->accel.x % 110 + hal_p->accel.x % 10;//set a function for the number generated from the accel, and set the remainder as the random number.
        app_p->remainderb = hal_p->accel.y % 95 + hal_p->accel.y % 10;//set a function for the number generated from the accel, and set the remainder as the random number.
        app_p->remainderc = hal_p->accel.x % 90 + hal_p->accel.x % 30;//set a function for the number generated from the accel, and set the remainder as the random number.
        app_p->remainderd = hal_p->accel.y % 80 + hal_p->accel.y % 25;//set a function for the number generated from the accel, and set the remainder as the random number.

        if (SWTimer_expired(&app_p->pointTimer) && app_p->remainder >= 8 && app_p->remainder <= 120 && app_p->remaindere >= 20 && app_p->remaindere <= 105){//set the appear speed for this orb and the appear range on the interface
            Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_GREEN);//draw a orb on the interface when random number is generated
            Graphics_fillCircle(&gfx_p->context, app_p->remainder, app_p->remaindere, 4);
            app_p->pointcount++;//orb number increase
            storecircle(app_p, hal_p, &hal_p->gfx);
            SWTimer_start(&app_p->pointTimer);
        }
        if (SWTimer_expired(&app_p->hazardTimer) && app_p->remaindera >= 8 && app_p->remaindera <= 120 && app_p->remainderb >= 20 && app_p->remainderb <= 105){//set the appear speed for this orb and the appear range on the interface
            Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_YELLOW);
            Graphics_fillCircle(&gfx_p->context, app_p->remaindera, app_p->remainderb, 4);
            app_p->hazardcount++;//orb number increase
            storecircle(app_p, hal_p, &hal_p->gfx);
            SWTimer_start(&app_p->hazardTimer);
        }
        if (SWTimer_expired(&app_p->dangerTimer) && app_p->remainderc >= 8 && app_p->remainderc <= 120 && app_p->remainderd >= 20 && app_p->remainderd <= 105){//set the appear speed for this orb and the appear range on the interface
            Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_RED);
            Graphics_fillCircle(&gfx_p->context, app_p->remainderc, app_p->remainderd, 4);
            app_p->dangercount++;//orb number increase
            storecircle(app_p, hal_p, &hal_p->gfx);
            SWTimer_start(&app_p->dangerTimer);
        }

        SWTimer_start(&app_p->orbTimer);

    }



}




void storecircle(Application* app_p, HAL* hal_p, GFX* gfx_p){

        app_p->pointcollide_x[app_p->point_counter] = app_p->remainder;//use array to store appeared orbs' locations.
        app_p->pointcollide_y[app_p->point_counter] = app_p->remaindere;//use array to store appeared orbs' locations.

        app_p->hazardcollide_x[app_p->hazard_counter] = app_p->remaindera;//use array to store appeared orbs' locations.
        app_p->hazardcollide_y[app_p->hazard_counter] = app_p->remainderb;//use array to store appeared orbs' locations.

        app_p->dangercollide_x[app_p->danger_counter] = app_p->remainderc;//use array to store appeared orbs' locations.
        app_p->dangercollide_y[app_p->danger_counter] = app_p->remainderd;//use array to store appeared orbs' locations.

}

void circlecollide(Application* app_p, HAL* hal_p, GFX* gfx_p){

    if (SWTimer_expired(&app_p->judgeTimer)){
        for (app_p->point_counter = 0; app_p->point_counter < app_p->pointcount;){
            if (pow((app_p->pointcollide_x[app_p->point_counter] - app_p->accelx), 2) + pow((app_p->accely - app_p->pointcollide_y[app_p->point_counter]), 2) <= 100){
                //if the distance between the center of the blue circle and the center of one orb is less than the sum of the circle and the orb's radius, then the orb will disappear
                Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
                Graphics_fillCircle(&gfx_p->context, app_p->pointcollide_x[app_p->point_counter], app_p->pointcollide_y[app_p->point_counter], 4);

                Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLUE);
                Graphics_fillCircle(&gfx_p->context, app_p->accelx, app_p->accely, 6);

                app_p->pointcollide_x[app_p->point_counter] = 0;
                app_p->pointcollide_y[app_p->point_counter] = 0;
                app_p->score++;//score increase by green orb collide
                app_p->life++;//life also increase.

            }

            app_p->point_counter++;

        }
        //////////////

        for (app_p->hazard_counter = 0; app_p->hazard_counter < app_p->hazardcount;){
            if ((pow((app_p->accelx - app_p->hazardcollide_x[app_p->hazard_counter]), 2) + pow((app_p->accely - app_p->hazardcollide_y[app_p->hazard_counter]), 2)) <= 100){
                //if the distance between the center of the blue circle and the center of one orb is less than the sum of the circle and the orb's radius, then the orb will disappear
                Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
                Graphics_fillCircle(&gfx_p->context, app_p->hazardcollide_x[app_p->hazard_counter], app_p->hazardcollide_y[app_p->hazard_counter], 4);
                ////
                Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLUE);
                Graphics_fillCircle(&gfx_p->context, app_p->accelx, app_p->accely, 6);

                app_p->hazardcollide_x[app_p->hazard_counter] = 0;
                app_p->hazardcollide_y[app_p->hazard_counter] = 0;
                app_p->life--;
                if (app_p->life <= 0){//if life is 0, the game turn to result screen
                    app_p->result++;
                    app_p->state = RESULT_SCREEN;
                    Application_showResultScreen(app_p, hal_p, &hal_p->gfx);
                }
            }
            app_p->hazard_counter++;
        }
        /////////////

        for (app_p->danger_counter = 0; app_p->danger_counter < app_p->dangercount;){
            if ((pow((app_p->accelx - app_p->dangercollide_x[app_p->danger_counter]), 2) + pow((app_p->accely - app_p->dangercollide_y[app_p->danger_counter]), 2)) <= 100){
                //if the distance between the center of the blue circle and the center of one orb is less than the sum of the circle and the orb's radius, then the orb will disappear
                Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLACK);
                Graphics_fillCircle(&gfx_p->context, app_p->dangercollide_x[app_p->danger_counter], app_p->dangercollide_y[app_p->danger_counter], 4);
                /////
                Graphics_setForegroundColor(&gfx_p->context, GRAPHICS_COLOR_BLUE);
                Graphics_fillCircle(&gfx_p->context, app_p->accelx, app_p->accely, 6);

                app_p->dangercollide_x[app_p->danger_counter] = 0;
                app_p->dangercollide_y[app_p->danger_counter] = 0;
                app_p->result++;
                app_p->state = RESULT_SCREEN;//if the red orb collide with the blue circle, the game turn to result screen
                Application_showResultScreen(app_p, hal_p, &hal_p->gfx);
            }
            app_p->danger_counter++;
        }
        SWTimer_start(&app_p->judgeTimer);
    }


}


void highscore(Application* app_p, HAL* hal_p){//recods three hightest scores
    if (app_p->score>0  && app_p->result == 1){
            app_p->sc1 = app_p->score;
        }
        ////////////////////////
        if (app_p->score > 0 && app_p->result == 2){
            if (app_p->score < app_p->sc1){
                app_p->sc2 = app_p->score;
            }
            else if (app_p->score >= app_p->sc1){
                app_p->sc2 = app_p->sc1;
                app_p->sc1 = app_p->score;
            }
        }

        //////////////////////////////////////////////
        if (app_p->score > 0 && app_p->result >= 3){
            if (app_p->score >= app_p->sc1){
                app_p->sc3 = app_p->sc2;
                app_p->sc2 = app_p->sc1;
                app_p->sc1 = app_p->score;

            }
            else if (app_p->score < app_p->sc1 && app_p->score >= app_p->sc2){
                app_p->sc3 = app_p->sc2;
                app_p->sc2 = app_p->score;
            }
            else if (app_p->score < app_p->sc2 && app_p->score >= app_p->sc3){
                app_p->sc3 = app_p->score;
            }

            else if (app_p->score < app_p->sc3){
                app_p->sc4 = app_p->score;
            }
        }
}





