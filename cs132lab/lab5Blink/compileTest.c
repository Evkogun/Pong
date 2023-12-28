#include "libopencm3/stm32/rcc.h"   //Needed to enable clocks for particular GPIO ports
#include "libopencm3/stm32/gpio.h"  //Needed to define things on the GPIO
#include <stdint.h>
#include "libopencm3/stm32/adc.h" // analogue inputs

void printScreen(uint32_t bottomScreen[], uint32_t topScreen[]);
void paddleController(uint32_t bottomScreen[], uint32_t topScreen[], int paddlecentre1, int paddlecentre2, int controlside);
int ball(uint32_t bottomScreen[], uint32_t topScreen[], int paddlecentre1, int paddlecentre2, int resethasrun);
void rowSelector(int rowSelector);
void scoreScreen(uint32_t bottomScreen[], uint32_t topScreen[], int resethasrun);
void gameOver(uint32_t bottomScreen[], uint32_t topScreen[], int score[], int resethasrun);
void scoreDisplay(uint32_t topScreen[], int score[])
int joystickDir(int controlside, int upordown);
void pDisplay(uint32_t bottomScreen[], uint32_t topScreen[], int resethasrun);

int main(void) {

    int resethasrun = 0;
    int controlside = 0;
    int paddlecentre1 = 15;
    int paddlecentre2 = 15;
    int gameover = 0;
    int score[] = {0, 0};


    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO5);            //GPIO Port Name, GPIO Mode, GPIO Push Up Pull Down Mode, GPIO Pin Number
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO5);   //GPIO Port Name, GPIO Pin Driver Type, GPIO Pin Speed, GPIO Pin Number
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO6);    
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO6);
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO7);    
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO7);
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8);  
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO8);
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9);  
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO9);
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO10);  
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO10);
    /*
    adc_power_off(ADC1);  //Turn off ADC register 1 whist we set it up
    adc_set_clk_prescale(ADC1, ADC_CCR_CKMODE_DIV1);  //Setup a scaling, none is fine for this
    adc_disable_external_trigger_regular(ADC1);   //We don't need to externally trigger the register...
    adc_set_right_aligned(ADC1);  //Make sure it is right aligned to get more usable values
    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_61DOT5CYC);  //Set up sample time
    adc_set_resolution(ADC1, ADC_CFGR1_RES_12_BIT);  //Get a good resolution
    adc_power_on(ADC1);  //Finished setup, turn on ADC register 1

    uint8_t channelArray = {1};  //Define a channel that we want to look at
    adc_set_regular_sequence(ADC1, 1, channelArray);  //Set up the channel
    adc_start_conversion_regular(ADC1);  //Start converting the analogue signal
    
    while(!(adc_eoc(ADC1)));  //Wait until the register is ready to read data

    uint32_t value = adc_read_regular(ADC1);  //Read the value from the register and channel
    */
    // add 96 * 32 clock ticks here
    //Make this only run once by placing the rest of the code in a loop that only ends when the game is over


    rcc_periph_clock_enable(RCC_ADC12); //Enable clock for ADC registers 1 and 2

    adc_power_off(ADC1);  //Turn off ADC register 1 whist we set it up

    adc_set_clk_prescale(ADC1, ADC_CCR_CKMODE_DIV1);  //Setup a scaling, none is fine for this
    adc_disable_external_trigger_regular(ADC1);   //We don't need to externally trigger the register...
    adc_set_right_aligned(ADC1);  //Make sure it is right aligned to get more usable values
    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_61DOT5CYC);  //Set up sample time
    adc_set_resolution(ADC1, ADC_CFGR1_RES_12_BIT);  //Get a good resolution

    adc_power_on(ADC1);  //Finished setup, turn on ADC register 1

    uint32_t bottomScreen[16];
    uint32_t topScreen[16];
    printScreen(bottomScreen, topScreen);
    
    while (gameover == 0){
        bottomScreen[15] = 4294967295;
        topScreen[0] = 4294967295;
        for (int i=0; i<15; i++){
            bottomScreen[i] = 2147483649;
            topScreen[i+1] = 2147483649;
        }
        
        //Putting the paddlecontroller here will mean it will overwrite the exmpty values, this means that the game will clear old paddlecontroller LEDS
        paddleController(bottomScreen, topScreen, paddlecentre1, paddlecentre2, controlside);
        controlside = 1;
        paddleController(bottomScreen, topScreen, paddlecentre1, paddlecentre2, controlside);
        controlside = 0;
        resethasrun = ball(bottomScreen, topScreen, paddlecentre1, paddlecentre2, resethasrun);
        if (resethasrun != 0){
            scoreScreen(bottomScreen, topScreen, resethasrun);
            score[resethasrun - 1]++;
        }
        else{
            scoreDisplay(topScreen, score);
            printScreen(bottomScreen, topScreen);
        }
        if (score[0] == 5){
            void gameOver(bottomScreen, topScreen, score, resethasrun);
            gameover = 1;
        }
        else if (score[1] == 5){
            void gameOver(bottomScreen, topScreen, score, resethasrun);
            gameover = 1;
        }
        for (volatile unsigned int tmr=1e6; tmr > 0; tmr--);
    }
    return 0;
}

