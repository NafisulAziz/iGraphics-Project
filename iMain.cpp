
#include "iGraphics.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")
#include <math.h>
#include <time.h>
#define screenwidth 1000
#define screenheight 600
#define numBalls 3
float a_x;
float b_y;

int flag = 1;
int count = 1;
int radius = 55;
int r = 0;
int game_state = -1;
int g = 0;
int b = 0;
int index = 0;
bool collision = false;
double collision_x, collision_y;
int cnt = 0;

struct highscore
{
    char name[100];
    int score;
    char score_str[100];
};

FILE *fp;
struct highscore player[5];
bool bomb_come = false;
double bomb_x, bomb_y, bomb_dx = 10, bomb_dy = 10;

bool tormuj_come = false, narikel_come = false;
double tormuj_x, tormuj_y, narikel_x, narikel_y;

float x[numBalls]; // Initial x positions for each ball
float y[numBalls]; // Initial y positions for each ball
int circlevisible[numBalls];
float vx[numBalls];
float vy[numBalls];

void defaultpos()
{
    int i;
    for (i = 0; i < numBalls; i++)
    {
        x[i] = 0 - rand() % 300;
        y[i] = rand() % 400;
        if (y[i] < (radius + 20))
        {
            y[i] = radius + 20;
        }
        vy[i] = 100.0 + 10 * i + count * 10;
    }
    for (int i = 0; i < numBalls; i++)
    {
        vx[i] = 100.0 + 10 * i + (count / 12) * 10;       // Initial velocity for each ball
        vy[i] = 100.0 + 10 * i + (count / 15) * (10 + i); // Initial velocity for each ball
        circlevisible[i] = 1;
    }
}

float dt = 0.1;
int speedfactor = 50;
int timer = 100;
int world = 1;

bool musicOn = true;

// bool music0_On = true;

char scoreshow[30] = "Your Score: ";
char score[30] = "Score: ";
char music[30] = "music\\music1.wav";
char music_0[40] = "music\\fruit-gum-distortion-15679.wav";
char sound[50] = "music\\steel-blade-slice-2-188214.wav";
char tmp[30] = "images\\tmp.bmp";
char tormuj[30] = "images\\borotormuj.bmp";
char narikel[30] = "images\\narikel.bmp";
char images[7][100] = {"images\\tormuj.bmp", "images\\Mango2.bmp", "images\\strawberry.bmp", "images\\about.bmp", "images\\bc1.bmp", "images\\mainbackground.bmp", "images\\sound.bmp"};
char images2[3][100] = {"images\\anarosh.bmp", "images\\kola.bmp", "images\\dab.bmp"};
char remaining[30] = "Remaining: ";
char highscore[30] = "images\\highscorebg.bmp";
char tmp_name[30];
char collisionimg[30] = "images\\collision.bmp";
void bomb_show()
{
    if (bomb_come)
    {
        iShowBMP2(bomb_x, bomb_y, "images\\bomb.bmp", 0);
    }
    if (tormuj_come)
    {
        iShowBMP2(tormuj_x, tormuj_y, tormuj, 0);
    }
    if (narikel_come)
    {
        iShowBMP2(narikel_x, narikel_y, narikel, 0);
    }
}

