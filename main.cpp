#include <iostream>
#include <cstring>
#include <math.h>
#include <limits>
#include <stdlib.h>
#include <fstream>
#include <istream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

const int SCREEN_WIDTH = 1040 ;
const int SCREEN_HEIGHT = 1040 ;

const int PlateSizeX = 200;
const int PlateSizeY = 20;
const int PlateY = SCREEN_HEIGHT - PlateSizeY - 30;

const int radius = 30;

const int con = 20;

void Game (SDL_Renderer *render, TTF_Font *font1);

void Menu(SDL_Renderer *render, TTF_Font *font1, TTF_Font *font2, TTF_Font *font3);

void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect, SDL_Color textColor);

void AddResult (int a);

void LoadRecords(SDL_Renderer *render, TTF_Font *font1, TTF_Font *font2);

char* Vvod();

class GameField
{
    private:
        void DrawBrick(SDL_Renderer *render, int x, int y, int* colors)
        {
            SDL_SetRenderDrawColor(render, colors[0], colors[1], colors[2], 255);
            SDL_Rect rect = {x, y, 100, 50};
            SDL_RenderFillRect(render, &rect);
            SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
            rect = {x, y, 100, 50};
            SDL_RenderDrawRect(render, &rect);
            rect = {x+1, y+1, 98, 48};
            SDL_RenderDrawRect(render, &rect);
            //rect = {x+2, y+2, 96, 46};
            //SDL_RenderDrawRect(render, &rect);
        }
        void CleanBrick(SDL_Renderer *render, int x, int y)
        {
            SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
            SDL_Rect rect = {x, y, 100, 50};
            SDL_RenderFillRect(render, &rect);
        }

    public:
        int **Level;
        int score;

        GameField()
        {
            Level = new int*[5];

            for (int i = 0; i < 5; i++)
            {
                Level[i] = new int[10];
            }
            score = 0;
        }
        ~GameField()
        {
            for (int i = 0; i < 5; i++)
            {
                delete []Level[i];
            }
            delete[] Level;
        }



        void Load (string filename)
        {
            int i, j;
            ifstream MyLevel;
            MyLevel.open(filename, ios_base::in);
            if (!MyLevel)
            {
                cout << "File open Error" << endl;
                return;
            }

            for(i = 0; i < 5; i++)
            {
                for(j = 0; j < 10; j++)
                {
                    MyLevel >> Level[i][j];
                }
            }

            MyLevel.close();
        }

        void Draw (SDL_Renderer *render)
        {
            int i, j;
            int colors[3];

            for (i = 0; i < 5; i++)
            {
                for (j = 0; j < 10; j++)
                {
                    switch(Level[i][j])
                    {
                        case 0:
                            CleanBrick(render, con + j * 100, con + i * 50);
                            break;

                        case 1:
                            colors[0] = 0;
                            colors[1] = 130;
                            colors[2] = 255;
                            DrawBrick(render, con + j * 100, con + i * 50, colors);
                            break;

                        case 2:
                            colors[0] = 255;
                            colors[1] = 65;
                            colors[2] = 203;
                            DrawBrick(render, con + j * 100, con + i * 50, colors);
                            break;

                        case 3:
                            colors[0] = 138;
                            colors[1] = 238;
                            colors[2] = 134;
                            DrawBrick(render, con + j * 100, con + i * 50, colors);
                            break;

                        case 4:
                            colors[0] = 242;
                            colors[1] = 196;
                            colors[2] = 62;
                            DrawBrick(render, con + j * 100, con + i * 50, colors);
                            break;
                    }
                }
            }
        }

        bool is_win ()
        {
            int i, j;
            for (i = 0; i < 5; i++)
            {
                for (j = 0; j < 10; j++)
                {
                    if(Level[i][j] != 0)
                        return false;
                }
            }
            return true;
        }

        /*void tick (SDL_Renderer *render, int key)
        {
            element *tmp;
            tmp = MyObjects.head;

            while(tmp)
            {
                tmp->Obj->Move(render, key, this);
                tmp = tmp->next;
            }
        }*/
};


class GameObject
{
    public:
        int x, y, type;

        int getX ()
        {
            return x;
        }
        int getY ()
        {
            return y;
        }

        int getType()
        {
            return type;
        }