void printScreen(uint32_t bottomScreen[], uint32_t topScreen[]){

    // This function prints the arrays bottomscreen and topscreen in that order
    
    // It goes in order or for loops (2 = top/bottom), (16 = 16 rows), (3 = 3 colour options), (32 = 32 bits in an interger)
    unsigned int rowDecimal = 0;
    int rowDecimalOverflow = 0;
    //Insert latch flip here
    gpio_clear(GPIOA, GPIO8);

    for (int i = 0; i<2; i++){
        for (int j = 0; j<16; j++){
            rowSelector((j + 15) % 16); // this was done since the top row would be displyed in the bottom, so I just changed the order of appearance
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
                        gpio_set(GPIOA, GPIO6);
                        //printf("1"); temp
                        
                    }
                    else{
                        //insert clear command here
                        gpio_clear(GPIOA, GPIO6);
                        //printf("0"); temp
                        
                    }
                }
                //printf("\n"); temp replacement for row selector
            }
        
        
        }
    }
    gpio_set(GPIOA, GPIO8);

}

void rowSelector(int rowSelector) {
    for (int r = 0; r < 4; r++) {
        int rowOverflow;
        rowOverflow = rowSelector % 2;
        rowSelector = rowSelector / 2;
        if (rowOverflow == 1) {
            switch (r) {
                case 0: gpio_set(GPIOA, GPIO2); break; // insert c2 printf("%d", 1);
                case 1: gpio_set(GPIOA, GPIO3); break; // insert c3
                case 2: gpio_set(GPIOA, GPIO4); break; // insert c4 ;)
                case 3: gpio_set(GPIOA, GPIO5); break; // insert c5 ect
            }
        } else {
            switch (r) {
                case 0: gpio_clear(GPIOA, GPIO2); break;
                case 1: gpio_clear(GPIOA, GPIO3); break;
                case 2: gpio_clear(GPIOA, GPIO4); break;
                case 3: gpio_clear(GPIOA, GPIO5); break;
            }
        }
    }
    //printf("\n");
}

//If you want to reset it set to clear and clock it a shitton of times 192*32

//When only a single line changes, should I refresh the entire page or just that line

