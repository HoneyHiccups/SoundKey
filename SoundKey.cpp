// SoundKey.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define no_debug
#include <iostream>
#include "Driver.h"

double on_pitch[24] =

{       // 0       1     2     3          4     5       6      7
         000.00,000.00,000.00,000.00,   000.00,000.00,000.00,000.00,
         261.63,293.66,329.63,349.23,   392.00,440.00,493.88,523.25,
         000.00,000.00,000.00,000.00,   000.00,000.00,000.00,000.00,
};

double pitch[24] =

{
         000.00,000.00,000.00,000.00,   000.00,000.00,000.00,000.00,
         000.00,000.00,000.00,000.00,   000.00,000.00,000.00,000.00,
         000.00,000.00,000.00,000.00,   000.00,000.00,000.00,000.00,
};
double power[24] = { 1 };
bool on_off[24] = { false };
double lerp_state[24] = { 0 };
double last_frame=0;
double vol = .1;
double vib = 1;

bool spacebar;
bool toggle;

double pitchmod = 0;
double output = 0;

void scalepitch() {

    for (int i = 0; i <= 7; i++) {
        on_pitch[i] = on_pitch[i + 8] * 4;
    }
    for (int i = 16; i <= 23; i++) {
        on_pitch[i] = on_pitch[i - 8] / 4;
    }
    
    for (int i = 0; i < 24; i++) {
        
        power[i] = 0;
        on_off[i] = 0;
        lerp_state[i] = 0;
        pitch[i] = on_pitch[i];
    }
#ifdef debug
    for (int i=0, j = 0; i < 24; i++,j++) {
        if (j == 8 || j == 16) { cout << endl; }
        wcout << on_pitch[i] << ", ";

    }
#endif
}

double makenoise(double dtime) {
    double texture_low = (/*pitch[0]+pitch[1] + pitch[2] + pitch[3] + pitch[4] + pitch[5] + pitch[6] + pitch[7] + pitch[8] + pitch[9] + pitch[10]
        + pitch[11] + pitch[12] + pitch[13] + pitch[14] + pitch[15] + */ pitch[16] + pitch[17] + pitch[18] + pitch[19] + pitch[20]
        + pitch[21] + pitch[22] + pitch[23])/8 ;
    double texture_hi = (pitch[0]+pitch[1] + pitch[2] + pitch[3] + pitch[4] + pitch[5] + pitch[6] + pitch[7] ) / 8;
    double texture_med = (+ pitch[8] + pitch[9] + pitch[10]+ pitch[11] + pitch[12] + pitch[13] + pitch[14] + pitch[15] ) / 8;

    output =
        (sin(pitch[1] * 2 * 3.14 * dtime) * power[1]) +
        (sin(pitch[2] * 2 * 3.14 * dtime) * power[2]) +
        (sin(pitch[3] * 2 * 3.14 * dtime) * power[3]) +
        (sin(pitch[0] * 2 * 3.14 * dtime) * power[0]) +
        (sin(pitch[4] * 2 * 3.14 * dtime) * power[4]) +
        (sin(pitch[5] * 2 * 3.14 * dtime) * power[5]) +
        (sin(pitch[6] * 2 * 3.14 * dtime) * power[6]) +
        (sin(pitch[7] * 2 * 3.14 * dtime) * power[7]) +
        (sin(pitch[8] * 2 * 3.14 * dtime) * power[8]) +
        (sin(pitch[9] * 2 * 3.14 * dtime) * power[9]) +
        (sin(pitch[10] * 2 * 3.14 * dtime) * power[10]) +
        (sin(pitch[11] * 2 * 3.14 * dtime) * power[11]) +
        (sin(pitch[12] * 2 * 3.14 * dtime) * power[12]) +
        (sin(pitch[13] * 2 * 3.14 * dtime) * power[13]) +
        (sin(pitch[14] * 2 * 3.14 * dtime) * power[14]) +
        (sin(pitch[15] * 2 * 3.14 * dtime) * power[15]) +
        (sin(pitch[16] * 2 * 3.14 * dtime) * power[16]) +
        (sin(pitch[17] * 2 * 3.14 * dtime) * power[17]) +
        (sin(pitch[18] * 2 * 3.14 * dtime) * power[18]) +
        (sin(pitch[19] * 2 * 3.14 * dtime) * power[19]) +
        (sin(pitch[20] * 2 * 3.14 * dtime) * power[20]) +
        (sin(pitch[21] * 2 * 3.14 * dtime) * power[21]) +
        (sin(pitch[22] * 2 * 3.14 * dtime) * power[22]) +
        (sin(pitch[23] * 2 * 3.14 * dtime) * power[23]) ;
        //+ sin(texture_low * 2 * 3.14 * dtime) + (sin(texture_hi * 2 * 3.14 * dtime) * .01) + (sin(texture_med * 2 * 3.14 * dtime) * 0.6);
    output = output * vol;
    return output;
}
void get_input();
void loadlerp(double g_time);
double clamp(double n, double lower, double upper);
void change_pitch(double g_t);

