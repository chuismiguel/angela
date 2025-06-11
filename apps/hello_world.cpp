#include <iostream>
#include <PiPCA9685/PCA9685.h>
#include <unistd.h>

int main() {
    std::cout << "Hello World from Pi Zero 2W!" << std::endl;
    return 0;

    PiPCA9685::PCA9685 pca{};

    pca.set_pwm_freq(60.0);

    while(true) {
    pca.set_pwm(9, 0, 4095); 
  }
}