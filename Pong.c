int main() {
    
    void printScreen(int bottomScreen[], int topScreen[]);
    void paddleController(int bottomScreen[], int topScreen[], int paddlecentre1, int paddlecentre2, int controlside);
    int resethasrun = 0;
    int controlside = 0;
    int paddlecentre1 = 15;
    int paddlecentre2 = 15;
    int balldirection = 0;
    int ballpositionx = 32768;
    int ballrow = 15;
    // add 96 * 32 clock ticks here
    resethasrun = 1;
    //Make this only run once by placing the rest of the code in a loop that only ends when the game is over
    

    unsigned int bottomScreen[16] = {2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 4294967295};
    unsigned int topScreen[16] = {4294967295, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649};
    
    //Putting the paddlecontroller here will meen it will overwrite the exmpty values, this means that the game will clear old paddlecontroller LEDS
    paddleController(bottomScreen, topScreen, paddlecentre1, paddlecentre2, controlside);
    controlside = 1;
    paddleController(bottomScreen, topScreen, paddlecentre1, paddlecentre2, controlside);
    printScreen(bottomScreen, topScreen);
    
    return 0;

}

void printScreen(int bottomScreen[], int topScreen[]){

void rowSelector(int rowSelector);
unsigned int rowDecimal = 0;
int rowDecimalOverflow = 0;
//Insert latch flip here


for (int i = 0; i<2; i++){
    for (int j = 0; j<16; j++){
        rowSelector(j);
        //Every time i increments, the row selector must increase by 1
        for (int k = 0; k<3; k++){
            if (i == 0){
                rowDecimal = bottomScreen[j];
            }
            else {
                rowDecimal = topScreen[j];
            }

            for (int l = 0; l<32; l++){
                rowDecimalOverflow = rowDecimal % 2;
                rowDecimal = rowDecimal/2;
                //since there is a constant border the int values will always be at least 32 binary bits long#
                if (rowDecimalOverflow == 1){
                    //Insert set commands here
                    printf("1"); //temp
                    
                }
                else{
                    //insert clear command here
                    printf("0"); //temp
                    
                }
            }
            printf("\n"); //temp replacement for row selector
        }
    
    
    }
}


}

void rowSelector(int rowSelector) {
    for (int r = 0; r < 4; r++) {
        int rowOverflow;
        rowOverflow = rowSelector % 2;
        rowSelector = rowSelector / 2;
        if (rowOverflow == 1) {
            switch (r) {
                case 0: printf("%d", 1); break; // insert c2
                case 1: printf("%d", 1); break; // insert c3
                case 2: printf("%d", 1); break; // insert c4 ;)
                case 3: printf("%d", 1); break; // insert c5 ect
            }
        } else {
            switch (r) {
                case 0: printf("%d", 0); break;
                case 1: printf("%d", 0); break;
                case 2: printf("%d", 0); break;
                case 3: printf("%d", 0); break;
            }
        }
    }
    printf("\n");
}

//If you want to reset it set to clear and clock it a shitton of times 192*32

//When only a single line changes, should I refresh the entire page or just that line

