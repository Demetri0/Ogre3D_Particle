#include "myapplication.h"

int main(void){
    MyApplication app;
    app.startUp();
    while( app.keepRunning() ){
        app.renderOneFrame();
    }
    return 0;
}
