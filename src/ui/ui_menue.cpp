#include "ui_menue.h"
#include <hardware_drivers/display/display.h>
#include <hardware_drivers/buttons/buttons.h>
#include <system/card_controller_queue.h>

extern Adafruit_SSD1306 display;

static MenuState currentState = MENU_SPLASH;

static GameConfig config = {
    .players      = 3,
    .cardsPerPlayer = 4
};

static GameProgress liveProgress = {};

static int  cursor       = 0;
static bool editing      = false;
static unsigned long splashStart = 0;

void menuInit()
{
    currentState = MENU_SPLASH;
    splashStart  = millis();
}

MenuState  menuGetState()  { return currentState; }
GameConfig menuGetConfig() { return config; }

void menuSetProgress(const GameProgress &p)
{
    liveProgress = p;

    if (currentState == MENU_GAME_PROGRESS)
    {
        if (p.jamDetected)
            currentState = MENU_GAME_JAM;
        else if (p.finished || p.deckEmpty)
            currentState = MENU_GAME_DONE;
    }
}



static char   tickerText[128]  = "";
static int    tickerOffset     = 0;
static uint32_t tickerLastMs   = 0;
static MenuState tickerScreen  = (MenuState)(-1); 

static void tickerSet(const char *text)
{
    if (currentState != tickerScreen)
    {
        tickerOffset  = 0;
        tickerLastMs  = millis();
        tickerScreen  = currentState;
    }
    strncpy(tickerText, text, sizeof(tickerText) - 1);
    tickerText[sizeof(tickerText) - 1] = '\0';
}