void paddleController(int bottomScreen[], int topScreen[], int paddlecentre1, int paddlecentre2, int controlside){
    
    // rows will be numbered 0-31
    // paddle centre should be 2-29
    int goUp = 0; 
    int goDown = 0; 
    int incrementAmount = 0;

    int paddlecentre = 0;

    if (controlside == 0){
        goUp = 0; //add input player 1 here
        goDown = 0; 
        incrementAmount = 4;
        paddlecentre = paddlecentre1;
    }
    else if (controlside == 1){
        goUp = 1; //add input player 2 here
        goDown = 1;
        incrementAmount = 536870912;
        paddlecentre = paddlecentre2;
    }
    
    // control side 0 is left

    if (goUp != 0){
        paddlecentre = paddlecentre - 1;
        if (paddlecentre == 1){
            paddlecentre = 2;
        }
        //add delay at the end
    }
    else if (goDown != 0){
        paddlecentre = paddlecentre + 1;
        if (paddlecentre == 63){
            paddlecentre = 62;
        }
    }

    int paddleup = paddlecentre - 1;
    int paddledown = paddlecentre + 1;

    //middle tiles cause problems, there are only 2 that overlap however, those being 15 and 16

    if (paddlecentre == 15){
        topScreen[14] = topScreen[14] + incrementAmount;
        topScreen[15] = topScreen[15] + incrementAmount;
        bottomScreen[0] = bottomScreen[0] + incrementAmount;
    }
    else if (paddlecentre == 16){
        topScreen[15] = topScreen[15] + incrementAmount;
        bottomScreen[0] = bottomScreen[0] + incrementAmount;
        bottomScreen[1] = bottomScreen[1] + incrementAmount;
    }
    else if (paddlecentre < 15){
        topScreen[paddleup] = topScreen[paddleup] + incrementAmount;
        topScreen[paddlecentre] = topScreen[paddlecentre] + incrementAmount;
        topScreen[paddledown] = topScreen[paddledown] + incrementAmount;
    }
    else if (paddlecentre > 16){
        bottomScreen[paddleup % 15] = bottomScreen[paddleup % 15] + incrementAmount;
        bottomScreen[paddlecentre % 15] = bottomScreen[paddlecentre % 15] + incrementAmount;
        bottomScreen[paddledown % 15] = bottomScreen[paddledown % 15] + incrementAmount;
    }
    
    if (controlside == 0){
        paddlecentre1 = paddlecentre;
    }
    else if (controlside == 1){
        paddlecentre2 = paddlecentre;
    }
}

void ball(int balldirection, int ballpositionx, int ballrow, int bottomScreen[], int topScreen[], int paddlecentre1, int paddlecentre2, int resethasrun){
    // ball direction 0 is up left, 1 is up right, 2 is down right, 3 is down left

    if (ballpositionx == 1 && ballpositionx == 1073741824){
        resethasrun = 1;
    }

    if (ballrow == 1 || ballrow == 31){
        switch(balldirection){
            case 0: balldirection = 3; break;
            case 1: balldirection = 2; break;
            case 2: balldirection = 1; break;
            case 3: balldirection = 0; break;
        }
    }
    if (paddlecentre1 == ballrow || paddlecentre1 + 1 == ballrow || paddlecentre1 - 1 == ballrow && ballpositionx == 4){
        switch(balldirection){
            case 0: balldirection = 1; break;
            case 3: balldirection = 2; break;
        }
    }
    if (paddlecentre2 == ballrow || paddlecentre2 + 1 == ballrow || paddlecentre2 - 1 == ballrow && ballpositionx == 536870912){
        switch(balldirection){
            case 1: balldirection = 0; break;
            case 2: balldirection = 3; break;
        }
    }

    if (balldirection == 0 || balldirection == 3){
        ballpositionx = ballpositionx / 2;
    }
    else if (balldirection == 1 || balldirection == 2){
        ballpositionx = ballpositionx * 2;
    }
    if (balldirection == 0 || balldirection == 1){
        ballrow--;
    }
    else if (balldirection == 2 || balldirection == 3){
        ballrow++;
    }

    int whichsection = 0;
    if (ballrow > 15){
        whichsection = 1;
    }
    int ballpositiony = ballrow % 15;

    if (whichsection == 0){
        topScreen[ballrow] = topScreen[ballrow] + ballpositionx;
    }
    else if (whichsection == 1){
        bottomScreen[ballrow] = bottomScreen[ballrow] + ballpositionx;
    }
    
    //add rebound mechanic with paddlecentre 1 and 2
}

// VERY IMPORTANT BALL ALGORITHM HAS BEEN UPDATED ADD IT TO CODE

/*
    if (ballrow == 1 || ballrow == 31){
        switch(balldirection){
            case 0: balldirection = 3; break;
            case 1: balldirection = 2; break;
            case 2: balldirection = 1; break;
            case 3: balldirection = 0; break;
        }
    }
    if (paddlecentre1 == ballrow || paddlecentre1 + 1 == ballrow || paddlecentre1 - 1 == ballrow && ballpositionx == 4){
        switch(balldirection){
            case 0: balldirection = 1; break;
            case 3: balldirection = 2; break;
        }
    }
    if (paddlecentre2 == ballrow || paddlecentre2 + 1 == ballrow || paddlecentre2 - 1 == ballrow && ballpositionx == 536870912){
        switch(balldirection){
            case 1: balldirection = 0; break;
            case 2: balldirection = 3; break;
        }
    }
*/