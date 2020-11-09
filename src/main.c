/**
 * Hello World Example
 * Created With Genesis-Code extension for Visual Studio Code
 * Use "Genesis Code: Compile" command to compile this program.
 **/
#include <genesis.h>
#include <resources.h>

int snakeX = 20;
int snakeY = 14;
int direccion = 1;
int frutaX = 5;
int frutaY = 5;
int debug = 0;
char tempText[];
int muerto;
int largoCola = 1;
int colasX[250];
int colasY[250];
int seccion = 0;

#define SCREENXMAX 39
#define SCREENYMAX 26
#define DERECHA 1
#define ARRIBA 2
#define IZQUIERDA 3
#define ABAJO 4
#define SECCIONMENU 0
#define SECCIONJUEGO 1
#define SECCIONGAMEOVER 2

void myJoyHandler( u16 joy, u16 changed, u16 state)
{
	if (joy == JOY_1)
	{

		if (state & BUTTON_START)
		{
			if (seccion==SECCIONMENU)
            {
                seccion = SECCIONJUEGO;
            }

		}
		else if (state & BUTTON_DOWN && direccion != ARRIBA)
		{
           direccion = ABAJO;
		}
        else if (state & BUTTON_UP && direccion != ABAJO)
		{
            direccion = ARRIBA;
		}
        else if (state & BUTTON_LEFT && direccion != DERECHA)
        {
            direccion = IZQUIERDA;
        }
        else if (state & BUTTON_RIGHT && direccion != IZQUIERDA)
        {
            direccion = DERECHA;
        }

	}
}

int main()
{

    JOY_init();
    JOY_setEventHandler(&myJoyHandler);

    VDP_setPalette(PAL1, imgSnake.palette->data);

    VDP_drawImage(VDP_PLAN_A, &imgSnake, 13, 2);

    while(1)
    {

        int parpadeoMenu = 0;

        PAL_setColor(0, RGB24_TO_VDPCOLOR(0x0033cc));
        VDP_setBackgroundColor(0);

        

        while (seccion==SECCIONMENU)
        {
            if (parpadeoMenu==0)
            {
                VDP_drawText("Presiona START para comenzar!!", 5, 22);
                parpadeoMenu = 1;
            } 
            else
            {
                VDP_clearText(5, 22, 30);
                parpadeoMenu = 0;
            }

            waitMs(250);
        }

        drawBorders();

        while (muerto==0)
        {
            // CLEAR SCREEN
            funClearScreen();
        
            // LOGIC
            funLogic();

            // DRAW
            funDraw();

            VDP_waitVSync();

            waitMs(100);

        }
            
        // MUERTE

        seccion = SECCIONGAMEOVER;

        funClearScreen();

        PAL_setColor(1, RGB24_TO_VDPCOLOR(0xcc0000));
        VDP_setBackgroundColor(1);

        VDP_drawText("Game over!!", 15, 8);

        uintToStr(largoCola, tempText, 0);
        VDP_drawText("Tu score fue de:", 9, 12);
        VDP_drawText(tempText, 29, 12);

        VDP_drawText("Start para volver a jugar!!", 6, 18);

        JOY_waitPressTime(JOY_ALL, BUTTON_START, 0);

        seccion = SECCIONMENU;

        SYS_hardReset();

    }

    return (0);
}



int drawBorders(){

    int n;
    for (n=0;n<SCREENXMAX+1;n++)
        VDP_drawText("X", n, 0);

    for (n=0;n<SCREENXMAX+1;n++)
        VDP_drawText("X", n, SCREENYMAX);

    for (n=1;n<SCREENYMAX;n++)
        VDP_drawText("X", 0, n);

    for (n=1;n<SCREENYMAX;n++)
        VDP_drawText("X", SCREENXMAX, n);

    return (0);
}


int funDraw()
{
    VDP_drawText("O", snakeX, snakeY);
    VDP_drawText("*", frutaX, frutaY);

    uintToStr(largoCola, tempText, 0);
    VDP_drawText("Score:", 14, 27);
    VDP_drawText(tempText, 22, 27);

    // dibujar cola

    if (largoCola>0)
    {
        int n;
        for (n=0;n<largoCola-1;n++)
        {
            VDP_drawText("o", colasX[n], colasY[n]);
        }
        
    }
    
    if (debug)
    {
        VDP_drawText("Fx:", 1, 27);
        uintToStr(frutaX, tempText, 0);
        VDP_drawText(tempText, 4, 27);
        

        VDP_drawText("Fy:", 8, 27);
        uintToStr(frutaY, tempText, 0);
        VDP_drawText(tempText, 11, 27);

        VDP_drawText("Sx:", 28, 27);
        uintToStr(snakeX, tempText, 0);
        VDP_drawText(tempText, 31, 27);


        VDP_drawText("Sy:", 34, 27);
        uintToStr(snakeY, tempText, 0);
        VDP_drawText(tempText, 37, 27);
    }

    return (0);
}

void funClearScreen()
{
    VDP_clearTextArea(1, 1, SCREENXMAX - 1, SCREENYMAX - 1);
    //VDP_clearText(snakeX, snakeY, 1);
    //VDP_clearText(frutaX, frutaY, 1);
}

void funFrutaComida()
{
    frutaX = (random() %  (35-4+1)+1);
    frutaY = (random() %  (25-4+1)+1);

    largoCola++;
}

int funLogic()
{
    if (!muerto)
    {

    // actualizar cola

        if (largoCola>0)
        {
            int n;

            for (n=largoCola;n>0;n--)
            {
                colasX[n] = colasX[n-1];
                colasY[n] = colasY[n-1];
            }
         
            colasX[0] = snakeX;
            colasY[0] = snakeY;
        }
    
        switch (direccion)
        {
        case ARRIBA:
            snakeY--;    
            break;

        case ABAJO:
            snakeY++;
            break;

        case DERECHA:
            snakeX++;
            break;

        case IZQUIERDA:
        snakeX--;
            break;
        
        default:
            break;
        }
    }

    // colisiones

    // MUERTE 
    // TOCAR BORDE

    if (snakeX == 0 || snakeX == SCREENXMAX || snakeY == 0 || snakeY == SCREENYMAX)
    {
        muerto = 1;
    }

    // tocar propia cola

    int n;
    for (n=0;n<largoCola;n++)
    {
        if (snakeX == colasX[n] & snakeY == colasY[n])
        { 
            muerto = 1;
        }
    }

    // al comer fruta

    if (snakeX == frutaX && snakeY == frutaY)
    {
        funFrutaComida();
    }

    return (0);
}