        virtual void Draw (SDL_Renderer *render)
        {

        }
        virtual bool Move (SDL_Renderer *render, GameField *Field, int key, int PlateX) {  }

        virtual void reset(SDL_Renderer *render) {}

};

struct element
{
    GameObject *Obj;
    element *next;
};

class List
{

    public:
        element *head, *tail;
        List()
        {
            head = NULL;
            tail = NULL;
        }
        ~List()
        {
            while (head)
            {
                tail = head->next;
                delete head;
                head = tail;
            }
        }
        int add_node(GameObject *out_data)
        {
            element *temp = new (nothrow) element;
            if (!temp)
            {
                cout << "Проблемы с памятью\n";
                cin.get();
                return 1;
            }

            temp->next = NULL;
            temp->Obj = out_data;

            if (head)
            {
                tail->next = temp;
                tail = temp;
            }
            else
            {
                head = temp;
                tail = temp;
                return 0;
            }
            return 0;
        }
};

class plate: public GameObject
{
    private:
        void draw1(SDL_Renderer *render, int r, int g, int b)
        {
            SDL_SetRenderDrawColor(render, r, g, b, 255);
            SDL_Rect rect = {x, PlateY, PlateSizeX, PlateSizeY};
            SDL_RenderFillRect(render, &rect);
        }
        void Left(SDL_Renderer *render)
        {
            draw1(render, 255, 255, 255);

            if ((x -= PlateSizeX/10) <= con)
                x = con + 1;

            draw1(render, 30, 144, 255);
        }
        void Right(SDL_Renderer *render)
        {
            draw1(render, 255, 255, 255);

            if ((x += PlateSizeX/10) + PlateSizeX >= SCREEN_WIDTH - con)
                x = SCREEN_WIDTH - con - 1 - PlateSizeX;

            draw1(render, 30, 144, 255);
        }

    public:
        plate() {x = SCREEN_WIDTH/2 - PlateSizeX/2, y = PlateY, type = 1;}
        ~plate() {}
        void Draw(SDL_Renderer *render)
        {
            draw1(render, 30, 144, 255);
        }

        void reset (SDL_Renderer *render)
        {
            x = SCREEN_WIDTH/2 - PlateSizeX/2, y = PlateY;
            Draw(render);
        }

        bool Move(SDL_Renderer *render, GameField *Field, int key, int PlateX)
        {
            switch (key)
            {
                case 1:
                    Left(render);
                    break;

                case 2:
                    Right(render);
                    break;
            }
            return false;
        }
};

class ball: public GameObject
{
    private:
        int k, tick;
        double angle;

        void DrawCircle(SDL_Renderer *render, int X0, int Y0, int r)
        {
            int d = r * 2, x2 = r - 1, y2 = 0, tx = 1, ty = 1;
            int flag = (tx - d);

            while (x2 >= y2)
            {
                SDL_RenderDrawPoint(render, X0 + x2, Y0 - y2);
                SDL_RenderDrawPoint(render, X0 + x2, Y0 + y2);
                SDL_RenderDrawPoint(render, X0 - x2, Y0 - y2);
                SDL_RenderDrawPoint(render, X0 - x2, Y0 + y2);
                SDL_RenderDrawPoint(render, X0 + y2, Y0 - x2);
                SDL_RenderDrawPoint(render, X0 + y2, Y0 + x2);
                SDL_RenderDrawPoint(render, X0 - y2, Y0 - x2);
                SDL_RenderDrawPoint(render, X0 - y2, Y0 + x2);

                if (flag <= 0)
                {
                    ++y2;
                    flag += ty;
                    ty += 2;
                }

                if (flag > 0)
                {
                    --x2;
                    tx += 2;
                    flag += (tx - d);
                }
            }
        }

        void DrawDisc(SDL_Renderer *render, int X0, int Y0, int r)
        {
            int i;
            for (i = r; i > 0; i--)
            {
                DrawCircle(render, X0, Y0, i);
            }
        }

        void draw1(SDL_Renderer *render, int r, int g, int b)
        {
            SDL_SetRenderDrawColor(render, r, g, b, 255);
            DrawDisc(render, x, y, radius);
        }

    public:
        ball() {x = SCREEN_WIDTH/2; y = SCREEN_HEIGHT /2; angle = M_PI/2; k = 10; tick = 0; type = 0;}
        ~ball() {}
        void Draw(SDL_Renderer *render)
        {
            draw1(render, 255, 69, 0);
        }