void iDraw()
{
    iClear();
    // if(game_state==0){
    //     //homemenue
    //     if((a_x>=320 && a_x <=610) && (b_y >= 357) && (b_y <= 460) )
    //     game_state = 1;//1 for game
    // }

    if ((a_x >= 320 && a_x <= 610) && (b_y >= 200 && b_y <= 310))
    {
        game_state = 2; // 2 for sound
    }
    if ((a_x >= 320 && a_x <= 610) && (b_y >= 50 && b_y <= 160))
    {
        game_state = 3; // 3 for about
    }
    if (game_state == 5)
    {
        iShowBMP(0, 0, "images\\over.bmp");
        iSetColor(0, 0, 0);
        iText(450, 500, scoreshow, GLUT_BITMAP_TIMES_ROMAN_24);
        iText(370, 450, "ENTER NAME: ", GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(255, 255, 255);
        iFilledRectangle(550, 445, 300, 30);
        iSetColor(0, 0, 0);
        iText(555, 450, tmp_name, GLUT_BITMAP_TIMES_ROMAN_24);
    }

    iSetColor(r, g + 255, b);
    if (game_state == -1)
        iShowBMP(0, 0, "images\\start.bmp");
    if (game_state == 0)
        iShowBMP(0, 0, "images\\bc1.bmp");
    if (game_state == 1)
    {
        if (world == 0)
            iShowBMP(0, 0, "images\\mainbackground.bmp");
        else
            iShowBMP(0, 0, "images\\mainbackground2.bmp");
        iText(450, 550, score, GLUT_BITMAP_TIMES_ROMAN_24);
        iText(450, 500, remaining, GLUT_BITMAP_TIMES_ROMAN_24);
        bomb_show();
        for (int i = 0; i < numBalls; ++i)
        {
            if (circlevisible[i] == 1)
            {
                int n = rand();
                // siFilledCircle(x[i], y[i], radius, 1000);
                if (world == 0)
                    iShowBMP2(x[i] - 70, y[i] - 70, images[i], 0);
                else
                    iShowBMP2(x[i] - 70, y[i] - 70, images2[i], 0);
                // printf("%f %f\n",x[i],y[i]);
            }
        }
        if (collision)
        {
            iShowBMP2(collision_x - 50, collision_y - 70, collisionimg, 0);
            cnt++;
            if (cnt == 10)
            {
                cnt = 0;
                collision = false;
            }
        }
    }
    if (game_state == 6)
    {
        iShowBMP(0, 0, "images\\HELP.bmp");
    }
    if (game_state == 2)
    {
        iShowBMP(0, 0, "images\\sound.bmp");
    }
    if (game_state == 3)
    {
        iShowBMP(0, 0, "images\\about.bmp");
    }
    if (game_state == -2)
    {
        iShowBMP(0, 0, "images\\tmp.bmp");
    }
    if (game_state == 4)
    {
        iShowBMP(0, 0, highscore);
        iSetColor(255, 255, 255);
        iText(100, 500, "1. ", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(100, 450, "2. ", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(100, 400, "3. ", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(100, 350, "4. ", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(100, 300, "5. ", GLUT_BITMAP_TIMES_ROMAN_24);
        for (int i = 0; i < 5; i++)
        {
            iText(150, 500 - (i * 50), player[i].name, GLUT_BITMAP_TIMES_ROMAN_24);
            iText(600, 500 - (i * 50), player[i].score_str, GLUT_BITMAP_TIMES_ROMAN_24);
        }
    }
}

void bomb_initialize()
{
    if (game_state == 1 && world == 0 && !bomb_come && rand() % 8 == 0 && count > 1)
    {
        bomb_come = true;
        bomb_x = -(rand() % 150);
        bomb_y = rand() % 100;
        if (bomb_y < (radius + 20))
        {
            bomb_y = radius + 20;
        }
    }
    if (game_state == 1 && world == 1 && !bomb_come && rand() % 3 == 0 && count > 1)
    {
        bomb_come = true;
        bomb_x = -(rand() % 150);
        bomb_y = rand() % 100;
        if (bomb_y < (radius + 20))
        {
            bomb_y = radius + 20;
        }
    }
    if (game_state == 1 && !tormuj_come && rand() % 5 == 0 && count > 5)
    {
        tormuj_come = true;
        tormuj_x = -(rand() % 150);
        tormuj_y = rand() % 100;
        if (tormuj_y < (radius + 20))
        {
            tormuj_y = radius + 20;
        }
    }
    if (game_state == 1 && !narikel_come && rand() % 5 == 0 && count > 5)
    {
        narikel_come = true;
        narikel_x = -(rand() % 150);
        narikel_y = rand() % 100;
        if (narikel_y < (radius + 20))
        {
            narikel_y = radius + 20;
        }
    }
}
void changeballs()
{
    if (game_state == 1)
    {
        for (int i = 0; i < numBalls; ++i)
        {
            if (circlevisible[i])
            {
                x[i] = x[i] + 1.5 * speedfactor * dt;
                y[i] = y[i] + vy[i] * dt;
                vx[i] = vx[i] - 6.8 * dt;
                vy[i] = vy[i] - 5.8 * dt;

                if (x[i] >= screenwidth + radius)
                {
                    /// printf("%0.2f %0.2f\n",x[i],y[i]);
                    // Ball reached right edge or bottom edge, make it invisibl
                    circlevisible[i] = 0;
                }
                if (circlevisible[0] + circlevisible[1] + circlevisible[2] == 0)
                {
                    defaultpos();
                    if (flag)
                    {
                        printf("here\n");
                        flag = 0;
                    }
                }

                if (y[i] > screenheight || y[i] < radius + 20)
                {
                    // Ball hits the top, reverse the velocity
                    vy[i] = -vy[i];
                }
            }
        }
    }
}

void iMouseMove(int mx, int my)
{
    for (int i = 0; i < numBalls; ++i)
    {
        if ((mx >= x[i] - radius) && (mx <= x[i] + radius) && (my >= y[i] - radius) && (my <= y[i] + radius))
        {
            // printf("Mouse over ball %d\n", i + 1);
            if (circlevisible[i] == 1)
            {
                collision = true;
                collision_x = x[i];
                collision_y = y[i];
                PlaySound(sound, NULL, SND_ASYNC);
                sprintf(score, "Score: %d", count++);
            }
            circlevisible[i] = 0;
            // printf("Score: %d\n",count++);
            if (circlevisible[0] == 0 && circlevisible[1] == 0 && circlevisible[2] == 0)
            {
                defaultpos();

                if (flag)
                {
                    printf("not here\n");
                    flag = 0;
                }
            }
        }
    }
    if (mx >= tormuj_x && mx <= tormuj_x + 70 && my >= tormuj_y && my <= tormuj_y + 70)
    {
        if (tormuj_come)
        {
            collision = true;
            collision_x = tormuj_x;
            collision_y = tormuj_y;
            PlaySound(sound, NULL, SND_ASYNC);
            sprintf(score, "Score: %d", count++);
        }
        tormuj_come = false;
    }
    if (mx >= narikel_x && mx <= narikel_x + 75 && my >= narikel_y && my <= narikel_y + 75)
    {
        if (narikel_come)
        {
            collision = true;
            collision_x = narikel_x;
            collision_y = narikel_y;
            PlaySound(sound, NULL, SND_ASYNC);
            sprintf(score, "Score: %d", count++);
        }
        narikel_come = false;
    }
    if (mx >= bomb_x && mx <= bomb_x + 55 && my >= bomb_y && my <= bomb_y + 55)
    {
        bomb_come = false;
        bomb_initialize();
        defaultpos();

        game_state = 5;
        sprintf(scoreshow, "Your Score: %d", count);
        // count = 1;
        timer = 100;
        sprintf(score, "Score: ");
    }
}

void iMouse(int button, int state, int mx, int my)
{

    if (game_state == 0 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 307 && mx <= 610 && my >= 357 && my <= 469)
    {

        game_state = -2;
        if (musicOn)
        {
            // musicOn = false;
            PlaySound(0, 0, 0);
        }
    }
    if (game_state == -2 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 354 && mx <= 645 && my >= 172 && my <= 253)
    {
        game_state = 1;
        world = 0;
    }
    if (game_state == -2 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 354 && mx <= 645 && my >= 58 && my <= 144)
    {
        game_state = 1;
        world = 1;
    }
    if (game_state == 0 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 320 && mx <= 610 && my >= 200 && my <= 310)
    {
        game_state = 2;
        printf("%d %d\n", mx, my);
    }
    if (game_state == 0 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 309 && mx <= 630 && my >= 55 && my <= 166)
    {
        game_state = 3;
    }
    if (game_state == 0 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 60 && mx <= 236 && my >= 24 && my <= 230)
    {
        game_state = 6;
    }
    if (game_state == 0 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 700 && mx <= 880 && my >= 114 && my <= 220)
    {
        game_state = 4;
        fp = fopen("score.txt", "r");
        for (int i = 0; i < 5; i++)
        {
            fscanf(fp, "%s %d", player[i].name, &player[i].score);
            sprintf(player[i].score_str, "%d", player[i].score);
        }
    }
    if (game_state == 2 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (mx >= 565 && mx <= 803 && my >= 110 && my <= 190)
        {
            musicOn = false;
            PlaySound(NULL, NULL, 0);
            game_state = 0;
        }
        else if (mx >= 160 && mx <= 400 && my >= 100 && my <= 195)
        {
            musicOn = true;
            PlaySound(music, NULL, SND_LOOP | SND_ASYNC);

            game_state = 0;
        }
    }
    if(game_state == 3){
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 316 && mx <= 686 && my >= 178 && my <= 230)
            game_state = 0;
    }
    if(game_state == 4){
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 347 && mx <= 647 && my >= 40 && my <= 105)
            game_state = 0;
    }
    if(game_state == 6){
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && mx >= 347 && mx <= 647 && my >= 40 && my <= 105)
            game_state = 0;
    }
    printf("%d %d\n", mx, my);
}

void iKeyboard(unsigned char key)
{
    if (game_state == -1 && (key == 'S' || key == 's'))
    {
        game_state = 0;
        PlaySound(music, NULL, SND_LOOP | SND_ASYNC);
    }
    if (game_state == 5 && key == '\r')
    {

        game_state = 0;
        fp = fopen("score.txt", "r");
        for (int i = 0; i < 5; i++)
        {
            fscanf(fp, "%s %d", player[i].name, &player[i].score);
            // sprintf(player[i].score_str,"%d",player[i].score);
        }
        fclose(fp);
        fp = fopen("score.txt", "w");
        int i;
        for (i = 0; i < 5; i++)
        {
            if (count > player[i].score)
            {
                int j;
                for (j = 4; j > i; j--)
                {
                    player[j].score = player[j - 1].score;
                    strcpy(player[j].name, player[j - 1].name);
                }
                player[i].score = count;
                if (strlen(tmp_name) == 0)
                    strcpy(player[i].name, "Player");
                else
                    strcpy(player[i].name, tmp_name);
                // sprintf(players[i].score_text,"%d",players[i].score);
                break;
            }
        }
        for (i = 0; i < 5; i++)
        {
            // printf("%s %d\n",player[i].name,player[i].score);
            fprintf(fp, "%s %d\n", player[i].name, player[i].score);
        }
        fclose(fp);
        tmp_name[0] = '\0';
        count = 1;
        index = 0;
    }
    if (game_state == 5)
    {
        if (key != '\b')
        {
            tmp_name[index] = key;
            index++;
            tmp_name[index] = '\0';
        }
        else
        {
            if (index > 0)
            {
                index--;
                tmp_name[index] = '\0';
            }
        }
    }
    if (musicOn)
    {
        PlaySound(music, NULL, SND_LOOP | SND_ASYNC);
    }

    // if(key == 'a') game_state = 5;
    if (game_state ==0 && key == 'q')
    {
        exit(0);
    }
    if (game_state == 6 && (key == 'B' || key == 'b'))
        game_state = 0;
    if (game_state == 3 && (key == 'B' || key == 'b'))
        game_state = 0;
    if (game_state == 4 && (key == 'B' || key == 'b'))
        game_state = 0;
}

void iSpecialKeyboard(unsigned char key)
{
    if (key == GLUT_KEY_END)
    {
        exit(0);
    }
}

void bomb_change()
{
    if (bomb_come)
    {
        bomb_x += bomb_dx;
        bomb_y += bomb_dy;
        if (bomb_y > screenheight - 55 || bomb_y < 0)
        {
            bomb_dy = -bomb_dy;
        }
        if (bomb_x > screenwidth)
        {
            bomb_come = false;
        }
    }
    if (tormuj_come)
    {
        tormuj_x += bomb_dx;
        tormuj_y += bomb_dy;
        if (tormuj_y > screenheight - 70 || tormuj_y < 0)
        {
            bomb_dy = -bomb_dy;
        }
        if (tormuj_x > screenwidth)
        {
            tormuj_come = false;
        }
    }
    if (narikel_come)
    {
        narikel_x += bomb_dx;
        narikel_y += bomb_dy;
        if (narikel_y > screenheight - 75 || narikel_y < 0)
        {
            bomb_dy = -bomb_dy;
        }
        if (narikel_x > screenwidth)
        {
            narikel_come = false;
        }
    }
}
void time_change()
{
    if (game_state == 1)
        timer--;
    if (timer == 0)
    {
        game_state = 5;
        count = 1;
        defaultpos();
        timer = 100;
    }
    sprintf(remaining, "Remaining: %d seconds", timer);
}
int main()
{
    for (int i = 0; i < numBalls; i++)
    {
        vy[i] = 100.0 + 10 * i; // Initial velocity for each ball
        circlevisible[i] = 1;
    }

    srand(time(0));
    iSetTimer(25, changeballs);
    iSetTimer(25, bomb_change);
    iSetTimer(1000, bomb_initialize);
    iSetTimer(1000, time_change);
    PlaySound(music_0, NULL, SND_LOOP | SND_ASYNC);
    // PlaySound("music\\steel-blade-slice-2-188214.wav", NULL, SND_ASYNC);
    iInitialize(screenwidth, screenheight, "project");
    return 0;
}
