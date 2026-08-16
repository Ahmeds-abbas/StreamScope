#pragma once

class BufferModel
{
public:
    void add(double seconds);
    void consume(double seconds);

    double level() const;
    bool empty() const;

private:
    double bufferSeconds_ = 0.0;
};
