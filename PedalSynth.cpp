#include "PedalApp.hpp"

int main(){
  PedalApp* app = createApp();
  while(runApp(app)){
    updateApp(app);
  }
  deleteApp(app);
}