static void tickerDraw(int y)
{
    const int charW     = 6;   
    int       textPxW   = strlen(tickerText) * charW;
    const int viewW     = 128;

    if (textPxW > viewW)
    {
        if (millis() - tickerLastMs >= 80)
        {
            tickerLastMs = millis();
            tickerOffset++;
            if (tickerOffset >= textPxW + viewW)  
                tickerOffset = 0;
        }
    }
    else
    {
        tickerOffset = 0;   
    }

    display.drawFastHLine(0, y - 2, viewW, SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(-tickerOffset, y);
    display.print(tickerText);

    if (textPxW > viewW)
    {
        display.setCursor(textPxW - tickerOffset + 8, y);
        display.print(tickerText);
    }
}

static void drawHeader(const char *title)
{
    display.fillRect(0, 0, 128, 12, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setTextSize(1);
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(title, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((128 - w) / 2, 2);
    display.print(title);
    display.setTextColor(SSD1306_WHITE);
}

static void drawProgressBar(int x, int y, int width, int height, int value, int maxVal)
{
    display.drawRect(x, y, width, height, SSD1306_WHITE);
    if (maxVal > 0)
    {
        int fill = (width - 2) * value / maxVal;
        if (fill > 0)
            display.fillRect(x + 1, y + 1, fill, height - 2, SSD1306_WHITE);
    }
}


void updateHome()
{
    if (buttonShortPress(STATE_BTN_UP))
    {
        cursor--;
        if (cursor < 0) cursor = 1;
    }
    if (buttonShortPress(STATE_BTN_DOWN))
    {
        cursor++;
        if (cursor > 1) cursor = 0;
    }
    if (buttonShortPress(STATE_BTN_SELECT))
    {
        if (cursor == 0)
        {
            currentState = MENU_GAME_SETUP;
            cursor = 0;
        }
        else
        {
            currentState = MENU_ABOUT;
        }
    }
}

void updateAbout()
{
    if (buttonLongPress(STATE_BTN_SELECT))
    {
        currentState = MENU_HOME;
        cursor = 0;
    }
}

void updateSetup()
{
    if (!editing)
    {
        if (buttonShortPress(STATE_BTN_UP))
        {
            cursor--;
            if (cursor < 0) cursor = 2;
        }
        if (buttonShortPress(STATE_BTN_DOWN))
        {
            cursor++;
            if (cursor > 2) cursor = 0;
        }
        if (buttonShortPress(STATE_BTN_SELECT))
        {
            if (cursor == 2)
            {
                liveProgress = {};          
                currentState = MENU_GAME_PROGRESS;

                CardControllerRequest req;
                req.isSendTask      = true;
                req.playerCount     = config.players;
                req.cardsPerPlayer  = config.cardsPerPlayer;
                xQueueSend(cardControllerRequest, &req, 0);
            }
            else
            {
                editing = true;
            }
        }
        if (buttonLongPress(STATE_BTN_SELECT))
        {
            currentState = MENU_HOME;
            cursor = 0;
        }
    }
    else
    {
        if (cursor == 0)   
        {
            if (buttonShortPress(STATE_BTN_UP))   config.players++;
            if (buttonShortPress(STATE_BTN_DOWN))  config.players--;
            config.players = constrain(config.players, 1, 8);
        }
        if (cursor == 1)  
        {
            if (buttonShortPress(STATE_BTN_UP))   config.cardsPerPlayer++;
            if (buttonShortPress(STATE_BTN_DOWN))  config.cardsPerPlayer--;
            config.cardsPerPlayer = constrain(config.cardsPerPlayer, 1, 13);
        }

        if (buttonShortPress(STATE_BTN_SELECT))
            editing = false;

        if (buttonLongPress(STATE_BTN_SELECT))
            editing = false;
    }
}

void updateProgress()
{
    if (buttonLongPress(STATE_BTN_SELECT))
    {
        currentState = MENU_HOME;
        cursor = 0;
    }
}

void updateDone()
{
    if (buttonShortPress(STATE_BTN_SELECT) || buttonLongPress(STATE_BTN_SELECT))
    {
        currentState = MENU_HOME;
        cursor = 0;
    }
}

void updateJam()
{
    if (buttonLongPress(STATE_BTN_SELECT))
    {
        currentState = MENU_HOME;
        cursor = 0;
    }
}

void menuUpdate()
{
    switch (currentState)
    {
    case MENU_SPLASH:
        if (millis() - splashStart > 2000)
            currentState = MENU_HOME;
        break;
    case MENU_HOME:         updateHome();     break;
    case MENU_ABOUT:        updateAbout();    break;
    case MENU_GAME_SETUP:   updateSetup();    break;
    case MENU_GAME_PROGRESS: updateProgress(); break;
    case MENU_GAME_DONE:    updateDone();     break;
    case MENU_GAME_JAM:     updateJam();      break;
    }
}


void drawSplash()
{
    display.clearDisplay();

    display.drawPixel(0,  0,  SSD1306_WHITE);
    display.drawPixel(127, 0,  SSD1306_WHITE);
    display.drawPixel(0,  63, SSD1306_WHITE);
    display.drawPixel(127, 63, SSD1306_WHITE);

    display.drawRect(1, 1, 126, 62, SSD1306_WHITE);

    display.setTextSize(2);
    int16_t x1, y1; uint16_t w, h;
    display.getTextBounds("OpenCard", 0, 0, &x1, &y1, &w, &h);
    display.setCursor((128 - w) / 2, 14);
    display.print("OpenCard");

    display.setTextSize(1);
    display.getTextBounds("Auto Dealer v1.0", 0, 0, &x1, &y1, &w, &h);
    display.setCursor((128 - w) / 2, 38);
    display.print("Auto Dealer v1.0");

    static uint8_t blink = 0;
    blink = (blink + 1) % 4;
    for (int i = 0; i < 4; i++)
    {
        if (i <= blink)
            display.fillCircle(50 + i * 8, 52, 2, SSD1306_WHITE);
        else
            display.drawCircle(50 + i * 8, 52, 2, SSD1306_WHITE);
    }

    display.display();
}

void drawHome()
{
    display.clearDisplay();
    drawHeader("OpenCard");

    display.setTextSize(1);

    const char *items[] = {"Start Game", "About"};
    for (int i = 0; i < 2; i++)
    {
        int y = 18 + i * 18;
        if (cursor == i)
        {
            display.fillRect(6, y - 1, 116, 11, SSD1306_WHITE);
            display.setTextColor(SSD1306_BLACK);
        }
        else
        {
            display.setTextColor(SSD1306_WHITE);
        }
        display.setCursor(10, y);
        display.print(items[i]);
    }

    display.setTextColor(SSD1306_WHITE);
    display.display();
}

void drawAbout()
{
    display.clearDisplay();
    drawHeader("About");

    display.setTextSize(1);

    display.setCursor(4, 15);
    display.print("OpenCard");

    display.setCursor(4, 25);
    display.print("by pavan");

    display.setCursor(4, 35);
    display.print("Open-source auto");
    display.setCursor(4, 44);
    display.print("card dealer");

    tickerSet("github.com/pavanb0/OpenCard   [Hold SEL] Back");
    tickerDraw(56);

    display.display();
}

void drawSetup()
{
    display.clearDisplay();
    drawHeader("Game Setup");

    display.setTextSize(1);

    {
        bool sel  = (cursor == 0);
        bool edit = sel && editing;
        int  y    = 15;
        if (sel && !edit) { display.fillRect(0, y-1, 128, 11, SSD1306_WHITE); display.setTextColor(SSD1306_BLACK); }
        else              { display.setTextColor(SSD1306_WHITE); }
        display.setCursor(4, y);
        display.print("Players:");
        if (edit)
        {
            display.setTextColor(SSD1306_WHITE);
            display.print(" <");
            display.print(config.players);
            display.print(">");
        }
        else
        {
            display.print(" ");
            display.print(config.players);
        }
    }

    {
        bool sel  = (cursor == 1);
        bool edit = sel && editing;
        int  y    = 27;
        display.setTextColor(SSD1306_WHITE);
        if (sel && !edit) { display.fillRect(0, y-1, 128, 11, SSD1306_WHITE); display.setTextColor(SSD1306_BLACK); }
        display.setCursor(4, y);
        display.print("Cards/pl:");
        if (edit)
        {
            display.setTextColor(SSD1306_WHITE);
            display.print(" <");
            display.print(config.cardsPerPlayer);
            display.print(">");
        }
        else
        {
            display.print(" ");
            display.print(config.cardsPerPlayer);
        }
    }

    display.setTextColor(SSD1306_WHITE);
    display.drawFastHLine(0, 40, 128, SSD1306_WHITE);

    {
        bool sel = (cursor == 2);
        int  y   = 43;
        if (sel)
        {
            display.fillRoundRect(24, y, 80, 12, 3, SSD1306_WHITE);
            display.setTextColor(SSD1306_BLACK);
        }
        else
        {
            display.drawRoundRect(24, y, 80, 12, 3, SSD1306_WHITE);
            display.setTextColor(SSD1306_WHITE);
        }
        display.setCursor(40, y + 2);
        display.print("[ START ]");
    }

    display.setTextColor(SSD1306_WHITE);
    tickerSet("[SEL]=Select/Confirm   [Long SEL]=Back/Exit Edit");
    tickerDraw(56);

    display.display();
}

void drawProgress()
{
    display.clearDisplay();
    drawHeader("Dealing...");

    display.setTextSize(1);

    char buf[24];
    snprintf(buf, sizeof(buf), "Card  %d / %d",
             liveProgress.currentCard,
             liveProgress.cardsPerPlayer);
    display.setCursor(4, 15);
    display.print(buf);

    snprintf(buf, sizeof(buf), "Player %d / %d",
             liveProgress.currentPlayer,
             liveProgress.totalPlayers);
    display.setCursor(4, 25);
    display.print(buf);

    int totalOps = liveProgress.totalPlayers * liveProgress.cardsPerPlayer;
    int doneOps  = 0;
    if (totalOps > 0)
    {
        int doneRounds  = max(0, liveProgress.currentCard - 1);
        int donePlayers = liveProgress.currentPlayer;
        doneOps = doneRounds * liveProgress.totalPlayers + donePlayers;
    }
    drawProgressBar(4, 36, 120, 7, doneOps, max(1, totalOps));

    int pct = (totalOps > 0) ? (doneOps * 100 / totalOps) : 0;
    snprintf(buf, sizeof(buf), "%d%%", pct);
    int16_t x1, y1; uint16_t w, h;
    display.getTextBounds(buf, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((128 - w) / 2, 46);
    display.print(buf);

    tickerSet("[Hold SEL] to abort deal and return home");
    tickerDraw(57);

    display.display();
}

void drawDone()
{
    display.clearDisplay();
    drawHeader("Done!");

    display.setTextSize(1);

    if (liveProgress.deckEmpty)
    {
        display.setCursor(4, 20);
        display.print("Deck ran empty.");
        display.setCursor(4, 32);
        display.print("Not all cards dealt.");
    }
    else
    {
        display.setCursor(4, 20);
        display.print("All cards dealt!");

        char buf[24];
        snprintf(buf, sizeof(buf), "%dp x %dc each",
                 liveProgress.totalPlayers,
                 liveProgress.cardsPerPlayer);
        display.setCursor(4, 34);
        display.print(buf);
    }

    display.drawFastHLine(20, 48, 88, SSD1306_WHITE);

    display.setCursor(10, 54);
    display.print("[SEL] Return Home");

    display.display();
}

void drawJam()
{
    display.clearDisplay();

    display.fillRect(0, 0, 128, 12, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.setTextSize(1);
    display.setCursor(32, 2);
    display.print("!! JAM !!");
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(4, 18);
    display.print("Card jam detected!");

    display.setCursor(4, 30);
    display.print("Please clear the jam");
    display.setCursor(4, 41);
    display.print("then hold SELECT.");

    char buf[32];
    snprintf(buf, sizeof(buf), "Stopped at P%d C%d",
             liveProgress.currentPlayer,
             liveProgress.currentCard);
    display.setCursor(4, 53);
    display.print(buf);

    display.display();
}

void menuDraw()
{
    switch (currentState)
    {
    case MENU_SPLASH:         drawSplash();   break;
    case MENU_HOME:           drawHome();     break;
    case MENU_ABOUT:          drawAbout();    break;
    case MENU_GAME_SETUP:     drawSetup();    break;
    case MENU_GAME_PROGRESS:  drawProgress(); break;
    case MENU_GAME_DONE:      drawDone();     break;
    case MENU_GAME_JAM:       drawJam();      break;
    }
}