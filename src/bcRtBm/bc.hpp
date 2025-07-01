// fileName: bc.hpp
#pragma once

#include "bc_common.hpp"
#include <array>

// Arayüzün derlenmesi için BusController sınıfının temel tanımı.
// İçi daha sonra doldurulacaktır.
class FrameComponent; // Forward declaration

class BusController {
public:
    static BusController& getInstance() {
        static BusController instance;
        return instance;
    }

    // Backend yazıldığında kullanılacak fonksiyonlar
    bool isInitialized() { return true; }
    int initialize(int deviceId) { return 0; }
    const char* getAIMError(int ret) { return "No Error"; }
    int defineFrameResources(FrameComponent* frame) { return 0; }
    int sendAcyclicFrame(FrameComponent* frame, std::array<unsigned short, 32>& data) { return 0; }

private:
    BusController() = default;
};