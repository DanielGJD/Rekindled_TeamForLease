#include "ComponentFinaleTwo.h"

namespace ForLeaseEngine {
    namespace Components {
        FinaleTwo::FinaleTwo(Entity& owner) : Component(owner), DeadForest(""), AliveForestTop(""), AliveForestBottom(""),
                                 PulseObject(""), Logo(""), Credits(""), Camera(""),
                                 StartFadeInTime(0), DelayToFadeOut1(0), FadeOut1Time(0),
                                 ScreenShakeAmount(0), DelayToFadeIn1(0), FadeIn1Time(0),
                                 PulseSpeed(0), DelayToFadeOut2(0), FadeOut2Time(0),
                                 DelayToFadeIn2(0), FadeIn2Time(0), ScrollSpeed(0),
                                 CreditsDelay(0), CurrentState(START_FADE_IN), StateTimer(0),
                                 PulseTimer(0) {}

        FinaleTwo::~FinaleTwo() {}

        ComponentType FinaleTwo::GetType() { return Type; }

        void FinaleTwo::Update() {
        }

        void FinaleTwo::Serialize(Serializer& root) {
            root.WriteUlonglong("Type", static_cast<unsigned long long>(Type));

            Serializer finale = root.GetChild("FinaleTwo");
            finale.WriteString("DeadForest", DeadForest);
            finale.WriteString("AliveForestTop", AliveForestTop);
            finale.WriteString("AliveForestBottom", AliveForestBottom);
            finale.WriteString("PulseObject", PulseObject);
            finale.WriteString("Logo", Logo);
            finale.WriteString("Credits", Credits);
            finale.WriteString("Camera", Camera);

            finale.WriteFloat("StartFadeInTime", StartFadeInTime);
            finale.WriteFloat("DelayToFadeOut1", DelayToFadeOut1);
            finale.WriteFloat("FadeOut1Time", FadeOut1Time);
            finale.WriteFloat("ScreenShakeAmount", ScreenShakeAmount);
            finale.WriteFloat("DelayToFadeIn1", DelayToFadeIn1);
            finale.WriteFloat("FadeIn1Time", FadeIn1Time);
            finale.WriteFloat("PulseSpeed", PulseSpeed);
            finale.WriteFloat("DelayToFadeOut2", DelayToFadeOut2);
            finale.WriteFloat("FadeOut2Time", FadeOut2Time);
            finale.WriteFloat("DelayToFadeIn2", DelayToFadeIn2);
            finale.WriteFloat("FadeIn2Time", FadeIn2Time);
            finale.WriteFloat("ScrollSpeed", ScrollSpeed);
            finale.WriteFloat("CreditsDelay", CreditsDelay);

            root.Append(finale, "FinaleTwo");
        }

        void FinaleTwo::Deserialize(Serializer& root) {
            Serializer finale = root.GetChild("FinaleTwo");
            finale.WriteString("DeadForest", DeadForest);
            finale.WriteString("AliveForestTop", AliveForestTop);
            finale.WriteString("AliveForestBottom", AliveForestBottom);
            finale.WriteString("PulseObject", PulseObject);
            finale.WriteString("Logo", Logo);
            finale.WriteString("Credits", Credits);
            finale.WriteString("Camera", Camera);

            finale.WriteFloat("StartFadeInTime", StartFadeInTime);
            finale.WriteFloat("DelayToFadeOut1", DelayToFadeOut1);
            finale.WriteFloat("FadeOut1Time", FadeOut1Time);
            finale.WriteFloat("ScreenShakeAmount", ScreenShakeAmount);
            finale.WriteFloat("DelayToFadeIn1", DelayToFadeIn1);
            finale.WriteFloat("FadeIn1Time", FadeIn1Time);
            finale.WriteFloat("PulseSpeed", PulseSpeed);
            finale.WriteFloat("DelayToFadeOut2", DelayToFadeOut2);
            finale.WriteFloat("FadeOut2Time", FadeOut2Time);
            finale.WriteFloat("DelayToFadeIn2", DelayToFadeIn2);
            finale.WriteFloat("FadeIn2Time", FadeIn2Time);
            finale.WriteFloat("ScrollSpeed", ScrollSpeed);
            finale.WriteFloat("CreditsDelay", CreditsDelay);
        }
    }
}
