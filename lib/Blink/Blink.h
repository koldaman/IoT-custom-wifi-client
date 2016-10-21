/**************************************************************
   Blink zajistuje blikani LED
   Je mozne nastavit PIN, delku rozviceni LED, delku zhasnuti LED a pocet opakovani
   PIN pracuje v rezimu pro zapis (OUTPUT)
 **************************************************************/

#include <Ticker.h>
#include <vector>

#ifndef BLINK_H
#define BLINK_H

class Blink {
  public:
    Blink(const int pinNumber);
    Blink(const int pinNumber, std::vector<int> intervals, const int repetitionCount);

    void init(std::vector<int> intervals, const int repetitionCount);
    void start();
    void stop();
  private:
    int    _pinNumber;
    std::vector<int> _intervals;
    int    _currentInterval;
    int    _repetitionCount;
    int    _count;
    bool   _stopRequested;
    int    _state;
    Ticker _ticker;
    static void changeState(Blink *blink);
};

#endif