int main()
{
    scalepitch();
    double max = 0;
    double min = 0;
    
    vector<wstring> devices = SDriver<short>::Enumerate();
   // wcout << devices.front() << endl;


    //for (auto d : devices) wcout << " foud output " << d << endl;

    SDriver<int> sound(devices[0], 44100, 1, 8, 512);

    sound.SetUserFunction(makenoise);
    wcout << "This is sound key press keys to make noise.\n";
    wcout << "Hold down space bar for vibrado" << endl;

    while (1 == 1) {

        get_input();
        loadlerp(sound.GetTime());
        change_pitch(sound.GetTime());
        


    }
    return 0;

    
}

double lerp(double a, double b, double w) {
    return a + w * (b - a);
}

void loadlerp(double g_time) {
    double delta = g_time-last_frame;
        last_frame = g_time;

    for (int i = 0; i < 24; i++) {
        if (on_off[i] == 1) {
            lerp_state[i]= clamp((lerp_state[i]+(delta + delta + delta + delta)), 0, 1);
        }
        else {
            lerp_state[i] = clamp((lerp_state[i] - (delta+delta+ delta + delta)), 0, 1);
        }
    }

}

void change_pitch(double g_t) {
    if (!spacebar) {
        for (int i = 0; i < 24; i++) {
            power[i] = lerp(0.0, 3, lerp_state[i]); //leroing the ampaltuide of each indvaual wave to 0 - sinwave +3 so it has a vibrado feel
        }
    }
    else {
        for (int i = 0; i < 24; i++) {
            power[i] = lerp(0.0, (sin(4.33 * 2 * 3.14 * g_t) + 3), lerp_state[i]); //leroing the ampaltuide of each indvaual wave to 0 - sinwave +3 so it has a vibrado feel
        }
    }
    for (int i = 0; i < 24; i++) {
        if (on_off[i] == 1){
            pitch[i] = on_pitch[i];
        }
        else {
            //pitch[i] = 0.0;
        }
       
    }

}

double clamp(double n, double lower, double upper) {
    return max(lower, min(n, upper));
}

void get_input() {
    on_off[23] = (GetAsyncKeyState(VK_OEM_2) & 0x8000);
    on_off[22] = (GetAsyncKeyState(VK_OEM_PERIOD) & 0x8000);
    on_off[21] = (GetAsyncKeyState(VK_OEM_COMMA) & 0x8000);
    on_off[20] = (GetAsyncKeyState('M') & 0x8000);
    on_off[19] = (GetAsyncKeyState('V') & 0x8000);
    on_off[18] = (GetAsyncKeyState('C') & 0x8000);
    on_off[17] = (GetAsyncKeyState('X') & 0x8000);
    on_off[16] = (GetAsyncKeyState('Z') & 0x8000);
    on_off[7] = (GetAsyncKeyState('P') & 0x8000);
    on_off[6] = (GetAsyncKeyState('O') & 0x8000);
    on_off[5] = (GetAsyncKeyState('I') & 0x8000);
    on_off[4] = (GetAsyncKeyState('U') & 0x8000);
    on_off[3] = (GetAsyncKeyState('R') & 0x8000);
    on_off[2] = (GetAsyncKeyState('E') & 0x8000);
    on_off[1] = (GetAsyncKeyState('W') & 0x8000);
    on_off[0] = (GetAsyncKeyState('Q') & 0x8000);
    on_off[15] = (GetAsyncKeyState(VK_OEM_1) & 0x8000);
    on_off[14] = (GetAsyncKeyState('L') & 0x8000);
    on_off[13] = (GetAsyncKeyState('K') & 0x8000);
    on_off[12] = (GetAsyncKeyState('J') & 0x8000);
    on_off[11] = (GetAsyncKeyState('F') & 0x8000);
    on_off[10] = (GetAsyncKeyState('D') & 0x8000);
    on_off[9] = (GetAsyncKeyState('S') & 0x8000);
    on_off[8] = (GetAsyncKeyState('A') & 0x8000);
    spacebar =  (GetAsyncKeyState(VK_SPACE) & 0x8000);
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
        if (!GetAsyncKeyState(VK_SPACE) & 0x8000) {
            spacebar = !spacebar;
            return;
        }


    }
}
