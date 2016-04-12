#include "ComponentFinaleTwo.h"
#include "Engine.h"
#include "State.h"
#include "LevelComponentRenderer.h"
#include "Interpolation.h"
#include "Random.h"

namespace ForLeaseEngine {
    namespace Components {
        FinaleTwo::FinaleTwo(Entity& owner) : Component(owner), DeadForest(""), AliveForestTop(""), AliveForestBottom(""),
                                 PulseObject(""), Logo(""), Credits(""), Camera(""),
                                 StartFadeInTime(0), DelayToFadeOut1(0), FadeOut1Time(0),
                                 ScreenShakeAmount(0), DelayToFadeIn1(0), FadeIn1Time(0),
                                 PulseSpeed(0), DelayToFadeOut2(0), FadeOut2Time(0),
                                 DelayToFadeIn2(0), FadeIn2Time(0), ScrollSpeed(0),
                                 CreditsDelay(0), CurrentState(START_FADE_IN), StateTimer(0),
                                 PulseTimer(0) {
            Entity* cameraObject = ForLease->GameStateManager().CurrentState().GetEntityByName(Camera);
            if(cameraObject) {
                CameraStartPosition = cameraObject->GetComponent<Components::Transform>()->Position;
            }
        }

        FinaleTwo::~FinaleTwo() {}

        ComponentType FinaleTwo::GetType() { return Type; }

