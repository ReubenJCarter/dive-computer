#ifndef BLUETOOTH_SYSTEM_HPP
#define BLUETOOTH_SYSTEM_HPP

#define BLUETOOTH_SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"
#define BLUETOOTH_CHARACTERISTIC_UUID "abcd1234-ab12-cd34-ef56-1234567890ab"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

class BluetoothSystem {

private:

    BLEServer* pServer = NULL;
    BLECharacteristic* pCharacteristic;
    bool deviceConnected = false;

    //Callbacks from the underlying bluetooth server
    class ServerCallbacks : public BLEServerCallbacks {
    public:
        ServerCallbacks(BluetoothSystem* parent);

    private:
        BluetoothSystem* bluetoothSystem;

        void onConnect(BLEServer* pServer); 
        void onDisconnect(BLEServer* pServer); 
    };

public:
    BluetoothSystem(); 
    void begin(); 
}; 

#endif // BLUETOOTH_SYSTEM_HPP