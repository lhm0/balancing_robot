#ifndef IMU_H
#define IMU_H

class IMU {
public:
    bool begin();
    void update();
    float getAngleX() const;
    float getGyroRateX() const;

private:
    float angleX = 0.0;
    float alpha = 0.98;
    unsigned long lastUpdate = 0;
};

#endif