        void reset (SDL_Renderer *render)
        {
            x = SCREEN_WIDTH/2 - PlateSizeX/2, y = SCREEN_HEIGHT /2; angle = M_PI/2;
            Draw(render);
        }
        bool Move(SDL_Renderer *render, GameField *Field, int key, int PlateX)
        {
            int k1, k2;
            if (tick == 0)
            {

                if (y + radius + sin(angle)*k >= PlateY)
                {
                    if (x >= PlateX && x <= PlateX + PlateSizeX)
                    {
                        angle = M_PI + (M_PI / 180) * (x - PlateX + 1);
                    }
                    else
                    {
                        return true;
                    }
                }

                else if (y - radius + sin(angle)*k <= con + 4)
                {
                    angle = 2 * M_PI - angle;
                }

                else if (x + radius + cos(angle)*k >= SCREEN_WIDTH - con - 4)
                {
                    angle = M_PI - angle;
                    //x = SCREEN_WIDTH - con - 4 - radius;
                }

                else if (x - radius + cos(angle)*k <= con + 4)
                {
                    angle = M_PI - angle;
                    //x = con + 4 + radius;
                }

                draw1(render, 255, 255, 255);
                if (((x - con) / 100) < 10 && ((y - radius - con) / 50) < 5)
                {
                    k1 = (x - con) / 100;
                    k2 = (y - radius - con) / 50;
                    if (Field->Level[k2][k1])
                    {
                        Field->Level[k2][k1]--;
                        Field->score += 10;

                        y = k2 * 50 + con + radius + 51;
                        angle = 2 * M_PI - angle;
                    }
                }
                if (((x - radius - con) / 100) < 10 && ((y - con) / 50) < 5)
                {
                    k1 = (x - radius - con) / 100;
                    k2 = (y - con) / 50;
                    if (Field->Level[k2][k1])
                    {
                        Field->Level[k2][k1]--;
                        Field->score += 10;

                        x = k1 * 100 + con + radius + 101;
                        angle = M_PI - angle;
                    }
                }

                if (((x + radius - con) / 100) < 10 && ((y - con) / 50) < 5)
                {
                    k1 = (x + radius - con) / 100;
                    k2 = (y - con) / 50;
                    if (Field->Level[k2][k1])
                    {
                        Field->Level[k2][k1]--;
                        Field->score += 10;

                        x = k1 * 100 + con - radius - 2;
                        angle = M_PI - angle;
                    }
                }
                if (((x - con) / 100) < 10 && ((y + radius - con) / 50) < 5)
                {
                    k1 = (x - con) / 100;
                    k2 = (y + radius - con) / 50;
                    if (Field->Level[k2][k1])
                    {
                        Field->Level[k2][k1]--;
                        Field->score += 10;

                        y = k2 * 50 + con - radius - 2;
                        angle = 2 * M_PI - angle;
                    }
                }
                Field->Draw(render);
                draw1(render, 255, 255, 255);
                if (((x += cos(angle)*k) + radius) >=  (SCREEN_WIDTH - con)) x = SCREEN_WIDTH - con - 1 - radius;
                else if ((x += cos(angle)*k) - radius <=  con) x = con + 1 + radius;
                if ((y += sin(angle)*k) - radius <= con) y = con + 1 + radius;
                draw1(render, 255, 69, 0);

            }
            if (tick++ > 3) tick = 0;
            return false;
        }
};



