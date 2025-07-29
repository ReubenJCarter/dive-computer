#include "BluetoothSystem.hpp"


BluetoothSystem::ServerCallbacks::ServerCallbacks(BluetoothSystem* parent) {
    bluetoothSystem = parent;
}

void BluetoothSystem::ServerCallbacks::onConnect(BLEServer* pServer) {
    bluetoothSystem->deviceConnected = true;
}

void BluetoothSystem::ServerCallbacks::onDisconnect(BLEServer* pServer) {
    bluetoothSystem->deviceConnected = false;
}


BluetoothSystem::BluetoothSystem() {
       
}

void BluetoothSystem::begin() {
    BLEDevice::init("Dive Logic - Wrasse V1");

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new BluetoothSystem::ServerCallbacks(this));

    BLEService *pService = pServer->createService(BLUETOOTH_SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
        BLUETOOTH_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    );

    pCharacteristic->setValue("Hello BLE");
    pService->start();
    pServer->getAdvertising()->start();
}

