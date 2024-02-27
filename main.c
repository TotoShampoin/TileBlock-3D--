#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_rotozoom.h>

int main(int argc, char *argv[])
{
    unsigned char map[16][16][16], map2[16][16][16];
    unsigned char mapLoc[256], picLoc[256], mapLoc2[300], picLoc2[300];
    int x = 0, y = 0, z = 0, pick = 0, s = 0;
    int curx = 0, cury = 0, curz = 0;

    FILE *config = NULL;
    config = fopen("config.txt","r");
    while(fgetc(config)!= '\"');
    for(x=0;x<256;x++)
    {
        mapLoc[x] = fgetc(config);
        if(strstr(mapLoc, ".PTB"))
        {
            mapLoc[x+1] = '\0';
            x = 256;
        }
    }
    while(fgetc(config)!= '\"'); while(fgetc(config)!= '\"');
    for(x=0;x<256;x++)
    {
        picLoc[x] = fgetc(config);
        if(strstr(picLoc, ".png"))
        {
            picLoc[x+1] = '\0';
            x = 256;
        }
    }
    fclose(config);

    FILE *mapFile = NULL;
    sprintf(mapLoc2, "Maps/%s", mapLoc);
    mapFile = fopen(mapLoc2,"rb");
        fgets(map, 4097, mapFile);
    fclose(mapFile);

    SDL_Surface *screen = NULL, *tileSet = NULL, *curs = NULL, *hint = NULL, *text = NULL, *circle = NULL;
    SDL_Surface *block = NULL, *rotblock = NULL;
    SDL_Rect tileSetInfo, tileSetPos, hintPos, textPos, circlePos;
    SDL_Rect blank;
    SDL_Event event;
    TTF_Font *pixel = NULL, *pixel2 = NULL;
    SDL_Color color = {0,0,0};

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    screen = SDL_SetVideoMode(1280, 640, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("TileBlock 3D", NULL);
    SDL_WM_SetIcon(IMG_Load(("Icon.png")), NULL);

    sprintf(picLoc2, "Cursor/%s", picLoc);
        curs = IMG_Load(picLoc2);
    sprintf(picLoc2, "TileSets/%s", picLoc);
        tileSet = IMG_Load(picLoc2);
            tileSetInfo.h = tileSet->h/16; tileSetInfo.w = tileSet->w/16;
    hint = IMG_Load("Hint.png");
        hintPos.x = 0; hintPos.y = screen->h - 120;
    circle = IMG_Load("Circle.png");
        circlePos.x = screen->w-310; circlePos.y = screen->h - 310;

    blank.x = 0; blank.y = 0;
    block = SDL_CreateRGBSurface(SDL_HWSURFACE, tileSetInfo.w, tileSetInfo.h, 32, 0, 0, 0, 0);

    pixel = TTF_OpenFont("Font/courbd.ttf",32);
    pixel2 = TTF_OpenFont("Font/cour.ttf",10);

    char blockVal[32] = "";
    int cont = 1, time, timer;
    double angle = 0.0;

    SDL_EnableKeyRepeat(300, 20);
    while(cont)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            cont = 0;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_PAGEUP:
                if(curz<15)
                    curz++;
                else
                    curz=0;
                break;
            case SDLK_PAGEDOWN:
                if(curz>0)
                    curz--;
                else
                    curz=15;
                break;

            case SDLK_UP:
                if(cury>0)
                    cury--;
                else
                    cury=15;
                break;
            case SDLK_DOWN:
                if(cury<15)
                    cury++;
                else
                    cury=0;
                break;
            case SDLK_LEFT:
                if(curx>0)
                    curx--;
                else
                    curx=15;
                break;
            case SDLK_RIGHT:
                if(curx<15)
                    curx++;
                else
                    curx=0;
                break;

            case SDLK_KP4:
                if(map[curz][cury][curx]>0)
                    map[curz][cury][curx] --;
                else
                    map[curz][cury][curx] = 255;
                break;
            case SDLK_KP6:
                if(map[curz][cury][curx]<255)
                    map[curz][cury][curx] ++;
                else
                    map[curz][cury][curx] = 0;
                break;

            case SDLK_KP7:
                for(x=0 ; x<16 ; x++)
                    for(y=0 ; y<16 ; y++)
                        for(z=0 ; z<16 ; z++)
                            map2[x][y][z] = map[x][z][15-y];
                for(x=0 ; x<16 ; x++)
                    for(y=0 ; y<16 ; y++)
                        for(z=0 ; z<16 ; z++)
                            map[x][y][z] = map2[x][y][z];
                break;
            case SDLK_KP9:
                for(x=0 ; x<16 ; x++)
                    for(y=0 ; y<16 ; y++)
                        for(z=0 ; z<16 ; z++)
                            map2[x][y][z] = map[x][15-z][y];
                for(x=0 ; x<16 ; x++)
                    for(y=0 ; y<16 ; y++)
                        for(z=0 ; z<16 ; z++)
                            map[x][y][z] = map2[x][y][z];
                break;

            case SDLK_s:
                mapFile = fopen(mapLoc2,"wb");
                    fwrite(map[0],sizeof(char),4097,mapFile);
                fclose(mapFile);
                s = 1;
                timer = time;
                sprintf(picLoc2, "Cursor/%s", picLoc);
                    curs = IMG_Load(picLoc2);
                sprintf(picLoc2, "TileSets/%s", picLoc);
                    tileSet = IMG_Load(picLoc2);
                        tileSetInfo.h = tileSet->h/16; tileSetInfo.w = tileSet->w/16;
                break;
            case SDLK_r:
                for(x=0;x<16;x++)
                    for(y=0;y<16;y++)
                        for(z=0;z<16;z++)
                            map[x][y][z] = 0;
                s = 2;
                timer = time;
                sprintf(picLoc2, "Cursor/%s", picLoc);
                    curs = IMG_Load(picLoc2);
                sprintf(picLoc2, "TileSets/%s", picLoc);
                    tileSet = IMG_Load(picLoc2);
                        tileSetInfo.h = tileSet->h/16; tileSetInfo.w = tileSet->w/16;
                break;
            case SDLK_l:
                mapFile = fopen(mapLoc2,"rb");
                    fgets(map, 4097, mapFile);
                fclose(mapFile);
                s = 3;
                timer = time;
                sprintf(picLoc2, "Cursor/%s", picLoc);
                    curs = IMG_Load(picLoc2);
                sprintf(picLoc2, "TileSets/%s", picLoc);
                    tileSet = IMG_Load(picLoc2);
                        tileSetInfo.h = tileSet->h/16; tileSetInfo.w = tileSet->w/16;
                break;


            case SDLK_ESCAPE:
                cont = 0;
                break;
            }
            break;
        }
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 128, 192, 255));

        for(x=0 ; x < 16 ; x++)
            for(y=0 ; y < 16 ; y++)
            {
                for(z=0 ; z < 16 ; z++)
                {
                    pick = map[z][y][x];
                    tileSetPos.x = (tileSet->w/2) + x*(tileSetInfo.w/2) - y*(tileSetInfo.w/2); tileSetPos.y = (tileSet->h/2) + x*(tileSetInfo.h/4) + y*(tileSetInfo.h/4) - z*(tileSetInfo.h/2);

                    tileSetInfo.x = pick%16 * tileSetInfo.w; tileSetInfo.y = pick/16 * tileSetInfo.h;

                    if((map[z+1][y][x]==0 || map[z][y+1][x]==0 || map[z][y][x+1]==0) || (z==15 || y==15 || x==15))
                        SDL_BlitSurface(tileSet, &tileSetInfo, screen, &tileSetPos);
                    if(curz==z)
                    {
                        if(pick)
                            SDL_BlitSurface(curs, NULL, screen, &tileSetPos);
                        tileSetPos.x = (tileSet->w + tileSet->w/4) + x*(tileSetInfo.w+1); tileSetPos.y = 8 + y*(tileSetInfo.h+1);
                        SDL_BlitSurface(tileSet, &tileSetInfo, screen, &tileSetPos);

                        sprintf(blockVal, "%d", pick);
                        text = TTF_RenderText_Blended(pixel2,blockVal, color);
                        SDL_BlitSurface(text, NULL, screen, &tileSetPos);

                        if(curx==x && cury==y)
                        {
                            SDL_BlitSurface(curs, NULL, screen, &tileSetPos);
                            tileSetPos.x = (tileSet->w/2) + x*(tileSetInfo.w/2) - y*(tileSetInfo.w/2); tileSetPos.y = (tileSet->h/2) + x*(tileSetInfo.h/4) + y*(tileSetInfo.h/4) - z*(tileSetInfo.h/2);
                            SDL_BlitSurface(curs, NULL, screen, &tileSetPos);
                        }
                    }
                }
            }

        sprintf(blockVal, "Block ID = %d", map[curz][cury][curx]);
        text = TTF_RenderText_Blended(pixel,blockVal, color);
        textPos.x = (tileSet->w + tileSet->w/4)-200 ; textPos.y = screen->h - 240;
            SDL_BlitSurface(text, NULL, screen, &textPos);
        sprintf(blockVal, "X=%d Y=%d Z=%d", curx, cury, curz);
        text = TTF_RenderText_Blended(pixel,blockVal, color);
        textPos.x = (tileSet->w + tileSet->w/4)-200 ; textPos.y = screen->h - 200;
            SDL_BlitSurface(text, NULL, screen, &textPos);
        if(s==1)
        {
            color.g = 32;
            sprintf(blockVal, "Saved", curx, cury, curz);
            text = TTF_RenderText_Blended(pixel,blockVal, color);
            textPos.x = (tileSet->w + tileSet->w/4)-200 ; textPos.y = screen->h - 160;
                SDL_BlitSurface(text, NULL, screen, &textPos);
            color.g = 0;
        }
        if(s==2)
        {
            color.g = 32;
            sprintf(blockVal, "Reset", curx, cury, curz);
            text = TTF_RenderText_Blended(pixel,blockVal, color);
            textPos.x = (tileSet->w + tileSet->w/4)-200 ; textPos.y = screen->h - 160;
                SDL_BlitSurface(text, NULL, screen, &textPos);
            color.g = 0;
        }
        if(s==3)
        {
            color.g = 32;
            sprintf(blockVal, "Loaded", curx, cury, curz);
            text = TTF_RenderText_Blended(pixel,blockVal, color);
            textPos.x = (tileSet->w + tileSet->w/4)-200 ; textPos.y = screen->h - 160;
                SDL_BlitSurface(text, NULL, screen, &textPos);
            color.g = 0;
        }
        time = SDL_GetTicks();
        if(time>timer+1000)
            s = 0;

        SDL_BlitSurface(circle, NULL, screen, &circlePos);
        for(x = -12 ; x < 12 ; x++)
        {
            pick = map[curz][cury][curx] + x;
            if(pick>255)
                pick = pick-256;
            if(pick<0)
                pick = pick+256;

            angle = x * 15;

            tileSetInfo.x = pick%16 * tileSetInfo.w; tileSetInfo.y = pick/16 * tileSetInfo.h;

            SDL_FillRect(block, NULL, SDL_MapRGB(block->format, 0, 0, 0));
            SDL_BlitSurface(tileSet, &tileSetInfo, block, &blank);
            SDL_SetColorKey(block, SDL_SRCCOLORKEY, SDL_MapRGB(block->format,0,0,0));

            rotblock = rotozoomSurface(block, angle, 1.0, 1);
            SDL_SetColorKey(rotblock, SDL_SRCCOLORKEY, SDL_MapRGB(rotblock->format,0,0,0));

            tileSetPos.x = screen->w - 155 + sin(x*(30*3.14/360))*128 - rotblock->w/2; tileSetPos.y = screen->h - 155 + cos(x*(30*3.14/360))*128 -rotblock->h/2;
            SDL_BlitSurface(rotblock, NULL, screen, &tileSetPos);
        }
        pick = map[curz][cury][curx];
        tileSetInfo.x = pick%16 * tileSetInfo.w; tileSetInfo.y = pick/16 * tileSetInfo.h;

        SDL_FillRect(block, NULL, SDL_MapRGB(block->format, 0, 0, 0));
            SDL_BlitSurface(tileSet, &tileSetInfo, block, &blank);
            SDL_SetColorKey(block, SDL_SRCCOLORKEY, SDL_MapRGB(block->format,0,0,0));

            rotblock = rotozoomSurface(block, 0.0, 3.0, 0);
            SDL_SetColorKey(rotblock, SDL_SRCCOLORKEY, SDL_MapRGB(rotblock->format,0,0,0));

        tileSetPos.x = screen->w - 155 - rotblock->w/2; tileSetPos.y = screen->h - 155 - rotblock->h/2;
        SDL_BlitSurface(rotblock, NULL, screen, &tileSetPos);
        SDL_BlitSurface(hint, NULL, screen, &hintPos);
        SDL_Flip(screen);
    }

    SDL_FreeSurface(tileSet); SDL_FreeSurface(curs); SDL_FreeSurface(hint);
    SDL_FreeSurface(circle); SDL_FreeSurface(text);
    TTF_CloseFont(pixel);
    SDL_FreeSurface(screen);
    SDL_Quit(); TTF_Quit();

    return EXIT_SUCCESS;
}