int main()
{
    char *fontname = "20810.ttf";
    char *fontname2 = "20051.ttf";

    if ((SDL_Init(SDL_INIT_EVERYTHING)))
    {
        cout << "Initialization Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window = nullptr;
    window = SDL_CreateWindow("ARCANOID by MadMasya", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if( window == NULL )
    {
        cout << "Window creating Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Renderer *render = nullptr ;
    render = SDL_CreateRenderer(window , -1 , SDL_RENDERER_ACCELERATED);
    if ( render == NULL )
    {
        cout << "Renderer creating Error: " << SDL_GetError() << endl ;
        return 1;
    }

    TTF_Init();
    TTF_Font *font1 = TTF_OpenFont(fontname, 150);
    if (!font1)
    {
        cout << "Error: Font not found "  << endl ;
        return 1;
    }

    TTF_Font *font2 = TTF_OpenFont(fontname2, 80);
    if (!font1)
    {
        cout << "Error: Font not found "  << endl ;
        return 1;
    }

    TTF_Font *font3 = TTF_OpenFont(fontname2, 18);
    if (!font1)
    {
        cout << "Error: Font not found "  << endl ;
        return 1;
    }

    Menu(render, font1, font2, font3);

    //Game(render);

    TTF_Quit();

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void Game (SDL_Renderer *render, TTF_Font *font1)
{
    int score, lives = 3, lvl = 0, N = 5;
    bool GameOver = false;

    element *tmp;
    List MyObjects;
    GameObject *Main_Plate;
    GameObject *Main_Ball;
    GameField Field;

    string MyLevels[N] = {"Level1", "Level2", "Level3", "Level4", "Level5"};

    SDL_Color textColor = {106, 44, 112, 0}, black = {255, 215, 0, 0}, red = {255, 0, 0, 255};

    char *Score = (char*) malloc(100 * sizeof(char));
    char *live = (char*) malloc(100 * sizeof(char));

    Main_Plate = new plate();

    Main_Ball = new ball();

    MyObjects.add_node(Main_Plate);
    MyObjects.add_node(Main_Ball);

    SDL_Texture *texture1, *texture2;
    SDL_Rect rect1, rect_heart, rect3;
    int key;
    //bool Lose;

    SDL_SetRenderDrawColor(render, 255, 215, 0, 255);
    SDL_RenderClear(render);

    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_Rect rect2 = {20, 20, 1000, 1000};
    SDL_RenderFillRect(render, &rect2);

    Main_Plate->Draw(render);
    Main_Ball->Draw(render);

    Field.Load(MyLevels[lvl]);
    Field.Draw(render);

    bool quit = false;
    SDL_Event event;
    while ( !quit )
    {
        key = 0;
        while ( SDL_PollEvent( &event ) )
        {
            if ( event.type == SDL_QUIT )
                quit = true ;

            if ( event.key.keysym.scancode == SDL_SCANCODE_ESCAPE )
                quit = true ;

            if (event.key.keysym.scancode == SDL_SCANCODE_LEFT)
            {
                key = 1;
            }
            if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT)
            {
                key = 2;
            }
        }

        tmp = MyObjects.head;
        while (tmp != NULL)
        {
            GameOver = tmp->Obj->Move(render, &Field, key, MyObjects.head->Obj->getX());
            if (GameOver)
            {
                lives--;
                SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
                SDL_RenderClear(render);
                SDL_RenderPresent(render);
                system("sleep 1");
                if(lives)
                {
                    SDL_SetRenderDrawColor(render, 255, 215, 0, 255);
                    SDL_RenderClear(render);

                    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
                    SDL_Rect rect2 = {20, 20, 1000, 1000};
                    SDL_RenderFillRect(render, &rect2);

                    Main_Ball->reset(render);
                    Main_Plate->reset(render);


                    Field.Draw(render);
                }
                else
                {
                    quit = true;
                }
            }
            tmp = tmp->next;
        }

        if(Field.is_win())
        {
            system("sleep 1");
            if(lvl++ == N)
                lvl = 0;

            Field.Load(MyLevels[lvl]);

            SDL_SetRenderDrawColor(render, 255, 215, 0, 255);
            SDL_RenderClear(render);

            SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
            SDL_Rect rect2 = {20, 20, 1000, 1000};
            SDL_RenderFillRect(render, &rect2);

            Field.Draw(render);

            Main_Ball->reset(render);
            Main_Plate->reset(render);
        }


        get_text_and_rect(render, 970, 0, live, font1, &texture2, &rect3, black);
        SDL_RenderCopy(render, texture2, NULL, &rect3);
        sprintf(live, "%s %d","Жизни: ", lives);
        get_text_and_rect(render, 950, 0, live, font1, &texture2, &rect3, red);
        SDL_RenderCopy(render, texture2, NULL, &rect3);


        get_text_and_rect(render, 0, 0, Score, font1, &texture1, &rect1, black);
        SDL_RenderCopy(render, texture1, NULL, &rect1);
        sprintf(Score, "%s %d","Счет: ", Field.score);
        get_text_and_rect(render, 0, 0, Score, font1, &texture1, &rect1, textColor);
        SDL_RenderCopy(render, texture1, NULL, &rect1);

        SDL_RenderPresent(render);
        SDL_Delay( 5 );
    }

    free(Score);
    free(live);
    AddResult (Field.score);

}

void DrawMenu (SDL_Renderer *render, TTF_Font *font1, TTF_Font *font2)
{
    SDL_Color textColor = {106, 44, 112, 0};

    SDL_Texture *texture1, *texture2, *texture3, *texture4;
    SDL_Rect rect1, rect2, rect3, rect4, rect5;

    SDL_SetRenderDrawColor(render, 249, 237, 105, 255);
    SDL_RenderClear(render);

    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    get_text_and_rect(render, 170, 100, "Arcanoid", font1, &texture1, &rect1, textColor);
    SDL_RenderCopy(render, texture1, NULL, &rect1);



    SDL_SetRenderDrawColor(render, 184, 59, 94, 255);
    textColor = {184, 59, 94, 255};

    get_text_and_rect(render, 405, 400, "Играть", font2, &texture2, &rect3, textColor);
    SDL_RenderCopy(render, texture2, NULL, &rect3);

    get_text_and_rect(render, 360, 550, "Рекорды", font2, &texture3, &rect4, textColor);
    SDL_RenderCopy(render, texture3, NULL, &rect4);

    get_text_and_rect(render, 405, 700, "Выход", font2, &texture4, &rect5, textColor);
    SDL_RenderCopy(render, texture4, NULL, &rect5);

    rect2 = {320, 400, 400, 100};
    SDL_RenderDrawRect(render, &rect2);
    rect2 = {321, 401, 398, 98};
    SDL_RenderDrawRect(render, &rect2);
    rect2 = {322, 402, 396, 96};
    SDL_RenderDrawRect(render, &rect2);
    rect2 = {323, 403, 394, 94};
    SDL_RenderDrawRect(render, &rect2);

    rect2 = {320, 550, 400, 100};
    SDL_RenderDrawRect(render, &rect2);
    rect2 = {321, 551, 398, 98};
    SDL_RenderDrawRect(render, &rect2);
    rect2 = {322, 552, 396, 96};
    SDL_RenderDrawRect(render, &rect2);
    rect2 = {323, 553, 394, 94};
    SDL_RenderDrawRect(render, &rect2);

    rect2 = {320, 700, 400, 100};
    SDL_RenderDrawRect(render, &rect2);
    rect2 = {321, 701, 398, 98};
    SDL_RenderDrawRect(render, &rect2);
    rect2 = {322, 702, 396, 96};
    SDL_RenderDrawRect(render, &rect2);
    rect2 = {323, 703, 394, 94};
    SDL_RenderDrawRect(render, &rect2);

    SDL_RenderPresent(render);
}

void Menu(SDL_Renderer *render, TTF_Font *font1, TTF_Font *font2, TTF_Font *font3)
{
    int mouseX, mouseY;
    bool quit = false, b1 = false, b2 = false, b3 = false;
    SDL_Event event;
    SDL_Color textColor = {184, 59, 94, 255};

    SDL_Texture *texture1, *texture2, *texture3, *texture4;
    SDL_Rect rect1, rect2, rect3, rect4, rect5;

    get_text_and_rect(render, 405, 400, "Играть", font2, &texture2, &rect3, textColor);

    get_text_and_rect(render, 360, 550, "Рекорды", font2, &texture3, &rect4, textColor);

    get_text_and_rect(render, 405, 700, "Выход", font2, &texture4, &rect5, textColor);

    DrawMenu(render, font1, font2);

    while ( !quit )
    {
        while ( SDL_PollEvent( &event ) )
        {
            if ( event.type == SDL_QUIT )
                quit = true ;

            if ( event.key.keysym.scancode == SDL_SCANCODE_ESCAPE )
                quit = true ;

            if(event.type == SDL_MOUSEMOTION)
            {
                mouseX = event.motion.x;
                mouseY = event.motion.y;
                //cout << mouseX << endl;

                if ((mouseX > 320) && (mouseX < 720) && (mouseY < 500) && (mouseY > 400))
                {
                    SDL_SetRenderDrawColor(render, 240, 138, 93, 255);
                    rect2 = {324, 404, 392, 92};
                    SDL_RenderFillRect(render, &rect2);
                    SDL_RenderCopy(render, texture2, NULL, &rect3);
                    b1 = true;
                }
                else if (b1)
                {
                    SDL_SetRenderDrawColor(render, 249, 237, 105, 255);
                    rect2 = {324, 404, 392, 92};
                    SDL_RenderFillRect(render, &rect2);
                    SDL_RenderCopy(render, texture2, NULL, &rect3);
                    b1 = false;
                }

                if ((mouseX > 320) && (mouseX < 720) && (mouseY < 650) && (mouseY > 550))
                {
                    SDL_SetRenderDrawColor(render, 240, 138, 93, 255);
                    rect2 = {324, 554, 392, 92};
                    SDL_RenderFillRect(render, &rect2);
                    SDL_RenderCopy(render, texture3, NULL, &rect4);
                    b2 = true;
                }
                else if (b2)
                {
                    SDL_SetRenderDrawColor(render, 249, 237, 105, 255);
                    rect2 = {324, 554, 392, 92};
                    SDL_RenderFillRect(render, &rect2);
                    SDL_RenderCopy(render, texture3, NULL, &rect4);
                    b2 = false;
                }

                if ((mouseX > 320) && (mouseX < 720) && (mouseY < 800) && (mouseY > 700))
                {
                    SDL_SetRenderDrawColor(render, 240, 138, 93, 255);
                    rect2 = {324, 704, 392, 92};
                    SDL_RenderFillRect(render, &rect2);
                    SDL_RenderCopy(render, texture4, NULL, &rect5);
                    b3 = true;
                }
                else if (b3)
                {
                    SDL_SetRenderDrawColor(render, 249, 237, 105, 255);
                    rect2 = {324, 704, 392, 92};
                    SDL_RenderFillRect(render, &rect2);
                    SDL_RenderCopy(render, texture4, NULL, &rect5);
                    b3 = false;
                }
            }

            if(event.button.button == SDL_BUTTON_LEFT)
            {
                if ((mouseX > 320) && (mouseX < 720) && (mouseY < 500) && (mouseY > 400))
                {
                    Game(render, font3);
                    DrawMenu(render, font1, font2);
                }

                if ((mouseX > 320) && (mouseX < 720) && (mouseY < 650) && (mouseY > 550))
                {
                    LoadRecords(render, font2, font3);
                    DrawMenu(render, font1, font2);
                }

                if ((mouseX > 320) && (mouseX < 720) && (mouseY < 800) && (mouseY > 700))
                {
                    quit = true;
                }
            }
        }
        SDL_RenderPresent(render);
        SDL_Delay(5);
    }

}

void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect, SDL_Color textColor)
{
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor1 = {0, 0, 0, 255};

    surface = TTF_RenderUTF8_Solid(font, text, textColor);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}

void LoadRecords(SDL_Renderer *render, TTF_Font *font1, TTF_Font *font2)
{
    int *records = new int [10];
    int val, tmp, i;
    bool quite = false;
    char *str = (char*) malloc(20 * sizeof(char));

    string filename = "Records.txt";
    ifstream File;

    File.open(filename, ios_base::out);
    if (!File)
    {
        cout << "File open Error" << endl;
        return;
    }

    for (i = 0; i < 10; i++)
    {
        records[i] = 0;
    }

    while (File >> val)
    {
        for (i = 0; i < 10; i++)
        {
            if (val > records[i])
            {
                tmp = records[i];
                records[i] = val;
                val = tmp;
            }
        }
    }

    File.close();

    SDL_Color textColor = {106, 44, 112, 0};

    SDL_Texture *texture1, *texture2;
    SDL_Rect rect1, rect2;

    SDL_SetRenderDrawColor(render, 249, 237, 105, 255);
    SDL_RenderClear(render);

    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    get_text_and_rect(render, 360, 50, "Рекорды", font1, &texture1, &rect1, textColor);
    SDL_RenderCopy(render, texture1, NULL, &rect1);

    textColor = {106, 44, 112, 0};

    for(i = 0; i < 10; i++)
    {
        sprintf(str, "Top %d: %d", i+1, records[i]);
        get_text_and_rect(render, 350, 200 + 75 * i, str, font1, &texture2, &rect2, textColor);
        SDL_RenderCopy(render, texture2, NULL, &rect2);

    }

    SDL_RenderPresent(render);

    delete []records;
    free(str);
    system("sleep 2");
}

void AddResult (int a)
{
    int exit = 1;
    char* text;

    if(!a)
        return;

    string filename = "Records.txt";


    ofstream File;

    File.open(filename, ios_base::app);
    if (!File)
    {
        cout << "File open Error" << endl;
        return;
    }

    File << a << " ";

    File.close();

    /*while (exit)
    {
        text = Vvod();
        if (text == NULL)
            break;
        cout << text << endl;

    }

    //cout << Text << endl;*/

}

/*char* Vvod()
{

    char text[100];
    SDL_Event event;
    int n = 0, k = 0;


    while (SDL_PollEvent(&event))
    {
        if(event.type == SDL_KEYDOWN )
        {
            if (event.key.keysym.sym == SDLK_UNDERSCORE)
            {
                             text[k]='_';
                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_a)
            {
                             text[k]='a';
                             k++; n++;
                        }
             if (event.key.keysym.sym == SDLK_b)
            {
                             text[k]='b';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_c)
            {
                             text[k]='c';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_d)
            {
                             text[k]='d';

                             k++; n++;
                        }

            if (event.key.keysym.sym == SDLK_e)
            {
                             text[k]='e';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_f)
            {
                             text[k]='f';

                             k++; n++;
                        }
             if (event.key.keysym.sym == SDLK_g)
            {
                             text[k]='g';

                             k++; n++;
                        }
             if (event.key.keysym.sym == SDLK_h)
            {
                             text[k]='h';

                             k++; n++;
                        }
             if (event.key.keysym.sym == SDLK_i)
            {
                             text[k]='i';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_j)
            {
                             text[k]='j';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_k)
            {
                             text[k]='k';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_l)
            {
                             text[k]='l';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_m)
            {
                             text[k]='m';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_n)
            {
                             text[k]='n';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_o)
            {
                             text[k]='o';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_p)
            {
                             text[k]='p';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_q)
            {
                             text[k]='q';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_r)
            {
                             text[k]='r';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_s)
            {
                             text[k]='s';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_t)
            {
                             text[k]='t';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_u)
            {
                             text[k]='u';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_v)
            {
                             text[k]='v';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_w)
            {
                             text[k]='w';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_x)
            {
                             text[k]='x';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_y)
            {
                             text[k]='y';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_z)
            {
                             text[k]='z';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_1 || event.key.keysym.sym == SDLK_KP_1)
            {
                             text[k]='1';

                             //s=(s+1)*10;
                             k++; n++;
                        }
             if (event.key.keysym.sym == SDLK_2 || event.key.keysym.sym == SDLK_KP_2)
            {
                             text[k]='2';

                             //s=(s+2)*10;
                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_3 || event.key.keysym.sym == SDLK_KP_3)
            {
                             text[k]='3';

                             //s=(s+3)*10;
                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_4 || event.key.keysym.sym == SDLK_KP_4)
            {
                             text[k]='4';

                             //s=(s+4)*10;
                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_5 || event.key.keysym.sym == SDLK_KP_5)
            {
                             text[k]='5';

                             //s=(s+5)*10;
                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_6 || event.key.keysym.sym == SDLK_KP_6)
            {
                             text[k]='6';

                             //s=(s+6)*10;
                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_7 || event.key.keysym.sym == SDLK_KP_7)
            {
                             text[k]='7';

                             //s=(s+7)*10;
                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_8 || event.key.keysym.sym == SDLK_KP_8)
            {
                             text[k]='8';

                             //s=(s+8)*10;
                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_9 || event.key.keysym.sym == SDLK_KP_9)
            {
                             text[k]='9';

                             //s=(s+9)*10;
                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_0 || event.key.keysym.sym == SDLK_KP_0)
            {
                             text[k]='0';

                             //s=(s+0)*10;
                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_EQUALS || event.key.keysym.sym == SDLK_KP_PLUS)
            {
                             text[k]='+';

                             k++; n++;
                        }
            if (event.key.keysym.sym == SDLK_MINUS || event.key.keysym.sym == SDLK_KP_MINUS)
            {
                             text[k]='-';

                             k++; n++;
                        }
            }
            if (event.key.keysym.sym == SDLK_KP_ENTER)
            {
                        return NULL;
            }
    }
}*/