void paddleController(uint32_t bottomScreen[], uint32_t topScreen[], int paddlecentre1, int paddlecentre2, int controlside){
    
    // rows will be numbered 0-31
    // paddle centre should be 2-29
    int goUp = 0; 
    int goDown = 0; 
    int incrementAmount = 0;

    int paddlecentre = 0;
    int upordown = 0; // direction 1 is assumed to be up
  
    

    goUp = joystickDir(controlside, upordown); 
    if (goUp == 0){
        upordown = 1;
        goDown = joystickDir(controlside, upordown); 
    }
    if (goUp + goDown != 0){
        if (controlside = 0){
            incrementAmount = 4;
            paddlecentre = paddlecentre1;
        }
        else {
            incrementAmount = 536870912;
            paddlecentre = paddlecentre2;
        }
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

int ball(uint32_t bottomScreen[], uint32_t topScreen[], int paddlecentre1, int paddlecentre2, int resethasrun){
    // ball direction 0 is up left, 1 is up right, 2 is down right, 3 is down left

    static int ballrow = 15;
    static int balldirection = 0;
    static int ballpositionx = 32768;

    if (ballpositionx == 1073741824){
        return resethasrun = 1;
    }
    else if (ballpositionx == 1){
        return resethasrun = 2;
    }
    else{
        return resethasrun = 0;
    }

    if (ballrow == 1 || ballrow == 31){
        balldirection++;
    }
    if ((paddlecentre1 == ballrow || paddlecentre1 + 1 == ballrow || paddlecentre1 - 1 == ballrow) && ballpositionx == 4){
        balldirection = balldirection + 1;
    }
    if ((paddlecentre2 == ballrow || paddlecentre2 + 1 == ballrow || paddlecentre2 - 1 == ballrow) && ballpositionx == 536870912){
        balldirection = balldirection + 1;
    }
    balldirection = balldirection % 4;

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

int joystickDir(int controlside, int upordown){
    
    int arrayposition = upordown + 2*controlside; // this assigns values depending on upordown and controlside (from 1 - 4)

    uint8_t channelArray[] = {1, 2, 6, 7};  
    adc_set_regular_sequence(ADC1, 1, channelArray[arrayposition]); 
    adc_start_conversion_regular(ADC1); 

    while(!(adc_eoc(ADC1))); // ADC1 is assumed to be the register

    uint32_t value = adc_read_regular(ADC1);
    if (value < 100){ // this is a random value assumed to be the threshold due to lack of time to test 
        return 1
    }
    else {
        return 0;
    }

}

void scoreScreen(uint32_t bottomScreen[], uint32_t topScreen[], int resethasrun){

    for (i=0; i<3; i++){

        bottomScreen[15] = 4294967295;
        topScreen[0] = 4294967295;
        for (int i=0; i<15; i++){
            bottomScreen[i] = 2147483649;
            topScreen[i+1] = 2147483649;
        } // makes the screen a blank border

        printScreen(bottomScreen, topScreen);
        for (volatile unsigned int tmr=1e5; tmr > 0; tmr--);

        // displays P1/P2 
        pDisplay(uint32_t bottomScreen[], uint32_t topScreen[], int resethasrun);

        //dispaly SCORES
        bottomScreen[3] = 4224660383;
        bottomScreen[4] = 2287028289;
        bottomScreen[5] = 2287028289;
        bottomScreen[6] = 2287028289;
        bottomScreen[7] = 4224660383;
        bottomScreen[8] = 2151893072;
        bottomScreen[9] = 2152024144;
        bottomScreen[10] = 2152286288;
        bottomScreen[11] = 4224791455;

        printScreen(bottomScreen, topScreen);
        for (volatile unsigned int tmr=1e5; tmr > 0; tmr--);
    }
    
    // this loop prints blank border screen, then P1/P2 SCORES, this gives the effect of flashing 3 times
}

void scoreDisplay(uint32_t topScreen[], int score[]){
    switch (score[0])
    {
    case 0:
        topScreen[3] = topScreen[3] + 1792;
        topScreen[4] = topScreen[4] + 1280;
        topScreen[5] = topScreen[5] + 1280;
        topScreen[6] = topScreen[6] + 1280;
        topScreen[7] = topScreen[7] + 1792;

        break;
    case 1:
        topScreen[3] = topScreen[3] + 512;
        topScreen[4] = topScreen[4] + 768;
        topScreen[5] = topScreen[5] + 512;
        topScreen[6] = topScreen[6] + 512;
        topScreen[7] = topScreen[7] + 1792; 

        break;
    case 2:
        topScreen[3] = topScreen[3] + 512;
        topScreen[4] = topScreen[4] + 1280;
        topScreen[5] = topScreen[5] + 1024;
        topScreen[6] = topScreen[6] + 512;
        topScreen[7] = topScreen[7] + 1792; 

        break;
    case 3:
        topScreen[3] = topScreen[3] + 1762;
        topScreen[4] = topScreen[4] + 1024;
        topScreen[5] = topScreen[5] + 1762;
        topScreen[6] = topScreen[6] + 1024;
        topScreen[7] = topScreen[7] + 1792; 

        break;
    case 4:
        topScreen[3] = topScreen[3] + 1280;
        topScreen[4] = topScreen[4] + 1280;
        topScreen[5] = topScreen[5] + 1792;
        topScreen[6] = topScreen[6] + 1024;
        topScreen[7] = topScreen[7] + 1024; 

        break;
    }
    switch (score[1])
    {
    case 0:
        topScreen[3] = topScreen[3] + 14680064;
        topScreen[4] = topScreen[4] + 10485760;
        topScreen[5] = topScreen[5] + 10485760;
        topScreen[6] = topScreen[6] + 10485760;
        topScreen[7] = topScreen[7] + 14680064; 

        break;
    case 1:
        topScreen[3] = topScreen[3] + 4194304;
        topScreen[4] = topScreen[4] + 6291456;
        topScreen[5] = topScreen[5] + 4194304;
        topScreen[6] = topScreen[6] + 4194304;
        topScreen[7] = topScreen[7] + 14680064; 

        break;
    case 2:
        topScreen[3] = topScreen[3] + 4194304;
        topScreen[4] = topScreen[4] + 10485760;
        topScreen[5] = topScreen[5] + 8388608;
        topScreen[6] = topScreen[6] + 4194304;
        topScreen[7] = topScreen[7] + 14680064; 

        break;
    case 3:
        topScreen[3] = topScreen[3] + 14680064;
        topScreen[4] = topScreen[4] + 8388608;
        topScreen[5] = topScreen[5] + 14680064;
        topScreen[6] = topScreen[6] + 8388608;
        topScreen[7] = topScreen[7] + 14680064; 

        break;
    case 4:
        topScreen[3] = topScreen[3] + 10485760;
        topScreen[4] = topScreen[4] + 10485760;
        topScreen[5] = topScreen[5] + 14680064;
        topScreen[6] = topScreen[6] + 8388608;
        topScreen[7] = topScreen[7] + 8388608; 

        break;
    
    
    }
    //2^8
    // 2^21 =
    
}

void gameOver(uint32_t bottomScreen[], uint32_t topScreen[], int score[], int resethasrun){

    for (i=0; i<3; i++){

        bottomScreen[15] = 4294967295;
        topScreen[0] = 4294967295;
        for (int i=0; i<15; i++){
            bottomScreen[i] = 2147483649;
            topScreen[i+1] = 2147483649;
        } // makes the screen a blank border

        printScreen(bottomScreen, topScreen);
        for (volatile unsigned int tmr=1e5; tmr > 0; tmr--);

        if (score[0] == 5){
            resethasrun = 2;
            pDisplay(uint32_t bottomScreen[], uint32_t topScreen[], int resethasrun);
        }
        else if (score[1] == 5){
            resethasrun = 1;
            pDisplay(uint32_t bottomScreen[], uint32_t topScreen[], int resethasrun);
        }

        //dispaly WINS
        bottomScreen[4] = 2403717393;
        bottomScreen[5] = 2169278737;
        bottomScreen[6] = 2169278737;
        bottomScreen[7] = 2404682401;
        bottomScreen[8] = 2288290465;
        bottomScreen[9] = 2288290465;
        bottomScreen[10] = 2403714113;


        printScreen(bottomScreen, topScreen);
        for (volatile unsigned int tmr=1e5; tmr > 0; tmr--);
    }
}

void pDisplay(uint32_t bottomScreen[], uint32_t topScreen[], int resethasrun){

    if (resethasrun == 2){
        topScreen[4] = 2155355905;
        topScreen[5] = 2156667137;
        topScreen[6] = 2164277505;
        topScreen[7] = 2164277505;
        topScreen[8] = 2164277505;
        topScreen[9] = 2164269313;
        topScreen[10] = 2155880193;
        topScreen[11] = 2151678209;
        topScreen[12] = 2149581057;
        topScreen[13] = 2148532481;
        topScreen[14] = 2148008193;
        topScreen[15] = 2180776193;
    }
    else{
        topScreen[4] = 2151685889;
        topScreen[5] = 2153783553;
        topScreen[6] = 2152743169;
        topScreen[7] = 2164277505;
        topScreen[8] = 2151694593;
        topScreen[9] = 2151686401;
        topScreen[10] = 2151685889;
        topScreen[11] = 2151678209;
        topScreen[12] = 2151678209;
        topScreen[13] = 2151678209;
        topScreen[14] = 2151678209;
        topScreen[15] = 2214068481;
    }

}








//Evgeny's how to guide to reading heirogyphics (outdated)


// void printScreen(int bottomScreen[], int topScreen[]){
// This function prints the arrays bottomscreen and topscreen in that order
// It goes in order or for loops (2 = top/bottom), (16 = 16 rows), (3 = 3 colour options), (32 = 32 bits in an interger)


//void rowSelector(int rowSelector) {
//This is basically a binary counter that selects rows (GPIO 2-5) as outputs

//void paddleController(int bottomScreen[], int topScreen[], int paddlecentre1, int paddlecentre2, int controlside){
//Bascially takes turns moving both paddles using integer values

//void ball(int balldirection, int ballpositionx, int ballrow, int bottomScreen[], int topScreen[], int paddlecentre1, int paddlecentre2, int resethasrun){
//spawns the ball, changes it's direction when hitting a wall and paddle, also sends the game-over signal

/*
    unsigned int32_t bottomScreen[16] = {0};
    unsigned int32_t topScreen[16] = {0};
    printScreen(bottomScreen, topScreen);
    
    while (resethasrun == 0){
        bottomScreen[16] = {2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 4294967295};
        topScreen[16] = {4294967295, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649, 2147483649};
        
        //Putting the paddlecontroller here will meen it will overwrite the exmpty values, this means that the game will clear old paddlecontroller LEDS
        paddleController(bottomScreen, topScreen, paddlecentre1, paddlecentre2, controlside);
        controlside = 1;
        paddleController(bottomScreen, topScreen, paddlecentre1, paddlecentre2, controlside);
        controlside = 0;
        ball(balldirection, ballpositionx, ballrow, bottomScreen, topScreen, paddlecentre1, paddlecentre2, resethasrun);
        printScreen(bottomScreen, topScreen);
        for (volatile unsigned int tmr=1e6; tmr > 0; tmr--);
    }
    return 0;
*/

// The top resets the code every time it's run by printing zeros all over the screen
// It creates a default value that is basically a square border, it then print player 1s paddle controller then player 2s paddle controller then the ball onto the screen, with a delay so that the movement of the ball/paddle isn't too fast