        void FinaleTwo::Update() {
            float dt = ForLease->FrameRateController().GetDt();
            StateTimer += dt;

            if(CurrentState == START) {
                ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->SetOverlayColor(1, 1, 1, 1);

                StateTimer = 0;
                CurrentState = START_FADE_IN;
            }
            else if(CurrentState == START_FADE_IN) {
                float fadeVal = 1 - StateTimer / StartFadeInTime;
                ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->SetOverlayColor(fadeVal, fadeVal, fadeVal, fadeVal);
                Entity* cameraObject = ForLease->GameStateManager().CurrentState().GetEntityByName(Camera);

                if(cameraObject) {
                    //Components::Transform* trans = cameraObject->GetComponent<Components::Transform>();
                    //std::cout << "[" << trans->Position[0] << "," << trans->Position[2] << "] -> [";
                    cameraObject->GetComponent<Components::Transform>()->Position = CameraStartPosition + Vector(RandomFloat(-ScreenShakeAmount, ScreenShakeAmount),
                                                                                                                 RandomFloat(-ScreenShakeAmount, ScreenShakeAmount));
                    //std::cout << trans->Position[0] << "," << trans->Position[1] << "]" << std::endl;
                }

                if(StateTimer >= StartFadeInTime) {
                    StateTimer = 0;
                    CurrentState = DELAY_FADE_OUT_1;
                }
            }
            else if(CurrentState == DELAY_FADE_OUT_1) {
                Entity* cameraObject = ForLease->GameStateManager().CurrentState().GetEntityByName(Camera);

                if(cameraObject) {
                    cameraObject->GetComponent<Components::Transform>()->Position = CameraStartPosition + Vector(RandomFloat(-ScreenShakeAmount, ScreenShakeAmount),
                                                                                                                 RandomFloat(-ScreenShakeAmount, ScreenShakeAmount));
                }
                if(StateTimer >= DelayToFadeOut1) {
                    StateTimer = 0;
                    CurrentState = FADE_OUT_1;
                }
            }
            else if(CurrentState == FADE_OUT_1) {
                float fadeVal = StateTimer / FadeOut1Time;
                ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->SetOverlayColor(fadeVal, fadeVal, fadeVal, fadeVal);
                Entity* cameraObject = ForLease->GameStateManager().CurrentState().GetEntityByName(Camera);

                if(cameraObject) {
                    cameraObject->GetComponent<Components::Transform>()->Position = CameraStartPosition + Vector(RandomFloat(-ScreenShakeAmount, ScreenShakeAmount),
                                                                                                                 RandomFloat(-ScreenShakeAmount, ScreenShakeAmount));
                }
                if(StateTimer >= FadeOut1Time) {
                    cameraObject->GetComponent<Components::Transform>()->Position = CameraStartPosition;
                    StateTimer = 0;
                    CurrentState = DELAY_FADE_IN_1;
                }
            }
            else if(CurrentState == DELAY_FADE_IN_1) {
                if(StateTimer >= DelayToFadeIn1) {
                    StateTimer = 0;
                    CurrentState = FADE_IN_1;

                    Entity* deadForest = ForLease->GameStateManager().CurrentState().GetEntityByName(DeadForest);
                    if(deadForest) {
                        Components::Sprite* sprite = deadForest->GetComponent<Components::Sprite>();

                        if(sprite) {
                            sprite->Visible = false;
                        }
                    }
                }
            }
            else if(CurrentState == FADE_IN_1) {
                float fadeVal = 1 - StateTimer / FadeIn1Time;
                ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->SetOverlayColor(fadeVal, fadeVal, fadeVal, fadeVal);
                Entity* pulseObject = ForLease->GameStateManager().CurrentState().GetEntityByName(PulseObject);
                Entity* cameraObject = ForLease->GameStateManager().CurrentState().GetEntityByName(Camera);
                if(pulseObject && cameraObject) {
                    Components::Transform* pulseTrans = pulseObject->GetComponent<Components::Transform>();
                    pulseTrans->Position += Vector(0, PulseSpeed * ForLease->FrameRateController().GetDt());
                    Components::Transform* cameraTrans = cameraObject->GetComponent<Components::Transform>();

                    if(pulseTrans->Position[1] - pulseTrans->ScaleY >= cameraTrans->Position[1] + cameraObject->GetComponent<Components::Camera>()->Size / 2) {
                        pulseTrans->Position = cameraTrans->Position - Vector(0, pulseTrans->ScaleY + cameraObject->GetComponent<Components::Camera>()->Size / 2);
                    }
                }

                if(StateTimer >= FadeIn1Time) {
                    StateTimer = 0;
                    CurrentState = DELAY_FADE_OUT_2;
                }
            }
            else if(CurrentState == DELAY_FADE_OUT_2) {
                Entity* pulseObject = ForLease->GameStateManager().CurrentState().GetEntityByName(PulseObject);
                Entity* cameraObject = ForLease->GameStateManager().CurrentState().GetEntityByName(Camera);
                if(pulseObject && cameraObject) {
                    Components::Transform* pulseTrans = pulseObject->GetComponent<Components::Transform>();
                    pulseTrans->Position += Vector(0, PulseSpeed * ForLease->FrameRateController().GetDt());
                    Components::Transform* cameraTrans = cameraObject->GetComponent<Components::Transform>();

                    if(pulseTrans->Position[1] - pulseTrans->ScaleY >= cameraTrans->Position[1] + cameraObject->GetComponent<Components::Camera>()->Size / 2) {
                        pulseTrans->Position = cameraTrans->Position - Vector(0, pulseTrans->ScaleY + cameraObject->GetComponent<Components::Camera>()->Size / 2);
                    }
                }

                if(StateTimer >= DelayToFadeOut2) {
                    StateTimer = 0;
                    CurrentState = FADE_OUT_2;
                }
            }
            else if(CurrentState == FADE_OUT_2) {
                float fadeVal = StateTimer / FadeOut2Time;
                ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->SetOverlayColor(0, 0, 0, fadeVal);

                Entity* pulseObject = ForLease->GameStateManager().CurrentState().GetEntityByName(PulseObject);
                Entity* cameraObject = ForLease->GameStateManager().CurrentState().GetEntityByName(Camera);
                if(pulseObject && cameraObject) {
                    Components::Transform* pulseTrans = pulseObject->GetComponent<Components::Transform>();
                    pulseTrans->Position += Vector(0, PulseSpeed * ForLease->FrameRateController().GetDt());
                    Components::Transform* cameraTrans = cameraObject->GetComponent<Components::Transform>();

                    if(pulseTrans->Position[1] - pulseTrans->ScaleY >= cameraTrans->Position[1] + cameraObject->GetComponent<Components::Camera>()->Size / 2) {
                        pulseTrans->Position = cameraTrans->Position - Vector(0, pulseTrans->ScaleY + cameraObject->GetComponent<Components::Camera>()->Size / 2);
                    }
                }

                if(StateTimer >= FadeOut2Time) {
                    StateTimer = 0;
                    CurrentState = DELAY_FADE_IN_2;

                    Components::Sprite* pulseSprite = pulseObject->GetComponent<Components::Sprite>();
                    if(pulseSprite) {
                        pulseSprite->Visible = false;
                    }
                }
            }
            else if(CurrentState == DELAY_FADE_IN_2) {
                if(StateTimer >= DelayToFadeOut2) {
                    StateTimer = 0;
                    CurrentState = FADE_IN_2;

                    Entity* logo = ForLease->GameStateManager().CurrentState().GetEntityByName(Logo);
                    Entity* camera = ForLease->GameStateManager().CurrentState().GetEntityByName(Camera);
                    if(logo && camera) {
                        camera->GetComponent<Components::Transform>()->Position = logo->GetComponent<Components::Transform>()->Position;
                    }
                }
            }
            else if(CurrentState == FADE_IN_2) {
                float fadeVal = 1 - StateTimer / FadeIn2Time;
                ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->SetOverlayColor(0, 0, 0, fadeVal);

                if(StateTimer >= FadeIn2Time) {
                    StateTimer = 0;
                    CurrentState = CREDITS_SCROLL;
                }
            }
            else if(CurrentState == CREDITS_SCROLL) {

            }
            else if(CurrentState == CREDITS_DELAY) {

            }
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
            finale.ReadString("DeadForest", DeadForest);
            finale.ReadString("AliveForestTop", AliveForestTop);
            finale.ReadString("AliveForestBottom", AliveForestBottom);
            finale.ReadString("PulseObject", PulseObject);
            finale.ReadString("Logo", Logo);
            finale.ReadString("Credits", Credits);
            finale.ReadString("Camera", Camera);

            finale.ReadFloat("StartFadeInTime", StartFadeInTime);
            finale.ReadFloat("DelayToFadeOut1", DelayToFadeOut1);
            finale.ReadFloat("FadeOut1Time", FadeOut1Time);
            finale.ReadFloat("ScreenShakeAmount", ScreenShakeAmount);
            finale.ReadFloat("DelayToFadeIn1", DelayToFadeIn1);
            finale.ReadFloat("FadeIn1Time", FadeIn1Time);
            finale.ReadFloat("PulseSpeed", PulseSpeed);
            finale.ReadFloat("DelayToFadeOut2", DelayToFadeOut2);
            finale.ReadFloat("FadeOut2Time", FadeOut2Time);
            finale.ReadFloat("DelayToFadeIn2", DelayToFadeIn2);
            finale.ReadFloat("FadeIn2Time", FadeIn2Time);
            finale.ReadFloat("ScrollSpeed", ScrollSpeed);
            finale.ReadFloat("CreditsDelay", CreditsDelay);
        }
    }
}
