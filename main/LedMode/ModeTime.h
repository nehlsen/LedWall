#ifndef LEDWALL_MODETIME_H
#define LEDWALL_MODETIME_H

#include "ModeText.h"

namespace LedWall::Mode {

class ModeTime : public ModeText
{
public:
    explicit ModeTime(LedMatrix& matrix);

    bool update() override;

    void readOptions(cJSON *root) override;
    bool writeOptions(cJSON *root) override;

    enum Variant {
        VariantTime = 0,
        VariantCountDown = 1, // count down from e.g. 10 Minutes
        VariantCountDownTo = 2, // count down to e.g. 31.12. 24:00
        VariantCountUp = 3, // just start to count time since "now"
        VariantCountSince = 4, // time since e.g. 1.1. 0:00
    };

    void setVariant(Variant variant);
    Variant getVariant() const;
    void setVariantParameter(int variantParameter);
    int getVariantParameter() const;

    void setVariantTime();
    void setVariantCountdown(int countdownSeconds);
//    void setVariantCountdownTo();
    void setVariantCountUp();
//    void setVariantCountSince();

private:
    int64_t m_lastTimeUpdate = 0;
    Variant m_variant = Variant::VariantTime;
    int m_variantParameter = 0;

    void showTime(bool isOddSecond);
    void showCountDown(bool isOddSecond);
    void showCountUp(bool isOddSecond);

    static std::string formatSeconds(int seconds, bool includeSplittingDots);
};

}

#endif //LEDWALL_MODETIME_H
